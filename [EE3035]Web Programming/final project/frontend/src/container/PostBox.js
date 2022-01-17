import { useEffect } from 'react';
import { useQuery } from '@apollo/client';
import { POST_SUBSCRIPTION, POSTS_QUERY } from '../graphql'; 
import { LoadingOutlined } from '@ant-design/icons'
import Post from '../component/Post'
import { Modal } from 'antd';

const PostBox = (props) => {

    const { data, loading, subscribeToMore, refetch } = useQuery(POSTS_QUERY, {
        variables: {
            store_id: props.store_id
        }
    });

    refetch();
    
    useEffect(() => {
        try{
            subscribeToMore({
                document: POST_SUBSCRIPTION,
                variables: {
                    store_id: props.store_id,
                },
                updateQuery: (prev, { subscriptionData }) => {
                    if(!subscriptionData.data) return prev;
                    return {
                        posts:  [...prev.posts, subscriptionData.data.post.data]
                    };
                }
            })
        }
        catch(e){}
    }, [subscribeToMore])

    return (
        <Modal
            title={props.title}
            visible={props.visible}
            footer={null}
            closable={true}
            width={"650px"}

            onCancel={() => props.setVisible(false)}
        >
            {(loading || data === undefined)?
            <div className='why2'>
            <LoadingOutlined style={{ fontSize: "70px" }}/>
            </div> : (
            <>
                <h3 style={{ textAlign:"left" }}>{ data.posts.length + ((data.posts.length > 1)? " posts":" post")}</h3>
                {data.posts.map((post) => { return <Post store={props.title} post_id={post.id} user_id={props.user_id}></Post>})}
            </>
            )}
        </Modal>
    )
} 

export default PostBox;