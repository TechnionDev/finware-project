import { Schema, model, ObjectId } from 'mongoose';
import sha256 from 'crypto-js/sha256';
import CryptoJS from 'crypto-js';

interface IMsftStock {
    price: number;
}

const msftStockSchema = new Schema<IMsftStock>({
    price: {
        type: Number,
        default: 0,
        required: true
    },
}, {
    timestamps: true,
    capped: { max: 1 } // To make sure there's only one MsftStock document in the collection
});

const MsftStock = model<IMsftStock>('MsftStock', msftStockSchema);

export default MsftStock;
