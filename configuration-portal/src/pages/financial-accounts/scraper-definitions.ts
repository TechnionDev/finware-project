export enum CompanyTypes {
  hapoalim = 'hapoalim',
  hapoalimBeOnline = 'hapoalimBeOnline',
  beinleumi = 'beinleumi',
  union = 'union',
  amex = 'amex',
  isracard = 'isracard',
  visaCal = 'visaCal',
  max = 'max',
  leumiCard = 'leumiCard',
  otsarHahayal = 'otsarHahayal',
  discount = 'discount',
  mizrahi = 'mizrahi',
  leumi = 'leumi',
  massad = 'massad',
  yahav = 'yahav',
  beyahadBishvilha = 'beyahadBishvilha'
}

export const SCRAPERS = {
  [CompanyTypes.hapoalim]: {
    name: 'Bank Hapoalim',
    loginFields: ['userCode', 'password'],
  },
  [CompanyTypes.hapoalimBeOnline]: { // TODO remove in Major version
    name: 'Bank Hapoalim',
    loginFields: ['userCode', 'password'],
  },
  [CompanyTypes.leumi]: {
    name: 'Bank Leumi',
    loginFields: ['username', 'password'],
  },
  [CompanyTypes.mizrahi]: {
    name: 'Mizrahi Bank',
    loginFields: ['username', 'password'],
  },
  [CompanyTypes.discount]: {
    name: 'Discount Bank',
    loginFields: ['id', 'password', 'num'],
  },
  [CompanyTypes.otsarHahayal]: {
    name: 'Bank Otsar Hahayal',
    loginFields: ['username', 'password'],
  },
  [CompanyTypes.leumiCard]: { // TODO remove in Major version
    name: 'Leumi Card',
    loginFields: ['username', 'password'],
  },
  [CompanyTypes.max]: {
    name: 'Max',
    loginFields: ['username', 'password'],
  },
  [CompanyTypes.visaCal]: {
    name: 'Visa Cal',
    loginFields: ['username', 'password'],
  },
  [CompanyTypes.isracard]: {
    name: 'Isracard',
    loginFields: ['id', 'card6Digits', 'password'],
  },
  [CompanyTypes.amex]: {
    name: 'Amex',
    loginFields: ['id', 'card6Digits', 'password'],
  },
  [CompanyTypes.union]: {
    name: 'Union',
    loginFields: ['username', 'password'],
  },
  [CompanyTypes.beinleumi]: {
    name: 'Beinleumi',
    loginFields: ['username', 'password'],
  },
  [CompanyTypes.massad]: {
    name: 'Massad',
    loginFields: ['username', 'password'],
  },
  [CompanyTypes.yahav]: {
    name: 'Bank Yahav',
    loginFields: ['username', 'nationalID', 'password'],
  },
  [CompanyTypes.beyahadBishvilha]: {
    name: 'Beyahad Bishvilha',
    loginFields: ['id', 'password'],
  },
};