import { useEffect, useState } from 'react';
import { useMutation, useQuery } from '@apollo/client';
import { COMMENT_SUBSCRIPTION, POST_QUERY, CREATECOMMENT_MUTATION } from '../graphql';
import { LoadingOutlined, UserOutlined } from '@ant-design/icons'
import "../App.css"
import status from '../hook/status';
import { Comment, Avatar, Input, Image } from 'antd';
import Comments from './Comments';

const { TextArea } = Input;

const Post = (props) => {
    const { data, loading, subscribeToMore, refetch } = useQuery(POST_QUERY, {
        variables: {
            post_id: props.post_id
        }
    });
    refetch();
    const [createComment] = useMutation(CREATECOMMENT_MUTATION);
    const [text, setText] = useState('');

    const submitComment = async () => {
        // Something...

        if(!props.user_id) { // check log in
            status.display({
                type: "error",
                msg: "Please log in first."
            })
            return;
        }
        if(!text){
            status.display({
                type: "error",
                msg: "comment cannot be empty."
            })
            return;
        }
        const payload =  await createComment({
            variables: {
                sender: props.user_id,
                text: text,
                post: props.post_id
            }
        })

        status.display({
            type: "success",
            msg: "comment replied."
        })
        console.log("submit!");
        setText('');
    }

    useEffect(() => {
        try{
            subscribeToMore({
                document: COMMENT_SUBSCRIPTION,
                variables: {
                    post_id: props.post_id
                },
                updateQuery: (prev, { subscriptionData }) => {
                    if(!subscriptionData.data) return prev;

                    return {
                        post: {
                            ...prev.post,
                            comments: [...prev.post.comments, subscriptionData.data.comment.data]
                        }
                    }
                }
            })
        }
        catch(e){}
    }, [subscribeToMore]);

    if(loading && data === undefined){
        return(
            <div className="post">
                <LoadingOutlined style={{ fontSize: "70px" }}/>
            </div>
        )
    }

    return (
        <div className="post">
            <div className='postweebao'>
                <h2 className='postTitle'>{props.store}</h2>
                <h3 className='postTitle1'>{data.post.title}</h3>
                <Comment
                    className='postInner'
                    author={<>
                                <a className='postMain'>{data.post.author+" · "+data.post.score+"⭐️"}</a>
                                {/* <Rate disabled defaultValue={data.post.score} style={{ fontSize:"12pt" }}/> */}
                            </>}
                    avatar={<Avatar icon={<UserOutlined />} src={data.post.img}/>}
                    content={
                        <p className='postMain'>{data.post.body}</p>
                    }
                />
                {(data.post.pimg !== null && data.post.pimg !== undefined && data.post.pimg !== '') ?
                    <Image
                        height={200}
                        style={{ margin: "0px 20px"  }}
                        src={data.post.pimg}
                        fallback="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAMIAAADDCAYAAADQvc6UAAABRWlDQ1BJQ0MgUHJvZmlsZQAAKJFjYGASSSwoyGFhYGDIzSspCnJ3UoiIjFJgf8LAwSDCIMogwMCcmFxc4BgQ4ANUwgCjUcG3awyMIPqyLsis7PPOq3QdDFcvjV3jOD1boQVTPQrgSkktTgbSf4A4LbmgqISBgTEFyFYuLykAsTuAbJEioKOA7DkgdjqEvQHEToKwj4DVhAQ5A9k3gGyB5IxEoBmML4BsnSQk8XQkNtReEOBxcfXxUQg1Mjc0dyHgXNJBSWpFCYh2zi+oLMpMzyhRcASGUqqCZ16yno6CkYGRAQMDKMwhqj/fAIcloxgHQqxAjIHBEugw5sUIsSQpBobtQPdLciLEVJYzMPBHMDBsayhILEqEO4DxG0txmrERhM29nYGBddr//5/DGRjYNRkY/l7////39v///y4Dmn+LgeHANwDrkl1AuO+pmgAAADhlWElmTU0AKgAAAAgAAYdpAAQAAAABAAAAGgAAAAAAAqACAAQAAAABAAAAwqADAAQAAAABAAAAwwAAAAD9b/HnAAAHlklEQVR4Ae3dP3PTWBSGcbGzM6GCKqlIBRV0dHRJFarQ0eUT8LH4BnRU0NHR0UEFVdIlFRV7TzRksomPY8uykTk/zewQfKw/9znv4yvJynLv4uLiV2dBoDiBf4qP3/ARuCRABEFAoBEgghggQAQZQKAnYEaQBAQaASKIAQJEkAEEegJmBElAoBEgghggQAQZQKAnYEaQBAQaASKIAQJEkAEEegJmBElAoBEgghggQAQZQKAnYEaQBAQaASKIAQJEkAEEegJmBElAoBEgghggQAQZQKAnYEaQBAQaASKIAQJEkAEEegJmBElAoBEgghggQAQZQKAnYEaQBAQaASKIAQJEkAEEegJmBElAoBEgghggQAQZQKAnYEaQBAQaASKIAQJEkAEEegJmBElAoBEgghggQAQZQKAnYEaQBAQaASKIAQJEkAEEegJmBElAoBEgghggQAQZQKAnYEaQBAQaASKIAQJEkAEEegJmBElAoBEgghggQAQZQKAnYEaQBAQaASKIAQJEkAEEegJmBElAoBEgghggQAQZQKAnYEaQBAQaASKIAQJEkAEEegJmBElAoBEgghggQAQZQKAnYEaQBAQaASKIAQJEkAEEegJmBElAoBEgghggQAQZQKAnYEaQBAQaASKIAQJEkAEEegJmBElAoBEgghgg0Aj8i0JO4OzsrPv69Wv+hi2qPHr0qNvf39+iI97soRIh4f3z58/u7du3SXX7Xt7Z2enevHmzfQe+oSN2apSAPj09TSrb+XKI/f379+08+A0cNRE2ANkupk+ACNPvkSPcAAEibACyXUyfABGm3yNHuAECRNgAZLuYPgEirKlHu7u7XdyytGwHAd8jjNyng4OD7vnz51dbPT8/7z58+NB9+/bt6jU/TI+AGWHEnrx48eJ/EsSmHzx40L18+fLyzxF3ZVMjEyDCiEDjMYZZS5wiPXnyZFbJaxMhQIQRGzHvWR7XCyOCXsOmiDAi1HmPMMQjDpbpEiDCiL358eNHurW/5SnWdIBbXiDCiA38/Pnzrce2YyZ4//59F3ePLNMl4PbpiL2J0L979+7yDtHDhw8vtzzvdGnEXdvUigSIsCLAWavHp/+qM0BcXMd/q25n1vF57TYBp0a3mUzilePj4+7k5KSLb6gt6ydAhPUzXnoPR0dHl79WGTNCfBnn1uvSCJdegQhLI1vvCk+fPu2ePXt2tZOYEV6/fn31dz+shwAR1sP1cqvLntbEN9MxA9xcYjsxS1jWR4AIa2Ibzx0tc44fYX/16lV6NDFLXH+YL32jwiACRBiEbf5KcXoTIsQSpzXx4N28Ja4BQoK7rgXiydbHjx/P25TaQAJEGAguWy0+2Q8PD6/Ki4R8EVl+bzBOnZY95fq9rj9zAkTI2SxdidBHqG9+skdw43borCXO/ZcJdraPWdv22uIEiLA4q7nvvCug8WTqzQveOH26fodo7g6uFe/a17W3+nFBAkRYENRdb1vkkz1CH9cPsVy/jrhr27PqMYvENYNlHAIesRiBYwRy0V+8iXP8+/fvX11Mr7L7ECueb/r48eMqm7FuI2BGWDEG8cm+7G3NEOfmdcTQw4h9/55lhm7DekRYKQPZF2ArbXTAyu4kDYB2YxUzwg0gi/41ztHnfQG26HbGel/crVrm7tNY+/1btkOEAZ2M05r4FB7r9GbAIdxaZYrHdOsgJ/wCEQY0J74TmOKnbxxT9n3FgGGWWsVdowHtjt9Nnvf7yQM2aZU/TIAIAxrw6dOnAWtZZcoEnBpNuTuObWMEiLAx1HY0ZQJEmHJ3HNvGCBBhY6jtaMoEiJB0Z29vL6ls58vxPcO8/zfrdo5qvKO+d3Fx8Wu8zf1dW4p/cPzLly/dtv9Ts/EbcvGAHhHyfBIhZ6NSiIBTo0LNNtScABFyNiqFCBChULMNNSdAhJyNSiECRCjUbEPNCRAhZ6NSiAARCjXbUHMCRMjZqBQiQIRCzTbUnAARcjYqhQgQoVCzDTUnQIScjUohAkQo1GxDzQkQIWejUogAEQo121BzAkTI2agUIkCEQs021JwAEXI2KoUIEKFQsw01J0CEnI1KIQJEKNRsQ80JECFno1KIABEKNdtQcwJEyNmoFCJAhELNNtScABFyNiqFCBChULMNNSdAhJyNSiECRCjUbEPNCRAhZ6NSiAARCjXbUHMCRMjZqBQiQIRCzTbUnAARcjYqhQgQoVCzDTUnQIScjUohAkQo1GxDzQkQIWejUogAEQo121BzAkTI2agUIkCEQs021JwAEXI2KoUIEKFQsw01J0CEnI1KIQJEKNRsQ80JECFno1KIABEKNdtQcwJEyNmoFCJAhELNNtScABFyNiqFCBChULMNNSdAhJyNSiECRCjUbEPNCRAhZ6NSiAARCjXbUHMCRMjZqBQiQIRCzTbUnAARcjYqhQgQoVCzDTUnQIScjUohAkQo1GxDzQkQIWejUogAEQo121BzAkTI2agUIkCEQs021JwAEXI2KoUIEKFQsw01J0CEnI1KIQJEKNRsQ80JECFno1KIABEKNdtQcwJEyNmoFCJAhELNNtScABFyNiqFCBChULMNNSdAhJyNSiEC/wGgKKC4YMA4TAAAAABJRU5ErkJggg=="
                    /> :
                    <></>
                }
            </div>
            <div className='commentBox'>
                <h3 style={{ textAlign:"left" }}>{ data.post.comments.length + ((data.post.comments.length > 1)? " comments":" comment")}</h3>
                { data.post.comments.map((comment) => {
                    return (
                        <Comments c={comment}/>
                    )
                })}
                <Input.Search
                    autoSize
                    style={{ width: 'calc(100% - 10px)', textAlign: "left" }}
                    value={text}
                    onChange={(e) => { setText(e.target.value); }}
                    enterButton="reply"
                    placeholder="write down your comment..."
                    onSearch={submitComment}
                ></Input.Search>
                {/* <Input.Group compact>
                    <TextArea style={{ width: 'calc(100% - 85px)', textAlign: "left" }} defaultValue={text} placeholder="write a comment..." onChange={(value) => setText(value)} autoSize />
                    <Button type="primary" onClick={submitComment}>Submit</Button>
                </Input.Group>            */}
            </div>
            <br/>
            <br/>
        </div>
    )
}

export default Post;