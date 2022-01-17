import React from 'react'
import Guide from './guide'

import { BrowserRouter as Router } from 'react-router-dom'

import './App.css'

function App(props) {
  document.title = '110-1 Hackathon 2'
  
  return (
    <Router>
      <Guide />
    </Router>
  );
}

export default App;
