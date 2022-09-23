import SettingsModel from '../models/Settings';

async function updateSettings(req, res, financeAccountsController) {
    console.log("Updating settings:", req.body);
    let oldSettings = await SettingsModel.findOneAndUpdate({}, {}, { upsert: true, new: true });
    if(req.body?.month_cycle_start_day != oldSettings.month_cycle_start_day) {
        financeAccountsController.updateFinancialData(true);
    }
    let settings = await SettingsModel.findOneAndUpdate({}, req.body, { upsert: true, new: true });
    res.json(settings);
}

async function getSettings(req, res) {
    let settings = await SettingsModel.findOneAndUpdate({}, {}, { upsert: true, new: true });
    res.json(settings);
}

export {
    updateSettings,
    getSettings
};