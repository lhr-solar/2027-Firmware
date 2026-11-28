"""Serialize passive topology YAML for the static viewer; never execute branch code."""

import yaml


class TopologyLoader(yaml.SafeLoader):
    def construct_mapping(self, node, deep=False):
        keys = [self.construct_object(key, deep=deep) for key, _ in node.value]
        if len(keys) != len(set(keys)):
            raise ValueError("Duplicate topology YAML key")
        return super().construct_mapping(node, deep=deep)


def topology_payload(content: bytes | None):
    """Optional YAML bytes -> versioned graph data; absent topology stays absent."""
    if content is None:
        return None
    topology = yaml.load(content.decode("utf-8"), Loader=TopologyLoader)
    if not isinstance(topology, dict) or topology.get("version") != 1:
        raise ValueError("Invalid topology version")
    buses, outputs = topology.get("buses"), topology.get("outputs")
    if not isinstance(buses, dict) or not isinstance(outputs, dict):
        raise TypeError("Topology requires buses and outputs mappings")
    endpoints = set(buses) | set(outputs)
    if set(buses) & set(outputs):
        raise ValueError("Topology bus/output names overlap")
    for name, spec in buses.items():
        if (
            not isinstance(spec, dict)
            or spec.get("format") not in ("classic", "can_fd")
            or type(spec.get("bitrate")) is not int
            or spec["bitrate"] <= 0
        ):
            raise ValueError(f"{name}: invalid topology bus metadata")
        if not isinstance(spec.get("forwards", []), list):
            raise TypeError(f"{name}: forwards must be a list")
        for route in spec.get("forwards", []):
            if (
                not isinstance(route, dict)
                or route.get("to") not in endpoints
                or not isinstance(route.get("via"), str)
            ):
                raise ValueError(f"{name}: invalid topology route")
            rule = route.get("filter", {})
            if not isinstance(rule, dict) or rule.get("mode", "exclude") not in (
                "include",
                "exclude",
            ):
                raise ValueError(f"{name}: invalid filter mode")
            for selector in ("messages", "transmitters", "networks"):
                if not isinstance(rule.get(selector, []), list) or any(
                    not isinstance(value, str) for value in rule.get(selector, [])
                ):
                    raise ValueError(f"{name}: {selector} selectors must be strings")
    for name, spec in outputs.items():
        if not isinstance(spec, dict) or not isinstance(spec.get("transport"), str):
            raise TypeError(f"{name}: invalid output metadata")
    return topology
