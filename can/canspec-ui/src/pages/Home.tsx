import { Link } from "react-router-dom";
import { useLayoutOutlet } from "../layoutContext";
import { matchesQuery } from "../search";

export function Home() {
  const { data, q } = useLayoutOutlet();

  const vehicles = data.vehicles.filter((v) => {
    if (!q.trim()) return true;
    const nets = v.networks.map((n) => `${n.id} ${n.filename}`).join(" ");
    return matchesQuery(q, v.id, nets);
  });

  return (
    <div>
      <h1 className="page-title">Vehicles</h1>
      <p className="page-sub">Choose a vehicle; its CAN networks are listed along the bottom of each card.</p>
      <div className="grid-cards">
        {vehicles.map((v) => {
          const networks = v.networks.filter((n) =>
            matchesQuery(q, n.id, n.filename, String(n.messages.length)),
          );
          return (
            <article key={v.id} className="vehicle-card">
              <Link
                className="vehicle-card-hit"
                to={`/${v.id}`}
                aria-label={`${v.id} — open vehicle page to choose a network`}
              />
              <div className="vehicle-card-head">
                <div className="card-label">Vehicle</div>
                <span className="vehicle-card-title">{v.id}</span>
                <div className="vehicle-card-meta">
                  {v.networks.length} network{v.networks.length === 1 ? "" : "s"}
                </div>
              </div>
              <div className="vehicle-card-nets" aria-label={`Networks for ${v.id}`}>
                {networks.length === 0 ? (
                  <p className="vehicle-card-empty">No networks match your filter.</p>
                ) : (
                  networks.map((n) => (
                    <Link
                      key={n.id}
                      className="ecu-chip vehicle-net-chip"
                      to={`/${v.id}/${n.id}`}
                      title={`${n.filename} · ${n.messages.length} message${n.messages.length === 1 ? "" : "s"}`}
                    >
                      {n.id}
                    </Link>
                  ))
                )}
              </div>
            </article>
          );
        })}
      </div>
      {vehicles.length === 0 && (
        <div className="empty-state">No vehicles match your filter.</div>
      )}
    </div>
  );
}
