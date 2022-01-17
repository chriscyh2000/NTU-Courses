import "./component.css";
import React, {useState, useEffect} from "react";
export default function screen(props){
    return (
        <div className="calculator-input">
            <div className="result">{props.value}</div>
        </div>
    );
}