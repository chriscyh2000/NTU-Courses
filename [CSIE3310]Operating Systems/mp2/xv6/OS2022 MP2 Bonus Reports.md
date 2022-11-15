# OS2022 MP2 Bonus Report

## 4.4.1 Pros and Cons of Demand Paging

**Explain the benefits and drawbacks of "demand paging" in following aspects.**

- Memory Utilization: 因為可以把用不到的資料swap到secondary storage，可以讓所有process所使用的virtual memory大小超過physical memory的大小，可以增加同時進行的process，memory utilization也增加。
- External fragmentation: 相較傳統的memory management，paging可以將memory切分成一個一個frame，每次process要memory都是以frame為單位，因此也降低了external fragmentation的機會。讓memory waste減少。
- Input/Output for demand paging: 減少input and output，因為只有在需要用到的時候才會要frame，也只會要真正需要的，而非整個memory。
- The size of physical memory: 可以取用超過physical memory大小的virtual memory，因為有swapping機制，將不需要的data先存在backing storage。
- Share the pages with demand paging: 可以更輕易地答道share page，因為可以透過改動page table entry來指向相同的frame，非常簡單就可以達成目的。
- Overheads due to interrupts, page tables access, and memory access time in demand paging: 因為demand paging只在需要的時候才去要frame，所以會常常遇到memory還沒load進來的狀況，interrupt會很常發生。在access page table的時候，雖然我們可以快速查找到entry，但也需要memory access的時間；此外如果data還在disk上，我們也需要disk I/O時間，這也會造成memory access的overhead。

## 4.4.2 Effective Memory Access Time Analysis

**Question 1 : Derived the EMAT formula of "Multi-level Paging" ( k level paging ), assumed that no pagefault occur in "Multi-level Page" access.**

Ans1: $EMAT=P \cdot$ $($TLB access time + memory access time$)+(1-P)\cdot($TLB access time $+$ memory access time $\cdot (k+1)$$)$
因為有$P$的機率會成功，花的時間會是TLB access time$+$memory access time。也會有$1-P$的機率失敗，此時還要額外memory access $k$次，因此得到上式。

**Question 2: **
Ans2: $100ns$
因為$180ns\ge0.8\cdot (20ns+$memory access time$)+0.2\cdot(20ns+(3+1)$memory access time$)$ ，memory access time最多會是$100ns$。