import './App.css'
import styled from 'styled-components'
import { useState, useRef } from 'react';
import ChatRoom from "./container/ChatRoom"
import SignIn from './container/SignIn'
import hooks from './hook/hooks';

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
  const { signedIn, setSignedIn } = hooks.useSignedIn();
  
  const [msgBody, setMsgBody] = useState('');
  const [me, setMe] = useState(saveMe, '');
  const bodyRef = useRef(null);

  return (
    <Wrapper>
      {(signedIn) ? (
        <ChatRoom
          me={me}
          bodyRef={bodyRef}
          setBody={setMsgBody}
          body={msgBody}
        ></ChatRoom>
      ) : (
        <SignIn
          me={me}
          setMe={setMe}
          setSignedIn={setSignedIn}
        ></SignIn>
      )}
    </Wrapper>
  );
}
export default App
