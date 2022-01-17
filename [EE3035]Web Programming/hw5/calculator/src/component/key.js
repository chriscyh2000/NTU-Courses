import React from "react";
import "./key.css";
export default function key(props){
    return(
        <button
            className={`${props.className}`}
            onClick={() => {props.func(props.keyValue)}}
        >{props.keyValue}{" "}</button>
    );
}