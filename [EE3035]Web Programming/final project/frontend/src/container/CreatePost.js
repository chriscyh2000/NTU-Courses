import { Input, Button, Rate } from 'antd';
import { useRef, useState } from 'react';
import "../App.css";
import status from '../hook/status';
import { CREATEPOST_MUTATION, FINDSTORE_MUTATION, CREATESTORE_MUTATION, STORE_SUBSCRIPTION } from '../graphql/index';
import { useMutation } from '@apollo/client';
import CreateStoreModal from '../component/CreateStoreModal';

export default function CreatePost(props){
    const [toRefetch, setToRefetch] = useState(false);

    // For creating store:
    const [inputName, setInputName] = useState('');
    const [inputLoc, setInputLoc] = useState('');

    const [visible, setVisible] = useState(false);
    // For post creation:
    const [createPost] = useMutation(CREATEPOST_MUTATION);
    const [findStore] = useMutation(FINDSTORE_MUTATION);

    const [title, setTitle] = useState('');
    const [restaurant, setRestaurant] = useState('');
    const [body, setBody] = useState('');
    const [rank, setRank] = useState(0);
    const [pimg, setPimg] = useState(null);
    const { TextArea } = Input;

    if(toRefetch){
        setToRefetch(false);
    }

    const handleSubmit = async () => {
        if(!props.user){
            status.display({ type: "error", msg: "Please sign in first" });
            return;
        }
        if(!title) {
            status.display({ type: "error", msg: "Please input the title first" });
            return;
        }
        if(!restaurant) {
            status.display({ type: "error", msg: "Please input the restaurant first" });
            return;
        }
        if(rank === 0) {
            status.display({ type: "error", msg: "Please select rank" });
            return;
        }
        if(!body) {
            status.display({ type: "error", msg: "Please input the content first" });
            return;
        }
        const store = await findStore({
            variables: {
                name: restaurant.trim()
            }
        });
        if(!store.data.findStore){
            setInputName(restaurant.trim());
            setVisible(true);
            status.display({ 
                type: "error", 
                msg: "The restaurant has not been created, pleae create it first." 
            });
            return;
        }
        if(pimg === null){
            const payload = await createPost({
                variables: {
                    title: title,
                    body: body,
                    score: rank,
                    author: props.user.name,
                    store: store.data.findStore.id,
                    img: props.user.image,
                    pimg: ''
                }
            })
            //refetch user data
            props.refetch();
            
            setRank(0);
            setRestaurant('');
            setBody('');
            setTitle('');
            status.display({
                type: "success", msg: "Submission succeeded"
            })
        }
        else{
            var reader = new FileReader();
            var baseString;
            reader.onloadend = async function () {
                baseString = reader.result;
                console.log(baseString)
                try{
                    const payload = await createPost({
                        variables: {
                            title: title,
                            body: body,
                            score: rank,
                            author: props.user.name,
                            store: store.data.findStore.id,
                            img: props.user.image,
                            pimg: baseString
                        }
                    })
                }catch(e){
                    status.display({
                        type: 'error',
                        msg: `Error: request entity too large`
                    })
                    setRank(0);
                    setRestaurant('');
                    setBody('');
                    setTitle('');
                    return;
                }
                setRank(0);
                setRestaurant('');
                setBody('');
                setTitle('');
                status.display({
                    type: "success", msg: "Submission succeeded"
                })
                props.refetch()
            }
            reader.readAsDataURL(pimg)
            setPimg(null);
        }
    }

    // For store creation: 
    const [createStore] = useMutation(CREATESTORE_MUTATION);
    const RequireCreate = async (name, location) => {
        const payload = await createStore({
            variables: {
                name: name,
                location: location
            }
        });
        setInputName('');
        setInputLoc('');
        if(!payload.data.createStore){
            status.display({
                type: 'error',
                msg: 'The data already exist'
            })
            return;
        }
        status.display({
            type: 'success',
            msg: 'Data uploaded'
        })
        setRank(0);
        setVisible(false);
    }
    const handleCreate = () => {
        let name = inputName;
        let location = inputLoc;
        setInputName('');
        setInputLoc('');
        if(name === undefined ||name.trim() === "" || name === ""){
            status.display({
                type: "error",
                msg: "Please enter a valid store name."
            })
            return;
        }
        name = name.trim();
        RequireCreate(name, location);
    }
    const handleCancel = () => {
        setInputLoc('');
        setVisible(false);
    }
    
    return (
        <>
            <div className="createPost">
                <p>Title:</p>
                <Input showCount value={title} maxLength={30} placeholder="Title" onChange={(e) => { setTitle(e.target.value); }} />
                <br />
                <br />
                <p>Restaurant:</p>
                <Input showCount value={restaurant} maxLength={30} placeholder="Restaurant" onChange={(e) => { setRestaurant(e.target.value); }}/>
                <br />
                <br />
                <div className="inlineParent">
                    <p>Rank:</p>
                    <Rate className='rate' value={rank} onChange={(value) => {setRank(value)}}/>
                    {/* {(rank >= 1)? <StarTwoTone className={"star"} style={{ fontSize: '32px' }} twoToneColor='#fee600' onClick={() => {setRank(1)}}/> : <StarOutlined className={"star"} style={{ fontSize: '32px' }} onClick={() => {setRank(1)}}/>}
                    {(rank >= 2)? <StarTwoTone className={"star"} style={{ fontSize: '32px' }} twoToneColor='#fee600' onClick={() => {setRank(2)}}/> : <StarOutlined className={"star"} style={{ fontSize: '32px' }} onClick={() => {setRank(2)}}/>}
                    {(rank >= 3)? <StarTwoTone className={"star"} style={{ fontSize: '32px' }} twoToneColor='#fee600' onClick={() => {setRank(3)}}/> : <StarOutlined className={"star"} style={{ fontSize: '32px' }} onClick={() => {setRank(3)}}/>}
                    {(rank >= 4)? <StarTwoTone className={"star"} style={{ fontSize: '32px' }} twoToneColor='#fee600' onClick={() => {setRank(4)}}/> : <StarOutlined className={"star"} style={{ fontSize: '32px' }} onClick={() => {setRank(4)}}/>}
                    {(rank >= 5)? <StarTwoTone className={"star"} style={{ fontSize: '32px' }} twoToneColor='#fee600' onClick={() => {setRank(5)}}/> : <StarOutlined className={"star"} style={{ fontSize: '32px' }} onClick={() => {setRank(5)}}/>} */}
                </div>
                <br />
                <p>Content:</p>
                <TextArea rows={10} value={body} placeholder="Content" showCount maxLength={3000} onChange={(e) => { setBody(e.target.value); }} />
                <br />
                <p>Image:</p>
                <input type={'file'} onChange={(event) => {setPimg(event.target.files[0])}}></input>
                <Button type="primary" onClick={handleSubmit}>Submit</Button>
            </div>
            <CreateStoreModal 
                visible={visible}
                onCancel={handleCancel}
                onCreateStore={handleCreate}
                inputName={inputName}
                inputLoc={inputLoc}
                nameOnChange={setInputName}
                locOnChange={setInputLoc}
            />
        </>
    )
}