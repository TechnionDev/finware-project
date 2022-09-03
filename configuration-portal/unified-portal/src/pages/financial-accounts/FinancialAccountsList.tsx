import React from 'react';
import "./FinancialAccountsList.css";
import { Button } from 'flowbite-react';
import { useEffect, useState } from 'react';
import { Link } from 'react-router-dom';
import { XCircleIcon } from "@heroicons/react/24/solid";


const CREDIT_STATE_ENDPOINT = "/api/financial-account";
const FA_DELETE_TEMPLATE = "/api/financial-account/{id}/delete";

function useFinancialAccountsState() {
    const [FAState, setFAState]: [any[], any] = useState([]);
    useEffect(() => {
        fetch(CREDIT_STATE_ENDPOINT)
            .then((response) => { return response.json(); })
            .then((data) => { setFAState(data); });
    }, []);
    return [FAState, setFAState];
}


function FinancialAccountsList() {
    const [FAState, setFAState] = useFinancialAccountsState();

    function deleteFA(FAId) {
        setFAState(FAState.filter(fa => fa._id != FAId));
        fetch(FA_DELETE_TEMPLATE.replace("{id}", FAId), {
            method: "POST"
        });
    }

    return (
        <div>
            {
                FAState.map((financialAccount: any) => {
                    return (
                        <div key={financialAccount._id} className="relative p-3 min-w-[200px] mb-3 credit-card-container">
                            <XCircleIcon onClick={() => deleteFA(financialAccount._id)} className="absolute top-1 right-2 h-8 w-8 text-red-700" />
                            <div className="font-bold text-center text-2xl"> {financialAccount.name} </div>
                            <div className="text-center capitalize text-md text-gray-500 dark:text-gray-700"> {financialAccount.company} </div>
                        </div>
                    )
                })
            }
            <div className="flex justify-center mb-1">
                <Button color="success">
                    <Link to="create">
                        Add Account
                    </Link>
                </Button>
            </div>
        </div >
    );
}

export default FinancialAccountsList;