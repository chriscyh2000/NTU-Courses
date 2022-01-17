import { useState, useRef } from "react";
import AddModal from './AddModal';
import { Input, Tabs } from 'antd';
import status from '../hook/statusDisplay';
import Title from "../components/Title";
import Message from "../components/Message";
import { useMutation } from '@apollo/client';
import { CREATE_CHATBOX_MUTATION, CREATE_MESSAGE_MUTATION } from "../grapgql/index";
import ChatBox from '../components/ChatBox';

const ChatRoom = (props) => {
    const { TabPane } = Tabs;
    const [key, setKey] = useState(0);
    const [visible, setVisible] = useState(false);

    const addRef = useRef(null);
    const [panes, setPanes] = useState([]);
    const [inChatBox, setInChatBox] = useState(false);
    const [addChatbox] = useMutation(CREATE_CHATBOX_MUTATION);
    const [sendMessage] = useMutation(CREATE_MESSAGE_MUTATION);

    const {
        me, 
        bodyRef, 
        setBody,
        body
    } = props


    const handleTabsOnChange = (key) => {
        setKey(key);
        setInChatBox(true);
    } 

    const addChatBox = () => {
        setVisible(true);
    }

    const handleCancel = () => {
        setVisible(false);
    }
    const removeChatBox = (targetKey) => {
        let newPanes = [];
        for(let i = 0; i < panes.length; ++i){
            if(i.toString() !== targetKey){
                newPanes.push(panes[i]); 
            } 
        }
        setPanes(newPanes);
    }
    const handleTabsEdit = (targetKey, action) => {
        if(action === "remove"){
            console.log('hello')
            removeChatBox(targetKey);
        }
        else if(action === "add"){
            addChatBox();
        }
    }
    const handleCreate = async () => {
        setVisible(false);
        const friendName = addRef.current.state.value;
        addRef.current.state.value = "";
        if(friendName.trim() === "" || friendName === "" || friendName.trim() === me){
            status.display({
                type: "error",
                msg: "Please enter a valid username."
            });
            return;
        }
        if(panes.some((p) => (p === friendName.trim()))){
            status.display({
                type: "error",
                msg: "You have a chatbox already."
            });
            return;
        }
        const nextKey = panes.length;
        await addChatbox({
            variables: {
                name1: friendName,
                name2: me
            }
        });
        setPanes([...panes, friendName]);
        setKey(nextKey-1);
    }
    return (
        <>
            <Title>
                <h1>{me}'s Chat Room</h1>
            </Title>
            <Message>
                <Tabs
                    type="editable-card"
                    onChange={handleTabsOnChange}
                    activeKey={key}
                    onEdit={handleTabsEdit}
                >{
                    panes.map((name, i) => {
                            return (
                                <TabPane tab={name} key={i} closable={true} style={{ height: "200px", overflow: "auto" }}>
                                    <ChatBox me={me} friend={name} ></ChatBox>
                                </TabPane>
                            )
                        })
                    }
                </Tabs>
            </Message>
            <Input.Search
                ref={bodyRef}
                value={body}
                onChange={(e) => { setBody(e.target.value); }}
                enterButton="Send"
                placeholder="Type a message here..."
                onSearch={(msg) => {
                    if(!msg){
                        status.display({
                            type: "error",
                            msg: "Please enter some messages..."
                        })
                        return;
                    }
                    if(!inChatBox){
                        status.display({
                            type: "error",
                            msg: "Please choose a chatbox first..."
                        })
                        return;
                    }
                    sendMessage({
                        variables: {
                            sender: me,
                            receiver: panes[key],
                            body: msg
                        }
                    })
                    setBody('');
                }}
            ></Input.Search>
            <AddModal
                visible={visible}
                onCancel={handleCancel}
                onCreate={handleCreate}
                inputRef={addRef}
            ></AddModal>
        </>
    );
}
export default ChatRoom;