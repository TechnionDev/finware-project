import React, { FormEvent } from 'react'
import { useParams, useNavigate } from 'react-router-dom';
import { useInput } from "../../shared/hooks/useInput";
import { Button, Label, Select, TextInput } from "flowbite-react";
import { SCRAPERS } from "./scraper-definitions";

const FINANCIAL_ACCOUNT_API = "/api/financial-account";

function capitalize(string) {
    return string.charAt(0).toUpperCase() + string.slice(1);
}

function InputElem({ credName }) {
    return <div className="mt-3">
        <div className="mb-1 block">
            <Label htmlFor={credName} value={capitalize(credName)} />
        </div>
        <TextInput name={credName} id={credName} placeholder="" required={true} type={credName === "password" ? "password": "text"} />
    </div>
    // switch (loginField) {
    //     case ""
    // }
}

async function handleSubmit(id: string | undefined, ev: FormEvent<HTMLFormElement>) {
    ev.preventDefault();
    const form = document.getElementById("new-card-form") as HTMLFormElement;
    if (!form) {
        return;
    }

    const formData = new FormData(form);
    formData.forEach((value, key) => console.log(`${key}: ${value}`));

    var formDataObj = {};
    formData.forEach((value, key) => formDataObj[key] = value);

    let endpoint = id ? `${FINANCIAL_ACCOUNT_API}/update/${id}` : `${FINANCIAL_ACCOUNT_API}/create`;
    let method = id ? "PUT" : "POST";


    return fetch(endpoint, {
        method,
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(formDataObj)
    });
}

function EditFinancialAccount() {
    const navigate = useNavigate();
    const { id } = useParams();
    const { value: company, bind } = useInput();
    console.log(company && SCRAPERS[company].loginFields.map(compName => <option value={compName}>{ }</option>));

    return (
        <div>
            <form id="new-card-form"
                onSubmit={async (ev) => {
                    await handleSubmit(id, ev);
                    navigate("../");
                }}>
                <div className="text-2xl mb-4"> Add a new credit card account: </div>
                <div id="comp-select">
                    <Select name="company" {...bind}>
                        <option value="">
                            -
                        </option>
                        {Object.keys(SCRAPERS).map(compName => <option key={compName} value={compName}>{capitalize(compName)}</option>)}
                    </Select>
                </div>
                {company ? <>
                    <InputElem credName="name" />
                    {SCRAPERS[company].loginFields.map((credName) => {
                        console.log("credname: ", credName);
                        return <InputElem credName={credName} />
                    })}
                    <div className="flex justify-center mt-6">
                        <Button type="submit">
                            Create
                        </Button>
                    </div>
                </>
                    : <></>}
            </form>
        </div >
    );
}

export {
    EditFinancialAccount
}