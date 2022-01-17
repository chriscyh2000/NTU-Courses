import React, { useState, useEffect } from 'react'
import moment from 'moment'
import instance from './instance'

import { useParams } from 'react-router-dom'
import { IconButton, Button, Typography } from '@material-ui/core'
import { Delete as DeleteIcon } from '@material-ui/icons'

function Post(props) {
  const { pid } = useParams()
  const [data, setData] = useState(null)

  // TODO 3-(2): complete getPostDetail function to get the full information of a post from database
  const getPostDetail = async () => {
    const { data: { message, post } } = await instance.get('/postDetail', { params: {pid} });
    console.log(message);
    setData(post);
  }

  // TODO 5-(2): complete delPost function to delete a post from database
  const delPost = () => {
      
    setTimeout(() => {
      props.navigate(-1)
    }, 300)
  }

  // TODO 3-(2): fetch the full information of a post from database
  useEffect(() => {
    getPostDetail();
  }, [])
  
  return (
    <div className="article-wrapper">
      <div id="goback-btn">
        <Button variant="contained" color="primary" id="goback-reply-btn" onClick={() => props.navigate(-1)}>Back</Button>
      </div>

      {data ?
        <div className="article-container">
          <div className="article-title" id="pid-detail-title">
            {data.title}

            {/* TODO 5-(2): add property to IconButton to trigger the delPost function when click */}
            <IconButton className="post-delete" size="small" id="pid-detail-del-btn">
              <DeleteIcon fontSize="inherit" />
            </IconButton>
          </div>
          <div className="article-time">
            <span id="pid-detail-time">{moment(data.timestamp).format('YYYY-MM-DD HH:mm:ss')}</span>
          </div>
          <div className="article-content-container">
            <Typography component={'span'} id="pid-detail-content">
              {data.content}
            </Typography>
          </div>
        </div> : <div className="article-container"><h1>Post not found</h1></div>
      }
    </div>
  );
}

export default Post
