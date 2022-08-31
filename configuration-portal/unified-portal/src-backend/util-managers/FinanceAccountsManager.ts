import { CompanyTypes, createScraper } from 'israeli-bank-scrapers';

export default async function scrapeAccount(credentials, company): Promise<[any, number]> {
    function getFirstDayOfMonth(year, month) {
        return new Date(year, month, 1);
    }

    try {
        var date = new Date(); // TODO: where is this from? I dont think this scrapes a full month as inteded
        date = getFirstDayOfMonth(
            date.getFullYear(),
            date.getMonth(),
        );

        const options = {
            companyId: company as CompanyTypes,
            verbose: true,
            startDate: date,
            combineInstallments: false,
            showBrowser: false
        };

        const scraper = createScraper(options);
        const scrapeResult = await scraper.scrape(credentials);
        var totalAmount = 0;
        if (scrapeResult.success) {
            console.log(JSON.stringify(scrapeResult.accounts));
            scrapeResult.accounts.forEach((account) => {
                console.log(`found ${account.txns.length} transactions for account number ${account.accountNumber}`);
                account.txns.forEach((txn) => {
                    totalAmount += txn.chargedAmount;
                    console.log(`Transaction ${txn.description}, with amount: ${txn.chargedAmount} is ${txn.status}`);
                })
            });
        } else {
            throw new Error(scrapeResult.errorType);
        }
        return [scrapeResult, totalAmount];
    } catch (e: any) {
        console.error(`scraping failed for the following reason: ${e.message}`);
    }
}
