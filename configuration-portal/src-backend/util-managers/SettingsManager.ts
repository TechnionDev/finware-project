import SettingsModel from '../models/Settings';

const HIDDEN_PASSWORD_VALUE = ''

async function updateSettings(req, res, financeAccountsController) {
    console.log("Updating settings:", req.body);
    let oldSettings = await SettingsModel.findOneAndUpdate({}, {}, { upsert: true, new: true });
    if (req.body?.month_cycle_start_day != oldSettings.month_cycle_start_day) {
        financeAccountsController.updateFinancialData(true);
    }
    if (req.body.password_hash == HIDDEN_PASSWORD_VALUE) {
        req.body.password_hash = undefined;
        delete req.body.password_hash;
    }
    let settings = await SettingsModel.findOneAndUpdate({}, req.body, { upsert: true, new: true });
    res.json(settings);
}

async function getSettings(req, res) {
    let settings = await SettingsModel.findOneAndUpdate({}, {}, { upsert: true, new: true });
    // Don't return the password
    settings.password_hash = HIDDEN_PASSWORD_VALUE;
    settings.password_salt = undefined;
    delete settings.password_hash;
    delete settings.password_salt;
    console.log('Getting settings without the password stuff,', settings);
    res.json(settings);
}

export {
    updateSettings,
    getSettings
};