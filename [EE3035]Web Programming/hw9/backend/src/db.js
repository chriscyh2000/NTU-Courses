import mongoose from 'mongoose';
const Schema  = mongoose.Schema;

const userSchema = new Schema({
    name: { type: String, required: true },
    password: { type: String, required: true }
    //chatBoxes: [{ type: mongoose.Types.ObjectId, ref: 'ChatBox' }],
});

const messageSchema = new Schema({
    sender: { type: mongoose.Types.ObjectId, ref: 'User' },
    body: { type: String, required: true }
});

const chatBoxSchema = new Schema({
    //users: [{ type: mongoose.Types.ObjectId, ref: 'User' }],
    name: { type: String, required: true },
    messages: [{ type: mongoose.Types.ObjectId, ref: 'Message' }]
});

const UserModel = mongoose.model('User', userSchema);
const MessageModel = mongoose.model('Message', messageSchema);
const ChatBoxModel  = mongoose.model('ChatBox', chatBoxSchema);

const db = {
    UserModel,
    MessageModel, 
    ChatBoxModel
}

export default db;