import { Schema, model, connect } from 'mongoose';

interface IFinancialBackend {
    name: string;
    company: string;
    username?: string;
    userCode?: string;
    password?: string;
    id?: string;
    nationalId?: string;
    card6Digits?: string;
}

const financialBackendSchema = new Schema<IFinancialBackend>({
    name: {
        type: String,
        required: true
    },
    company: {
        type: String,
        required: true
    },
    username: {
        type: String,
        required: false
    },
    userCode: {
        type: String,
        required: false
    },
    password: {
        type: String,
        required: false
    },
    id: {
        type: String,
        required: false
    },
    nationalId: {
        type: String,
        required: false
    },
    card6Digits: {
        type: String,
        required: false
    }
}, { timestamps: true });

const FinancialBE = model<IFinancialBackend>('FinancialBE', financialBackendSchema);


export default FinancialBE;