import { Input, Button, Space } from 'antd';
import {UserOutlined} from "@ant-design/icons"
import Title from "../components/Title.js"
import { useState, useRef } from 'react'
import SignUpModal from './SignUpModal'
import { LOGIN_MUTATION, SIGNUP_MUTATION } from "../grapgql/index";
import status from '../hook/statusDisplay';
import bcrypt from "bcryptjs";
import { useMutation } from '@apollo/react-hooks'

const SignIn = (props) => {
    const saltRounds = 10;
    const [signup] = useMutation(SIGNUP_MUTATION);
    const [login] = useMutation(LOGIN_MUTATION);
    // const { loading, error, data, subscribeToMore } = useQuery(USER_QUERY);
    const password_hash = async (password) => {
        const hash = await bcrypt.hash(password, saltRounds);
        return hash;
    } 
    const RequireSignIn = async (name, password) => {
        console.log("loaded into requireSignIn");
        // subscribeToMore({
        //     document: 
        // })
        const payload = await login({
            variables: {
                name: name,
                password: password
            }
        })
        console.log(payload);
        if(payload.data.login.message === 'User not found.'){
            console.log("user does not exist.");
            status.display({type: "error", msg: "User does not exist."});
            setSignedIn(false);
            return;
        }
        else{
            const check = await bcrypt.compare(password, payload.data.login.password);
            if(check){
                status.display({type: "success", msg: "Successfully log in."});
                setSignedIn(true);
            }
            else{
                status.display({type: "error", msg: "Wrong password."});
                setSignedIn(false);
            }
        }
    }

    const RequireSignUp = async (name, password) => {
        const hash = await password_hash(password);
        const payload = await signup({
            variables: {
                name: name,
                password: hash
            }
        })
        console.log(payload);
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

    const { me, setMe, setSignedIn } = props;

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
            status.display({
                type: "error",
                msg: "Please enter a valid username."
            })
            return;
        }
        RequireSignUp(name, passWord);
    }

    return (
        <>
            <Title>
                <h1>My Chat Room</h1>
                <Space>

                <Button type="secondary" onClick={handleClick}>
                    Sign Up
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
                        status.display({
                            type: "error",
                            msg: "Missing user name!"
                        })
                    }
                    else RequireSignIn(me, password);
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