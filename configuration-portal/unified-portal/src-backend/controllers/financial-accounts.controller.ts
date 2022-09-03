import scrapeFinancialBE from "../util-managers/FinanceAccountsManager";
import FinancialBE from "../models/FinancialBE";
import { BluetoothController } from "../controllers";
import Settings from "../models/Settings";
import { ScaperScrapingResult } from "israeli-bank-scrapers/lib/scrapers/base-scraper";

class FinanceAccountsController {
    private bluetoothController: BluetoothController;

    constructor(bluetoothController) {
        this.bluetoothController = bluetoothController;
    }

    async fetchCreditCards(_req, res) {
        const all = await FinancialBE.find({}).select({ "name": 1, "company": 1 });
        res.json(all);
    }

    async createFinancialAccount(req, res) {
        await FinancialBE.create({
            ...req.body,
        }).then((_result) => {
            res.json({ status: "SUCCESS" });
        }).catch((err) => {
            console.log('Error while adding a financialBE: ', err);
            res.json({ status: "ERROR", message: err });
        });
    }

    async removeFinancialAccount(req, res) {
        FinancialBE.findOneAndDelete({ _id: req.params.id }).then((_result) => {
            res.json({ status: "SUCCESS" });
        }).catch((err) => {
            console.log('Error while removing a financialBE: ', err);
            res.json({ status: "ERROR", message: err });
        });
    }

    async updateFinancialData() {
        console.log('Scraping started for financial backends');
        try {
            let [settings, accounts] = await Promise.all([Settings.findOne({}), FinancialBE.find({})]);
            let cycle_start_date = new Date();
            cycle_start_date.setDate(settings.month_cycle_start_day);
            const now = new Date();

            let scrapeJobs = [];
            let scrapeAccounts = [];
            for (let account of accounts) {
                // Check the difference in time (rounded up)
                let hours_sinse_last_scrape = (now.getTime() - account.last_scrape.getTime()) / 1000 / 60 / 60;
                if (hours_sinse_last_scrape < settings.scrape_frequency_hours) {
                    console.log('Account ', account.name, ' was recently updated. Skipping.');
                    continue;
                }
                console.log("Scraping for account: ", account.name);
                scrapeFinancialBE(account, account.company, cycle_start_date).then((scrapeResult: ScaperScrapingResult) => {
                    // Update account
                    account.last_scrape = now;
                    account.scrape_result = scrapeResult;
                    account.save();
                    console.log('Scrape results for ', account.name, ': ', scrapeResult);
                });
                scrapeJobs.push();
                scrapeAccounts.push(account);
            }
            await Promise.all(scrapeJobs);
        } catch (err) {
            console.log('There was an error while scraping: ', err);
        };
    }
}

export default FinanceAccountsController;