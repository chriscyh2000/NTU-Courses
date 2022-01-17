const Subscription = {
    comment: {
        async subscribe(parent, args, { db, pubsub }, info){
            const post = await db.PostModel.findById(args.id);
            // console.log(post);
            if(!post) {
                throw new Error('Error: Post cannot be found when trying to subscribe.');
            }
            return pubsub.asyncIterator(`PostId: ${args.id}`);
        }
    },
    post: {
        async subscribe(parent, args, { db, pubsub }, info){
            const store = await db.StoreModel.findById(args.id);
            // console.log(post);
            if(!store) {
                throw new Error('Error: Store cannot be found when trying to subscribe.');
            }
            return pubsub.asyncIterator(`StoreId: ${args.id}`);
        }
    },
    store: {
        async subscribe(parent, args, { pubsub }, info){
            return pubsub.asyncIterator('All stores');
        }
    }
}

export default Subscription;