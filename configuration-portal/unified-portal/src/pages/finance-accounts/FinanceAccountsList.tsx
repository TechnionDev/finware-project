import { Button } from 'flowbite-react';
import { useEffect, useState } from 'react';
import { Link } from 'react-router-dom';

const CREDIT_STATE_ENDPOINT = "/api/financial-account";

function useFinanceAccountsState() {
  const [FAState, setFAState] = useState([]);
  useEffect(() => {
    fetch(CREDIT_STATE_ENDPOINT)
      .then((response) => { return response.json(); })
      .then((data) => { setFAState(data); });
  }, []);
  return FAState;
}


function FinanceAccountList() {
  const FAState = useFinanceAccountsState();

  return (
    <div>
      {
        FAState.map((financialAccount: any) => {
          return <div key={financialAccount._id} className="border border-white p-3 mb-3">
            <div> Name: {financialAccount.name} </div>
            <div> Company: {financialAccount.company} </div>
          </div>
        })
      }
      <div className="flex justify-center">
        <Button>
          <Link to="create">
            Add Account
          </Link>
        </Button>
        <Button color="purple" onClick={() => fetch("/api/financial-account/scrape", {method:"POST"})}>
        Scrape Financial Accounts
        </Button>
      </div>
    </div >
  );
}

export default FinanceAccountList;