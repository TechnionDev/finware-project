import React, { useEffect, useState } from 'react'

const SETTINGS_URL = "/api/settings";

function useSettingsState() {
    const [state, setState]: [any[], any] = useState([]);
    useEffect(() => {
        fetch(SETTINGS_URL, { method: "GET" })
            .then((response) => { return response.json(); })
            .then((data) => { setState(data); });
    }, []);
    return [state, setState];
}


function ConfigPage() {
    const [state, setState] = useSettingsState();

    return (
        <div>
            <div>1. E-INK refresh rate</div>
            <div>{JSON.stringify(state)}</div>
        </div>
    );
}

export default ConfigPage