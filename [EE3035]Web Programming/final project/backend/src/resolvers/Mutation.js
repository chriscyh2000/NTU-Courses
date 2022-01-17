import { uuid } from 'uuidv4';
const checkUser = async (DB, name) => {
    const user = await DB.UserModel.findOne({ name: name });
    return user;
}

const newUser = async (DB, name, password) => {
    const NewUser = await DB.UserModel({
        id: uuid(),
        name: name,
        biography: '',
        password: password,
        posts: [],
        favs: [],
        image: ''
    });
    await NewUser.save();
    return NewUser;
}

const Mutation = {
    async createPost(parent, args, { db, pubsub }, info){
        const { title, body, score, author, store, img, pimg } = args.data;
        const Store = await db.StoreModel.findById(store);
        const Author = await db.UserModel.findOne({ name: author }); // use author: String? or ID?
        if(!Store){
            throw new Error('Error: Store cannot find while creating post.');
        }
        Store.score += score;
        Store.postCount++;

        if(!Author){
            throw new Error('Error: User cannot find while creating post.')
        }

        const newPost = await db.PostModel({
            id: uuid(),
            title: title,
            body: body,
            author : author,
            comments: [],
            store: { id: store, name: Store.name },
            score: score,
            img: img,
            pimg: pimg
        });

        Store.posts.push(newPost);
        Author.posts.push(newPost);

        await Store.save();
        await Author.save();
        await newPost.save();

        pubsub.publish(`StoreId: ${Store.id}`, {
            post: {
                mutation: 'CREATED',
                data: newPost
            }
        });

        pubsub.publish(`All stores`, {
            store: {
                mutation: 'UPDATED',
                data: Store
            }
        })

        return newPost;
    },
    
    async createComment(parent, args, { db, pubsub }, info){
        const { text, sender, post } = args.data;
        const Post = await db.PostModel.findById(post);
        const Sender = await db.UserModel.findById(sender);
        if(!Post){
            throw new Error('Error: Cannot find the post while creating comment.');
        }

        const newComment = await db.CommentModel({
            img: Sender.image,
            text: text,
            sender: Sender.name,
            post: post
        });

        Post.comments.push(newComment.id);
        await Post.save();
        await newComment.save();

        pubsub.publish(`PostId: ${post}`, {
            comment: {
                mutation: 'CREATED',
                data: newComment
            }
        });

        return newComment;
    },

    async createStore(parent, args, { db, pubsub }, info){
        const { name, location } = args.data;

        const Store = await db.StoreModel.findById(args.id);

        if(Store) return null;

        const id = uuid();
        const newStore = await db.StoreModel({
            id: id,
            name: name,
            score: 0,
            location: location,
            posts: [],
            postCount: 0
        })
        await newStore.save();

        pubsub.publish(`All stores`, {
            store: {
                mutation: 'CREATED',
                data: newStore
            }
        })
        return newStore;
    },

    async login(parent, { name, password }, { db }, info){
        const USR = await checkUser(db, name);
        if(!USR || USR.password === '-1'){
            return {
                message: "User not found.",
                user: null
            }
        }
        return {
            message: "User exist.",
            user: USR
        }
    },
    
    async signup(parent, { name, password }, { db }, info){
        const USR = await checkUser(db, name);
        if(!USR){
            newUser(db, name, password);
        }
        else if(USR.password === '-1'){
            USR.password = password;
            await USR.save();
        }
        else{
            return {
                message: "User exist.",
                password: null
            }
        }
        return {
            message: "Sign-up succeeded.",
            password: null
        }
    },
    async findStore(parent, { name }, { db }, info){
        const STR = await db.StoreModel.findOne({ name: name });
        if(!STR) return null;
        else return STR;
    },

    async findStoreById(parent, { id }, { db }, info){
        const STR = await db.StoreModel.findById(id);
        if(!STR) return null;
        else return STR;
    },

    async addFav(parent, { storeId, userId }, { db }, info){
        const STR = await db.StoreModel.findById(storeId);
        const USR = await db.UserModel.findById(userId);
        USR.favs.push(STR);
        await USR.save();
        return STR;
    },

    async removeFav(parent, { storeId, userId }, { db }, info){
        const STR = await db.StoreModel.findById(storeId);
        const USR = await db.UserModel.findById(userId);
        const favs = [];
        for(let i = 0; i < USR.favs.length; ++i){
            if(USR.favs[i] != STR.id) favs.push(USR.favs[i]);
        }
        USR.favs = favs;
        await USR.save();
        return STR;
    },

    async editProfile(parent, { userId, biography }, { db }, info){
        const USR = await db.UserModel.findById(userId);
        if(USR){
            USR.biography = biography;
            USR.save();
            return USR;
        }
        return null;
    },
    async uploadUserImage(parent, { image, name }, { db }, info){
        const USR = await db.UserModel.findOne({ name:  name });
        const cmts = await db.CommentModel.find({});
        for(let i = 0; i < cmts.length; ++i){
            if(cmts[i].sender === name) cmts[i].img = image;
            cmts[i].save();
        }
        const posts = await db.PostModel.find({});
        for(let i = 0; i < posts.length; ++i){
            if(posts[i].author === name) posts[i].img = image;
            posts[i].save();
        }
        if(!USR) {
            throw new Error('Error: cannot find user while uploading images');
        }
        USR.image = image;
        USR.save();
        return image;
    }
}

export default Mutation;