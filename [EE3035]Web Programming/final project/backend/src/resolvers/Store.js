const Store = {
    async posts(parent, args, { db }, info){
        const pts =  await parent.posts.map( async (postId) => {
            return await db.PostModel.findById(postId);
        });
        return pts;
    }
}

export default Store;