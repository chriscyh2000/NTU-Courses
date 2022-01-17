# [110-1]Web Programming Final

## [Group 46] NTU CSIEat 😍



* 組員：

​	B07902056 資工四 郭瑋喆

​	B07902142 資工四 許庭維

​	B08902149 資工三 徐晨祐



* 服務內容：

「吃什麼」永遠是一個大難題。別擔心！本網頁提供美食的集合，並提供會員新增貼文及回覆貼文的功能，便利的與其他用戶討論！

想要撰寫貼文的餐廳不在列表？本網頁也提供會員新增餐廳的功能，可以與其他用戶分享新餐廳。

提供最高評比、最新評論、搜尋等不同篩選條件，使餐廳選擇更加便利；

另外，也支援將喜歡的餐廳加入最愛、查看歷史貼文、修改自我介紹與頭貼等功能。

肚子餓、推薦好店、踩雷警告，通通到 NTU CSIEat 😍 就對了！



* Deploy 連結：https://ntucsieat.herokuapp.com/



* Demo 影片連結：https://youtu.be/1--dj9WnDDk



* 使用框架：

​	**<前端>**
​	react, antd, apollo, bcrypt, graphql, styled-components, subscriptions-transport-ws, web-vitals	

​	**<後端>**
​	express, bcrypt, graphql-yoga, mongoose, ws

​	**<資料庫>**
​	mongoDB


* 限制：照片上傳限制在3mb以內。


* 心得：

​	 B07902056 郭瑋喆

我在final project中主要做前端功能與設計，在修這堂課之前有修過資工系的物件導向程式設計，也做過project，而我認為他跟網路React傳遞參數的概念很相似，因此前端寫起來問題不大。但是對於前後端GraphQL接口與後端subscription不熟悉等問題使我們花了很多時間去設計與理解，但因此讓我們更熟悉這些功能的使用與運作方式。整理來說我覺得這堂課課程內容非常扎實，有好好上課跟寫作業真的讓我對於網路服務這塊有更深的理解，也獲取了滿滿的知識！


​	 B07902142 許庭維

這堂課真的讓我獲益良多！從一開始對前端只有一點概念、後端完全不懂，到最後可以多人合作project做出可以實際使用的網站，真的有滿滿的成就感！做final project中也遇到了許多問題，像是React state寫錯造成infinite loop、前後端接口寫錯對不起來，又或著package版本問題使deploy花了許多時間debug，這些都讓我對前後端更加熟悉！最後也謝謝組員的合作與凱瑞，讓我們可以做出比當初預設目標更好、更完整的的網站！

​	 B08902149 徐晨祐

這學期投入了很大量的時間在這個final project，讓我把前後端的技術全部又複習了一遍。從一開始對網路服務這個領域只知道什麼是html的狀態進步到能夠自己掌握前後端所有技術，也認識了很多與後端溝通的方法。此外也要特別感謝兩位隊友辛苦的付出，才能讓我有機會參與做出一個完整的網路服務project的機會，很開心自己當初有選這門課，收穫滿滿！



* Install

step1. `cd backend && yarn install`

step2. `cd frontend && yarn install`

step3. add a `.env` file to `./backend` like this:

```
MONGO_URL=mongodb+srv://<username>:<password>@<collectionName>.tpecx.mongodb.net/<databaseName>?retryWrites=true&w=majority
```

step4. `yarn start`

step5. `yarn server` in another terminal (in order to initialize database)

step6. comment `backend/src/mongo.js` line 18 and save


* 負責項目：

​	 B07902056 郭瑋喆

前端React hook操作、container與component網頁設計。

​	 B07902142 許庭維

擴充網頁功能、前後端接口、更改成Deploy版本、local storage儲存資料與認證。

​	 B08902149 徐晨祐

建立資料庫、GrpahQL功能設計(Query, Mutation, Subscription)、前後端接口、上傳圖片、使用資訊加密。
