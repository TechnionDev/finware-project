class BluetoothController {
    private resolvePairingRequest;
    private rejectPairingRequest;
    private bleManager;

    public totalAmount: number = 0;
    public gattInformation = { 'bankInfo': 0 };


    constructor({ bankInfo, refreshRate, goal, daysLeft }) {

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