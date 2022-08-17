
// Include node-ble
const ble = require('node-ble');

// Listen for connections
ble.on('connect', function(peripheral) {
  console.log('Connected to ' + peripheral.uuid);
  peripheral.discoverServices();
});

// Listen for services


