import WebSocket from "ws";
import express from "express";
import mongoose from 'mongoose';
import dotenv from 'dotenv-defaults';
import http from "http";
import Message from "./model/message";
import { sendData, sendStatus, initData } from "./wssConnect";
import Account from "./model/account";


dotenv.config();
if (!process.env.MONGO_URL) {
    console.error('No MONGO_URL')
    process.exit(1)
}
mongoose.connect(process.env.MONGO_URL, {
    useNewUrlParser: true,
    useUnifiedTopology: true,
}).then(() => console.log("mongo db connection created"));

const db = mongoose.connection;
const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({server});

const broadcastMsg = (data, status) => {
    wss.clients.forEach((client) => {

        sendData(data, client);
        sendStatus(status, client);
    })
}

db.once('open', () => {
    console.log("MongoDB connected!");
    wss.on('connection', (ws) => {
        initData(ws);
        ws.onmessage = async (bytestr) => {
            const { data } = bytestr;
            const [task, payload] = JSON.parse(data);
            switch(task){
                case 'input': {
                    const { sender, receiver, body } = payload;
                    const msg = new Message({ sender, receiver, body });
                    try{
                        await msg.save();
                    }catch(err){
                        throw new Error("Message DB save error: " + err);
                    } 
                    broadcastMsg(['output', [payload]], { type: 'success', msg: 'Message sent.' });
                    break;
                }
                case 'clear': {
                    try{
                        await Message.deleteMany({ sender: payload.me });
                        console.log("Deleting all messages successed!");
                    }catch(err){
                        throw new Error("Message DB save error: " + err);
                    }
                    Message.find().sort({ created_at: -1 }).limit(100).exec((err, res) => {
                        if(err) throw err;
                        broadcastMsg(['clear', res], { type: 'success', msg: 'Deletion Successed.' });
                    })
                    break;
                }
                case 'clear_account': {
                    try{
                        await Account.deleteMany({});
                        await Message.deleteMany({});
                        console.log("Deleting all accounts successed!");
                    }catch(err){
                        throw new Error("Account DB save error: " + err);
                    }
                    sendData({ type: "success", msg: "Deletion Successed" }, ws);
                    break;
                }
                case 'sign_in': {
                    const { userName } = payload;
                    const user = await Account.findOne({ userName });
                    if(user){
                        sendData(["signIn", user.userPassword], ws)                        
                    }else{
                        sendData(["signIn", "Fail"], ws)
                        sendStatus({
                            type: 'error',
                            msg: 'User not found!'
                        }, ws)
                    }
                    break;
                }
                case 'sign_up': {
                    const { userName, userPassword } = payload;
                    const usr = Account.find({ userName: userName });
                    if(usr){
                        sendData(['status', { type: "error", msg: "User already exist." }], ws);
                    }
                    else{
                        const account = new Account({ userName, userPassword });
                        try{
                            await account.save();
                        }catch(err){
                            throw new Error("Account DB save error: " + err);
                        }
                        sendData(['status', { type: "success", msg: "Sign-up succeeded." }], ws);
                    }
                    
                    break;
                }
                default: break;
            }
        }
    });

    const port = process.env.PORT || 4000;
    server.listen(port, () =>{
        console.log(`Listening on http://localhost:${port}`);
    })
})