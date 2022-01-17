import { Modal, Input } from 'antd';

const AddModal = (props) => {
    const { visible, onCreate, onCancel, inputRef } = props;
    return (
        <Modal
            title="Enter the person you want to talk to:"
            visible={visible}
            onOk={onCreate}
            onCancel={onCancel}
            okText="Create"
        >
            <Input placeholder="Name" ref={inputRef}/>
        </Modal>
    )
}

export default AddModal;