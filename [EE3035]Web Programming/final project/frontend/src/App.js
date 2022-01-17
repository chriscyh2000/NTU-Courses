import './App.css';
import React from 'react';
import 'antd/dist/antd.css';
import './index.css';
import styled from 'styled-components'
import { MenuOutlined, SmileOutlined } from '@ant-design/icons'
import { PageHeader, Tabs, Button, Descriptions, Input } from 'antd';
import { useState, useRef } from 'react'
import bcrypt from "bcryptjs";
import { useMutation } from '@apollo/react-hooks'
import { LOGIN_MUTATION, SIGNUP_MUTATION } from "./graphql/index";
import status from './hook/status';
import SignInModal from './component/SignInModal';
import SignUpModal from './component/SignUpModal';
import LeftMenuModal from './container/LeftMenuModal';
import Profile from './container/Profile';
import CreatePost from './container/CreatePost';
import StoreBox from './container/StoreBox';
import Favorite from './container/Favorite';
import Pro from './container/Pro'
import MyPost from './container/MyPost';
import Contact from './container/Contact'


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
  // localStorage.clear();
  const saltRounds = 10;
  const [signedIn, setSignedIn] = useState(false);
  const [user, setUser] = useState(null);
  const [signUpVisible, setSignUpVisible] = useState(false);
  const [signInVisible, setSignInVisible] = useState(false);
  const [currentPage, setCurrentPage] = useState("home");

  const [signup] = useMutation(SIGNUP_MUTATION);
  const [login] = useMutation(LOGIN_MUTATION);

  const password_hash = async (password) => {
    const hash = await bcrypt.hash(password, saltRounds);
    return hash;
  } 


  // Init by localStorage:
  
  const init = async() => {
    const userName = localStorage.getItem('name');
    const userPassword = localStorage.getItem('password');
    if(userName !== null && userPassword !== null){
      const initPayload = await login({
        variables: {
            name: userName,
            password: userPassword
        }
      });
      if(initPayload.data.login.message === 'User not found.'){
        console.log("user does not exist.");
        localStorage.clear();
        return;
      }
      else{
          const check = await bcrypt.compare(userPassword, initPayload.data.login.user.password);
          if(check){
              setUser(initPayload.data.login.user);
              setSignedIn(true);
              setSignInVisible(false);
          }
          else{
            localStorage.clear();
          }
      }
    }
  }
  if(user === null && localStorage.getItem('name') !== null) init();

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
        setSignUpVisible(false);
    }
    else{
        status.display({ 
            type: 'error',
            msg: 'User name has been used.'
        })
    }
  }
  const handleSignUp = () => {
    const name = nameRef1.current.state.value;
    const passWord = passwordRef1.current.state.value || "";
    const enterAgain = enterAgainRef.current.state.value || "";
    nameRef1.current.state.value = "";
    passwordRef1.current.state.value = "";
    enterAgainRef.current.state.value = "";
    if(name === undefined || name.trim() === "" || name === ""){
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
            localStorage.setItem('name', name);
            localStorage.setItem('password', password);
        }
        else{
            status.display({type: "error", msg: "Wrong password."});
            setSignedIn(false);
        }
    }
  }
  const handleSignIn = () => {
    const name = nameRef2.current.state.value;
    const passWord = passwordRef2.current.state.value || "";
    nameRef2.current.state.value = "";
    passwordRef2.current.state.value = "";
    if(name === undefined ||name.trim() === "" || name === ""){
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
  const [menuOut, setMenuOut] = useState(false);

  // for tabs:
  const [activeKey, setActiveKey] = useState(1);
  const changeActiveKey = (newKey) => {
    setActiveKey(newKey);
  }
  // refetch:
  const refetch = async () => {
    const payload = await login({
      variables: {
          name: user.name,
          password: 'temp'
      }
    })
    setUser(payload.data.login.user);
  }

  console.log("in app.js")
  return(
    <>
      <Wrapper> {/* className={theme === "dark"? "wrapper_dark":"wrapper_light"} */}
        <PageHeader
          className="site-page-header-responsive"
          onBack={() => {}}
          title="NTU CSIEat 😍"
          backIcon={<MenuOutlined onClick={() => setMenuOut(!menuOut)}/>}

          extra={(!signedIn) ? ([
              <Button key="2" onClick={() => { 
                if(localStorage.getItem('name') === null) 
                  setSignUpVisible(true) 
                }}
              >Sign Up</Button>,
              <Button key="1" type="primary" onClick={() => { 
                if(localStorage.getItem('name') === null)
                  setSignInVisible(true) 
                }}>
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
                    localStorage.clear();
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
            (currentPage === "home")?
              <div className='why2'>
                <Tabs defaultActiveKey="1" centered="true" activeKey={activeKey} onChange={changeActiveKey}>
                  <TabPane tab="Popular" key="1">
                    <StoreBox type="popular" refetch={refetch} userFavs={(user === null) ? null : user.favs} uid={(user === null) ? null : user.id}/>
                  </TabPane>
                  <TabPane tab="Recent" key="2">
                    <StoreBox type="recent" refetch={refetch} userFavs={(user === null) ? null : user.favs} uid={(user === null) ? null : user.id}/>
                  </TabPane>
                  <TabPane tab="Find" key="3">
                    <StoreBox type="find" refetch={refetch} userFavs={(user === null) ? null : user.favs} uid={(user === null) ? null : user.id}/>
                  </TabPane>
                </Tabs>
              </div> :(
            (currentPage === "setting")?
              <h1>This is setting</h1> : (
            (currentPage === "create")?
              <CreatePost user={user} refetch={refetch}/> : (
            (currentPage === "upgrade")?
              <Pro /> : (
            (currentPage === "profile") ?
              <Profile user={user} refetch={refetch}/> : 
            (currentPage === "favorite") ?  
              <Favorite refetch={refetch} user={user}/> :
            (currentPage === "my") ?
              <MyPost user={user} /> :
            (currentPage === "contact") ?
              <Contact /> :
              <></>
            ))))
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
        <LeftMenuModal page={setCurrentPage} menu={setMenuOut} />
      </div>: <></>}
      
    </>
  );
}

export default App;