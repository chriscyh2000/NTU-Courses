import { LoadingOutlined } from '@ant-design/icons'
import Post from '../component/Post'
import { FINDSTOREBYID_MUTATION } from '../graphql/index';
import { useMutation } from '@apollo/client'
import { Empty } from 'antd'
const MyPost = (props) => {
    if(props.user === null){
        return(
            <div className="storeBox">
                <>
                    <h3>Please sign in first.</h3>
                    <br/>
                    <br/>
                    <Empty/>
                </>
            </div>
        )
    }
    const { id, name, posts } = props.user;

    return (
        <div style={{ width: "650px", textAlign: "center", margin: "0px auto" }}>
            { (posts.length === 0)?
                <>
                    <h3 >No post.</h3>
                    <br/>
                    <br/>
                    <Empty/>
                </> :
                <>
                    <h3 style={{ textAlign:"left" }}>{ posts.length + ((posts.length > 1)? " posts":" post")}</h3>
                    {   posts.map((post) => { 
                            return <Post store={post.store.name} post_id={post.id} user_id={id}></Post>
                        })
                    }
                </>
            }
            
        </div>
    )
} 

export default MyPost;