import { CompanyTypes, createScraper, ScraperOptions } from 'israeli-bank-scrapers';
import { ScaperScrapingResult } from 'israeli-bank-scrapers/lib/scrapers/base-scraper';
// Import path
import path from 'path';

const FAILURE_SCREENSHOT_DIR = path.resolve("./build/static/media");

export default async function scrapeFinancialBE(financialBE, company: String, startDate: Date, failureScreenshotPath = null): Promise<ScaperScrapingResult> {
    let options: ScraperOptions;
    // Log failure screenshot path and location
    failureScreenshotPath = path.join(FAILURE_SCREENSHOT_DIR, failureScreenshotPath + ".jpg");
    console.log(`Failure screenshot will be saved in ${failureScreenshotPath}`);
    options = {
        companyId: company as CompanyTypes,
        verbose: true,
        startDate: startDate,
        combineInstallments: false,
        storeFailureScreenShotPath: failureScreenshotPath || undefined,
        showBrowser: process.env.NODE_ENV?.toLowerCase().includes('dev'),
        defaultTimeout: 30000, // 30 second timeout
    };

    // Scrape is existing results are outdated
    const scrapeResult = await createScraper(options).scrape(financialBE);
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
        console.log('Total for ', financialBE.name, ' is ', totalAmount);
    }

    return scrapeResult;
}
