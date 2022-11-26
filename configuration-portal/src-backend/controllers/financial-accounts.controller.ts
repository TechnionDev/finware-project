import slugify from "slugify";

import scrapeFinancialBE from "../util-managers/FinanceAccountsManager";
import FinancialBE, { ValidationStatus } from "../models/FinancialBE";
import { BluetoothController } from "../controllers";
import Settings from "../models/Settings";
import { ScaperScrapingResult } from "israeli-bank-scrapers/lib/scrapers/base-scraper";
import { getCycleStartDate, getCycleDayCount, getDateIndexInCycle, emailOverBudgetNotification } from "./utils";


const SCRAPING_FREQUENCY_MAXIMUM = 24;

class FinanceAccountsController {
    private bluetoothController: BluetoothController;

    private getAmountCharged(txn) {
        let realAmount = txn.chargedAmount;
        if (txn.status === "pending" && txn.originalCurrency === "ILS") {
            let installments = txn.installments?.total || 1;
            realAmount = txn.originalAmount / installments;
        }
        return realAmount;
    }

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

    async updateBluetoothFromDB() {
        let settings = await Settings.findOneAndUpdate({}, {}, { upsert: true, new: true });
        const cycleStartDate = getCycleStartDate(settings.month_cycle_start_day);
        const now = new Date();
        let fbes = await FinancialBE.find({});
        let bankInfo = {};
        let graphData = { data: new Array(getCycleDayCount(settings.month_cycle_start_day)).fill(0) };
        for (const fbe of fbes) {
            if (fbe.scrape_result.success == false) {
                continue;
            }
            bankInfo[fbe.name] = fbe.scrape_result.accounts.reduce((companySum, account) => {
                return companySum + account.txns.reduce((accountSum, txn) => {
                    return accountSum + this.getAmountCharged(txn);
                }, 0);
            }, 0);

            for (let account of fbe.scrape_result.accounts) {
                for (let txn of account.txns) {
                    const txnCycleIndex = getDateIndexInCycle(cycleStartDate, new Date(txn.date));
                    graphData.data[txnCycleIndex] += -this.getAmountCharged(txn);
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
    }

    async updateFinancialData(forceUpdate = false, target_fbe_id = null) {
        try {
            let [settings, financialBackends] = await Promise.all([Settings.findOneAndUpdate({}, {}, { upsert: true, new: true }), FinancialBE.find({})]);
            const now = new Date();
            const cycleStartDate = getCycleStartDate(settings.month_cycle_start_day);
            console.log(`Scraping started for financial backends. Scraping from date: ${cycleStartDate.toTimeString()}. Current time: ${now.toISOString()}`);

            let scrapeJobsQueue = [];
            for (let financialBE of financialBackends) {
                if (target_fbe_id && target_fbe_id != financialBE._id) {
                    console.log(`Specific fbe scrape requested (${target_fbe_id}), skipping fbe: ${financialBE._id}`)
                    continue;
                }
                // Check the difference in time (rounded up)
                let hours_since_last_scrape = (now.getTime() - financialBE.last_scrape.getTime()) / 1000 / 60 / 60;
                if (!forceUpdate && !financialBE.scrape_result?.success == false && hours_since_last_scrape < SCRAPING_FREQUENCY_MAXIMUM) {
                    console.log('Account', financialBE.name, `was updated ${Math.round(hours_since_last_scrape)} (<${SCRAPING_FREQUENCY_MAXIMUM}) hours ago. Skipping.`);
                    continue;
                }

                console.log("Scraping account:", financialBE.name, ". Starting from:", cycleStartDate);
                financialBE.last_scrape = now;
                financialBE.validation_status = ValidationStatus.QUEUED;
                await financialBE.save();

                scrapeJobsQueue.push(() => {
                    // Update validation_status to in progress (we don't need to wait for the success)
                    FinancialBE.findOneAndUpdate({ _id: financialBE._id }, { validation_status: ValidationStatus.INPROGRESS }).catch((err) => {
                        console.log('Error while updating financialBE validation_status: ', err);
                    });
                    return scrapeFinancialBE(financialBE, financialBE.company, cycleStartDate, slugify(financialBE.name)).then(async (scrapeResult: ScaperScrapingResult) => {
                        console.log('Scraping done for', financialBE.name);
                        // Update account
                        if (scrapeResult.success) {
                            console.log('successfully scraped with results: ', scrapeResult);
                            await FinancialBE.findOneAndUpdate({ _id: financialBE._id }, { validation_status: ValidationStatus.VALIDATED, scrape_result: scrapeResult });
                        } else {
                            console.log('failed with: ', scrapeResult);
                            await FinancialBE.findOneAndUpdate({ _id: financialBE._id }, { validation_status: ValidationStatus.FAILED, scrape_result: scrapeResult });
                        }
                    })
                });
            }
            // Await scrape jobs in queue
            for (let job of scrapeJobsQueue) {
                await job();
            }
            // await Promise.all(scrapeJobsQueue); // Commented out due to serializing the scrape jobs (was too heavy on reasources on the poor RPi)

            await this.updateBluetoothFromDB();
            // Send email notification if needed
            let spendingSum = /*  positive */ -Object.values(this.bluetoothController.gattInformation.bankInfo).reduce((sum, val) => { return sum + val; });
            spendingSum += settings.base_spent;
            if (spendingSum > settings.expense_budget) {
                await emailOverBudgetNotification(settings, spendingSum);
            }
        } catch (err) {
            console.log('There was an error while scraping: ', err);
        };

    }
}

export default FinanceAccountsController;