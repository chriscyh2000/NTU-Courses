import { useState, useRef } from "react";
import AddModal from './AddModal';
import { Button, Input, Typography, Tabs, Space } from 'antd';

import Title from "../components/Title";
import Message from "../components/Message";

const { Paragraph } = Typography
const { TabPane } = Tabs

function useForceToUpdate(){
    const [val, setVal] = useState(0);
    return () => setVal(val+1);
}

const ChatRoom = (props) => {
    const [key, setKey] = useState(0);
    const [visible, setVisible] = useState(false);
    const addRef = useRef(null);
    const [panes, setPanes] = useState([]);
    const [inChatBox, setInChatBox] = useState(false);
    // const forceUpdate = useForceToUpdate();

    const {
        messages, 
        sendClearTask, 
        me, 
        bodyRef, 
        sendMessage, 
        status_display, 
        setBody,
        body
    } = props

    const handleTabsOnChange = (key) => {
        setKey(key);
        setInChatBox(true);
    } 

    const removeChatBox = (targetKey) => {
        let newPanes = panes;
        let nextKey = key;
        newPanes.splice(targetKey, 1);
        if(key === targetKey){
            nextKey = 0;
            setInChatBox(false);
        }
        setKey(nextKey);
        setPanes(newPanes);
    }
    const addChatBox = () => {
        setVisible(true);
    }

    const handleCancel = () => {
        setVisible(false);
    }

    const handleTabsEdit = (targetKey, action) => {
        if(action === "remove"){
            removeChatBox(targetKey);
            // forceUpdate();
        }
        else if(action === "add"){
            addChatBox();
        }
    }
    const handleCreate = () => {
        setVisible(false);
        const friendName = addRef.current.state.value;
        addRef.current.state.value = "";
        if(friendName.trim() === "" || friendName === ""){
            status_display({
                type: "error",
                msg: "Please enter a valid username."
            });
            return;
        }
        if(panes.some((p) => (p === friendName.trim()))){
            status_display({
                type: "error",
                msg: "You have a chatbox already."
            });
            return;
        }
        const nextKey = panes.length;
        setPanes([...panes, friendName]);
        setKey(nextKey);
    }
    return (
        <>
            <Title>
                <h1>{me}'s Chat Room</h1>
                <Button
                    type="primary"
                    danger onClick={() => {
                        sendClearTask(me);
                    }}
                >
                    Clear
                </Button>
            </Title>
            <Message>
                <Tabs
                    type="editable-card"
                    onChange={handleTabsOnChange}
                    activeKey={key}
                    onEdit={handleTabsEdit}
                >{
                    panes.map((name, i) => (
                        <TabPane tab={name} key={i} closable={true} style={{ height: "200px", overflow: "auto" }}>
                            {(messages.length === 0) ? (
                                <p style={{ color: '#ccc' }}>
                                    No messages...
                                </p>
                            ) : (
                                (messages.map(({sender, receiver, body}, i) => {
                                    console.log(`sender: ${sender} me: ${me}, receiver: ${receiver} pane: ${panes[key]}, body: ${body}`);
                                    return (sender === me && receiver === panes[key]) ? (
                                        <p className="App-message" key={i} align="right">
                                            <Space align="end">
                                                <Paragraph type="secondary" ellipsis={{rows: 1000}} style={{maxWidth: "200px", margin:"0", borderRadius:"5px", backgroundColor: "#bbbbbb", padding: "0 5px", textAlign:"left"}}  >{body}</Paragraph>
                                                {` ${sender}`}
                                            </Space>
                                        </p>
                                    ) : (
                                        (sender === panes[key] && receiver === me) ? (
                                            <p className="App-message" key={i}>
                                                {`${sender} `}
                                                <Space align="end">
                                                    <Paragraph type="secondary" ellipsis={{rows: 1000}} style={{maxWidth: "200px", margin:"0", borderRadius:"5px", backgroundColor: "#0000ff66", color:"white", padding: "0 5px", textAlign:"left"}}  >{body}</Paragraph>
                                                </Space>
                                            </p>
                                        ) : (
                                            (sender === me && receiver === me && panes[key] === me) ? (
                                                <p className="App-message" key={i} align="right">
                                                    <Space align="end">
                                                        <Paragraph type="secondary" ellipsis={{rows: 1000}} style={{maxWidth: "200px", margin:"0", borderRadius:"5px", backgroundColor: "#bbbbbb", padding: "0 5px", textAlign:"left"}}  >{body}</Paragraph>
                                                        {sender}
                                                    </Space>
                                                </p>
                                            ) : (<></>)
                                        )
                                    )
                                })
                            ))}
                        </TabPane>
                    ))
                }</Tabs>
            </Message>
            <Input.Search
                ref={bodyRef}
                value={body}
                onChange={(e) => { setBody(e.target.value); }}
                enterButton="Send"
                placeholder="Type a message here..."
                onSearch={(msg) => {
                    if(!msg){
                        status_display({
                            type: "error",
                            msg: "Please enter some messages..."
                        })
                        return;
                    }
                    if(!inChatBox){
                        status_display({
                            type: "error",
                            msg: "Please choose a chatbox first..."
                        })
                        return;
                    }
                    sendMessage({ sender: me, receiver: panes[key], body: body });
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