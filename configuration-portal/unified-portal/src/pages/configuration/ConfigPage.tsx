import React, { useEffect, useState, FormEvent } from 'react';
import { Button, Label, TextInput } from "flowbite-react";
import { useNavigate } from 'react-router-dom';

const SETTINGS_READ_URL = "/api/settings/get";
const SETTINGS_UPDATE_URL = "/api/settings/update";

const IGNORE_FIELDS = ["createdAt", "updatedAt", "_id", "__v"];


function useSettings() {
    const [settings, setSettings]: [any[], any] = useState([]);
    useEffect(() => {
        fetch(SETTINGS_READ_URL, { method: "GET" })
            .then((response) => { return response.json(); })
            .then((data) => { setSettings(data); });
    }, []);
    return [settings, setSettings];
}

async function handleSubmit(ev: FormEvent<HTMLFormElement>) {
    ev.preventDefault();
    const form = document.getElementById("settings-form") as HTMLFormElement;
    if (!form) {
        return;
    }

    const formData = new FormData(form);
    formData.forEach((value, key) => console.log(`${key}: ${value}`));

    var formDataObj = {};
    formData.forEach((value, key) => formDataObj[key] = value);

    let endpoint = SETTINGS_UPDATE_URL;
    let method = "PUT";

    return fetch(endpoint, {
        method,
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(formDataObj)
    });
}

function ConfigPage() {
    const [settings, setSettings] = useSettings();
    const navigate = useNavigate();

    return (
        <div className="min-w-[240px]">
            <div className="text-2xl mb-4">Edit Settings</div>
            <form id="settings-form" onSubmit={async (ev) => {
                await handleSubmit(ev);
                navigate("../");
            }}>
                {Object.entries(settings).map(([key, value]: [string, any]) => {
                    return IGNORE_FIELDS.includes(key) ? <></> : <div>
                        <div className="mt-3">
                            <div className="mb-1 block">
                                <Label htmlFor={key} value={key} />
                            </div>
                            <TextInput name={key} id={key} placeholder="" required={true} type={key === "password" ? "password" : "text"} defaultValue={value} />
                        </div>
                    </div>
                })}

                <div className="flex justify-center mt-5">
                    <Button type="submit">
                        Save
                    </Button>
                </div>
            </form>


        </div>
    );
}

export default ConfigPage;