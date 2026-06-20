import { useEffect, useMemo, useState } from "react";
import { Link, useNavigate } from "react-router-dom";
import { MessageCard } from "./MessageCard";
import { messageMatchesSearch, type IdSearchFormat } from "../search";
import type { CanMessage, CanNetwork, SearchFields, Vehicle } from "../types";

const ECU_NONE = "__none__";

export type MessageSort = "id-asc" | "id-desc" | "name-asc" | "name-desc";

/** All ECU names from DBC nodes plus every sender appearing on messages, plus (no sender) when needed. */
function buildEcuOptions(network: CanNetwork): string[] {
  const set = new Set<string>();
  for (const n of network.nodes) {
    if (n.name) set.add(n.name);
  }
  for (const m of network.messages) {
    if (m.senders.length === 0) set.add(ECU_NONE);
    else m.senders.forEach((s) => set.add(s));
  }
  const list = Array.from(set);
  list.sort((a, b) => {
    if (a === ECU_NONE) return 1;
    if (b === ECU_NONE) return -1;
    return a.localeCompare(b);
  });
  return list;
}

function sortMessages(messages: CanMessage[], sort: MessageSort): CanMessage[] {
  const arr = [...messages];
  switch (sort) {
    case "id-asc":
      arr.sort((a, b) => a.frameId - b.frameId);
      break;
    case "id-desc":
      arr.sort((a, b) => b.frameId - a.frameId);
      break;
    case "name-asc":
      arr.sort((a, b) => a.name.localeCompare(b.name));
      break;
    case "name-desc":
      arr.sort((a, b) => b.name.localeCompare(a.name));
      break;
    default:
      break;
  }
  return arr;
}

