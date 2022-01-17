import { DoubleRightOutlined, HeartTwoTone, HeartOutlined, HeartFilled } from "@ant-design/icons";
import PostBox from "../container/PostBox";
import { useState } from 'react';
import status from "../hook/status";
import Post from "./Post";
import { ADDFAV_MUTATION, REMOVEFAV_MUTATION, FINDSTORE_MUTATION } from "../graphql/index";
import { useMutation } from "@apollo/client";
const Store = (props) => {
    const { name, id, score, postCount, location } = props.store;
    const rank = (score / ((postCount === 0) ? 1 : postCount)).toFixed(1);
    const [unfold, setUnfold] = useState(false);
    const [toRefetch, setToRefetch] = useState(false);
 
    const [addFav] = useMutation(ADDFAV_MUTATION);
    const [removeFav] = useMutation(REMOVEFAV_MUTATION);
    const [findStore] = useMutation(FINDSTORE_MUTATION);

    const [color, setColor] = useState(props.initColor);

    if(props.initColor !== color){
        setColor(props.initColor)
    }

    if(toRefetch){
        setToRefetch(false);
    }
    const handleOnClick = async () => {
        if(props.uid === null){
            status.display({
                type: 'error',
                msg: 'Please sign in first.'
            })
            return;
        }
        if(color == 'red'){
            setColor('');
            await removeFav({
                variables: {
                    store_id: id,
                    user_id: props.uid
                }
            })
            setToRefetch(true);
            props.refetch();
        }
        else{
            setColor('red');
            await addFav({
                variables: {
                    store_id: id,
                    user_id: props.uid
                }
            })
            setToRefetch(true);
            props.refetch();
        }
    }

    return (
        <>
        <div className="store">
            <div className="storeInformation">
                <h1 style={ {borderBottom: "1px solid gray"} }>{name}</h1>
                <h2 style={ { marginTop: "15px", marginBottom: "15px"} }>{location}</h2>
                <div className="inlineParent">
                    {/* <StarTwoTone style={ {fontSize: "32px"} } twoToneColor="#fee600" className="inlineChild"/> */}
                    <h2 className="inlineChild">⭐️&nbsp;&nbsp;{`${rank}`}</h2>
                </div>
            </div>
            <div className="storeOther">
                {(color === "red")? <HeartFilled className="storeGo2" style={ {fontSize: "32px", color:"#ff0000"} } twoToneColor="#ff0000" onClick={() => handleOnClick()} /> : <HeartOutlined className="storeGo2" style={ {fontSize: "32px"} } onClick={() => handleOnClick()}/>}
                <br/>
                <br/>
                <br/>
                <br/>
                <DoubleRightOutlined className="storeGo2" onClick={() => { setUnfold(!unfold) }} style={{fontSize:"24px"}} />
            </div>
            { unfold? <PostBox store_id={id} visible={unfold} title={name} setVisible={setUnfold} store={name} user_id={props.uid}></PostBox> : <></> }
        </div>
        </>
    )
} 

export default Store;