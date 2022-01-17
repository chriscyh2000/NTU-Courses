import mongoose from 'mongoose';
import dotenv from 'dotenv-defaults';
import dataInit from './upload';

const connection = () => {
    dotenv.config();
    if (!process.env.MONGO_URL) {
        console.error('No MONGO_URL')
        process.exit(1)
    }
    mongoose.connect(process.env.MONGO_URL, {
        useNewUrlParser: true,
        useUnifiedTopology: true,
    }).then(() => console.log("MongoDB connection created!"));

    const db = mongoose.connection;

    // dataInit();
}
const mongo = {
    connect: connection
};
export default mongo;