import './App.css'
import styled from 'styled-components'
import { Button, Input, Tag, message } from 'antd'
import { useState, useEffect, useRef } from 'react';
import ChatRoom from "./container/ChatRoom"
import SignIn from './container/SignIn'
import useChat from './hook/useChat';

const LOCALSTORAGE_KEY = 'save-me';

const Wrapper = styled.div`
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  height: 100vh;
  width: 500px;
  margin: auto;
`

function App() {
  const saveMe = localStorage.getItem(LOCALSTORAGE_KEY);

  const { messages, status, signedIn, sendMessage, sendClearTask, requireSignIn, requireSignUp, sendClearAccountTask, status_display } = useChat();
  const [userName, setUserName] = useState('');
  const [msgBody, setMsgBody] = useState('');
  const [me, setMe] = useState(saveMe, '');
  const bodyRef = useRef(null);

  useEffect(() => {
    status_display(status)
  }, [status]);

  return (
    <Wrapper>
      {(signedIn) ? (
        <ChatRoom
          messages={messages}
          sendClearTask={sendClearTask}
          me={me}
          bodyRef={bodyRef}
          sendMessage={sendMessage}
          status_display={status_display}
          setBody={setMsgBody}
          body={msgBody}
        ></ChatRoom>
      ) : (
        <SignIn
          me={me}
          setMe={setMe}
          status_display={status_display}
          requireSignIn={requireSignIn}
          requireSignUp={requireSignUp}
          cleaarAccount={sendClearAccountTask}
        ></SignIn>
      )}
    </Wrapper>
  );
}

export default App
