import React, { useEffect, useState, FormEvent } from 'react';
import { Button, Label, TextInput } from "flowbite-react";
import { NavigateFunction, useNavigate } from 'react-router-dom';

const DO_LOGIN_URL = "/api/login";

function useSettings() {
    const [settings, setSettings]: [any[], any] = useState([]);
    useEffect(() => {
        fetch(DO_LOGIN_URL, { method: "GET" })
            .then((response) => { return response.json(); })
            .then((data) => { setSettings(data); });
    }, []);
    return settings;
}

function showPasswordError() {
    let errFld = document.getElementById('err-field');
    if (errFld) {
        errFld.innerText = "Invalid password";
        errFld.style.color = "red";
    }
    let passwordFld = document.getElementById('password');
    if (passwordFld) {
        passwordFld.style.borderColor = "red";
    }
}

async function handleSubmit(ev: FormEvent<HTMLFormElement>, navigate: NavigateFunction) {
    ev.preventDefault();
    const form = document.getElementById("login-form") as HTMLFormElement;
    if (!form) {
        return;
    }

    const formData = new FormData(form);
    formData.forEach((value, key) => console.log(`${key}: ${value}`));

    var formDataObj = {};
    formData.forEach((value, key) => formDataObj[key] = value);

    let endpoint = DO_LOGIN_URL;
    let method = "POST";

    fetch(endpoint, {
        method,
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(formDataObj)
    }).then((response) => {
        // Check if http status is OK
        if (response.ok) {
            navigate("/");
        } else {
            showPasswordError();
        }
    }).catch((err) => {
        showPasswordError();
    });
}

function ConfigPage() {
    const settings = useSettings();
    const navigate = useNavigate();

    return (
        <div className="min-w-[240px]">
            <div className="text-2xl mb-4">Login Page</div>
            <form id="login-form" onSubmit={async (ev) => {
                await handleSubmit(ev, navigate);
            }}>
                <div className="mt-3">
                    <div className="mb-1 block">
                        <Label htmlFor="password" value="Enter login password" />
                    </div>
                    <div className="sub-desc" id="err-field"></div>
                    <TextInput name="password" id="password" placeholder="Password" required={true} type="password" />
                </div>
                <div className="flex justify-center mt-5">
                    <Button type="submit">
                        Login
                    </Button>
                </div>
            </form>

        </div>
    );
}

export default ConfigPage;