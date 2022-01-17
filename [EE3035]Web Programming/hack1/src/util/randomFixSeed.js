/****************************************************************************
  FileName      [ randomFixSeed.js ]
  PackageName   [ src/util ]
  Author        [ Cheng-Hua Lu, Chin-Yi Cheng ]
  Synopsis      [ This file makes random numbers. ]
  Copyright     [ 2021 10 ]
****************************************************************************/

var seedrandom = require('seedrandom');
const randomFixSeed = seedrandom('Over my dead body');

export default function randomNum(min = 0, max){
    return Math.floor(randomFixSeed() * (max - min + 1) + min);
}
