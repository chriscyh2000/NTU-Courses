import { Modal, Input } from 'antd';

export default function SignUpModal(props){
    return (
        <Modal
            title="Sign In: "
            visible={props.visible}
            onOk={props.onSignIn}
            onCancel={props.onCancel}
            onText="Sign In"
        >
            <Input placeholder="Name" ref={props.nameRef}></Input>
            <Input.Password placeholder="Password" ref={props.passwordRef}></Input.Password>
        </Modal>
    )
}