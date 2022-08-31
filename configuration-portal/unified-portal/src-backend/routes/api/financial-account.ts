import express from "express";
import { FinanceAccountsController } from "../../controllers";

export default function financialRouterFactory(financeAccountsController: FinanceAccountsController) {
  const financeAccountsRouter = express.Router();

  financeAccountsRouter.get("/", financeAccountsController.fetchCreditCards);
  financeAccountsRouter.post("scrape", financeAccountsController.scrape);

  return financeAccountsRouter;
};
