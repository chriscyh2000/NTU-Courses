import { useState } from 'react';
import bcrypt from "bcryptjs";
import { message } from 'antd'
const client = new WebSocket('ws://localhost:4000');

const sendData = async (data) => {
    await client.send(JSON.stringify(data));
};

const saltRounds = 10;

const useChat = () => {
    const [messages, setMessages] = useState([]);
    const [signedIn, setSignedIn] = useState(false);
    const [status, setStatus] = useState({});
    const [passwordCache, setPasswordCache] = useState("");

    const status_display = (payload) => {
        if(payload.msg){
            const { type, msg } = payload;
            const content = { content: msg, duration: 0.5 };
            switch(type){
                case 'success':{
                    message.success(content);
                    break;
                }
                case 'info':{
                    message.info(content);
                    break;
                }
                default: {
                    message.error(content);
                    break;
                }
            }
        }
    }
    const sendClearTask = async (Me) => {
        await sendData(['clear', { me: Me }]);

    }
    const sendMessage = async (payload) => {
        await sendData(["input", payload]);
        console.log(payload);
    }
    const password_hash = async (password) => {
        const hash = await bcrypt.hash(password, saltRounds);
        return hash;
    } 
    const requireSignIn = async (name, password) => {
        console.log("loaded into requireSignIn");
        const payload = { userName: name };
        sendData(['sign_in', payload]);
        setPasswordCache(password);
    }
    const requireSignUp = async (name, password) => {
        const hash = await password_hash(password);
        const payload = { userName: name, userPassword: hash };
        await sendData(['sign_up', payload]);
    }
    const sendClearAccountTask = async () => {
        await sendData(['clear_account', null]);
    }
    client.onmessage = async (bytestr) => {
        const { data } = bytestr;
        const [task, payload] = JSON.parse(data);
        switch(task){
            case 'status': {
                setStatus(payload);
                break;
            }
            case 'output': {
                console.log(messages);
                setMessages(() => 
                    [...messages, ...payload]
                );
                break;
            }
            case 'init': {
                setStatus({type: 'success', msg: 'Successfully loaded message record.'});
                setMessages(payload);
                break;
            }
            case 'clear': {
                setMessages(payload);
                break;
            }
            case 'signIn': {
                if(payload !== 'Fail'){
                    const check = await bcrypt.compare(passwordCache, payload);
                    if (check){
                        setSignedIn(true)
                        setPasswordCache("")
                        setStatus({
                            type: 'success',
                            msg: 'Log In.'
                        })
                    }else{
                        setSignedIn(false)
                        setStatus({
                            type: 'error',
                            msg: 'Wrong password!'
                        })
                    }
                }
                else{
                    setSignedIn(false);
                }
                break;
            }
            default: break;
        }
    }
    return {
        messages,
        status,
        signedIn,
        sendMessage,
        sendClearTask,
        requireSignIn,
        requireSignUp,
        sendClearAccountTask,
        status_display
    };
};
export default useChat;