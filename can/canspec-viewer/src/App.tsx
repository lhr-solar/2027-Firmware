import { Route, Routes } from "react-router-dom";
import { LoadingPage } from "./pages/LoadingPage";
import { Layout } from "./Layout";
import { Home } from "./pages/Home";
import { NetworkPage } from "./pages/NetworkPage";
import { TopologyPage } from "./pages/TopologyPage";
import { SummaryPage } from "./pages/SummaryPage";

export default function App() {
  return (
    <Routes>
      <Route element={<Layout />}>
        <Route index element={<Home />} />
        <Route path="loading" element={<LoadingPage />} />
        <Route path="topology" element={<TopologyPage />} />
        <Route path="summary" element={<SummaryPage />} />
        <Route path="summary/:networkId" element={<SummaryPage />} />
        <Route path="n/:networkId" element={<NetworkPage />} />
      </Route>
    </Routes>
  );
}
