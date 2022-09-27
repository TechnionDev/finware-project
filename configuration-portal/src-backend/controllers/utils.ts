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