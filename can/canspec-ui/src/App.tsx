import { Route, Routes } from "react-router-dom";
import { Layout } from "./Layout";
import { Home } from "./pages/Home";
import { VehiclePage } from "./pages/VehiclePage";
import { NetworkPage } from "./pages/NetworkPage";

export default function App() {
  return (
    <Routes>
      <Route element={<Layout />}>
        <Route index element={<Home />} />
        <Route path=":vehicleId" element={<VehiclePage />} />
        <Route path=":vehicleId/:networkId" element={<NetworkPage />} />
      </Route>
    </Routes>
  );
}
