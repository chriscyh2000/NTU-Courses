let all_arr = [], active_arr = [], completed_arr = [];
let remain = 0, total = 0;
class item_node{
    constructor(TODO){
        this.node = document.createElement("li");
        this.node.classList.add("todo-app__item");
        this.checkBox_node = document.createElement("div");
        this.checkBox_node.classList.add("todo-app__checkbox");
        let checkBox_ele1 = document.createElement("input");
        let checkBox_ele2 = document.createElement("label");
        checkBox_ele1.id = "2";
        checkBox_ele1.type = "checkbox";
        checkBox_ele2.for = "2";
        this.checkBox_node.appendChild(checkBox_ele1);
        this.checkBox_node.appendChild(checkBox_ele2);
        this.node.appendChild(this.checkBox_node);
        this.description_node = document.createElement("h1");
        this.description_node.textContent = TODO;
        this.description_node.classList.add("todo-app__item-detail");
        this.node.appendChild(this.description_node);
        this.x_node = document.createElement("img");
        this.x_node.src = "./img/x.png";
        this.x_node.classList.add("todo-app__item-x");
        this.node.appendChild(this.x_node);
    }
    get Node(){
        return this.node;
    }
    get CheckBox_Node(){
        return this.checkBox_node;
    }
    get Description_Node(){
        return this.description_node;
    }
    get X_Node(){
        return this.x_node;
    }
}

class item{
    constructor(TODO, index){
        this.todo_item = TODO;
        this.status = "active";
        this.element = new item_node(TODO);
        this.element.CheckBox_Node.childNodes[1].addEventListener("click", this.change_status.bind(this));
        this.element.X_Node.addEventListener("click", this.delete.bind(this));
        this.index = index;
    }
    get Status(){
        return this.status;
    }
    get Element(){
        return this.element;
    }
    get Index(){
        return this.index;
    }
    set_idx(i){
        this.index = i;
    }
    change_status(){
        if(this.status === "active"){
            this.status = "completed";
            this.element.Description_Node.style.textDecoration = "line-through";
            this.element.description_node.style.opacity = 0.5;
            this.element.CheckBox_Node.childNodes[1].style.background = "#26ca299b";
            remain--;
            if(remain === all_arr.length-1){
                clear_button.style.opacity = 1;
                clear_button.addEventListener("click", clear_completed);
                clear_button.childNodes[0].style.cursor = "pointer";
            }
            record_node.childNodes[0].textContent = `${remain} left`;
            /*修改remain 顯示的數量 */
        }
        else{
            this.status = "active";
            this.element.Description_Node.style.textDecoration = "none";
            this.element.description_node.style.opacity = 1;
            this.element.CheckBox_Node.childNodes[1].style.background = "rgba(99, 99, 99, 0.698)";
            remain++;
            if(remain === all_arr.length){
                clear_button.style.opacity = 0;
                clear_button.removeEventListener("click", clear_completed);
                clear_button.childNodes[0].style.cursor = "default";
            }
            record_node.childNodes[0].textContent = `${remain} left`;
            /*修改remain 顯示的數量 */
        }
        completed_arr = all_arr.filter(item => item.Status === "completed");
        active_arr = all_arr.filter(item => item.Status === "active");
        adjust();
    }
    delete(){
        for(var i = this.index + 1; i < all_arr.length; ++i){
            all_arr[i].set_idx(i-1);
        }
        all_arr.splice(this.index, 1);
        if(this.status === "active"){
            remain--;
            record_node.childNodes[0].textContent = `${remain} left`;
            active_arr = all_arr.filter(item => item.Status === "active");
        }
        else completed_arr = all_arr.filter(item => item.Status === "completed");
        if(remain === all_arr.length){
            clear_button.style.opacity = 0;
            clear_button.removeEventListener("click", clear_completed);
            clear_button.childNodes[0].style.cursor = "default";
        }
        --total;
        if(total === 0){
            root_node.removeChild(footer_node);
        }
        if(buttons.childNodes[0].classList[0] === "selected");
            list_node.removeChild(this.element.Node);
        adjust();
        /*修改remain 顯示的數量 */
        /*如果全部被刪除就把下面的button都拔掉 */
    }
}
function add(){
    if(input_node.value === "") return;

    let newElement = new item(input_node.value, total);
    all_arr[all_arr.length] = newElement;
    console.log(all_arr.length);
    active_arr[active_arr.length] = newElement;
    if(all_arr.length === 1){
        /*加上button */
    }
    if(buttons.childNodes[2].classList.length === 0)
        list_node.appendChild(newElement.Element.Node);
    ++remain;
    record_node.childNodes[0].textContent = `${remain} left`;
    if(remain > 0){
        footer_node.appendChild(clear_button);
    }
    ++total;
    if(total === 1){
        root_node.appendChild(footer_node);
        select(0);
    }
    input_node.value = "";
}

