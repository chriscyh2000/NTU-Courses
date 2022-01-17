let Number = 0;
function genNumber(){
    let num = 0;
    while(num === 0){
        num = Math.floor(Math.random() * 100);
    }
    Number = num;
}
function getNumber(){
    return Number;
}
export {genNumber, getNumber};