import React from 'react';
import classNames from 'classnames';
import { Button, TextInput } from "flowbite-react";
import { Connection } from "./BluetoothPage";

function PasskeyConfirmation({ btConnection, onDecline, onAccept, passkeyBind, loading }) {
    return (
        <>
            {btConnection === Connection.PAIRING ?
                <section className={classNames("my-8", "flex", "justify-center")}>
                    <div className="max-w-[85%] py-4 px-7" style={{ border: "1px solid #60687687" }}>
                        <div className="text-4xl">
                            <div className="text-center text-base text-gray-400 mb-2">Passkey</div>
                            <div>
                                <TextInput name="passkey" id="passkey" placeholder="123456" required={true} type="number" {...passkeyBind} />
                            </div>
                        </div>
                        <div className="mt-2 flex flex-row items-center mb-1">
                            <svg aria-hidden="true" focusable="false" data-prefix="fas" data-icon="exclamation-triangle" className="w-10 h-10 mr-2 fill-current" role="img" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 576 512"><path fill="yellow" d="M569.517 440.013C587.975 472.007 564.806 512 527.94 512H48.054c-36.937 0-59.999-40.055-41.577-71.987L246.423 23.985c18.467-32.009 64.72-31.951 83.154 0l239.94 416.028zM288 354c-25.405 0-46 20.595-46 46s20.595 46 46 46 46-20.595 46-46-20.595-46-46-46zm-43.673-165.346l7.418 136c.347 6.364 5.609 11.346 11.982 11.346h48.546c6.373 0 11.635-4.982 11.982-11.346l7.418-136c.375-6.874-5.098-12.654-11.982-12.654h-63.383c-6.884 0-12.356 5.78-11.981 12.654z"></path></svg>
                            <div className="ml-1 text-sm font-medium text-gray-200">
                                Do not share or enter the passkey in any other device
                            </div>
                        </div>
                        {!loading &&
                            <div className="mt-5 flex justify-center">
                                <Button.Group>
                                    <Button color="success" onClick={(e) => { onAccept();}}>
                                        Pair
                                    </Button>
                                    <Button color="gray" onClick={onDecline}>
                                        Reject
                                    </Button>
                                </Button.Group>
                            </div>
                        }
                    </div>
                </section>
                : <></>}
        </>
    );
}

export default PasskeyConfirmation;