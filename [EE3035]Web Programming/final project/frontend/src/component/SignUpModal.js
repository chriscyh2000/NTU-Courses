import { Modal, Input } from 'antd';
import { useState, useEffect } from 'react';

export default function SignUpModal(props){
    const [value1, setValue1] = useState('');
    const [value2, setValue2] = useState('');
    const [same, setSame] = useState(true);

    useEffect(() => {
        setSame(value1 === value2 || (value1 === '' && value2 === ''));
    }, [value1, value2])
    return (
        <Modal
            title="Sign Up: "
            visible={props.visible}
            onOk={props.onSignUp}
            onCancel={props.onCancel}
            onText="Sign Up"
        >
            <Input placeholder="Name" ref={props.nameRef}></Input>
            <Input.Password 
                placeholder="Password" 
                ref={props.passwordRef} 
                onChange={() => {setValue1(props.passwordRef.current.state.value)}}
            ></Input.Password>
            <Input.Password 
                placeholder="Enter your password again" 
                ref={props.enterAgainRef}
                onChange={() => {setValue2(props.enterAgainRef.current.state.value)}}
            ></Input.Password>
            {
                (same) ? (
                    <></>
                ) : (
                    <p style={{color: "red"}}>The password you input is not the same</p>
            )}
        </Modal>
    )
}