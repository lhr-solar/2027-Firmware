import { Route, Routes } from "react-router-dom";
import { Layout } from "./Layout";
import { Home } from "./pages/Home";
import { NetworkPage } from "./pages/NetworkPage";
import { SummaryPage } from "./pages/SummaryPage";

export default function App() {
  return (
    <Routes>
      <Route element={<Layout />}>
        <Route index element={<Home />} />
        <Route path="summary" element={<SummaryPage />} />
        <Route path="summary/:networkId" element={<SummaryPage />} />
        <Route path="n/:networkId" element={<NetworkPage />} />
      </Route>
    </Routes>
  );
}
