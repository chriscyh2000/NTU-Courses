import { useState } from 'react';
import { useMutation } from '@apollo/client';
import { UserOutlined, UploadOutlined } from '@ant-design/icons';
import { EDITPROFILE_MUTATION, UPLOADUSERIMAGE_MUTATION } from '../graphql';
import status from '../hook/status'
import { Avatar, Descriptions, Button, Modal, Input, Upload, message } from 'antd';
const util = require("util");
const Profile = (props) => {
    const [value, setValue] = useState('');
    const [toRefetch, setToRefetch] = useState(false);
    const { TextArea } = Input;
    const [biography, setBiography] = useState((props.user === null) ? '' : props.user.biography); 
    const [file, setFile] = useState(null);
    const [uploadUserImage] = useMutation(UPLOADUSERIMAGE_MUTATION);
     
    if(toRefetch){
        props.refetch();
        setToRefetch(false);
    }

    const [visible, setVisible] = useState(false);
    const [editProfile] = useMutation(EDITPROFILE_MUTATION);
    const requireSubmit = async (bio) => {
        const payload = await editProfile({
            variables: {
                userId: props.user.id,
                bio: bio
            }
        })
        if(payload === null) {
            status.display({
                type: 'error',
                msg: 'Error: edit profile error.'
            })
        }
        status.display({
            type: 'success',
            msg: 'Successfully edited your profile.'
        })
        setBiography(bio);
        setVisible(false);

        const newData = JSON.parse(JSON.stringify(props.user));
        newData.biography = bio;
        props.refetch()
    }
    const handleSubmit = () => {
        const bio = value;
        setValue('');
        if(bio === ''){
            status.display({
                type: 'error',
                msg: 'Input your bio first!'
            })
            return;
        }
        requireSubmit(bio);
    }

    if(props.user === null){
        return <p>Please log in first</p>
    }
    console.log("in profile.js", props.user)
    return (
        <div style={{ textAlign:"left", margin:"0px auto", width:"400px"}}>
            <Avatar size={64} icon={<UserOutlined />} src={(props.user.image !== '') ? props.user.image : ''}/>
            <br></br>
            <br></br>
            <Descriptions title="Profile:" column={1}>
                <Descriptions.Item label="User Name">{props.user.name}</Descriptions.Item>
                <Descriptions.Item label="About Me">{biography}</Descriptions.Item>
            </Descriptions>
            <Button onClick={() => {setVisible(true)}} >Edit Bio</Button>
            <Modal visible={visible} onCancel={() => {setVisible(false)}} onOk={handleSubmit} closable={false}>
                <TextArea rows={10} 
                    placeholder="Introduce yourself..." 
                    showCount 
                    maxLength={300} 
                    onChange={(e) => { setValue(e.target.value) }} 
                    value={value}>
                </TextArea>
            </Modal>
            <br></br>
            <br></br>
            <h4>Change profile photo</h4>
            {/* <Upload name={'file'} accept={"image/*"} onChange={(event) => {setFile(event.target.file[0])}}>
                <Button icon={<UploadOutlined/>}>Click to upload an image</Button>
            </Upload> */}
            <input type={'file'} icon={<UploadOutlined/>} onChange={(event) => {setFile(event.target.files[0])}}></input>
            { (file !== null) ? 
            <Button onClick={async () => {
                if(file === null) return;
                var reader = new FileReader();
                var baseString;
                reader.onloadend = async function () {
                    baseString = reader.result;
                    await uploadUserImage({
                        variables: {
                            image: baseString,
                            name: props.user.name
                        }
                    })
                    requireSubmit(props.user.biography)
                    props.refetch()
                }
                reader.readAsDataURL(file)
                setFile(null);
            }}>
                Submit
            </Button> : <></>}
        </div>
    )
}

export default Profile;