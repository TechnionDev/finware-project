import mongoose from "mongoose";
import { CreditCardCtl } from "./controllers"
import FinancialBE from "./models/FinancialBE";

mongoose.connect('mongodb://localhost/finware', null, () => {
    const fbe = new FinancialBE({
        "name": "Second Credit Card (1)",
        "accountPwd": "pwd1",
        "accountUsername": "acnt_name1"
    });
    fbe.save().then(() => {
        (async () => {
            console.log(await CreditCardCtl.fetchCreditCards());
        })();
    });

});


