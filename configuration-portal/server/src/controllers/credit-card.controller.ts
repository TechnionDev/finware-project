import scrapeAccount from "../managers/FinancialAccountsManager";
import FinancialBE from "../models/FinancialBE";

async function fetchCreditCards(req?, res?) {
    const all = await FinancialBE.find({}).select({ "name": 1, "company": 1 });
    return all;
}

async function createFinancialAccount(req, res) {
    console.log(req.body);
    await FinancialBE.create({
        ...req.body
    });
    res.json({ status: "SUCCESS" });
}

async function scrape() {
    const accounts = await FinancialBE.find({});
    let total = 0;
    for (const account of accounts) {
        console.log(account);
        const [accountScrapeResult, accountTotal]: [any, number] = await scrapeAccount(account, account.company);
        total+= accountTotal;
    }
    return total;
}

export default {
    fetchCreditCards,
    createFinancialAccount,
    scrape
}


