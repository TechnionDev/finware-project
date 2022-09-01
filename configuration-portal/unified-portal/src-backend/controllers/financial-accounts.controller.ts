import scrapeFinancialBE from "../util-managers/FinanceAccountsManager";
import FinancialBE from "../models/FinancialBE";
import { BluetoothController } from "../controllers";

class FinanceAccountsController {
    private bluetoothController: BluetoothController;

    constructor(bluetoothController) {
        this.bluetoothController = bluetoothController;
    }

    async fetchCreditCards(req, res) {
        const all = await FinancialBE.find({}).select({ "name": 1, "company": 1 });
        res.json(all);
    }

    async createFinancialAccount(req, res) {
        await FinancialBE.create({
            ...req.body
        });
        // TODO: update finances information?
        res.json({ status: "SUCCESS" });
    }

    async removeFinancialAccount(req, res) {
        await FinancialBE.findOneAndDelete({ _id: req.params.id });
        // TODO: update finances information?
        res.json({ status: "SUCCESS" });
    }

    async scrape(req, res, next) {
        try {
            const accounts = await FinancialBE.find({});
            let total = 0;
            for (const account of accounts) {
                console.log("Found account: ", account);
                const [accountScrapeResult, accountTotal]: [any, number] = await scrapeFinancialBE(account, account.company);
                total += accountTotal;
            }

            this.bluetoothController.gattInformation.bankInfo = total;

            console.log("Total amount: ", total);
            res.json({ amount: total });
        } catch (err) { next(err) };
    }
}

export default FinanceAccountsController;