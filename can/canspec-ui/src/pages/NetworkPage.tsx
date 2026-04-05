import { useMemo } from "react";
import { useParams } from "react-router-dom";
import { NetworkExplorer } from "../components/NetworkExplorer";
import { findNetwork, findVehicle } from "../data";
import { useLayoutOutlet } from "../layoutContext";

type Resolved =
  | { ok: true; vehicle: NonNullable<ReturnType<typeof findVehicle>>; network: NonNullable<ReturnType<typeof findNetwork>> }
  | { ok: false; error: string };

export function NetworkPage() {
  const { vehicleId, networkId } = useParams();
  const { data } = useLayoutOutlet();

  const resolved: Resolved = useMemo(() => {
    if (!vehicleId || !networkId) {
      return { ok: false, error: "Missing vehicle or network." };
    }
    const vehicle = findVehicle(data, vehicleId);
    if (!vehicle) {
      return { ok: false, error: `Unknown vehicle: ${vehicleId}` };
    }
    const network = findNetwork(vehicle, networkId);
    if (!network) {
      return { ok: false, error: `Unknown network: ${networkId}` };
    }
    return { ok: true, vehicle, network };
  }, [data, vehicleId, networkId]);

  if (!resolved.ok) {
    return <p className="err">{resolved.error}</p>;
  }

  return (
    <NetworkExplorer vehicle={resolved.vehicle} network={resolved.network} allVehicles={data.vehicles} />
  );
}
