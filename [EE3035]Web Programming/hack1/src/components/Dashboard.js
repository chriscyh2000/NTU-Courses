/****************************************************************************
  FileName      [ Dashnoard.js ]
  PackageName   [ src/components ]
  Author        [ Cheng-Hua Lu, Chin-Yi Cheng ]
  Synopsis      [ This file generates the Dashboard. ]
  Copyright     [ 2021 10 ]
****************************************************************************/

import React, { useEffect, useState } from 'react';
import "./css/Dashboard.css"
let timeIntervalId;

export default function Dashboard({remainFlagNum, gameOver}){
    let [time, setTime] = useState(0);
    let [sTime, setSTime] = useState(0);
    {/* -- TODO 7 -- */}
    {/* Useful Hint: Try to understand the difference between time and sTime. */}
    const c = setInterval(() => {
            setTime(time+1);
            // setSTime(time+1);
        }, 1000)
    useEffect(() => {
        if(gameOver){
            clearInterval(c);
            setSTime(time);
            setTime(0);
        }
    }, [gameOver]);

    useEffect(() => {
        
      
    }, []);


    return (
        <div className = "dashBoard" >
            <div id = 'dashBoard_col1' >
                <div className = 'dashBoard_col'>
                    <p className = 'icon'>🚩</p>
                    {remainFlagNum}
                </div>
            </div>
            <div id = 'dashBoard_col2' >
                <div className = 'dashBoard_col'>
                    <p className = 'icon'>⏰</p>
                    {gameOver ? sTime : time}
                </div>
            </div>
        </div>
    );
}
