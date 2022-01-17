const Query = {
    async stores(parent, args, { db }, info){
        if(!args.query) return await db.StoreModel.find({});
    },

    async post(parent, args, { db }, info){
        return await db.PostModel.findById(args.postId);
    },

    async posts(parent, args, { db }, info){
        let post_arr = [];
        if(!args.query) post_arr = await db.PostModel.find({});
        else{
            const store = await db.StoreModel.findById(args.query);
            for(let i = 0; i < store.posts.length; ++i){
                const post = await db.PostModel.findById(store.posts[i]);
                post_arr.push(post);
            }
        }
        return post_arr;
    },

    async comments(parent, args, { db }, info){
        let comment_arr = [];
        const post = await db.PostModel.findById(args.post);
        comment_arr = post.comments.map( async (comment) => {
            return await db.CommentModel.findById(comment);
        })
        return comment_arr;
    },

    async user(parent, args, { db }, info){
        return await db.UserModel.findOne({ name: args.name });
    },

    async store(parent, args, { db }, info){
        return await db.StoreModel.findOne({ name: args.name });
    }
}

export default Query;