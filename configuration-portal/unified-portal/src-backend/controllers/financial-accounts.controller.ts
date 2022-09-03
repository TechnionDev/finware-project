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
            this.updateFinancialData(true);
            res.json({ status: "SUCCESS" });
        }).catch((err) => {
            console.log('Error while adding a financialBE: ', err);
            res.json({ status: "ERROR", message: err });
        });
    }

    async removeFinancialAccount(req, res) {
        FinancialBE.findOneAndDelete({ _id: req.params.id }).then((_result) => {
            this.updateFinancialData(true);
            res.json({ status: "SUCCESS" });
        }).catch((err) => {
            console.log('Error while removing a financialBE: ', err);
            res.json({ status: "ERROR", message: err });
        });
    }

    async updateFinancialData(forceUpdate=false) {
        console.log('Scraping started for financial backends');
        try {
            let [settings, fbes] = await Promise.all([Settings.findOne({}), FinancialBE.find({})]);
            let cycle_start_date = new Date();
            cycle_start_date.setDate(settings.month_cycle_start_day);
            cycle_start_date.setMonth(cycle_start_date.getMonth() - 1);
            const now = new Date();

            let scrapeJobs = [];
            for (let fbe of fbes) {
                // Check the difference in time (rounded up)
                let hours_since_last_scrape = (now.getTime() - fbe.last_scrape.getTime()) / 1000 / 60 / 60;
                if (!forceUpdate && hours_since_last_scrape < settings.scrape_frequency_hours) {
                    console.log('Account ', fbe.name, ' was recently updated. Skipping.');
                    continue;
                }
                console.log("Scraping for account: ", fbe.name, "from start date: ", cycle_start_date);
                scrapeJobs.push(
                    scrapeFinancialBE(fbe, fbe.company, cycle_start_date).then((scrapeResult: ScaperScrapingResult) => {
                        // Update account
                        fbe.last_scrape = now;
                        fbe.scrape_result = scrapeResult;
                        fbe.save();
                        console.log('Scrape results for ', fbe.name, ': ', scrapeResult);
                    })
                );
            }
            await Promise.all(scrapeJobs);
            let bankInfo = {};
            for (const fbe of fbes) {
                bankInfo[fbe.company] = fbe.scrape_result.accounts.reduce((companySum, account) => {
                    return companySum + account.txns.reduce((accountSum, txn) => {
                        return accountSum + txn.originalAmount;
                    }, 0);
                }, 0);
            }
            console.log("Settings bank info to: ", bankInfo)
            this.bluetoothController.gattInformation.bankInfo = bankInfo;
        } catch (err) {
            console.log('There was an error while scraping: ', err);
        };
    }
}

export default FinanceAccountsController;