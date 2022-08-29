"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
Object.defineProperty(exports, "__esModule", { value: true });
const israeli_bank_scrapers_1 = require("israeli-bank-scrapers");
function scrapeAccount(credentials, company) {
    return __awaiter(this, void 0, void 0, function* () {
        function getFirstDayOfMonth(year, month) {
            return new Date(year, month, 1);
        }
        try {
            var date = new Date(); // TODO: where is this from? I dont think this scrapes a full month as inteded
            date = getFirstDayOfMonth(date.getFullYear(), date.getMonth());
            const options = {
                companyId: company,
                verbose: true,
                startDate: date,
                combineInstallments: false,
                showBrowser: true
            };
            const scraper = (0, israeli_bank_scrapers_1.createScraper)(options);
            const scrapeResult = yield scraper.scrape(credentials);
            var totalAmount = 0;
            if (scrapeResult.success) {
                console.log(JSON.stringify(scrapeResult.accounts));
                scrapeResult.accounts.forEach((account) => {
                    console.log(`found ${account.txns.length} transactions for account number ${account.accountNumber}`);
                    account.txns.forEach((txn) => {
                        totalAmount += txn.originalAmount;
                        console.log(`Transaction ${txn.description}, with amount: ${txn.originalAmount} is ${txn.status}`);
                    });
                });
            }
            else {
                throw new Error(scrapeResult.errorType);
            }
            return [scrapeResult, totalAmount];
        }
        catch (e) {
            console.error(`scraping failed for the following reason: ${e.message}`);
        }
    });
}
exports.default = scrapeAccount;
