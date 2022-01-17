import { useEffect } from "react";
import { Typography, Space } from 'antd';
import { useQuery } from '@apollo/client';
import { CHATBOX_QUERY, CHATBOX_SUBSCRIPTION } from "../grapgql/index";

const ChatBox = (props) => {
    const { Paragraph } = Typography;
    const { me, friend } = props;
    const arr = [me, friend];
    const chatBoxName = arr.sort().join('_');
    const { data, loading, subscribeToMore } = useQuery(CHATBOX_QUERY, {
        variables: { name: chatBoxName }
    });

    useEffect(() => {
        try{
            subscribeToMore({
                document: CHATBOX_SUBSCRIPTION,
                variables: {
                    name: chatBoxName
                },
                updateQuery: (prev, { subscriptionData }) => {
                    if(!subscriptionData.data) return prev;
                    
                    return {
                        chatbox: {
                            ...prev.chatbox,
                            messages: [...prev.chatbox.messages, subscriptionData.data.chatBox.data]
                        }
                    };
                }
            })
        }
        catch(e){}
    }, [subscribeToMore]);

    if(loading) return (
        <div>
            <p>
                loading...
            </p>
        </div>
    )
    else if(!data || data.chatbox.messages.length === 0) return (
        <div>
            <p>
                No messages...
            </p>
        </div>
    )
    return (
        <div>
            {  data.chatbox.messages.map(({ sender, body }, i)=> {
                return ((sender.name === me) ? (
                    <p className="App-message" key={i} align="right">
                        <Space align="end">
                            <Paragraph type="secondary" ellipsis={{rows: 1000}} style={{maxWidth: "200px", margin:"0", borderRadius:"5px", backgroundColor: "#bbbbbb", padding: "0 5px", textAlign:"left"}}  >{body}</Paragraph>
                            {` ${me}`}
                        </Space>
                    </p>
                ) : (
                    <p className="App-message" key={i}>
                        {`${sender.name} `}
                        <Space align="end">
                            <Paragraph type="secondary" ellipsis={{rows: 1000}} style={{maxWidth: "200px", margin:"0", borderRadius:"5px", backgroundColor: "#0000ff66", color:"white", padding: "0 5px", textAlign:"left"}}  >{body}</Paragraph>
                        </Space>
                    </p>
                ))
            })}
        </div>
    )
}

export default ChatBox;