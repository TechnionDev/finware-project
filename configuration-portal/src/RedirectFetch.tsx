import { NavigateFunction, useNavigate } from "react-router-dom";

export default function (url: string, options: object, navigate: NavigateFunction) {
    return fetch(url, options).then((response) => {
        if (response.ok) return response.json();
        throw Error()
    }).catch(() => {
        navigate('/login');
    });
};