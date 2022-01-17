import React, {useState, useEffect, Fragment} from 'react';
import x from './img/x.png'
function Todo_app(){
  const [all, setAll] = useState([]);
  const [todos, setTodos] = useState([]);
  const [rem, setRem] = useState(0);
  const [total, setTotal] = useState(0);
  const [input, setInput] = useState("");
  const [curdir, setCurdir] = useState("all");

  const handleSubmit = (e) => {
    if(e.keyCode === 13){
      if(input === "") return;
      let newArr = all.slice(0, all.length);
      let idx = newArr.length;
      newArr[newArr.length] = {name: input, status: "active", index: idx};
      setAll(newArr);
      setTodos(newArr);
      setTotal(newArr.length);
      setRem(newArr.filter(e => e.status === "active").length);
      if(curdir !== "all"){
        setTodos(newArr.filter(e => (curdir === "active")? (e.status === "active") : (e.status === "completed")));
      }
      setInput("");
    }
  };
  const handleClick = ({type, index}) => {
    let newArr = all.slice(0, all.length);
    if(type === "remove"){
      newArr.splice(index, 1);
      for(let i = 0; i < newArr.length; ++i){
        newArr[i].index = i;
      }
    } 
    else{
      if(newArr[index].status === "completed"){
        newArr[index].status = "active";
      }
      else{
        newArr[index].status = "completed";
      }
    }
    setRem(newArr.filter(e => e.status === "active").length);
    setTotal(newArr.length);
    setAll(newArr);
    setTodos((curdir === "all") ? newArr : ((curdir === "active")? newArr.filter(e => e.status === "active") : newArr.filter(e => e.status === "completed")));
    
  };
  const handleButClick = (dir) =>{
    setCurdir(dir);
    let newArr = all.slice(0, all.length);
    setTodos((dir === "all") ? newArr : ((dir === "active")? newArr.filter(e => e.status === "active") : newArr.filter(e => e.status === "completed")));
  }
  const clear_all = () =>{
    let arr = all.slice(0, all.length);
    let newArr = arr.filter(e => (e.status === "active"));
    setRem(newArr.length);
    setTotal(newArr.length);
    for(let i = 0; i < newArr.length; ++i){
      console.log(newArr[i].name);
      newArr[i].index = i;
    }
    setAll(newArr);
    setTodos((curdir === "all") ? newArr : ((curdir === "active")? newArr.filter(e => e.status === "active") : newArr.filter(e => e.status === "completed")));
  };
  return (
    <Fragment>
      <header className="todo-app__header">
        <h1 className="todo-app__title">todos</h1>
      </header>
      <div className = "todo-app__main">
        <input className = "todo-app__input" 
          placeholder = "What needs to be done?"
          onKeyDown = {(e) => handleSubmit(e)} 
          value={input} 
          onChange={e => setInput(e.target.value)}
        />
        <ul className = "todo-app__list" id = "todo-list">
          {todos.map((e) => 
            <li className = "todo-app__item">
              <div className = "todo-app__checkbox">
                <input id={e.index} onClick={() => {handleClick({type: "change status", index: e.index})}} type = "checkbox"></input>
                <label htmlFor={e.index} style={{background:(e.status === "active") ? "rgba(99, 99, 99, 0.698)" : "#26ca299b"}}></label>
              </div>
              <h1 className="todo-app__item-detail" 
                style={{textDecoration: (e.status === "active") ? "none" : "line-through",
                        opacity: (e.status === "active") ? 1 : 0.5
                      }}
              >
                {e.name}
              </h1>
              <img src={x} onClick={() => {handleClick({type: "remove", index: e.index})}} className = "todo-app__item-x"></img>
            </li>)
          }
        </ul>
      </div>
      {(total > 0 )? 
      (<footer className = "todo-app__footer" id = "todo-footer">
        <div className = "todo-app__total">{`${rem} left`}</div>
        <ul className = "todo-app__view-buttons">
          <button onClick = {() => {handleButClick("all");}} style={{color: (curdir==="all")? "red" : "black"}}>All</button>
          <button onClick = {() => {handleButClick("active");}} style={{color: (curdir==="active")? "red" : "black"}}>Active</button>
          <button onClick = {() => {handleButClick("completed");}} style={{color: (curdir==="completed")? "red" : "black"}}>Completed</button>
        </ul>
        <div className="todo-app__clean"><button onClick={clear_all} style={(total !== rem)? {opacity: 1, cursor: "pointer"}:{opacity: 0, cursor: "default"}}>Clear Completed</button></div>
        </footer>) : <div></div>
      }
      
    </Fragment>
  )
}

export default Todo_app;