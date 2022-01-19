## ADA hw4 p1

這題首先要先找出所有maximal clique的數量，可以參考：https://www.youtube.com/watch?v=132XR-RLNoY，主要可以看以下演算法即可，![Screen Shot 2021-12-28 at 6.48.39 PM](/Users/chriscyhxx/Desktop/Screen Shot 2021-12-28 at 6.48.39 PM.png)

要注意的是第4行pseudo code指的不是選「鄰居在$P$內的數量最多的點$u$」，而是直接選degree最高者即可。此外這些取聯集、交集的操作都可以用bitset解決。

接下來是建立constraint的方式。假設maximal cliques的數量為$|C|$，令$x_i$為一個boolean表示$c_i$是否最後有被click，可知$0\leq x_i\leq1$；此外由於一個vertex只會被選到一次，所以該vertex所屬的所有maximal clique之$x_i$加總必等於1，也就是說如果vertex u包含在$c_1, c_4, c_6$這幾個clique，那麼可以得到兩個constraint分別是$x_1+x_4+x_6\leq 1$、$-x_1-x_4-x_6\geq -1$。而最終被選到的clique數量（也就是我們要極小化的答案）為$\Sigma^{|C|}_{i=0}x_i$，由於助教給的solver是極大化$d_1x_1+d_2x_2+d_3x_3+...+d_{|C|}x_{|C|}$，所以應該讓

$\{d_1, d_2, \cdots, d_{|C|}\}=\{-1, -1, -1. \cdots, -1\}$，此外也要記得給solver的constraint是$A\cdot x \leq b$。





