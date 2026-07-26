---
name: usb-kernel-recover
description: Use when a USB device or fixture attached to the ci HIL rig's Linux host is stuck, hung, not enumerating, or wedged after a failed flash or test, or when processes touching USB (testusb, JLinkExe, uhubctl, libusb tools) start hanging in D state. Linux-kernel-side only — a bus owned by a TinyUSB host is out of reach (reset the target / cycle its VBUS instead); the rig's probes and serial fixtures always remain in scope.
---

# USB Recovery on the HIL Rig (Linux kernel side)

Run this skill's `scripts/usb_recover.sh` with `sudo` (abbreviated to
`usb_recover.sh` in the examples below). It wraps the sysfs reset actions, a
uhubctl power-cycle escalator, and a resolver:

```bash
sudo usb_recover.sh resolve    /dev/ttyACM3    # /dev node -> busport (e.g. 3-4.7); also ttyUSB*, sg*
sudo usb_recover.sh authorized <busport>       # deauthorize+reauthorize: re-enumerate, no VBUS cut
sudo usb_recover.sh rebind     <busport>       # usb driver unbind+bind: re-probe
sudo usb_recover.sh hub-cycle  <busport>       # uhubctl VBUS cycle of the feeding port, walking parent hub
                                               # -> root port until the device re-enumerates
sudo usb_recover.sh pci-rebind <pciaddr>       # whole HCD controller unbind+bind, e.g. 0000:02:00.0
sudo usb_recover.sh pci-reset  <pciaddr>       # PCI function-level reset: kills URBs at HW level, no device lock
sudo usb_recover.sh pci-bind   <pciaddr> [drv] # re-bind a DRIVERLESS controller (auto-tries xHCI drivers)
```

`hub-cycle` caveats: leaf hubs that gang (or fake) port power switching bounce
**all siblings** on that hub when cycled; a **self-powered** leaf hub keeps
downstream VBUS up, so cycling it only resets its uplink — that's why the walk
escalates to the root port, where the Renesas cards' per-port power (ppps) is
real. A device that is wedged but bus-powered from a switching hub gets a true
power cycle; one on a self-powered hub may only get a re-enumeration.

## Decide first: is anything stuck in D state?

```bash
ps -eo pid,stat,wchan:30,cmd | awk '$2 ~ /D/'
```

**If yes** (uninterruptible sleep, typically a usbfs ioctl — e.g. testusb inside
`usb_sg_wait`): run `pci-reset` and NOTHING ELSE first:

```bash
sudo usb_recover.sh pci-reset <pciaddr>
```

FLR kills the URBs at the hardware level without taking the per-device lock;
the ioctl then returns and the convoy unwinds on its own.

**Not every controller supports FLR.** The Renesas uPD720201 (`0000:01:00.0`)
has no reset method — `pci-reset` fails with `Inappropriate ioctl for device`
(ENOTTY). On those, there is no clean software D-state cure — a VM reboot is NOT
reliable (downstream hubs can latch up across the PCIe reset and need a physical
replug); ask the operator for a full PVE host power cycle instead. Do NOT
fall through to `pci-rebind` (see next).

**`pci-rebind` can strand the controller driverless.** Its unbind succeeds but,
with a D-state process still holding a URB, the *re-bind* hangs — leaving the
PCI device with **no driver** (`/sys/bus/pci/devices/<addr>/driver` gone) and the
whole controller's fixtures offline. A second `pci-rebind` then dies with "no
driver bound". Recover with `pci-bind <addr>` (re-attaches the xHCI driver);
if that also hangs because the D-state URB is unkillable, only a full PVE host
power cycle (operator action) recovers. The Renesas binds via `xhci-pci-renesas` (firmware loader), others via
`xhci_hcd` — `pci-bind` auto-tries both, or pass the driver explicitly.

**Ordering is critical.** `authorized`/`rebind`/`pci-rebind` all take the
per-device lock the stuck ioctl holds — they block and join the convoy, and
soon every libusb tool (uhubctl, JLinkExe) hangs too. Worse, a blocked
`pci-rebind` grabs the PCI device lock on its way in, which `pci-reset` also
needs: once a rebind has been attempted and is stuck, even FLR deadlocks and
**only a full PVE host power cycle recovers**. pci-reset first (if supported), and never
`pci-rebind` a D-state wedge.

**If no** (device merely dead or silent), escalate gently:

1. `authorized <busport>` — re-enumerates just that device
2. `rebind <busport>` — re-probe; also worth trying on the parent hub's busport
3. `hub-cycle <busport>` — VBUS cycle of the feeding port, walking up to the
   root port; may bounce sibling fixtures on ganged hubs
4. `pci-rebind <pciaddr>` — last resort: bounces every fixture on that controller

## Finding targets

```bash
grep -l <SERIAL> /sys/bus/usb/devices/*/serial          # serial -> busport (dir name)
readlink -f /sys/bus/usb/devices/usb<N>                  # bus N -> its PCI addr in the path
```

Rig layout (2026-07-15, two Renesas uPD720201 cards; bus numbers renumber every
boot — re-derive with `readlink`): AMD `0000:02:00.0` = the debug-probe tree
(J-Links, ST-Links, WCH-Links), no port power switching; Renesas `0000:01:00.0`
and `0000:03:00.0` = DUT device hubs + serial fixtures, and ALL their root-hub
ports have real per-port power (`ppps`, 4+4 each) — `sudo uhubctl -l <bus> -p
<port> -a cycle` cuts VBUS to the leaf hub on that port. The 1a40:0201 leaf
hubs themselves claim "ganged" switching but do not actually cut power.

## Common mistakes

- `resolve` takes a **/dev node**, not a busport or serial ("no such device node").
- `authorized`/`rebind` take a **busport** (`3-4.7`); `pci-rebind`/`pci-reset`
  take a **PCI addr**.
- Command produces no output and doesn't return → it is blocked on the device
  lock: a D-state holder exists; see above.
- Trying `pci-rebind` on a D-state hang — its re-bind hangs and strands the
  controller **driverless**; recover with `pci-bind <addr>`, or a PVE host power
  cycle if the D-state URB is unkillable. Use `pci-reset` (if supported) for D-state, never
  `pci-rebind`.
- Running `pci-reset` on a controller without FLR support (Renesas) → ENOTTY;
  no software recovery — needs a PVE host power cycle.
- A J-Link reset (`r; go`) does not disconnect a wedged DUT from the host: the
  DWC2 soft-connect pullup stays up through a core halt, so stuck URBs stay stuck.
