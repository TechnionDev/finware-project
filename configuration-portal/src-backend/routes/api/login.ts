import express from "express";
import { doLogin, checkSessionKey } from "../../util-managers/SettingsManager";
import { StatusCodes } from 'http-status-codes';
export default function loginRouterFactory() {
    const loginRouter = express.Router();

    loginRouter.post("/login", doLogin);
    loginRouter.use(async (req, res, next) => {
        if (await checkSessionKey(req.cookies['sessionKey'])) {
            next();
        } else {
            res.status(StatusCodes.FORBIDDEN).json({ message: "Login required" });
        }
    });
    return loginRouter;
};
