import React from 'react';
import classNames from 'classnames';
import { useEffect } from 'react';
import { useState } from 'react';
import './BluetoothPage.css';
import ResetModal from "./ResetModal";
import PasskeyConfirmation from './PasskeyConfirmation';
import { useInput } from "../../shared/hooks/useInput";
import { Spinner } from 'flowbite-react';
import { NavigateFunction, useNavigate } from 'react-router-dom';
import RedirectFetch from '../../RedirectFetch';

const BT_STATE_ENDOINT = "/api/bluetooth/current-state";
const BT_RESET_ENDPOINT = "/api/bluetooth/reset";
const BT_ACCEPT_PAIRING_ENDPOINT = "/api/bluetooth/accept";
const BT_REJECT_PAIRING_ENDPOINT = "/api/bluetooth/reject";

const POLLING_INTERVAL = 1000;
let awaitingResponse = false;
let lastRequest = new Date();

export enum Connection {
    CONNECTED = "CONNECTED",
    PAIRING = "PAIRING",
    ADVERTISING = "ADVERTISING"
}

interface BTState {
    connection: Connection | null,
    passkey: string | null,
};

function useBtState(setLoading, navigate: NavigateFunction) {
    const [btState, setBtState] = useState<BTState>();

    useEffect(() => {
        function updateBtState() {
            // Check we're not awaiting a response from the last 20 seconds
            if (awaitingResponse && lastRequest > new Date(new Date().getTime() - 20000)) {
                return;
            }
            awaitingResponse = true;
            lastRequest = new Date();
            RedirectFetch(BT_STATE_ENDOINT, {}, navigate)
                .then((data: BTState) => {
                    awaitingResponse = false;
                    setBtState((lastBTState: any) => {
                        if (data.connection !== lastBTState?.connection || lastBTState?.connection === Connection.ADVERTISING) {
                            setLoading(false);
                        }
                        return data;
                    });
                });
        }
        updateBtState();
        const int = setInterval(updateBtState, POLLING_INTERVAL);
        return () => clearInterval(int);
    }, [setLoading]);

    return btState;
}

async function resetBTConnection(navigate: NavigateFunction) {
    return RedirectFetch(BT_RESET_ENDPOINT, {
        method: 'POST'
    }, navigate);
}

async function acceptPairing(passkey, navigate: NavigateFunction) {
    return RedirectFetch(BT_ACCEPT_PAIRING_ENDPOINT, {
        method: 'POST',
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ passkey: passkey })
    }, navigate);
}

async function rejectPairing(navigate: NavigateFunction) {
    return RedirectFetch(BT_REJECT_PAIRING_ENDPOINT, {
        method: 'POST'
    }, navigate);
}

function BluetoothPage() {
    const [loading, setLoading] = useState(false);
    const navigate = useNavigate();
    const btState = useBtState(setLoading, navigate);
    const [isOpen, setOpen] = useState(false);
    const { value: passkey, setValue: setPasskey, bind: passkeyBind } = useInput();

    useEffect(() => {
        function setClosed() {
            setOpen(false);
        }
        document.body.addEventListener('click', setClosed);

        return function cleanup() {
            document.body.removeEventListener('click', setClosed);
        }
    }, []);

    return (
        <div className="bt-inner-wrapper">
            <section className="bt-connection-status flex justify-center">
                <div>
                    <div className="flex flex-row items-center mb-1">
                        <svg className="text-green-600 w-6 h-6" data-darkreader-inline-fill="" fill="currentColor" viewBox="0 0 20 20" xmlns="http://www.w3.org/2000/svg"><path fill-rule="evenodd" d="M18 10a8 8 0 11-16 0 8 8 0 0116 0zm-7-4a1 1 0 11-2 0 1 1 0 012 0zM9 9a1 1 0 000 2v3a1 1 0 001 1h1a1 1 0 100-2v-3a1 1 0 00-1-1H9z" clip-rule="evenodd"></path></svg>
                        <div className="ml-1 text-md font-medium text-gray-200">
                            Current connection status
                        </div>
                    </div>
                    <div className="inline-block text-black bg-gray-200 shadow overflow-hidden rounded-md">
                        <div className="px-12 py-3">
                            <div className={classNames("capitalize", "mt-1", "text-3xl", "sm:mt-0", "sm:col-span-2", "font-bold",
                                {
                                    "text-rose-600": btState && btState.connection === Connection.ADVERTISING,
                                    "text-blue-600": btState && btState.connection === Connection.PAIRING,
                                    "text-green-600": btState && btState.connection === Connection.CONNECTED
                                })
                            }>
                                {(btState && btState.connection?.toLowerCase()) || "Loading..."}
                            </div>
                        </div>
                    </div>
                    <div className="mt-2">
                        <ResetModal show={isOpen}
                            onOpen={() => { setOpen(true) }}
                            onAccept={async () => { setOpen(false); setLoading(true); await resetBTConnection(navigate); }}
                            onClose={() => { setOpen(false) }} />
                    </div>
                </div>
            </section>
            <PasskeyConfirmation
                btConnection={btState?.connection}
                passkeyBind={passkeyBind}
                onAccept={async () => { setLoading(true); acceptPairing(passkey, navigate); setPasskey(""); }}
                onDecline={async () => { setLoading(true); rejectPairing(navigate); }}
                loading={loading} />

            {loading &&
                <div className="text-center mt-3">
                    <Spinner color="info" />
                </div>
            }
        </div>
    );
}

export default BluetoothPage;