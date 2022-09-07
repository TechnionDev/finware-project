import SettingsModel from '../models/Settings';

async function updateSettings(req, res) {
    console.log("Updating settings:", req.body);
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