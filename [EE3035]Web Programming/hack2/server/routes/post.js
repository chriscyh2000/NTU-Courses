import express from 'express'
import Post from '../models/post'
import moment from 'moment'
const router = express.Router()
// TODO 2-(1): create the 1st API (/api/allPosts)
router.get('/allPosts', async (req, res) => {
    const existing = await Post.find({});
    console.log(existing);
    if(!existing.length || !existing){
        res.status(403).send({
            message: "error",
            data: null
        });
    }
    else{
        existing.sort(function (a, b){
           return a.timestamp - b.timestamp;
        });
        res.status(200).send({
            message: "success",
            data: existing
        });   
    }
})
// TODO 3-(1): create the 2nd API (/api/postDetail)
router.get('/postDetail', async (req, res) => {
    const existing = await Post.find({postId: req.query.pid});
    if(!existing.length || !existing){
        res.status(403).send({
            message: "error",
            post: null
        });
    }
    else{
        res.status(200).send({
            message: "success",
            post: existing[0]
        });   
    }
})
// TODO 4-(1): create the 3rd API (/api/newPost)
router.post('/newPost', async (req, res) => {
    try{
        const newPost = new Post({
            postId: req.query.pid, 
            title: req.query.title, 
            content: req.query.content, 
            timestamp: req.query.time
        });
        newPost.save();
        res.status(200).send({
            message: 'success'
        });
    }
    catch(error) {
        res.status(403).send({
            message: 'error', 
            post: null
        });
    }
})
// TODO 5-(1): create the 4th API (/api/post)
router.delete('/post', async (req, res) => {
    try{
        Post.deleteOne({
            postId: req.query.pid
        });
        res.status(200).send({
            message: 'success'
        });
    }
    catch(error){
        res.status(403).send({
            message: 'error', 
            post: null
        });
    }
})
export default router