import React, { useEffect, useState, FormEvent } from 'react';
import { Button, Label, TextInput } from "flowbite-react";
import { NavigateFunction, NavLink, useNavigate } from 'react-router-dom';
import RedirectFetch from '../../RedirectFetch';

const SETTINGS_READ_URL = "/api/settings/get";
const SETTINGS_UPDATE_URL = "/api/settings/update";

const IGNORE_FIELDS = ["createdAt", "updatedAt", "dictionary", "_id", "__v"];


function useSettings(navigate: NavigateFunction) {
    const [settings, setSettings]: [any[], any] = useState([]);
    useEffect(() => {
        RedirectFetch(SETTINGS_READ_URL, { method: "GET" }, navigate)
            .then((data) => { setSettings(data) })
            .catch(() => {
                navigate('/login');
            });
    }, []);
    return settings;
}

async function handleSubmit(ev: FormEvent<HTMLFormElement>, navigate: NavigateFunction) {
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

    return RedirectFetch(endpoint, {
        method,
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(formDataObj)
    }, navigate);
}

function ConfigPage() {
    const navigate = useNavigate();
    const settings = useSettings(navigate);
    if (!settings) {
        return <></>;
    }
    return (
        <div className="min-w-[240px]" style={{ "margin": "5px" }}>
            <div className="text-2xl mb-4">Edit Settings</div>
            <form id="settings-form" onSubmit={async (ev) => {
                await handleSubmit(ev, navigate);
                navigate("../");
            }}>
                {Object.entries(settings).map(([key, value]: [string, any]) => {
                    return [key, IGNORE_FIELDS.includes(key) ? <></> : <div>
                        <div className="mt-3">
                            <div className="mb-1 block">
                                <Label htmlFor={key}>
                                    {settings['dictionary'][key]}<br />
                                    {settings['dictionary'][key + "_EXT"] ? <div dangerouslySetInnerHTML={{ __html: settings['dictionary'][key + "_EXT"] }} /> : <></>}
                                </Label>

                            </div>
                            <TextInput name={key} id={key} placeholder="" required={key !== "password_hash"} type={key === "password_hash" ? "password" : "text"} defaultValue={value} />
                        </div>
                    </div>
                    ]
                }).sort((a, b): any => {
                    let dir = 1;
                    if (b[0] === "password_hash") return -dir;
                    else if (a[0] === "password_hash") return dir;
                    else if (a[0] < b[0]) return -dir;
                    else if (a[0] > b[0]) return dir;
                    else return 0;
                }).map(x => x[1])
                }

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