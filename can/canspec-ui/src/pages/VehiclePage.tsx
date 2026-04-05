import { Link, useParams } from "react-router-dom";
import { findVehicle } from "../data";
import { useLayoutOutlet } from "../layoutContext";
import { matchesQuery } from "../search";

export function VehiclePage() {
  const { vehicleId } = useParams();
  const { data, q } = useLayoutOutlet();

  if (!vehicleId) {
    return <p className="err">Missing vehicle.</p>;
  }

  const vehicle = findVehicle(data, vehicleId);
  if (!vehicle) {
    return <p className="err">Unknown vehicle: {vehicleId}</p>;
  }

  const networks = vehicle.networks.filter((n) =>
    matchesQuery(q, n.id, n.filename, String(n.messages.length)),
  );

  return (
    <div>
      <h1 className="page-title">{vehicle.id}</h1>
      <p className="page-sub">CAN networks defined for this vehicle.</p>
      <div className="grid-cards">
        {networks.map((n) => (
          <Link key={n.id} className="card" to={`/${encodeURIComponent(vehicle.id)}/${encodeURIComponent(n.id)}`}>
            <div className="card-label">Network</div>
            <div className="card-title">{n.id}</div>
            <div style={{ marginTop: "0.35rem", fontSize: "0.8rem", color: "var(--text-muted)" }}>
              {n.filename}
            </div>
            <div style={{ marginTop: "0.5rem", fontSize: "0.85rem", color: "var(--text-muted)" }}>
              {n.messages.length} message{n.messages.length === 1 ? "" : "s"}
            </div>
          </Link>
        ))}
      </div>
      {networks.length === 0 && (
        <div className="empty-state">No networks match your filter.</div>
      )}
    </div>
  );
}
