import SettingsModel from '../models/Settings';
import SessionModel from '../models/Session';
import sha256 from 'crypto-js/sha256';
import CryptoJS from 'crypto-js';
import { StatusCodes } from 'http-status-codes';

const HIDDEN_PASSWORD_VALUE = ''

async function updateSettings(req, res, financeAccountsController) {
    console.log("Updating settings:", req.body);
    let oldSettings = await SettingsModel.findOneAndUpdate({}, {}, { upsert: true, new: true });
    if (req.body.password_hash == HIDDEN_PASSWORD_VALUE) {
        req.body.password_hash = undefined;
        delete req.body.password_hash;
    } else {
        console.log(`password change detected (set to "${req.body.password_hash}")`);
    }
    let settings = await SettingsModel.findOneAndUpdate({}, req.body, { upsert: true, new: true });
    if (req.body?.month_cycle_start_day != oldSettings.month_cycle_start_day) {
        financeAccountsController.updateFinancialData(true);
    }
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

async function doLogin(req, res) {
    console.log('In doLogin');
    let settings = await SettingsModel.findOneAndUpdate({}, {}, { upsert: true, new: true });
    let hashed_attemped = sha256(req.body.password + settings.password_salt).toString();
    console.log('hashed attempt is', hashed_attemped);
    console.log('true hashed', settings.password_hash);
    if (settings.password_hash == hashed_attemped) {
        // Create a session
        let sessionKey = CryptoJS.lib.WordArray.random(128 / 8).toString();
        let session = new SessionModel({ 'key': sessionKey });
        res.cookie('sessionKey', sessionKey);
        res.status(StatusCodes.ACCEPTED).json({ success: true });
        session.save();
    } else {
        res.status(StatusCodes.UNAUTHORIZED).json({ success: false });
    }
}

async function checkSessionKey(key) {
    let session = await SessionModel.findOne({ 'key': key });
    return session != null;
}

export {
    updateSettings,
    getSettings,
    doLogin,
    checkSessionKey
};