/****************************************************************************
  FileName      [ HomePage.js ]
  PackageName   [ src/components ]
  Author        [ Cheng-Hua Lu, Chin-Yi Cheng ]
  Synopsis      [ This file generates the Home page.  ]
  Copyright     [ 2021 10 ]
****************************************************************************/



import React, { useState } from 'react';
import { useEffect } from 'react';
import './css/HomePage.css';

{/* -- TODO 2 -- */}
const HomePage = ({startGameOnClick, mineNumOnChange, boardSizeOnChange, mineNum, boardSize /* -- something more... -- */}) => {
    const [showPanel, setShowPanel] = useState(false);      // A boolean variable. If true, the controlPanel will show.
    const [error, setError] = useState(false);              // A boolean variable. If true, means that the numbers of mines and the board size are invalid to build a game.

    {/* Some functions may be added here! */}
    const ShowPanel = () => {
      if(showPanel)
        setShowPanel(false);
      else setShowPanel(true);
    }
    useEffect(() => {
      setError(mineNum > boardSize*boardSize)
    }, [mineNum, boardSize]);

    return(
      <div className = 'HomeWrapper'>
          <p className = 'title'>MineSweeper</p>
            {/* -- TODO 1-1 -- */}
            <button className="btn" onClick={() => {
              if(!error)
                startGameOnClick()
              }}>Start Game</button>
            {/* -- TODO 6-2 -- */}
            {/* Useful Hint: <input type = 'range' min = '...' max = '...' defaultValue = '...'> */}
            {/* Useful Hint: Error color: '#880000', default text color: '#0f0f4b', invisible color: 'transparent' */}
            {/* Reminder: The defaultValue of 'mineNum' is 10, and the defaultValue of 'boardSize' is 8. */}
            <div className="controlContainer">
              <button className="btn" onClick={()=>{ShowPanel()}}>Difficulty Adjustment</button>
              {(showPanel)? 
                <div className="controlWrapper">
                  <div className="error" style={{color: '#880000'}}>{(error)? "ERROR: Mines number and board size are invalid!" : ""}</div>
                  <div className="controlPanel">
                    <div className="controlCol">
                      <p className="controlTitle">Mines Number</p>
                      <input type="range" min="1" max="50" defaultValue="10" onChange={(e) => {mineNumOnChange(e)}}></input>
                      <p className="controlNum" style={{color: (error) ? '#880000' : '0f0f4b'}}>{mineNum}</p>
                    </div>
                    <div className="controlCol">
                      <p className="controlTitle">Board Size(nxn)</p>
                      <input type="range" min="1" max="20" defaultValue="8" onChange={(e) => {boardSizeOnChange(e)}}></input>
                      <p className="controlNum" style={{color: (error) ? '#880000' : '0f0f4b'}}>{boardSize}</p>
                    </div>
                  </div>
                </div> 
                : <></>}
            </div>
      </div>
    );

}
export default HomePage;   