import SettingsModel from '../models/Settings';

async function updateSettings(req, res) {
    console.log("Updating settings:", req.body);
    let settings = await SettingsModel.findOneAndUpdate({}, req.body, { new: true });
    res.json(settings);
}



async function getSettings(req, res) {
    let settings = await SettingsModel.findOne({});
    res.json(settings);
}

export {
    updateSettings,
    getSettings
};