import React from 'react';
import "./FinancialAccountsList.css";
import { Button, Spinner } from 'flowbite-react';
import { useEffect, useState } from 'react';
import { Link } from 'react-router-dom';
import { XCircleIcon, CheckIcon, XMarkIcon } from "@heroicons/react/24/solid";
import { ErrorScreenshotModal } from './ErrorScreenshotModal';


const CREDIT_STATE_ENDPOINT = "/api/financial-account";
const FA_DELETE_TEMPLATE = "/api/financial-account/{id}/delete";
const FE_LOGOS_ENDPOINTS = "/fe-logos";

const POLLING_INTERVAL = 1000;

const ValidationStatus = {
    VALIDATED: "VALIDATED",
    FAILED: "FAILED",
    INPROGRESS: "INPROGRESS"
}

function useFinancialAccountsState() {
    const [FAState, setFAState]: [any[], any] = useState([]);
    useEffect(() => {
        function updateFAState() {
            fetch(CREDIT_STATE_ENDPOINT)
                .then((response) => { return response.json(); })
                .then((data) => { setFAState(data); });
        }
        updateFAState();
        const int = setInterval(updateFAState, POLLING_INTERVAL);
        return () => clearInterval(int);

    }, []);
    return FAState;
}

function FinancialAccount({ _id, name, company, last_scrape, validation_status, scrape_result, onDelete }) {
    const [isOpen, setOpen] = useState(false);

    useEffect(() => {
        function setClosed() {
            setOpen(false);
        }
        document.body.addEventListener('click', setClosed);

        return function cleanup() {
            document.body.removeEventListener('click', setClosed);
        }
    }, []);

    let status;
    switch (validation_status) {
        case ValidationStatus.INPROGRESS:
            status = <><Spinner color="info" /> <div className="ml-2 text-blue-800">Validating</div></>
            break;
        case ValidationStatus.VALIDATED:
            status = <>
                <CheckIcon className="text-green-700 h-7 w-6" />
                <div className="ml-1 text-green-600">Validated</div>
            </>
            break;
        case ValidationStatus.FAILED:
            status = <>
                <div>
                    <div className="flex flex-row mb-1">
                        <XMarkIcon className="text-rose-600 h-6 w-6" />
                        <div className="ml-1 text-rose-700">Failed</div>
                    </div>
                    <ErrorScreenshotModal errorMessage={scrape_result?.errorMessage} errorType={scrape_result?.errorType}
                        name={name} show={isOpen} onClose={setOpen.bind(null, false)} onOpen={setOpen.bind(null, true)} />
                </div>
            </>
            break;
        default:
            break;
    }

    let compLogoSrc = FE_LOGOS_ENDPOINTS + `/${company?.toLowerCase()}.png`;
    return (<div className="overflow-hidden bg-white shadow rounded-lg relative text-gray-900">
        <XCircleIcon onClick={() => onDelete(_id)} className="absolute top-1 right-1 h-8 w-8 text-red-600 cursor-pointer" />
        <div className="px-4 py-5 sm:px-6">
            <h3 className="text-xl font-bold leading-6 text-gray-900">{name}</h3>
        </div>
        <div className="border-t border-gray-200">
            <dl>
                <div className="bg-gray-50 px-4 py-3 grid grid-cols-3 gap-4">
                    <dt className="flex items-center text-base font-medium text-gray-500">Company</dt>
                    <div className="col-span-2">
                        <div className="flex items-center">
                            <img className=" mr-1 w-[18px]" src={compLogoSrc} alt={`${company} logo`} />
                            <dd className="text-base text-gray-900 mt-0 capitalize">{company}</dd>
                        </div>
                    </div>
                </div>
                <div className="bg-white px-4 py-3 grid grid-cols-3 gap-4">
                    <dt className="flex items-center text-base font-medium text-gray-500">Last Checked</dt>
                    <div className="col-span-2 flex items-center">
                        <dd className="text-base text-gray-900 mt-0 capitalize">{new Date(last_scrape).getTime() !== new Date(0).getTime() ? new Date(last_scrape).toLocaleString() : "N/A"}</dd>
                    </div>
                </div>
                <div className="bg-gray-50 px-4 py-3 grid grid-cols-3 gap-4">
                    <dt className="flex items-center text-base font-medium text-gray-500">Validation</dt>
                    <div className="col-span-2 flex items-center">
                        {status}
                    </div>
                </div>
            </dl>
        </div>
    </div>);
}


function FinancialAccountsList() {
    const FAState = useFinancialAccountsState();

    function deleteFA(FAId) {
        // setFAState(FAState.filter(fa => fa._id !== FAId));
        fetch(FA_DELETE_TEMPLATE.replace("{id}", FAId), {
            method: "POST"
        });
    }

    return (
        <div className="w-[100%]">
            <div className="mx-auto w-[80%] max-w-[600px] mb-3 grid gap-y-6">
                {
                    FAState.map((financialAccount: any) => {
                        /* return (
                            <div key={financialAccount._id} className="relative py-3 px-12 min-w-[200px] mb-3 credit-card-container">
                                <XCircleIcon onClick={() => deleteFA(financialAccount._id)} className="absolute top-1 right-1 h-7 w-7 text-red-700" />
                                <div className="font-bold text-center text-2xl"> {financialAccount.name} </div>
                                <div className="text-center capitalize text-md text-gray-500 dark:text-gray-700"> {financialAccount.company} </div>
                            </div>
                        ) */
                        return <FinancialAccount key={financialAccount._id} {...financialAccount} onDelete={deleteFA} />
                    })
                }
            </div>
            {FAState && FAState.length ?
                <div className="flex justify-center mb-5">
                    <Button color="gray" onClick={() => fetch("/api/financial-account/scrape", { method: "POST" })}>
                        Force Update All
                    </Button>
                </div> : <></>
            }
            <div className="flex justify-center mb-1">
                <Button color="info">
                    <Link to="create">
                        Add Account
                    </Link>
                </Button>
            </div>
        </div >
    );
}

export default FinancialAccountsList;