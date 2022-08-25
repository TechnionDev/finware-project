import { CompanyTypes, createScraper } from 'israeli-bank-scrapers';

import readline from 'readline';
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

export default async function (callback) {
     rl.question("Username: ", function (username) {
         rl.question("Password: ", async function (password) {
            function getFirstDayOfMonth(year, month) {
                return new Date(year, month, 1);
            }

            try {
                var date = new Date();
                date = getFirstDayOfMonth(
                    date.getFullYear(),
                    date.getMonth(),
                );
                // read documentation below for available options
                const options = {
                    companyId: CompanyTypes.max,
                    verbose: true,
                    startDate: date,
                    combineInstallments: false,
                    showBrowser: false
                };

                // read documentation below for information about credentials
                const credentials = {
                    username: username,
                    password: password
                };

                const scraper = createScraper(options);
                const scrapeResult = await scraper.scrape(credentials);
                var totalAmount = 0;
                if (scrapeResult.success) {
                    console.log(JSON.stringify(scrapeResult.accounts));
                    scrapeResult.accounts.forEach((account) => {
                        console.log(`found ${account.txns.length} transactions for account number ${account.accountNumber}`);
                        account.txns.forEach((txn) => {
                            totalAmount += txn.originalAmount;
                            console.log(`Transaction ${txn.description}, with amount: ${txn.originalAmount} is ${txn.status}`);
                        })
                    });
                    callback(scrapeResult, totalAmount);
                } else {
                    throw new Error(scrapeResult.errorType);
                }
            } catch (e: any) {
                console.error(`scraping failed for the following reason: ${e.message}`);
            }
            rl.close();
        });
    });
}