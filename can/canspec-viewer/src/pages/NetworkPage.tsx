import { useMemo } from "react";
import { useParams } from "react-router-dom";
import { NetworkExplorer } from "../components/NetworkExplorer";
import { allNetworks, findNetworkById } from "../data";
import { useLayoutOutlet } from "../layoutContext";

export function NetworkPage() {
  const { networkId } = useParams();
  const { data } = useLayoutOutlet();
  const networks = allNetworks(data);

  const network = useMemo(() => {
    if (!networkId) return undefined;
    return findNetworkById(data, networkId);
  }, [data, networkId]);

  if (!networkId) {
    return <p className="err">Missing network.</p>;
  }
  if (!network) {
    return <p className="err">Unknown network: {networkId}</p>;
  }

  return <NetworkExplorer network={network} networks={networks} />;
}
