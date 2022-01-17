import { Modal, Input } from 'antd';

export default function CreateStoreModal(props){
    return (
        <Modal
            title="Input the information about the store: "
            visible={props.visible}
            onOk={props.onCreateStore}
            onCancel={props.onCancel}
            onText="Create Store"
        >
            <Input 
                placeholder="Store Name" 
                value={props.inputName} 
                onChange={(e) => { props.nameOnChange(e.target.value) }}
            ></Input>
            <Input 
                placeholder="Location" 
                ref={props.locationRef}
                value={props.inputLoc} 
                onChange={(e) => { props.locOnChange(e.target.value) }}
            ></Input>
        </Modal>
    )
}