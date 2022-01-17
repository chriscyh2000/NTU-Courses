import mongoose from 'mongoose';
import express from 'express';
import cors from 'cors';
import bodyParser from 'body-parser';
import dotenv from 'dotenv-defaults';
import ScoreCard from './model/ScoreCard';
dotenv.config();
mongoose.connect(process.env.MONGO_URL, {
    useNewUrlParser: true,
    useUnifiedTopology: true,
}).then((res) => console.log("mongo db connection created"));
const app = express();
app.use(cors());
app.use(bodyParser.json());

const addScoreCard = async (name, subject, score, res) => {
    const existing = await ScoreCard.findOneAndUpdate({Name: name, Subject: subject}, {Score: score});
    if(!existing){
        const newCard = new ScoreCard({Name: name, Subject: subject, Score: score});
        newCard.save();
        res.json({message: `Adding (${name}, ${subject}, ${score})`, card: 'Added!'});
    }
    else{
        res.json({message: `Update (${name}, ${subject}, ${score})`, card: 'Updated!'});
    }
};
const queryDB = async (type, queryString, res) => {
    if(type === "name"){
        const existing = await ScoreCard.find({Name: queryString});
        if(!existing.length) res.json({messages: 0, message: [`${type} (${queryString}) not found!`]});
        else{
            let str = [];
            for (var i = 0; i < existing.length; ++i) {
                str[str.length] = `(${existing[i].Name}, ${existing[i].Subject}, ${existing[i].Score})`;
            }
            console.log(str);
            res.json({messages: str, message: `${type} (${queryString}) found!`});
        }
    }
    else{
        const existing = await ScoreCard.find({Subject: queryString});
        if(!existing.length) res.json({messages: 0, message: [`${type} (${queryString}) not found!`]});
        else{
            let str = [];
            for (var i = 0; i < existing.length; ++i) {
                str[str.length] = `(${existing[i].Name}, ${existing[i].Subject}, ${existing[i].Score})`;
            }
            console.log(str);
            res.json({messages: str, message: `${type} (${queryString}) found!`});
        }
    }
};
const deleteDB = async () => {
    try{
        await ScoreCard.deleteMany({});
        console.log("Deletion Successed!");
    }
    catch(e) {throw new Error("Deletion failed!")};
};
app.post('/api/create-card', (req, res) => {
    addScoreCard(req.body.name, req.body.subject, req.body.score, res);
});
app.get('/api/query-cards', (req, res) => {
    queryDB(req.query.type, req.query.queryString, res);
});
app.delete('/api/clear-db', (_, res) => {
    deleteDB();
    res.json({ message: 'Database cleared' })
});
const port = process.env.PORT || 5000;
app.listen(port, () => {
    console.log(`Server is up on port ${port}.`)
});