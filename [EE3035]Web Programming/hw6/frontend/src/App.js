import './App.css';
import React, {useState} from 'react';
import {startGame, guess, restart} from './axios'
function App() {
  const [start, setStart] = useState(false);
  const [hasWon, setHasWon] = useState(false);
  const [message, setMsg] = useState('');
  const [input, setInput] = useState('');
  const handleSubmit = async() =>{
    const number = input;
    setInput('');
    await guess(number, setMsg, setHasWon);
  };
  const startTrigger = async()=>{
    setStart(true);
    await startGame(setMsg);
  };
  const restartGame = async() =>{
    await restart(setHasWon, setMsg);
  };
  return (
    (!start) ? 
    <button type="button" onClick={startTrigger}>Start Game</button> : 
    <div>
      <input 
        placeholder="Input a number." 
        value={input} 
        onChange={(e) => {setInput(e.target.value)}}
      />
      <button onClick={handleSubmit} disabled={hasWon || input === ''}>guess!</button>
      <p>{message}</p>
      {(hasWon) ? <button onClick={restartGame}>Restart</button> : <></>}
    </div>
  );
}

export default App;