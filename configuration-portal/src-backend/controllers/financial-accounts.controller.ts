import slugify from "slugify";

import scrapeFinancialBE from "../util-managers/FinanceAccountsManager";
import FinancialBE, { ValidationStatus } from "../models/FinancialBE";
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
            const all = await FinancialBE.find({}).select({ "name": 1, "company": 1, "last_scrape": 1, "validation_status": 1, "scrape_result": 1 });
            res.json(all);
        } catch (err) {
            console.log('Error while fetching credit cards: ', err);
            res.json({ status: "ERROR", message: err });
        }
    }

    async createFinancialAccount(req, res) {
        FinancialBE.create({
            ...req.body,
            validation_status: ValidationStatus.INPROGRESS
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

    async scrapeFinancialAccount(req, res) {
        console.log(`Force scraping financial account: ${req.params.id}`)
        this.updateFinancialData(true, req.params.id);
        res.json({ status: "SUCCESS" });
    }

    async updateFinancialData(forceUpdate = false, target_fbe_id = null) {
        try {
            let [settings, fbes] = await Promise.all([Settings.findOneAndUpdate({}, {}, { upsert: true, new: true }), FinancialBE.find({})]);
            const now = new Date();
            const cycleStartDate = getCycleStartDate(settings.month_cycle_start_day);
            console.log(`Scraping started for financial backends. Scraping from date: ${cycleStartDate.toDateString()}`);

            let scrapeJobs = [];
            for (let fbe of fbes) {
                if (target_fbe_id && target_fbe_id != fbe._id) {
                    console.log(`Specific fbe scrape requested (${target_fbe_id}), skipping fbe: ${fbe._id}`)
                    continue;
                }
/*                 // Check the difference in time (rounded up)
                let hours_since_last_scrape = (now.getTime() - fbe.last_scrape.getTime()) / 1000 / 60 / 60;
                if (!forceUpdate && hours_since_last_scrape < settings.scrape_frequency_hours) {
                    console.log('Account', fbe.name, `was updated ${hours_since_last_scrape} (<${settings.scrape_frequency_hours}) hours ago. Skipping.`);
                    continue;
                } */

                console.log("Scraping account:", fbe.name, ". Starting from:", cycleStartDate);
                fbe.last_scrape = now;
                fbe.validation_status = ValidationStatus.INPROGRESS;
                await fbe.save();

                scrapeJobs.push(
                    scrapeFinancialBE(fbe, fbe.company, cycleStartDate, slugify(fbe.name)).then(async (scrapeResult: ScaperScrapingResult) => {
                        console.log('Scraping done for', fbe.name);
                        // Update account
                        if (scrapeResult.success) {
                            fbe.validation_status = ValidationStatus.VALIDATED;
                            console.log('results: ', scrapeResult);
                        } else {
                            fbe.validation_status = ValidationStatus.FAILED;
                            console.log('failed with: ', scrapeResult);
                        }
                        fbe.scrape_result = scrapeResult;
                        await fbe.save();
                    })
                );
            }
            await Promise.all(scrapeJobs);

            fbes = await FinancialBE.find({});
            let bankInfo = {};
            let graphData = { data: new Array(getCycleDayCount(settings.month_cycle_start_day)).fill(0) };
            for (const fbe of fbes) {
                if (fbe.scrape_result.success == false) {
                    continue;
                }
                bankInfo[fbe.name] = fbe.scrape_result.accounts.reduce((companySum, account) => {
                    return companySum + account.txns.reduce((accountSum, txn) => {
                        return accountSum + txn.chargedAmount;
                    }, 0);
                }, 0);

                for (let account of fbe.scrape_result.accounts) {
                    for (let txn of account.txns) {
                        const txnCycleIndex = getDateIndexInCycle(cycleStartDate, new Date(txn.date));
                        graphData.data[txnCycleIndex] += -txn.chargedAmount;
                    }
                }
            }
            graphData.data.splice(getDateIndexInCycle(cycleStartDate, now) + 1);

            let newBankInfo = bankInfo;
            console.log("Settings bank info to: ", newBankInfo);
            console.log("Settings graphData to: ", graphData);
            this.bluetoothController.gattInformation.bankInfo = newBankInfo;
            this.bluetoothController.gattInformation.graphData = graphData;
            this.bluetoothController.gattInformation.sumDiff = Math.round(graphData.data.at(-2));

        } catch (err) {
            console.log('There was an error while scraping: ', err);
        };
    }
}

export default FinanceAccountsController;