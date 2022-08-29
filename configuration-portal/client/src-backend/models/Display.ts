import { Schema, model } from 'mongoose';

interface IDisplay {
    name: string;
    id: string;
    update_frequency_minutes: number;
    last_refresh: Date;
}

const displaySchema = new Schema<IDisplay>({
    name: {
        type: String,
        required: true,
        default: 'MyDisplay'
    },
    id: {
        type: String,
        required: true
    },
    update_frequency_minutes: {
        type: Number,
        required: true
    },
    last_refresh: {
        type: Date,
        required: false
    },
});

displaySchema.pre('save', function (next) {
    const now = new Date();
    if (!this.last_refresh) {
        this.last_refresh = now;
    }
    next();
});

const Display = model<IDisplay>('Display', displaySchema);


export default Display;