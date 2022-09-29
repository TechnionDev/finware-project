class BluetoothController {
    private resolvePairingRequest;
    private rejectPairingRequest;
    private bleManager;
    private connection = "PAIRING";

    public totalAmount: number = 0;
    public gattInformation = { bankInfo: {}, refreshRate: 0, goal: 0, graphData: {}, sumDiff: 0};


    constructor() {
        setTimeout(()=>console.log("Gatt information: ", this.gattInformation), 15000);
    }

    fetchState(req, res) {
        res.json({ connection: this.connection, passcode: null });
    }

    reset(req, res) {
        setTimeout(()=>this.connection="ADVERTISING", 3500)
        res.json({ status: "SUCCESS" });
    }

    acceptPairing(req, res) {
        setTimeout(()=>this.connection="CONNECTED", 3500);
        res.json({ status: "SUCCESS" });
    }

    rejectPairing(req, res) {
        res.json({ status: "SUCCESS" });
    }

}

export default BluetoothController;