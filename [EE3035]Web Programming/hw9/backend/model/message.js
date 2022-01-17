import mongoose from 'mongoose';
const Schema = mongoose.Schema;

const MessageSchema = new Schema({
    sender: String,
    receiver: String,
    body: String
})

const Message = mongoose.model('Message', MessageSchema);
export default Message;