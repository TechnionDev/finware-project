"use strict";
exports.__esModule = true;
var mongoose_1 = require("mongoose");
var financialBackendSchema = new mongoose_1.Schema({
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
    }
}, { timestamps: true });
var FinancialBE = (0, mongoose_1.model)('FinancialBE', financialBackendSchema);
exports["default"] = FinancialBE;
