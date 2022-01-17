import axios from "axios";
const instance = axios.create({baseURL: 'http://localhost:4000/api/guess'});

const startGame = async (setMsg) => {
    const {data : {msg}} = await instance.post('/start');
    setMsg(msg);
};

const guess = async (num, setMsg, setHasWon) => {
    try{
        const response = await instance.get('/guess', { params: { num } });
        setMsg(response.data.msg);
        if(response.data.id === 1) setHasWon(true);
    } catch(error){
        setMsg(`ERROR: ${num} is not a legal number(1-100).`);
    }
};

const restart = async(setHasWon, setMsg) => {
    setHasWon(false);
    const response = await instance.post('/restart');
    setMsg(response.data.msg)
}
export {startGame, guess, restart};