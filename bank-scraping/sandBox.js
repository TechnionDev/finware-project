import {CompanyTypes, createScraper} from 'israeli-bank-scrapers';

function getFirstDayOfMonth(year, month) {
    return new Date(year, month, 1);
}

(async function () {
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
            showBrowser: true
        };

        // read documentation below for information about credentials
        const credentials = {
            username: '',
            password: ''
        };

        const scraper = createScraper(options);
        const scrapeResult = await scraper.scrape(credentials);
        var totalAmount = 0;
        if (scrapeResult.success) {
            scrapeResult.accounts.forEach((account) => {
                console.log(`found ${account.txns.length} transactions for account number ${account.accountNumber}`);
                account.txns.forEach((txn)=> {
                    totalAmount += txn.originalAmount;
                    console.log(`Transaction ${txn.description}, with amount: ${txn.originalAmount} is ${txn.status}`);
                })

            });
        } else {
            throw new Error(scrapeResult.errorType);
        }
    } catch (e) {
        console.error(`scraping failed for the following reason: ${e.message}`);
    }
})();