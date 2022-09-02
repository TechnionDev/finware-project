import scrapeFinancialBE from "../util-managers/FinanceAccountsManager";
import FinancialBE from "../models/FinancialBE";
import { BluetoothController } from "../controllers";
import Settings from "../models/Settings";

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

    async scrape() {
        console.log('Scraping started for financial backends');
        try {

            // 
            let [settings, accounts] = await Promise.all([Settings.findOne({}), FinancialBE.find({})]);
            console.log('User settings are: ', settings);
            let cycle_start_date = new Date();
            cycle_start_date.setDate(settings.month_cycle_start_day);
            const now = new Date();

            let total = 0;
            let scrapeJobs = [];
            let scrapeAccounts = [];
            for (let account of accounts) {
                // Check the difference in time (rounded up)
                let hours_sinse_last_scrape = Math.ceil((now.getTime() - account.last_scrape.getTime()) / 1000 / 60 / 60);
                if (hours_sinse_last_scrape < settings.scrape_frequency_hours) {
                    console.log('Account ', account.name, ' was recently scraped. Skipping.');
                    continue;
                }
                console.log("Found account: ", account);
                scrapeFinancialBE(account, account.company, cycle_start_date).then((result) => {
                    // Update account
                    let scrapeResult = result[0];
                    account.last_scrape = now;
                    account.scrape_result = scrapeResult;
                    account.save();
                });
                scrapeJobs.push();
                scrapeAccounts.push(account);
            }
            let results = await Promise.all(scrapeJobs);
            // for (let i = 0; i < scrapeAccounts.length; i++) {

            // }
            // let 

            // if (process.platform == 'linux') { TODO: Make alternative (query from the bluetooth listener the DB)
            //     this.bluetoothController.gattInformation.bankInfo = total;
            // }

            console.log("Total amount: ", results);
            // res.json({ amount: total }); // TODO: This is broken. Fix me!
        } catch (err) {
            console.log('There was an error while scraping: ', err);
        };
    }
}

export default FinanceAccountsController;