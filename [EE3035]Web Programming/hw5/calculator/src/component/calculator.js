import "./component.css";
import React, {useState, useEffect} from "react";
import Screen from "./screen";
import Keypad from "./keypad";
function Calculator(){
    const [preValue, setPreValue] = useState(null);
    const [curValue, setCurValue] = useState(0);
    const [operator, setOperator] = useState(null);
    const [onDot, setOnDot] = useState(false);
    const [reInput, setReInput] = useState(false);

    function checkDigit(floatVal){
        let n = (''+floatVal).split('.');
        if(n[1].length > 12){
            return floatVal.toFixed(12);
        } 
        else return floatVal;
    }
    const handlePercent = (op) =>{
        if(curValue === null) return;
        let n = (''+curValue).split('.');
        let len = (n.length === 1) ? 0 : n[1].length;
        setCurValue((parseFloat(curValue) * 0.01).toFixed(len+2));
        // n = (''+curValue).split('.');
        // len = (n.length === 1) ? 0 : n[1].length;
        
        setOnDot(true);
    }
    const handleDot = (op) => {
        if(curValue === null) return;
        setOnDot(true);
    };
    const handleNeg = (op) => {
        if(curValue === null) return;
        setCurValue(curValue * -1);
    }
    const handleClear = (op) => {
        if(curValue === null) return;
        setCurValue(0);
        setOnDot(false);
    };
    const handleOp = (op) => {
        if(curValue === "ERROR") return;
        let prevOp = op;
        setOperator(op);
        if(curValue === null) return;
        setOnDot(false);
        setPreValue(curValue);
        setCurValue(null);
    };
    const handleCalc = (op) => {
        if(curValue === null || preValue === null) return;
        setReInput(true);
        if(operator === '+')
            setCurValue(curValue+preValue);
        else if(operator === '-')
            setCurValue(preValue-curValue);
        else if(operator === '*')
            setCurValue(curValue*preValue);
        else {
            if(curValue === 0){
                setCurValue("ERROR");
            }
            else if(preValue%curValue != 0){
                setCurValue(checkDigit(Number.parseFloat(preValue) / Number.parseFloat(curValue)));
            }
            else setCurValue(preValue/curValue);
        }
            
        setPreValue(null);
        setOperator(null);
        setOnDot(false);
    };
    const handleNum = (num) => {
        if(onDot){
            num = parseFloat(num);
            let n = (''+curValue).split('.');
            let len = (n.length === 1) ? 0 : n[1].length;
            for(let i = 0; i < len + 1; ++i){
                num *= 0.1;
            }
            let newVal = parseFloat(curValue) + num;
            setCurValue(newVal.toFixed(len+1));
        }
        else{
            if(curValue === null || curValue < 0){
                setCurValue(num);
            }
            else if(reInput){
                setReInput(false);
                setCurValue(num);
            }
            else setCurValue(curValue * 10 + num);
        }
    }
    return (
        <div className="calculator">
            <Screen value={curValue}></Screen>
            <Keypad func={{hop: handleOp, hcalc: handleCalc, 
                            hnum: handleNum, hclear: handleClear,
                            hdot: handleDot, hneg: handleNeg, hpcnt: handlePercent}}/>
        </div>
    );
}
export default Calculator;