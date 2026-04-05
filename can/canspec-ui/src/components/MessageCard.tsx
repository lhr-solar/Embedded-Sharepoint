import type { IdSearchFormat } from "../search";
import type { CanMessage, CanSignal } from "../types";

function formatMessageId(msg: CanMessage, idFormat: IdSearchFormat): string {
  return idFormat === "decimal" ? String(msg.frameId) : msg.hexId;
}

function fmt(v: unknown): string {
  if (v === null || v === undefined) return "—";
  if (typeof v === "boolean") return v ? "yes" : "no";
  if (typeof v === "number" && Number.isNaN(v)) return "—";
  return String(v);
}

function bitRange(s: CanSignal): string {
  const end = s.startBit + s.lengthBits - 1;
  return `${s.startBit}–${end}`;
}

/** Map DBC byte order strings to common CAN naming. */
function endianLabel(byteOrder: string): string {
  const b = byteOrder.toLowerCase();
  if (b === "little_endian" || b.includes("little")) {
    return "Intel (little-endian)";
  }
  if (b === "big_endian" || b.includes("big")) {
    return "Motorola (big-endian)";
  }
  return byteOrder;
}

export function MessageCard({ msg, idFormat }: { msg: CanMessage; idFormat: IdSearchFormat }) {
  const metaParts = [
    msg.cycleTimeMs != null ? `cycle ${msg.cycleTimeMs} ms` : null,
    msg.protocol ? String(msg.protocol) : null,
    msg.isFd ? "CAN FD" : null,
    msg.busName ? `bus ${msg.busName}` : null,
  ].filter(Boolean);

  return (
    <article className="msg-card">
      <header className="msg-card-head">
        <div className="msg-card-title-row">
          <span className={`msg-card-id${idFormat === "decimal" ? " msg-card-id--decimal" : ""}`}>
            {formatMessageId(msg, idFormat)}
          </span>
          <span className="msg-card-name">{msg.name}</span>
        </div>
        <div className="msg-card-meta">
          <span>DLC {msg.dlc}</span>
          {msg.isExtended ? <span className="msg-badge">ext</span> : <span className="msg-badge">std</span>}
          {msg.senders.length > 0 ? (
            <span className="msg-card-ecu">
              ECU: {msg.senders.join(", ")}
            </span>
          ) : (
            <span className="msg-card-ecu muted-sm">no sender in DBC</span>
          )}
        </div>
        {metaParts.length > 0 ? (
          <p className="msg-card-meta-line">{metaParts.join(" · ")}</p>
        ) : null}
        {msg.comment ? <p className="msg-card-comment">{msg.comment}</p> : null}
        {Object.keys(msg.attributes).length > 0 ? (
          <div className="msg-attr-inline">
            {Object.entries(msg.attributes).map(([k, v]) => (
              <div key={k} className="msg-attr-pair">
                <span className="msg-attr-k">{k}</span>
                <span className="msg-attr-v">{fmt(v)}</span>
              </div>
            ))}
          </div>
        ) : null}
      </header>

      {msg.signals.length > 0 ? (
        <div className="msg-signals">
          {msg.signals.map((s) => (
            <div key={s.name} className="sig-block">
              <div className="sig-name-row">
                <span className="sig-name">{s.name}</span>
              </div>
              <div className="sig-meta-row">
                <span className="sig-dt">{s.dataType}</span>
                <span className="sig-bits">
                  bits {bitRange(s)} <span className="muted-sm">({s.lengthBits} bits)</span>
                </span>
                {s.unit ? <span className="sig-unit">unit {s.unit}</span> : null}
              </div>
              {(s.scale != null ||
                s.offset != null ||
                s.minimum != null ||
                s.maximum != null) && (
                <div className="sig-formula mono-sm">
                  {s.scale != null || s.offset != null ? (
                    <>
                      {s.scale != null ? `scale=${fmt(s.scale)}` : null}
                      {s.scale != null && s.offset != null ? " · " : null}
                      {s.offset != null ? `offset=${fmt(s.offset)}` : null}
                    </>
                  ) : null}
                  {(s.minimum != null || s.maximum != null) && (
                    <>
                      {s.scale != null || s.offset != null ? " · " : null}
                      range [{fmt(s.minimum)}, {fmt(s.maximum)}]
                    </>
                  )}
                </div>
              )}
              <div className="sig-extra mono-sm">{endianLabel(s.byteOrder)}</div>
              {(s.initial != null || s.invalid != null || s.rawInitial != null || s.rawInvalid != null) && (
                <div className="sig-extra mono-sm">
                  init {fmt(s.initial)} / invalid {fmt(s.invalid)}
                  {" · "}
                  raw {fmt(s.rawInitial)} / {fmt(s.rawInvalid)}
                </div>
              )}
              {s.multiplexerSignal ? (
                <div className="sig-mux mono-sm">
                  parent {s.multiplexerSignal} → values [{s.multiplexerIds?.join(", ") ?? "—"}]
                </div>
              ) : null}
              {s.choices.length > 0 ? (
                <div className="sig-choices">
                  <span className="sig-choices-label">values:</span>{" "}
                  {s.choices.map((c) => (
                    <span key={c.value} className="choice-pair">
                      <span className="cv">{c.value}</span>
                      <span>=</span>
                      <span>{c.label}</span>
                    </span>
                  ))}
                </div>
              ) : null}
              {s.comment ? <p className="sig-comment">{s.comment}</p> : null}
            </div>
          ))}
        </div>
      ) : (
        <p className="muted-sm pad-sm">No signals defined.</p>
      )}
    </article>
  );
}
