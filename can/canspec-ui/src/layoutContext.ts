import { useOutletContext } from "react-router-dom";
import type { CanspecPayload } from "./types";

export type LayoutOutletContext = {
  data: CanspecPayload;
  q: string;
  path: string;
};

export function useLayoutOutlet() {
  return useOutletContext<LayoutOutletContext>();
}
