import scrapeFinancialBE from "../util-managers/FinanceAccountsManager";
import FinancialBE from "../models/FinancialBE";
import { BluetoothController } from "../controllers";
import Settings from "../models/Settings";
import { ScaperScrapingResult } from "israeli-bank-scrapers/lib/scrapers/base-scraper";
import { getCycleStartDate, getCycleDayCount, getDateIndexInCycle } from "./utils";

class FinanceAccountsController {
    private bluetoothController: BluetoothController;

    constructor(bluetoothController) {
        this.bluetoothController = bluetoothController;
    }

    async fetchCreditCards(_req, res) {
        try {
            const all = await FinancialBE.find({}).select({ "name": 1, "company": 1 });
            res.json(all);
        } catch (err) {
            console.log('Error while fetching credit cards: ', err);
            res.json({ status: "ERROR", message: err });
        }
    }

    async createFinancialAccount(req, res) {
        FinancialBE.create({
            ...req.body,
        }).then((_result) => {
            this.updateFinancialData();
            res.json({ status: "SUCCESS" });
        }).catch((err) => {
            console.log('Error while adding a financialBE: ', err);
            res.json({ status: "ERROR", message: err });
        });
    }

    async removeFinancialAccount(req, res) {
        FinancialBE.findOneAndDelete({ _id: req.params.id }).then((_result) => {
            this.updateFinancialData();
            res.json({ status: "SUCCESS" });
        }).catch((err) => {
            console.log('Error while removing a financialBE: ', err);
            res.json({ status: "ERROR", message: err });
        });
    }

    async updateFinancialData(forceUpdate = false) {
        try {
            let [settings, fbes] = await Promise.all([Settings.findOneAndUpdate({}, {}, { upsert: true, new: true }), FinancialBE.find({})]);
            const now = new Date();
            const cycleStartDate = getCycleStartDate(settings.month_cycle_start_day);
            console.log(`Scraping started for financial backends. Scraping from date: ${cycleStartDate.toDateString()}`);

            let scrapeJobs = [];
            for (let fbe of fbes) {
                // Check the difference in time (rounded up)
                let hours_since_last_scrape = (now.getTime() - fbe.last_scrape.getTime()) / 1000 / 60 / 60;
                if (!forceUpdate && hours_since_last_scrape < settings.scrape_frequency_hours && fbe.scrape_result.success) {
                    console.log('Account ', fbe.name, ' was recently updated. Skipping.');
                    continue;
                }
                console.log("Scraping for account: ", fbe.name, "from start date: ", cycleStartDate);
                scrapeJobs.push(
                    scrapeFinancialBE(fbe, fbe.company, cycleStartDate).then((scrapeResult: ScaperScrapingResult) => {
                        console.log('Scraping done for', fbe.name);
                        // Update account
                        if (scrapeResult.success) {
                            fbe.last_scrape = now;
                            fbe.scrape_result = scrapeResult;
                            fbe.save();
                            console.log('results: ', scrapeResult);
                        } else {
                            console.log('failed with: ', scrapeResult);
                        }
                    })
                );
            }
            await Promise.all(scrapeJobs);
            let bankInfo = {};
            let graphData = { data: new Array(getCycleDayCount(settings.month_cycle_start_day)).fill(0) };
            for (const fbe of fbes) {
                if (fbe.scrape_result.success == false) {
                    continue;
                }
                bankInfo[fbe.company] = fbe.scrape_result.accounts.reduce((companySum, account) => {
                    return companySum + account.txns.reduce((accountSum, txn) => {
                        return accountSum + txn.originalAmount;
                    }, 0);
                }, 0);

                for (let account of fbe.scrape_result.accounts) {
                    for (let txn of account.txns) {
                        const txnCycleIndex = getDateIndexInCycle(cycleStartDate, new Date(txn.date));
                        graphData.data[txnCycleIndex] += -txn.chargedAmount;
                    }
                }
            }
            graphData.data = graphData.data.map((sum => value => sum += value)(0));
            graphData.data.splice(getDateIndexInCycle(cycleStartDate, now) + 1);
            console.log("Settings bank info to: ", bankInfo)
            console.log("Settings graphData to: ", graphData);
            this.bluetoothController.gattInformation.bankInfo = bankInfo;
            this.bluetoothController.gattInformation.graphData = graphData;
        } catch (err) {
            console.log('There was an error while scraping: ', err);
        };
    }
}

export default FinanceAccountsController;