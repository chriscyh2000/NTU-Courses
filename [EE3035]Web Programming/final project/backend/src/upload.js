import { uuid } from 'uuidv4';
import db from './db';
const example_users = [
    {
        id: uuid(),
        name: "weebaoweewee",
        biography: 'I am a teacher',
        password: "12345",
        posts: [],
        favs: [],
        image: ''
    },
    {
        id: uuid(),
        name: "kuo202020",
        biography: 'I am a teacher',
        password: "202020",
        posts: [],
        favs: [],
        image: ''
    }
]
const example_stores = [
    {
        id: uuid(),
        name: "McDonald Daan",
        score: 3,
        location: "Daan, Taipei",
        posts: [],
        postCount: 1
    },
    {
        id: uuid(),
        name: "YU poke",
        score: 5,
        location: "Daan, Taipei",
        posts: [],
        postCount: 1
    }
]
const example_posts = [
    {
        id: uuid(),
        title: "My First Time Eating McDonald in Taipei",
        body: "Not so good ==.",
        author: '',
        comments: [],
        store: {},
        score: 3,
        pimg: ''
    },
    {
        id: uuid(),
        title: "My Favorite restaurant in Gongguan",
        body: "So delicious!!😍",
        author: '',
        comments: [],
        store: {},
        score: 3,
        pimg: ''
    }
]


const dataInit = async () => {
    await db.PostModel.deleteMany({});
    await db.UserModel.deleteMany({});
    await db.StoreModel.deleteMany({});
    await db.CommentModel.deleteMany({});
    
    // await db.UserModel.insertMany(example_users);
    // await db.StoreModel.insertMany(example_stores);

    // const users = await db.UserModel.find({});
    // const stores = await db.StoreModel.find({});
    // example_posts[0].author = users[0].name;
    // example_posts[1].author = users[1].name;
    // example_posts[0].store = { id: stores[0].id, name: stores[0].name };
    // example_posts[1].store = { id: stores[1].id, name: stores[1].name };
    // await db.PostModel.insertMany(example_posts);

    // const posts = await db.PostModel.find({});
    // users[0].posts.push(posts[0]);
    // stores[0].posts.push(posts[0]);
    // users[1].posts.push(posts[1]);
    // stores[1].posts.push(posts[1]);
    // await users[0].save();
    // await users[1].save();
    // await stores[0].save();
    // await stores[1].save();

    console.log("Database initialized!");
};

export default dataInit;