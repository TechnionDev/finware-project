import express from "express";
import { FinancialAccountsController } from "../../controllers";

export default function financialRouterFactory(financialAccountsController: FinancialAccountsController) {
  const financialAccountsRouter = express.Router();

  financialAccountsRouter.get("/", financialAccountsController.fetchCreditCards.bind(financialAccountsController));
  financialAccountsRouter.post("/create", financialAccountsController.createFinancialAccount.bind(financialAccountsController))
  financialAccountsRouter.post("/:id/delete", financialAccountsController.removeFinancialAccount.bind(financialAccountsController));
  financialAccountsRouter.post("/scrape", financialAccountsController.updateFinancialData.bind(financialAccountsController, true));


  return financialAccountsRouter;
};
