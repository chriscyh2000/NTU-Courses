/****************************************************************************
  FileName      [ MineSweeper.js ]
  PackageName   [ src/containers ]
  Author        [ Cheng-Hua Lu, Chin-Yi Cheng ]
  Synopsis      [ The control and main page of MineSweeper. ]
  Copyright     [ 2021 10 ]
****************************************************************************/

import React, { useState } from 'react';
import './MineSweeper.css';
import Board from '../components/Board'
import HomePage from '../components/HomePage'

const MineSweeper = () => {
    const [startGame, setStartGame] = useState(false);      // A boolean variable. If true, show the Board, else show the HomePage.
    const [mineNum, setMineNum] = useState(10);             // A integer variable to store the number of mines in the game. The default value is 10.
    const [boardSize, setBoardSize] = useState(8);          // A integer variable to store the board size in the game. The default value is 8.
    
    {/* Useful Hint: The four functions below only need to fill up with those three hook functions above. */}
    {/* -- TODO 1-2 -- */}
    const startGameOnClick = () => {
        setStartGame(true);
    }
    {/* -- TODO 6-2 -- */}
    const mineNumOnChange = (e) => {
        setMineNum(e.target.value);
    }
    {/* -- TODO 6-2 -- */}
    const boardSizeOnChange = (e) => {
        setBoardSize(e.target.value);
    }
    {/* -- TODO 5-2 -- */}
    const backToHomeOnClick = () => {
        setStartGame(false);
    }

    return( 
        <div className='mineSweeper'>  
            {/* -- TODO 1-1 -- */} 
            {(startGame)? <Board boardSize={boardSize} mineNum={mineNum} backToHome={backToHomeOnClick}/> : 
            <HomePage startGameOnClick={startGameOnClick} mineNumOnChange={mineNumOnChange} boardSizeOnChange={boardSizeOnChange} 
            mineNum={mineNum} boardSize={boardSize}/>}
            {/* Useful Hint: If ... <HomePage startGameOnClick = {startGameOnClick} .../> else <Board .../> */}
            {/* Reminder: You can refer to the structure file in p.6 of Hack1.pdf. */}
             
        </div>
    );
}
export default MineSweeper;