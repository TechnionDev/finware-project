import React from 'react';
import "./FinancialAccountsList.css";
import { Button } from 'flowbite-react';
import { useEffect, useState } from 'react';
import { Link } from 'react-router-dom';


const CREDIT_STATE_ENDPOINT = "/api/financial-account";

function useFinancialAccountsState() {
  const [FAState, setFAState] = useState([]);
  useEffect(() => {
    fetch(CREDIT_STATE_ENDPOINT)
      .then((response) => { return response.json(); })
      .then((data) => { setFAState(data); });
  }, []);
  return FAState;
}


function FinancialAccountsList() {
  const FAState = useFinancialAccountsState();
  return (
    <div>
      {
        FAState.map((financialAccount: any) => {
          return (
            <div key={financialAccount._id} className="p-3 mb-3 credit-card-container">
              <div className="font-bold text-center text-2xl"> {financialAccount.name} </div>
              <div className="text-center capitalize text-md text-gray-500 dark:text-gray-700"> {financialAccount.company} </div>
            </div>
          )
        })
      }
      <div className="flex justify-center">
        <Button.Group>
        <Button color="success">
          <Link to="create">
            Add Account
          </Link>
        </Button>
        <Button color="gray" onClick={() => fetch("/api/financial-account/scrape", { method: "POST" })}>
          Scrape Financial Accounts
        </Button>
        </Button.Group>
      </div>
    </div >
  );
}

export default FinancialAccountsList;