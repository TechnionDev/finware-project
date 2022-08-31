import scrapeAccount from "../util-managers/FinanceAccountsManager";
import FinancialBE from "../models/FinancialBE";
import { BluetoothController } from "../controllers";

class FinanceAccountsController {
    private bluetoothController;

    constructor(bluetoothController: BluetoothController) {
        this.bluetoothController = bluetoothController;
    }

    async fetchCreditCards(req, res) {
        const all = await FinancialBE.find({}).select({ "name": 1, "company": 1 });
        res.json(all);
    }

    async createFinancialAccount(req, res) {
        console.log(req.body);
        await FinancialBE.create({
            ...req.body
        });
        res.json({ status: "SUCCESS" });
    }

    async scrape(req, res, next) {
        const accounts = await FinancialBE.find({});
        let total = 0;
        for (const account of accounts) {
            console.log(account);
            const [accountScrapeResult, accountTotal]: [any, number] = await scrapeAccount(account, account.company);
            total += accountTotal;
        }
        this.bluetoothController.totalAmount = total;
        console.log("Total amount: ", total);
        res.json({ amount: total });
    }
}

export default FinanceAccountsController;