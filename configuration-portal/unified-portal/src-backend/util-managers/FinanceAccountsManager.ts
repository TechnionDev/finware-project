import { CompanyTypes, createScraper } from 'israeli-bank-scrapers';
import { ScaperScrapingResult } from 'israeli-bank-scrapers/lib/scrapers/base-scraper';

export default async function scrapeFinancialBE(account, company: String, startDate: Date): Promise<ScaperScrapingResult> {
    try {
        const options = {
            'companyId': company as CompanyTypes,
            'verbose': true,
            'startDate': startDate,
            'combineInstallments': false,
            'showBrowser': false
        };

        // Scrape is existing results are outdated
        let scrapeResult: ScaperScrapingResult;
        const scraper = createScraper(options);
        scrapeResult = await scraper.scrape(account); // TODO: if this fails the code crashes ...
        var totalAmount = 0;
        if (scrapeResult.success) {
            console.log(scrapeResult.accounts);
            scrapeResult.accounts.forEach((account) => {
                console.log(`found ${account.txns.length} transactions for account number ${account.accountNumber}`);
                account.txns.forEach((txn) => {
                    totalAmount += txn.chargedAmount;
                    // console.log(`Transaction ${txn.description}, with amount: ${txn.chargedAmount} is ${txn.status}`);
                })
            });
        } else {
            throw new Error(scrapeResult.errorType);
        }
        console.log('Total for ', account.name, ' is ', totalAmount);

        return scrapeResult;
    } catch (e: any) {
        console.error(`scraping failed for the following reason: ${e.message}`);
    }
}
