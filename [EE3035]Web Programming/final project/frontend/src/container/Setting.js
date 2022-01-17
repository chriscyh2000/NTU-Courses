import './App.css';
import React from 'react';
import 'antd/dist/antd.css';
import './index.css';
import styled from 'styled-components'
import { MenuOutlined, SmileOutlined } from '@ant-design/icons'
import { PageHeader, Tabs, Button, Descriptions } from 'antd';
import { useState, useRef } from 'react'
import bcrypt from "bcryptjs";
import { useMutation } from '@apollo/react-hooks'
import { LOGIN_MUTATION, SIGNUP_MUTATION } from "../graphql/index";
import status from '../hook/status';
import SignInModal from './SignInModal';
import SignUpModal from './SignUpModal';
import LeftMenuModal from './LeftMenuModal';
import { normalizeConfig } from '@apollo/client/cache/inmemory/helpers';

const Wrapper = styled.div`
  minwidth: 600px;
  display: flex;
  flex-direction: column;
  align-items: center;
  height: 100vh;
  width: 100%;
  margin: auto;
`
const { TabPane } = Tabs;

const renderContent = (column = 2) => (
  <Descriptions size="small" column={column}>
    <Descriptions.Item label=""></Descriptions.Item>
    <Descriptions.Item label=""></Descriptions.Item>
  </Descriptions>
);

const Content = ({ children, extra }) => (
  <div className="content">
    <div className="main">{children}</div>
    <div className="extra">{extra}</div>
  </div>
);
const App= () => {
  const saltRounds = 10;
  const [signedIn, setSignedIn] = useState(false);
  const [user, setUser] = useState(null);
  const [signUpVisible, setSignUpVisible] = useState(false);
  const [signInVisible, setSignInVisible] = useState(false);

  const [signup] = useMutation(SIGNUP_MUTATION);
  const [login] = useMutation(LOGIN_MUTATION);

  const password_hash = async (password) => {
    const hash = await bcrypt.hash(password, saltRounds);
    return hash;
  } 

  // For SignUp:
  
  const nameRef1 = useRef(null);
  const passwordRef1 = useRef(null);
  const enterAgainRef = useRef(null);
  const RequireSignUp = async (name, password) => {
    const hash = await password_hash(password);
    const payload = await signup({
        variables: {
            name: name,
            password: hash
        }
    })
    if(payload.data.signup.message === 'Sign-up succeeded.'){
        status.display({ 
            type: 'success',
            msg: 'Sign-up succeeded.'
        })
    }
    else{
        status.display({ 
            type: 'error',
            msg: 'User name has been used.'
        })
    }
  }
  const handleSignUp = () => {
    setSignUpVisible(false);
    const name = nameRef1.current.state.value;
    const passWord = passwordRef1.current.state.value || "";
    const enterAgain = enterAgainRef.current.state.value || "";
    nameRef1.current.state.value = "";
    passwordRef1.current.state.value = "";
    enterAgainRef.current.state.value = "";
    if(name.trim() === "" || name === ""){
        status.display({
            type: "error",
            msg: "Please enter a valid username."
        })
        return;
    }
    if(passWord !== enterAgain){
      status.display({
          type: "error",
          msg: "The password tou input is not the same."
      })
      return;
    }
    RequireSignUp(name, passWord);
  }
  const handleCancelSignedUp = () => {
    setSignUpVisible(false);
  }

  // For SignIn:
  const nameRef2 = useRef(null);
  const passwordRef2 = useRef(null);
  const RequireSignIn = async (name, password) => {
    const payload = await login({
      variables: {
          name: name,
          password: password
      }
    })
    if(payload.data.login.message === 'User not found.'){
        console.log("user does not exist.");
        status.display({ type: "error", msg: "User does not exist." });
        setSignedIn(false);
        return;
    }
    else{
        const check = await bcrypt.compare(password, payload.data.login.user.password);
        if(check){
            status.display({type: "success", msg: "Successfully log in."});
            setUser(payload.data.login.user);
            setSignedIn(true);
            setSignInVisible(false);
        }
        else{
            status.display({type: "error", msg: "Wrong password."});
            setSignedIn(false);
        }
    }
  }
  const handleSignIn = () => {
    setSignUpVisible(false);
    const name = nameRef2.current.state.value;
    const passWord = passwordRef2.current.state.value || "";
    nameRef2.current.state.value = "";
    passwordRef2.current.state.value = "";
    if(name.trim() === "" || name === ""){
        status.display({
            type: "error",
            msg: "Please enter a valid username."
        })
        return;
    }
    RequireSignIn(name, passWord);
  }
  const handleCancelSignedIn = () => {
    setSignInVisible(false);
  }

  // for menu:
  const [menuOut, setmenuOut] = useState(false);
  const [theme, setTheme] = useState("light");

  return(
    <>
      <Wrapper>
        <PageHeader
          className="site-page-header-responsive"
          onBack={() => window.history.back()}
          title="NTU CSIEat 😍"
          backIcon={<MenuOutlined onClick={() => setmenuOut(!menuOut)}/>}

          extra={(!signedIn) ? ([
              <Button key="2" onClick={() => { setSignUpVisible(true) }}>Sign Up</Button>,
              <Button key="1" type="primary" onClick={() => { setSignInVisible(true) }}>
                Sign In
              </Button>
            ]) : (
              <div className="rightupper">
                <div className='why'><SmileOutlined /></div>
                <div className="why">
                  <p>&nbsp;&nbsp;Welcome, {user.name}&nbsp;&nbsp;</p>
                </div>
                <div className="why">
                  <Button key="1" onClick={() => { 
                    setSignedIn(false); 
                    setUser(null);
                  }}>
                    Log Out
                  </Button>
                </div>
              </div>
            )
          }

          footer={
            <></>
          }
        >
          <Content >{renderContent()}</Content>
        </PageHeader>    
        <SignUpModal 
          visible={signUpVisible}
          onCancel={handleCancelSignedUp}
          onSignUp={handleSignUp}
          nameRef={nameRef1}
          passwordRef={passwordRef1}
          enterAgainRef={enterAgainRef}
        />
        <SignInModal 
          visible={signInVisible}
          onCancel={handleCancelSignedIn}
          onSignIn={handleSignIn}
          nameRef={nameRef2}
          passwordRef={passwordRef2}
        />
      </Wrapper>
      {menuOut?
      <div className="menu" >
        <LeftMenuModal theme={theme}/>
      </div>: <></>}
      
    </>
  );
}

export default App;