import mongoose from 'mongoose';
const Schema = mongoose.Schema;

const AccountSchema = new Schema({
    userName: String,
    userPassword: String
})

const Account = mongoose.model('Account', AccountSchema);
export default Account;