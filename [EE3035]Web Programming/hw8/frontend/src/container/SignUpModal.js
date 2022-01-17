import { Modal, Input } from 'antd';

export default function SignUpModal(props){
    return (
        <Modal
            title="Enter your user name and password: "
            visible={props.visible}
            onOk={props.onSignUp}
            onCancel={props.onCancel}
            onText="Sign Up"
        >
            <Input placeholder="Name" ref={props.nameRef}></Input>
            <Input.Password placeholder="Password" ref={props.passwordRef}></Input.Password>
        </Modal>
    )
}