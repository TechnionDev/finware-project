import express from "express";
import { FinanceAccountsController } from "../../controllers";

export default function financialRouterFactory(financeAccountsController: FinanceAccountsController) {
  const financeAccountsRouter = express.Router();

  financeAccountsRouter.get("/", financeAccountsController.fetchCreditCards.bind(financeAccountsController));
  financeAccountsRouter.post("/scrape", financeAccountsController.scrape.bind(financeAccountsController));

  return financeAccountsRouter;
};