export function NetworkExplorer({
  vehicle,
  network,
  allVehicles,
}: {
  vehicle: Vehicle;
  network: CanNetwork;
  allVehicles: Vehicle[];
}) {
  const navigate = useNavigate();
  const [search, setSearch] = useState("");
  const [searchIn, setSearchIn] = useState<SearchFields>({
    ids: true,
    ecus: true,
    msgNames: true,
    sigNames: true,
  });
  const [selectedEcus, setSelectedEcus] = useState<Set<string>>(new Set());
  const [messageSort, setMessageSort] = useState<MessageSort>("id-asc");
  const [idSearchFormat, setIdSearchFormat] = useState<IdSearchFormat>("hex");

  const ecuOptions = useMemo(() => buildEcuOptions(network), [network]);

  useEffect(() => {
    setSelectedEcus(new Set(ecuOptions));
  }, [vehicle.id, network.id, ecuOptions]);

  const filteredMessages = useMemo(() => {
    return network.messages.filter((m) => {
      let ecuOk = false;
      if (m.senders.length === 0) {
        ecuOk = selectedEcus.has(ECU_NONE);
      } else {
        ecuOk = m.senders.some((s) => selectedEcus.has(s));
      }
      if (!ecuOk) return false;
      return messageMatchesSearch(m, search, searchIn, idSearchFormat);
    });
  }, [network.messages, selectedEcus, search, searchIn, idSearchFormat]);

  const sortedMessages = useMemo(
    () => sortMessages(filteredMessages, messageSort),
    [filteredMessages, messageSort],
  );

  const toggleEcu = (id: string) => {
    setSelectedEcus((prev) => {
      const n = new Set(prev);
      if (n.has(id)) n.delete(id);
      else n.add(id);
      return n;
    });
  };

  const selectAllEcus = () => setSelectedEcus(new Set(ecuOptions));
  const deselectAllEcus = () => setSelectedEcus(new Set());

  const selectAllSearchFields = () =>
    setSearchIn({ ids: true, ecus: true, msgNames: true, sigNames: true });
  const deselectAllSearchFields = () =>
    setSearchIn({ ids: false, ecus: false, msgNames: false, sigNames: false });

  const toggleSearchField = (key: keyof SearchFields) => {
    setSearchIn((prev) => ({ ...prev, [key]: !prev[key] }));
  };

  return (
    <div className="dbc-viewer">
      <div className="dbc-toolbar">
        <div className="dbc-heading">
          <h1 className="dbc-title">DBC viewer</h1>
          <p className="dbc-subtitle">
            <span className="mono-sm">{network.filename}</span>
            <span className="dot-sep">·</span>
            {network.messages.length} messages
            <span className="dot-sep">·</span>
            {network.nodes.length} nodes
            <span className="dot-sep">·</span>
            {network.valueTables.length} value tables
          </p>
        </div>

        <div className="dbc-select-row">
          <label className="dbc-field">
            <span className="dbc-label">Vehicle</span>
            <select
              className="dbc-select"
              value={vehicle.id}
              onChange={(e) => {
                const v = allVehicles.find((x) => x.id === e.target.value);
                const first = v?.networks[0];
                if (v && first) navigate(`/${encodeURIComponent(v.id)}/${encodeURIComponent(first.id)}`);
              }}
            >
              {allVehicles.map((v) => (
                <option key={v.id} value={v.id}>
                  {v.id}
                </option>
              ))}
            </select>
          </label>
          <label className="dbc-field">
            <span className="dbc-label">DBC file</span>
            <select
              className="dbc-select"
              value={network.id}
              onChange={(e) => navigate(`/${vehicle.id}/${e.target.value}`)}
            >
              {vehicle.networks.map((n) => (
                <option key={n.id} value={n.id}>
                  {n.filename}
                </option>
              ))}
            </select>
          </label>
          <label className="dbc-field dbc-field-grow">
            <span className="dbc-label">Search</span>
            <input
              type="search"
              className="dbc-search"
              placeholder="IDs, ECUs, messages, signals, enums…"
              value={search}
              onChange={(e) => setSearch(e.target.value)}
              autoComplete="off"
            />
          </label>
          <label className="dbc-field dbc-field-sort">
            <span className="dbc-label">Sort messages</span>
            <select
              className="dbc-select"
              value={messageSort}
              onChange={(e) => setMessageSort(e.target.value as MessageSort)}
            >
              <option value="id-asc">ID · ascending</option>
              <option value="id-desc">ID · descending</option>
              <option value="name-asc">Name · A–Z</option>
              <option value="name-desc">Name · Z–A</option>
            </select>
          </label>
        </div>

        <div className="dbc-scope-row">
          <span className="dbc-label">Search in</span>
          <div className="dbc-scope-checks">
            {(
              [
                ["ids", "CAN IDs"],
                ["ecus", "Sender ECUs"],
                ["msgNames", "Messages & comments"],
                ["sigNames", "Signals & enums"],
              ] as const
            ).map(([key, label]) => (
              <label key={key} className="chk-label">
                <input
                  type="checkbox"
                  className="chk"
                  checked={searchIn[key]}
                  onChange={() => toggleSearchField(key)}
                />
                <span>{label}</span>
              </label>
            ))}
          </div>
          <div className="dbc-scope-actions">
            <button type="button" className="btn-text" onClick={selectAllSearchFields}>
              All
            </button>
            <button type="button" className="btn-text" onClick={deselectAllSearchFields}>
              None
            </button>
          </div>
        </div>

        <div className="dbc-ecu-bar">
          <span className="dbc-label">ECUs</span>
          <span className="dbc-ecu-hint">nodes ∪ senders</span>
          <div className="dbc-ecu-actions">
            <button type="button" className="btn-text" onClick={selectAllEcus}>
              All
            </button>
            <button type="button" className="btn-text" onClick={deselectAllEcus}>
              None
            </button>
          </div>
          <div className="dbc-ecu-chips">
            {ecuOptions.map((id) => (
              <button
                key={id}
                type="button"
                className={`ecu-chip${selectedEcus.has(id) ? " ecu-chip-on" : ""}`}
                onClick={() => toggleEcu(id)}
              >
                {id === ECU_NONE ? "— no sender" : id}
              </button>
            ))}
          </div>
        </div>

        <div className="dbc-id-format-row">
          <span className="dbc-label">CAN ID</span>
          <div
            className={`id-format-segmented${idSearchFormat === "decimal" ? " id-format-segmented--decimal" : ""}`}
            role="group"
            aria-label="CAN ID display and search format"
          >
            <button
              type="button"
              className={`id-format-btn${idSearchFormat === "hex" ? " id-format-btn--active" : ""}`}
              onClick={() => setIdSearchFormat("hex")}
              aria-pressed={idSearchFormat === "hex"}
            >
              Hex
            </button>
            <button
              type="button"
              className={`id-format-btn${idSearchFormat === "decimal" ? " id-format-btn--active" : ""}`}
              onClick={() => setIdSearchFormat("decimal")}
              aria-pressed={idSearchFormat === "decimal"}
            >
              Decimal
            </button>
          </div>
        </div>

        <div className="breadcrumb compact">
          <Link to="/">Vehicles</Link>
          <span className="bc-sep">/</span>
          <Link to={`/${vehicle.id}`}>{vehicle.id}</Link>
          <span className="bc-sep">/</span>
          <span className="bc-current">{network.id}</span>
        </div>
      </div>

      <div className="dbc-scroll">
        <details className="vt-details messages-details" open>
          <summary>
            Messages ({sortedMessages.length})
          </summary>
          <div className="messages-details-body">
            {sortedMessages.length === 0 ? (
              <p className="empty-state">No messages match ECU and search filters.</p>
            ) : (
              sortedMessages.map((m) => (
                <MessageCard key={`${m.frameId}-${m.name}`} msg={m} idFormat={idSearchFormat} />
              ))
            )}
          </div>
        </details>

        {network.valueTables.length > 0 && (
          <details className="vt-details">
            <summary>Global value tables ({network.valueTables.length})</summary>
            <div className="vt-list">
              {network.valueTables.map((vt) => (
                <details key={vt.name} className="vt-item">
                  <summary className="vt-name">{vt.name}</summary>
                  <div className="table-wrap">
                    <table className="data data-compact">
                      <thead>
                        <tr>
                          <th>Value</th>
                          <th>Description</th>
                        </tr>
                      </thead>
                      <tbody>
                        {vt.entries.map((e) => (
                          <tr key={e.value}>
                            <td className="mono">{e.value}</td>
                            <td>{e.label}</td>
                          </tr>
                        ))}
                      </tbody>
                    </table>
                  </div>
                </details>
              ))}
            </div>
          </details>
        )}

        {Object.keys(network.dbcAttributes).length > 0 && (
          <details className="vt-details">
            <summary>DBC file attributes</summary>
            <dl className="attr-dl">
              {Object.entries(network.dbcAttributes).map(([k, v]) => (
                <div key={k} className="attr-row">
                  <dt>{k}</dt>
                  <dd>{String(v)}</dd>
                </div>
              ))}
            </dl>
          </details>
        )}

        {network.nodes.length > 0 && (
          <details className="vt-details">
            <summary>Network nodes ({network.nodes.length})</summary>
            <p className="nodes-line">{network.nodes.map((n) => n.name).join(", ")}</p>
          </details>
        )}
      </div>
    </div>
  );
}
