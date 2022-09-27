import { Schema, model } from 'mongoose';

interface ISession {
    key: string;
}

const SessionSchema = new Schema<ISession>({
    key: {
        type: String,
        required: true
    },
}, { timestamps: true });

SessionSchema.pre('save', function (next) {
    next();
});

const Session = model<ISession>('Session', SessionSchema);


export default Session;