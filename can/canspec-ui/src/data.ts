import type { CanspecPayload, Vehicle } from "./types";

let cache: CanspecPayload | null = null;

function jsonUrl(): string {
  const base = import.meta.env.BASE_URL;
  if (base === "./" || base === ".") {
    return "./canspec-data.json";
  }
  const withSlash = base.endsWith("/") ? base : `${base}/`;
  return `${withSlash}canspec-data.json`;
}

export async function loadCanspecData(): Promise<CanspecPayload> {
  if (cache) return cache;
  const res = await fetch(jsonUrl());
  if (!res.ok) {
    throw new Error(`Failed to load CAN data (${res.status})`);
  }
  cache = (await res.json()) as CanspecPayload;
  return cache;
}

export function findVehicle(data: CanspecPayload, vehicleId: string): Vehicle | undefined {
  return data.vehicles.find((v) => v.id === vehicleId);
}

export function findNetwork(vehicle: Vehicle, networkId: string) {
  return vehicle.networks.find((n) => n.id === networkId);
}
