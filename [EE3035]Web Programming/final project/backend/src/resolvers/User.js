const User = {
    async posts(parent, args, { db }, info){
        const pts = await parent.posts.map( async (postId) => {
            return await db.PostModel.findById(postId);
        });
        return pts;
    },
    async favs(parent, args, { db }, info){
        const favs = await parent.favs.map( async (favId) => {
            return await db.StoreModel.findById(favId);
        });
        return favs;
    }
}

export default User;