let main_node = document.createElement("div");
let input_node = document.createElement("input");
let list_node = document.createElement("ul");
let root_node = document.getElementById("root");

input_node.addEventListener("keydown", function(e){
    if(e.key === "Enter"){
        add();
    }
})

main_node.classList.add("todo-app__main");
input_node.classList.add("todo-app__input");
list_node.classList.add("todo-app__list");
list_node.id = "todo-list";
main_node.appendChild(input_node);
main_node.appendChild(list_node);
root_node.appendChild(main_node);
input_node.placeholder = "What needs to be done?";

let footer_node = document.createElement("footer");
footer_node.classList.add("todo-app__footer");
footer_node.id = "todo-footer";

let record_node = document.createElement("div");
record_node.classList.add("todo-app__total");

record_node.appendChild(document.createElement("p"));
record_node.childNodes[0].textContent = "";
footer_node.appendChild(record_node);

let buttons = document.createElement("ul");
buttons.classList.add("todo-app__view-buttons");

buttons.appendChild(document.createElement("button"));
buttons.appendChild(document.createElement("button"));
buttons.appendChild(document.createElement("button"));
buttons.childNodes[0].textContent = "All";
buttons.childNodes[1].textContent = "Active";
buttons.childNodes[2].textContent = "Completed";
footer_node.appendChild(buttons);

let clear_button = document.createElement("div");
clear_button.classList.add("todo-app__clean");
clear_button.appendChild(document.createElement("button"));
clear_button.childNodes[0].textContent = "Clear completed";
clear_button.childNodes[0].style.cursor = "default";
clear_button.style.opacity = 0;
footer_node.appendChild(clear_button);

function clear_completed(){
    clear_button.style.opacity = 0;
    clear_button.removeEventListener("click", clear_button);
    clear_button.childNodes[0].style.cursor = "default";
    total -= completed_arr.length;
    remain = total;
    completed_arr = [];
    all_arr = all_arr.filter(item => item.Status === "active");
    for(let i = 0; i < all_arr.length; ++i){
        all_arr[i].set_idx(i);
    }
    console.log(all_arr);
    record_node.childNodes[0].textContent = `${remain} left`;
    if(total === 0){
        root_node.removeChild(footer_node);
    }
    if(buttons.childNodes[0].classList[0] === "selected"){
        list_node.innerHTML = "";
        for(let i = 0; i < all_arr.length; ++i){
            list_node.appendChild(all_arr[i].element.Node);
        }
    }
    else if(buttons.childNodes[2].classList[0] === "selected"){
        list_node.innerHTML = "";
    }
}
function adjust(){
    
    if(buttons.childNodes[1].classList[0] === "selected"){
        console.log("hello");
        list_node.innerHTML = "";
        for(let i = 0; i < active_arr.length; ++i){
            list_node.appendChild(active_arr[i].element.Node);
        }
    }
    else if(buttons.childNodes[2].classList[0] === "selected"){
        list_node.innerHTML = "";
        for(let i = 0; i < completed_arr.length; ++i){
            list_node.appendChild(completed_arr[i].element.Node);
        }
    }
}

function select(i){
    // console.log("fuck");
    for(let i = 0; i < 3; ++i){
        buttons.childNodes[i].classList = [];
    }
    buttons.childNodes[i].classList.add("selected");
    list_node.innerHTML = "";
    if(i === 0){
        for(let j = 0; j < all_arr.length; ++j){
            list_node.appendChild(all_arr[j].element.Node);
        }
    }
    else if(i === 1){
        for(let j = 0; j < active_arr.length; ++j){
            list_node.appendChild(active_arr[j].element.Node);
        }
    }
    else{
        for(let j = 0; j < completed_arr.length; ++j){
            list_node.appendChild(completed_arr[j].element.Node);
        }
    }
}
function f0(){
    select(0);
}
function f1(){
    select(1);
}
function f2(){
    select(2);
}
buttons.childNodes[0].addEventListener("click", f0);
buttons.childNodes[1].addEventListener("click", f1);
buttons.childNodes[2].addEventListener("click", f2);