import React from "react";
import Key from "./key.js";
export default function Keypad(props){
    return (
        <div className="calculator-keypad">
            <div className="keys-function">
                <Key keyValue={"c"} func={props.func.hclear}></Key>
                <Key keyValue={"\xB1"} func={props.func.hneg}></Key>
                <Key keyValue={"%"} func={props.func.hpcnt}></Key>
            </div>
            <div className="keys-operators">
                <Key keyValue={"+"} func={props.func.hop}></Key>
                <Key keyValue={"-"} func={props.func.hop}></Key>
                <Key keyValue={"*"} func={props.func.hop}></Key>
                <Key keyValue={"/"} func={props.func.hop}></Key>
                <Key keyValue={"="} func={props.func.hcalc}></Key>
            </div>
            <div className="keys-numbers">
                <Key keyValue={9} func={props.func.hnum}></Key>
                <Key keyValue={8} func={props.func.hnum}></Key>
                <Key keyValue={7} func={props.func.hnum}></Key>
                <Key keyValue={6} func={props.func.hnum}></Key>
                <Key keyValue={5} func={props.func.hnum}></Key>
                <Key keyValue={4} func={props.func.hnum}></Key>
                <Key keyValue={3} func={props.func.hnum}></Key>
                <Key keyValue={2} func={props.func.hnum}></Key>
                <Key keyValue={1} func={props.func.hnum}></Key>
                <Key 
                    className="key-dot"
                    keyValue={"."}
                    func={props.func.hdot}
                />
                <Key
                    className="key-zero"
                    keyValue={0}
                    func={props.func.hnum}
                />
            </div>
        </div>
    );
}