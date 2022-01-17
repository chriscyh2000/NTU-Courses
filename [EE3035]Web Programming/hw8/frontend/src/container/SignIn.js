import { Input, Button, Space } from 'antd';
import {UserOutlined} from "@ant-design/icons"
import Title from "../components/Title.js"
import { useState, useRef } from 'react'
import SignUpModal from './SignUpModal'

const SignIn = (props) => {
    const {
        me, setMe, status_display, requireSignIn, requireSignUp, clearAccount
    } = props;

    const [password, setPassword] = useState('');
    const [visible, setVisible] = useState(false);
    const nameRef = useRef(null);
    const passwordRef = useRef(null);

    const handleClick = () => {
        setVisible(true);
    }
    const handleCancel = () => {
        setVisible(false);
    }

    const handleSignUp = () => {
        setVisible(false);
        const name = nameRef.current.state.value;
        const passWord = passwordRef.current.state.value || "";
        nameRef.current.state.value = "";
        passwordRef.current.state.value = "";
        if(name.trim() === "" || name === ""){
            status_display({
                type: "error",
                msg: "Please enter a valid username."
            })
            return;
        }
        requireSignUp(name, passWord);
    }

    return (
        <>
            <Title>
                <h1>My Chat Room :)</h1>
                <Space>

                <Button type="secondary" onClick={handleClick}>
                    Sign Up
                </Button> 

                <Button type="primary" danger onClick={()=>{
                    clearAccount(); 
                    status_display({
                    type: "success",
                    msg: "clear Account"
                    })
                }}>
                    Clear
                </Button>

                </Space>
            </Title>
            <Input.Search
                prefix={<UserOutlined />}
                value={me} 
                onChange={(e)=> setMe(e.target.value)}
                enterButton="Sign In"
                placeholder="Enter your name"
                onSearch={(me) => {
                    console.log("clicked!");
                    if(me === ""){
                        status_display({
                            type: "error",
                            msg: "Missing user name!"
                        })
                    }
                    else requireSignIn(me, password);
                }}
            ></Input.Search>
            <Input.Password
                value={password} 
                onChange={(e)=> setPassword(e.target.value)}
                placeholder="Password"
            ></Input.Password>
            <SignUpModal
                visible={visible}
                onCancel={handleCancel}
                onSignUp={handleSignUp}
                nameRef={nameRef}
                passwordRef={passwordRef}
            ></SignUpModal>
        </>
    );
}

export default SignIn;