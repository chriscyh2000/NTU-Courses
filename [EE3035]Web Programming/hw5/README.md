# WP1101 HW5
* 請先進入calculator資料夾中執行以下指令:
```
$ npm install
$ npm start
```
* 我是參考iphone內建的計算機介面，但功能的實現上稍有不同。
* 一次僅支援個摁算，不可連續運算。
* 不支援連續運算，所以如果輸入`2*5-6`，答案會output `-1`
* 如果輸入完operator後再按其他operator，會以較後輸入的operator來運算。
* 若執行除法運算時，除數輸入0，會output出`ERROR`。
* 點擊C鍵會將目前的輸入的value清空。
* 點擊%鍵會將數字乘上0.01。
* 若點按+/-鑑後再輸入數字，會把目前的value清空重新計算。