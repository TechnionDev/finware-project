import nodemailer from "nodemailer";

export const getCycleStartDate = (cycleStartDay: number): Date => {
    const now = new Date();
    const cycleStartDate = new Date(now.getFullYear(), now.getMonth(), cycleStartDay);
    if (now.getDate() < cycleStartDay) {
        cycleStartDate.setMonth(cycleStartDate.getMonth() - 1);
    }
    return cycleStartDate;
}

export const getCycleEndDate = (cycleStartDay: number): Date => {
    const cycleStartDate = getCycleStartDate(cycleStartDay);
    const cycleEndDate = new Date(cycleStartDate);
    cycleEndDate.setMonth(cycleEndDate.getMonth() + 1);
    cycleEndDate.setSeconds(cycleEndDate.getSeconds() - 1);
    return cycleEndDate;
}

export const datediff = (dateStart, dateEnd) => {
    let loop = new Date(dateStart);
    let count = 0;
    while (loop < dateEnd) {
        count++;
        loop.setDate(loop.getDate() + 1);
    }
    return count;
}
export const getCycleDayCount = (cycleStartDay: number): number => {
    const cycleStartDate = getCycleStartDate(cycleStartDay);
    const cycleEndDate = getCycleEndDate(cycleStartDay);
    return datediff(cycleStartDate, cycleEndDate);
}

export const getDateIndexInCycle = (cycleStartDate: Date, date: Date): number => {
    date = new Date(date.getFullYear(), date.getMonth(), date.getDate());
    return datediff(cycleStartDate, date);
}

export const emailOverBudgetNotification = async (settings, currentSpending: number) => {
    if (!settings.should_send_over_budget_notification) {
        console.log("Not sending over budget notification as the setting is disabled");
        return;
    }
    console.log('Sending email notification');

    let mailOptions = {
        from: '"Finware Spending Police 🚔" <' + settings.smtp_account_email + '>',
        to: settings.send_notification_to_email,
        subject: settings.over_budget_email_subject,
        html: `<h1>${settings.over_budget_email_subject}</h1>
        If you got this notice, it means that you overspent this month.<br><br>
        You should probably tone it down or adjust your budget in the settings of the app.<br><br>
        You can go to <a href="https://finware.local/">Finware Settings</a> to get to the configuration portal.<br><br>
        Your budget is set to ${settings.expense_budget} and you spesnt ${currentSpending} this month.`
    };
    console.log('Email options: ', mailOptions);
    let transporter = nodemailer.createTransport({
        host: settings.smtp_account_server, // TODO: Replace with something real
        port: 465,
        secure: true, // true for 465, false for other ports
        auth: {
            user: settings.smtp_account_email, // generated ethereal user
            pass: settings.smtp_account_password, // generated ethereal password
        },
    });

    // send mail with defined transport object
    transporter.sendMail(mailOptions).then((info) => {
        console.log("Message sent: %s", info.messageId);
        // Preview only available when sending through an Ethereal account
        console.log("Preview URL: %s", nodemailer.getTestMessageUrl(info));
    }).catch((err) => {
        console.error(err);
    });
}