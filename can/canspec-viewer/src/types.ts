export interface ChoiceEntry {
  value: number;
  label: string;
}

export interface SignalConversion {
  kind: string;
  scale?: number;
  offset?: number;
  isFloat?: boolean;
  choices?: ChoiceEntry[];
}

export interface CanSignal {
  name: string;
  message: string;
  startBit: number;
  lengthBits: number;
  byteOrder: string;
  isSigned: boolean;
  isFloat: boolean;
  dataType: string;
  conversion: SignalConversion;
  choices: ChoiceEntry[];
  scale: number;
  offset: number;
  minimum: number | null;
  maximum: number | null;
  unit: string;
  comment: string;
  isMultiplexer: boolean;
  multiplexerSignal: string | null;
  multiplexerIds: number[] | null;
  spn: number | null;
  initial: number | null;
  invalid: number | null;
  rawInitial: number | null;
  rawInvalid: number | null;
}

export interface CanMessage {
  name: string;
  frameId: number;
  hexId: string;
  isExtended: boolean;
  dlc: number;
  senders: string[];
  cycleTimeMs: number | null;
  sendType?: string | null;
  isFd: boolean;
  protocol: string | null;
  busName: string;
  isMultiplexed: boolean;
  attributes: Record<string, string | number | boolean | null>;
  comment: string;
  signals: CanSignal[];
}

export interface ValueTable {
  name: string;
  entries: ChoiceEntry[];
}

export interface CanNetwork {
  id: string;
  filename: string;
  nodes: { name: string }[];
  dbcAttributes: Record<string, string | number | boolean | null>;
  valueTables: ValueTable[];
  messages: CanMessage[];
}

export interface Vehicle {
  id: string;
  networks: CanNetwork[];
}

export interface CanspecPayload {
  vehicles: Vehicle[];
  topology?: CanTopology | null;
}

export type SearchFields = {
  ids: boolean;
  ecus: boolean;
  msgNames: boolean;
  sigNames: boolean;
};

export interface RouteFilter {
  mode?: "include" | "exclude";
  messages?: string[];
  transmitters?: string[];
  networks?: string[];
}
export interface ForwardRoute {
  to: string;
  via: string;
  filter?: RouteFilter;
}
export interface TopologyBus {
  dbc: string;
  format: "can_fd" | "classic";
  bitrate: number;
  brs?: boolean;
  data_bitrate?: number;
  forwards?: ForwardRoute[];
}
export interface CanTopology {
  version: number;
  buses: Record<string, TopologyBus>;
  outputs: Record<string, { transport: string }>;
}
