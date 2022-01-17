/****************************************************************************
  FileName      [ App.js ]
  PackageName   [  ]
  Author        [ Cheng-Hua Lu, Chin-Yi Cheng ]
  Synopsis      [  ]
  Copyright     [ 2021 10 ]
****************************************************************************/

import React from 'react';
import './App.css';
import MineSweeper from './containers/MineSweeper';

const App = () => {
  return (
    <div className = 'App'>
      <MineSweeper/>
    </div>
  );
}

export default App;
