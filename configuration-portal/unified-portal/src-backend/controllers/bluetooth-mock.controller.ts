class BluetoothController {
    private resolvePairingRequest;
    private rejectPairingRequest;
    private bleManager;

    public totalAmount: number = 0;
    public gattInformation = { bankInfo: {}, refreshRate: 0, goal: 0, cycleStartDate: 0 }


    constructor({ refreshRate, goal, cycleStartDate }) {
        this.gattInformation.refreshRate = refreshRate;
        this.gattInformation.goal = goal;
        this.gattInformation.cycleStartDate = cycleStartDate;
        setInterval(()=>console.log("Gatt information: ", this.gattInformation), 15000);
    }

    fetchState(req, res) {
        res.json({ connection: "ADVERTISING", passcode: null });
    }

    reset(req, res) {
        res.json({ status: "SUCCESS" });
    }

    acceptPairing(req, res) {
        res.json({ status: "SUCCESS" });
    }

    rejectPairing(req, res) {
        res.json({ status: "SUCCESS" });
    }

}

export default BluetoothController;