/****************************************************************************
  FileName      [ Board.js ]
  PackageName   [ src/components ]
  Author        [ Cheng-Hua Lu, Chin-Yi Cheng ]
  Synopsis      [ This file generates the Board. ]
  Copyright     [ 2021 10 ]
****************************************************************************/

import React, { useEffect, useState } from 'react';
import Cell from './Cell';
import Modal from './Modal';
import Dashboard from './Dashboard';
import createBoard from '../util/createBoard';
import { revealed } from '../util/reveal';
import './css/Board.css'


const Board = ({ boardSize, mineNum, backToHome }) => {
    const [board, setBoard] = useState([]);                     // An 2-dimentional array. It is used to store the board.
    const [nonMineCount, setNonMineCount] = useState(0);        // An integer variable to store the number of cells whose value are not '💣'.
    const [mineLocations, setMineLocations] = useState([]);     // An array to store all the coordinate of '💣'.
    const [gameOver, setGameOver] = useState(false);            // A boolean variable. If true, means you lose the game (Game over).
    const [remainFlagNum, setRemainFlagNum] = useState(0);      // An integer variable to store the number of remain flags.
    const [win, setWin] = useState(false);                      // A boolean variable. If true, means that you win the game.

    useEffect(() => {
        // Calling the function
        freshBoard();
    }, []);

    // Creating a board
    const freshBoard = () => {
        {/* -- TODO 3-1 -- */}
        {/* Useful Hint: createBoard(...) */}
        let tmp = createBoard(boardSize, mineNum);
        setBoard(tmp.board);
        setMineLocations(tmp.mineLocations);
        setNonMineCount(boardSize*boardSize-mineNum);
        setRemainFlagNum(mineNum);
    }

    const restartGame = () => {
        {/* -- TODO 5-2 -- */}
        {/* Useful Hint: freshBoard() */}
        setGameOver(false);
        freshBoard();
        setWin(false);
    }

    // On Right Click / Flag Cell
    const updateFlag = (e, x, y) => {
        // To not have a dropdown on right click
        e.preventDefault();
        // Deep copy of a state
        {/* -- TODO 3-2 -- */}
        {/* Useful Hint: A cell is going to be flagged. 'x' and 'y' are the xy-coordinate of the cell. */}
        {/* Reminder: If the cell is already flagged, you should unflagged it. Also remember to update the board and the remainFlagNum. */}
        {/* Reminder: The cell can be flagged only when it is not revealed. */}
        if(board[x][y].revealed) return;
        if(board[x][y].flagged && remainFlagNum<mineNum){
            setRemainFlagNum(remainFlagNum+1);
            board[x][y].flagged = false;
            setBoard(board);
        }
        else if(remainFlagNum > 0){
            setRemainFlagNum(remainFlagNum-1);
            board[x][y].flagged = true;
            setBoard(board);
        } 
    };
    function dfs(x, y, tmp){
        if(x < 0 || y < 0 || x >= boardSize || y >= boardSize) return;
        if(tmp[x][y].revealed || tmp[x][y].flagged) return;
        if(tmp[x][y].value !== 0){
           tmp[x][y].revealed = true;
           return;
        } 
        tmp[x][y].revealed = true;
        for(let i = x-1; i <= x+1; ++i){
            for(let j = y-1; j <= y+1; ++j){
                if(i === x && j === y) continue;
                dfs(i, j, tmp);
            }
        }
    }
    const revealCell = (x, y) => {
        {/* -- TODO 4-1 -- */}
        {/* Reveal the cell */}
        {/* Useful Hint: The function in reveal.js may be useful. You should consider if the cell you want to reveal is a location of mines or not. */}
        {/* Reminder: Also remember to handle the condition that after you reveal this cell then you win the game. */}
        if(board[x][y].revealed || board[x][y].flagged || win || gameOver) return;
        let tmp = board.slice();
        for(let i = 0; i < mineNum; ++i){
            if(x === mineLocations[i][0] && y === mineLocations[i][1]){
                for(let j = 0; j < mineNum; ++j){
                    if(tmp[mineLocations[j][0]][mineLocations[j][1]].flagged) continue;
                    tmp[mineLocations[j][0]][mineLocations[j][1]].revealed = true;
                }
                setBoard(tmp);
                setGameOver(true);
                return;
            }
        }
        dfs(x, y, tmp);
        setBoard(tmp);
        let n = 0;
        for(let x = 0; x < boardSize; ++x){
            for(let y = 0; y < boardSize; ++y){
                if(tmp[x][y].revealed) ++n;
            }
        }
        setNonMineCount(boardSize*boardSize-n-mineNum);
        if(boardSize*boardSize-n-mineNum=== 0){
            setWin(true);
            setGameOver(true);
        }
        setNonMineCount(boardSize*boardSize-n-mineNum);
    };

    return(
        <div className = 'boardPage' >
            <div className = 'boardWrapper'>
            {/* This line of code is just for testing. Please delete it if you finish this function. */}
            
            {/* -- TODO 3-1 -- */}
            {/* Useful Hint: The board is composed of BOARDSIZE*BOARDSIZE of Cell (2-dimention). So, nested 'map' is needed to implement the board.  */}
            {/* Reminder: Remember to use the component <Cell> and <Dashboard>. See Cell.js and Dashboard.js for detailed information. */}
            </div>
            <div className="boardContainer">
                <Dashboard remainFlagNum={remainFlagNum}/>
                {board.map((row,index) => <div id={`row${index}`} style={{display: "flex"}}>
                    {row.map((c, idx)=><Cell id={`${index}-${idx}`} rowIdx={index} colIdx={idx} detail={c} revealCell={revealCell} updateFlag={updateFlag}/>)}
                    </div>)}
            </div>
            {(gameOver)? <Modal restartGame={restartGame} win={win} backToHome={backToHome}/> : <></>}
        </div>
    ); 

}

export default Board