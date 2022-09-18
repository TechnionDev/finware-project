import { Schema, model, ObjectId } from 'mongoose';


interface ISettings {
    display_refresh_frequency_minutes: number;
    scrape_frequency_hours: number;
    month_cycle_start_day: number;
    expense_budget: number;
    currency_symbol: string;
}

const settingsSchema = new Schema<ISettings>({
    display_refresh_frequency_minutes: {
        type: Number,
        default: 120,
        required: true
    },
    scrape_frequency_hours: {
        type: Number,
        default: 24,
        required: true
    },
    month_cycle_start_day: {
        type: Number,
        max: 28, // To be compatible with February too
        min: 1,
        default: 2,
        required: true
    },
    expense_budget: {
        type: Number,
        default: 2500,
        required: true,
    },
    currency_symbol: {
        type: String,
        default: '₪',
        required: true
    }
}, {
    timestamps: true,
    capped: { max: 1 } // To make sure there's only one Settings document in the collection
});

const Settings = model<ISettings>('Settings', settingsSchema);

export default Settings;
