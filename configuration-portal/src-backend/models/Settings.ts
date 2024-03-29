import { Schema, model, ObjectId } from 'mongoose';
import sha256 from 'crypto-js/sha256';
import CryptoJS from 'crypto-js';

interface ISettings {
    display_refresh_frequency_minutes: number;
    /* scrape_hour: number; */
    month_cycle_start_day: number;
    expense_budget: number;
    session_timeout_hours: number;
    base_spent: number;
    should_send_over_budget_notification: boolean;
    smtp_account_email: string;
    smtp_account_password: string;
    smtp_account_server: string;
    smtp_account_server_port: number;
    send_notification_to_email: string;
    over_budget_email_subject: string;
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
    /*     scrape_hour: {
            type: Number,
            default: 4, // Default is at 04:00:00 (AM)
            required: true
        }, */
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
    should_send_over_budget_notification: {
        type: Boolean,
        default: false,
        required: true
    },
    smtp_account_email: {
        type: String,
        default: '',
        required: true
    },
    smtp_account_password: {
        type: String,
        default: '',
        required: true
    },
    smtp_account_server: {
        type: String,
        default: 'smtp.gmail.com',
        required: true
    },
    smtp_account_server_port: {
        type: Number,
        default: 465,
        required: true
    },
    send_notification_to_email: {
        type: String,
        default: '',
        required: true
    },
    over_budget_email_subject: {
        type: String,
        default: 'Over Budget Warning',
        required: true
    },
    password_hash: {
        type: String,
        default: 'f9d21446893eda115d76e2b664f8ca3b6cb270b087de5700ac30e17186a22222', // Default password is Password,
        required: true
    },
    password_salt: {
        type: String,
        default: "c530a0c8c41de872e9e5945c66f0fa6dda525c5860b71",
        required: true
    },
    base_spent: {
        type: Number,
        default: 0,
        required: true
    },
    dictionary: {
        type: Object,
        default: {
            display_refresh_frequency_minutes: "Frequency of E-Ink display updates (e.g. 480)",
            // scrape_hour: "Hour to update the credit cards information (1 to 24 including) ",
            month_cycle_start_day: "Start day of the month (1 to 28 including)",
            expense_budget: "Budget (e.g. 5000)",
            currency_symbol: "Currency symbol (e.g. ₪)",
            session_timeout_hours: "Hours before logout from configuration portal",
            should_send_over_budget_notification: "Send notifications if budget is exceeded? (false for no, true for yes)",
            smtp_account_email: "Account email to send email from",
            smtp_account_password: "Account password to send email from",
            smtp_account_server: "The server to use SMTP with (e.g. smtp.gmail.com)",
            smtp_account_server_EXT: 'For some email providers (like <a class="user-link" href="https://myaccount.google.com/apppasswords" target="_blank">Google</a>) you may need to generate a one time password',
            smtp_account_server_port: "The SMTP server port (e.g. 465)",
            send_notification_to_email: "Email address to nofity",
            over_budget_email_subject: "Subject to over budget notifications",
            base_spent: "Base spending amount to add in the main page (to be used for rent and such)",
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
    let password_hash = this.getUpdate()['password_hash'];
    if (password_hash) {
        console.log("Detected password change")
        let salt = CryptoJS.lib.WordArray.random(128 / 8).toString();
        this.set('password_salt', salt);
        console.log(`Hashing password "${password_hash}"\nwith salt "${salt}"`);
        this.set('password_hash', sha256(password_hash + salt).toString());
    }
    next();
});

const Settings = model<ISettings>('Settings', settingsSchema);

export default Settings;
