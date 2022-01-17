import { LoadingOutlined, UserOutlined } from '@ant-design/icons'
import "../App.css"
import { Comment, Avatar } from 'antd';

const Comments = (props) => {
    const { c } = props;
    return (
        <Comment
            className='comment'
            author={<a className='postMain'>{c.sender}</a>}
            avatar={<Avatar icon={<UserOutlined />} src={c.img}/>}
            content={<p className='postMain'>{c.text}</p>}
        />
    )
}

export default Comments;