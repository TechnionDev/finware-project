import React from "react"
import { Button, Modal } from 'flowbite-react';

export default function ResetModal({ onOpen, onAccept, onClose, show }) {
    return <>
        <div onClick={(e) => { e.stopPropagation(); }}>
            <Button onClick={onOpen}>
                Reset Bluetooth
            </Button>
        </div>
        <Modal
            show={show}
            onClose={onClose}
        >

            <div onClick={(e) => { e.stopPropagation(); }}>
                <Modal.Header>
                    <div className="flex flex-row items-center">
                        <div>
                            <svg aria-hidden="true" focusable="false" data-prefix="fas" data-icon="exclamation-triangle" className="w-4 h-4 mr-2 fill-current" role="img" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 576 512">
                                <path fill="yellow" d="M569.517 440.013C587.975 472.007 564.806 512 527.94 512H48.054c-36.937 0-59.999-40.055-41.577-71.987L246.423 23.985c18.467-32.009 64.72-31.951 83.154 0l239.94 416.028zM288 354c-25.405 0-46 20.595-46 46s20.595 46 46 46 46-20.595 46-46-20.595-46-46-46zm-43.673-165.346l7.418 136c.347 6.364 5.609 11.346 11.982 11.346h48.546c6.373 0 11.635-4.982 11.982-11.346l7.418-136c.375-6.874-5.098-12.654-11.982-12.654h-63.383c-6.884 0-12.356 5.78-11.981 12.654z"></path>
                            </svg>
                        </div>
                        <div>
                            Bluetooth Reset
                        </div>

                    </div>
                </Modal.Header>

                <Modal.Body>
                    <div className="space-y-6">
                        <p className="text-base leading-relaxed text-gray-500 dark:text-gray-300">
                            Please note that resetting the bluetooth will disconnect the RaspberryPi from the E-Ink display if they are currently connected.
                        </p>
                    </div>
                </Modal.Body>
                <Modal.Footer>
                    <Button onClick={onAccept}>
                        Reset
                    </Button>
                    <Button color="gray" onClick={onClose} >
                        Cancel
                    </Button>
                </Modal.Footer>
            </div>
        </Modal>
    </>;
}