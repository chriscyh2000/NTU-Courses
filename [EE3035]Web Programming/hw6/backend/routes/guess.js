import express from 'express'
import {genNumber, getNumber} from '../cores/getNumber'
const router = express.Router();
function roughScale(x, base) {
    const parsed = parseInt(x, base);
    if (isNaN(parsed)) { return 0; }
    return parsed;
}
router.post('/start', (_, res) => {
    genNumber();
    res.json({msg: "The game has started."});
});
router.get('/guess', (req, res) => {
    const number = getNumber();
    const guessed = roughScale(req.query.num, 10);
    if(!guessed || guessed > 100 || guessed < 1){
        res.status(406).send({msg: "Not a legal number."});
    }
    else if(number === guessed){
        res.send({msg: `You won! The number was ${number}.`, id: 1});
    }
    else if(number > guessed){
        res.send({msg: "Bigger."});
    }
    else res.send({msg: "Smaller."});
})
router.post('/restart', (_, res) => {
    genNumber();
    res.json({msg: "The game has restarted."});
})
export default router;