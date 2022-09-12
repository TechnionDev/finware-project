import React from "react"
import slugify from 'slugify';
import { Button, Modal } from 'flowbite-react';

const FAILURE_SCREENSHOT_ENDPOINT = "/static/media";

export const ErrorScreenshotModal = ({ name, errorMessage, errorType, onOpen, onClose, show }) => {
    return <>
        <div onClick={(e) => { e.stopPropagation(); }}>
            <Button onClick={onOpen} color="gray"><span className="text-gray-600">View Error</span></Button>
        </div>
        <Modal show={show} onClose={onClose} >
            <div onClick={(e) => { e.stopPropagation(); }}>
                <Modal.Header>
                    <div className="flex flex-row items-center">
                        <div>
                            Error Screenshot
                        </div>
                    </div>
                </Modal.Header>

                <Modal.Body>
                    <div className="space-y-6">
                        <p className="text-base leading-relaxed text-gray-500 dark:text-gray-300 break-all">
                            <span className="font-bold">Error Type:</span> {errorType}
                            {errorMessage ?
                                <> <br /> <span className="font-bold">Error Message:</span> <br /> {errorMessage} </> : <></>
                            }
                        </p>

                        <img src={`${FAILURE_SCREENSHOT_ENDPOINT}/${slugify(name)}.jpg`} alt={"Failure screenshot"} />
                    </div>
                </Modal.Body>
                <Modal.Footer>
                    <Button color="info" onClick={onClose} >
                        Close
                    </Button>
                </Modal.Footer>
            </div>
        </Modal>
    </>;
}