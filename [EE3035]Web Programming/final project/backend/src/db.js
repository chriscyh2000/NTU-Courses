import mongoose from 'mongoose';
const Schema = mongoose.Schema;

const storeSchema = new Schema({
    name: { type: String, required: true},
    score: { type: Number, required: true },
    location: { type: String, required: true },
    posts: [{type: mongoose.Types.ObjectId, ref: 'Post'}],
    postCount: { type: Number, required: true }
});

const userSchema = new Schema({
    name: { type: String, required: true },
    biography: { type: String, required: false},
    password: { type: String, required: true },
    posts: [{type: mongoose.Types.ObjectId, ref: 'Post'}],
    favs: [{type: mongoose.Types.ObjectId, ref: 'Store'}],
    image: { type: String, required: false }
});

const commentSchema = new Schema({
    text: { type: String, required: true },
    sender: { type: String, required: true },
    post: { type: mongoose.Types.ObjectId, ref: 'Post' },
    img: { type: String, required: false }
});


const postSchema = new Schema({
    title: { type: String, required : true },
    body: { type: String, required: true },
    author: { type: String, required: true},
    comments: [{ type: mongoose.Types.ObjectId, ref: 'comment' }],
    store: { type: Object, required: true},
    score: { type: Number, required: true },
    img: { type: String, required: false },
    pimg: { type: String, required: false }
});

const StoreModel = mongoose.model('Store', storeSchema);
const UserModel = mongoose.model('User', userSchema);
const CommentModel = mongoose.model('Comment', commentSchema);
const PostModel = mongoose.model('Post', postSchema);

const db = {
    StoreModel,
    UserModel,
    CommentModel,
    PostModel
}

export default db;