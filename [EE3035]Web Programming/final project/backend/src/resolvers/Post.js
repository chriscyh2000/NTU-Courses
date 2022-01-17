const Post = {
    async comments(parent, args, { db }, info){
        let cmts = [];
        for(let i = 0; i < parent.comments.length; ++i){
            const cmt = await db.CommentModel.findById(parent.comments[i]);
            cmts.push(cmt);
        }
        // console.log(cmts);
        return cmts;
    },
    async store(parent, args, { db }, info){
        return parent.store;
    }
}

export default Post;