import React from "react";
import { Button, Label, TextInput } from "flowbite-react";

function FinanceAccountForm({bind}) {
    return (
        <div>
            Add a new credit card account:
            <div>
                <div className="mb-2 block">
                    <Label
                        htmlFor="email3"
                        value="Your email"
                    />
                </div>
                <TextInput
                    id="email3"
                    type="email"
                    placeholder="name@flowbite.com"
                    required={true}
                    helperText={<>We'll never share your details. Read our <a href="/forms" className="font-medium text-blue-600 hover:underline dark:text-blue-500">Privacy Policy</a>.</>}
                    {...bind}
                />
            </div>
            <div className="flex justify-center">
                <Button>
                    Create
                </Button>
            </div>
        </div>
    );
}

export default FinanceAccountForm;