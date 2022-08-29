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
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const FinancialAccountsManager_1 = __importDefault(require("../managers/FinancialAccountsManager"));
const FinancialBE_1 = __importDefault(require("../models/FinancialBE"));
function fetchCreditCards(req, res) {
    return __awaiter(this, void 0, void 0, function* () {
        const all = yield FinancialBE_1.default.find({}).select({ "name": 1, "company": 1 });
        return all;
    });
}
function createFinancialAccount(req, res) {
    return __awaiter(this, void 0, void 0, function* () {
        console.log(req.body);
        yield FinancialBE_1.default.create(Object.assign({}, req.body));
        res.json({ status: "SUCCESS" });
    });
}
function scrape() {
    return __awaiter(this, void 0, void 0, function* () {
        const accounts = yield FinancialBE_1.default.find({});
        let total = 0;
        for (const account of accounts) {
            console.log(account);
            const [accountScrapeResult, accountTotal] = yield (0, FinancialAccountsManager_1.default)(account, account.company);
            total += accountTotal;
        }
        return total;
    });
}
exports.default = {
    fetchCreditCards,
    createFinancialAccount,
    scrape
};
