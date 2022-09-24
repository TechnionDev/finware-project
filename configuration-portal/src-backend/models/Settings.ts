import { Schema, model, ObjectId } from 'mongoose';
import sha256 from 'crypto-js/sha256';
import CryptoJS from 'crypto-js';

interface ISettings {
    display_refresh_frequency_minutes: number;
    scrape_frequency_hours: number;
    month_cycle_start_day: number;
    expense_budget: number;
    session_timeout_hours: number;
    password_hash: string;
    password_salt: string;
    dictionary: object;
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
    session_timeout_hours: {
        type: Number,
        default: 750, // 1 Month
        required: true
    },
    password_hash: {
        type: String,
        default: '7b54f535f932ff864ae211fead423268dd16a7d9bbaa0bbc56670da0'
            + 'dc50d2904edaf9eccbda4981ce510d3cf67a32ac99645985161f43c7242c9c893192f3d4', // Default password is Password,
        required: true
    },
    password_salt: {
        type: String,
        default: "c530a0c8c41de872e9e5945c66f0fa6dda525c5860b71",
        required: true
    },
    dictionary: {
        type: Object,
        default: {
            display_refresh_frequency_minutes: "Frequency of E-Ink display updates (e.g. 480)",
            scrape_frequency_hours: "Update frequency of financial information (e.g. 24)",
            month_cycle_start_day: "Start day of the month (1 to 28 including)",
            expense_budget: "Budget (e.g. 5000)",
            currency_symbol: "Currency symbol (e.g. ₪)",
            session_timeout_hours: "Hours before logout from configuration portal (e.g. 8)",
            password_hash: "New password (leave empty to keep unchanged)",
            password_salt: "SALT: If you see this, it's a bug",
        }
    }
}, {
    timestamps: true,
    capped: { max: 1 } // To make sure there's only one Settings document in the collection
});

// Pre save, check for updating the password field
settingsSchema.pre('findOneAndUpdate', async function (next) {
    if (this.getUpdate()['password_hash']) {
        let salt = CryptoJS.lib.WordArray.random(128 / 8);
        this.set('password_salt', salt);
        this.set('password_hash', sha256(this.getQuery().password_hash, salt));
    }
    next();
});

const Settings = model<ISettings>('Settings', settingsSchema);

export default Settings;
