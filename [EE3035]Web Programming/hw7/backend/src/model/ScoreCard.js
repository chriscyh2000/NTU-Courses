import mongoose from 'mongoose'
const Schema = mongoose.Schema;
const scorecardSchema = new Schema({
    Name: String,
    Subject: String,
    Score: String
})
const ScoreCard = mongoose.model('ScoreCard', scorecardSchema);
export default ScoreCard;