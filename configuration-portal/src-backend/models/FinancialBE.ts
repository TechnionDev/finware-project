import { Schema, model } from 'mongoose';
import SettingsModel from './Settings';
import { ScaperScrapingResult } from "israeli-bank-scrapers/lib/scrapers/base-scraper";

export enum ValidationStatus {
    VALIDATED = "VALIDATED",
    FAILED = "FAILED",
    INPROGRESS = "INPROGRESS"
}


interface IFinancialBackend {
    name: string;
    company: string;
    username?: string;
    userCode?: string;
    password?: string;
    id?: string;
    nationalId?: string;
    card6Digits?: string;
    last_scrape: Date;
    scrape_result: ScaperScrapingResult;
    validation_status?: String;
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
    },
    last_scrape: {
        type: Date,
        default: new Date(0),
        required: true
    },
    scrape_result: {
        type: Object,
        default: { success: true, accounts: [] },
        required: true
    },
    validation_status: {
        type: String,
        default: ValidationStatus.INPROGRESS,
        required: false
    }
}, { timestamps: true });

// Before returning result, add 'outdated' field
financialBackendSchema.pre('save', function (next) {
    // TODO: scrape in an async manner
    // Get scrape frequency from settings
    if (!this.scrape_result) {
        this.scrape_result = { success: false, accounts: [] };
    }
    next();
});
const FinancialBE = model<IFinancialBackend>('FinancialBE', financialBackendSchema);

export default FinancialBE;