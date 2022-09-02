import { Schema, model } from 'mongoose';
import SettingsModel from './Settings';
import { FinancialAccountsController } from '../controllers';


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
    scrape_result: object;
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
        required: true
    },

}, { timestamps: true });

// Before returning result, add 'outdated' field
financialBackendSchema.post('save', function (doc) {
    // TODO: scrape in an async manner
    // Get scrape frequency from settings
    SettingsModel.findOne({}, (err, settings) => {
        if (err) {
            console.log("Failed to find settings object");
            return;
        }
        const now = new Date();
        if ((now.getTime() - this.last_scrape.getTime()) / 1000 / 60 / 60 > settings.scrape_frequency_hours) {
            
        }

    });
    // // Check if 'last_scrape' is older than 'scrape_frequency_hours' hours
    // this.outdated = (now.getTime() - this.last_scrape.getTime()) / 1000 / 60 / 60 > this.scrape_frequency_hours;
    // next();
});
const FinancialBE = model<IFinancialBackend>('FinancialBE', financialBackendSchema);

export default FinancialBE;