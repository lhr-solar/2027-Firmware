import assert from 'node:assert/strict';
import { test } from 'node:test';
import { calculateLoading, frameBits, loadPercent, tcpRate } from '../src/loading.ts';

const message = (frameId, extra = {}) => ({name:`M${frameId}`, frameId, hexId:`0x${frameId.toString(16)}`, isExtended:false, isFd:false, dlc:8, senders:['Device'], cycleTimeMs:100, sendType:'Cyclic', attributes:{}, ...extra});
function fixture() {
  return {
    vehicles:[{id:'car', networks:[
      {filename:'Elcon.dbc', messages:[message(1)]},
      {filename:'Charger.dbc', messages:[message(2)]},
      {filename:'Car.dbc', messages:[message(3)]},
      {filename:'DAQ.dbc', messages:[message(4), message(5)]},
    ]}],
    topology:{version:1, buses:{
      Elcon:{dbc:'dbc/Elcon.dbc',format:'classic',bitrate:250000,forwards:[{to:'Charger'}]},
      Charger:{dbc:'dbc/Charger.dbc',format:'classic',bitrate:250000,forwards:[{to:'Car', filter:{messages:['0x3']}}]},
      Car:{dbc:'dbc/Car.dbc',format:'can_fd',bitrate:1000000,forwards:[{to:'Charger',filter:{mode:'include',messages:['0x3']}},{to:'TCP'}]},
      DAQ:{dbc:'dbc/DAQ.dbc',format:'can_fd',bitrate:1000000,forwards:[{to:'TCP',filter:{messages:['0x4']}}]},
    },outputs:{TCP:{transport:'tcp'}}}
  };
}
test('multi-hop forwarding, reverse exclusions and filtered DAQ reach TCP', () => {
  const result = calculateLoading(fixture());
  assert.deepEqual([...result.resolved.get('Car').values()].map(r=>r.message.frameId).sort(),[1,2,3]);
  assert.deepEqual([...result.resolved.get('Charger').values()].map(r=>r.message.frameId).sort(),[1,2,3]);
  assert.deepEqual([...result.resolved.get('TCP').values()].map(r=>r.message.frameId).sort(),[1,2,3,5]);
});
test('mirrors deduplicate, transmitter exclusions block pass-through, empty include blocks all', () => {
  const data = fixture();
  data.vehicles[0].networks[1].messages.push(message(3));
  let result = calculateLoading(data);
  assert.equal(result.resolved.get('TCP').size,4);
  data.topology.buses.Elcon.forwards[0].filter={transmitters:['Device']};
  data.topology.buses.DAQ.forwards[0].filter={mode:'include',messages:[]};
  result = calculateLoading(data);
  assert.deepEqual([...result.resolved.get('TCP').values()].map(r=>r.message.frameId).sort(),[2,3]);
});
test('actual per-message loops, conflicting IDs and unsupported BRS fail visibly', () => {
  let data = fixture();
  data.topology.buses.Charger.forwards[0].filter={messages:[]};
  assert.throws(()=>calculateLoading(data),/forwarding loop/);
  data=fixture();data.vehicles[0].networks[1].messages.push(message(1,{dlc:4}));
  assert.throws(()=>calculateLoading(data),/conflicting forwarded ID/);
  data=fixture();data.topology.buses.Car.brs=true;
  assert.throws(()=>calculateLoading(data),/BRS/);
});
test('pending and triggered traffic excluded; known wire/TCP calculations', () => {
  const data = fixture();
  data.vehicles[0].networks[0].messages.push(message(6,{sendType:'Triggered',cycleTimeMs:null}),message(7,{attributes:{LayoutPending:1}}));
  const result=calculateLoading(data);
  assert.equal(result.omitted.length,2);
  assert.deepEqual(frameBits(message(1)),[111,135]);
  assert.deepEqual(frameBits(message(1,{isFd:true,dlc:64})),[579,713]);
  const rows=[{source:'Elcon',message:message(1),fps:10}];
  assert.ok(Math.abs(loadPercent(rows,125000)[0] - 0.888) < 1e-10);
  assert.equal(loadPercent(rows,125000)[1],1.08);
  assert.deepEqual(tcpRate(rows),{payload:0.64,application:3.04,uplink:6.24,ack:1.6});
});
