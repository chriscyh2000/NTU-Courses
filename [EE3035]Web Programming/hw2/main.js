const album_list = [];
var countN = 0;
class small_size_pic{
    constructor(parent, idx, large, mysrc, photogrph){
        this.myPhotograph = photogrph;
        this.myLargePic = large;
        this.myParent = parent;
        this.myIndex = idx;
        this.myNode = document.createElement("img");
        this.myNode.src = mysrc;
        this.myParent.appendChild(this.myNode);
        this.myNode.addEventListener("click", this.select.bind(this));
        this.myNode.addEventListener("dblclick", this.delete.bind(this));
    }
    setMyIndex(i){
        this.myIndex = i;
    }
    select(){
        document.getElementById("pnode2").textContent = `目前在瀏覽第${this.myIndex+1}張`;
        if(this.myPhotograph.cur_Idx === this.myIndex){
            this.myLargePic.childNodes[1].childNodes[0].src = this.myPhotograph.imgArray[this.myIndex];
            this.myNode.classList.add("selected");
            return;
        }
        if(this.myPhotograph.cur_Idx === 0){
            this.myLargePic.childNodes[0].childNodes[0].classList.remove("disabled");

        }
        else if(this.myPhotograph.cur_Idx === this.myPhotograph.imgArray.length - 1 ||
                this.myPhotograph.cur_Idx === this.myPhotograph.imgArray.length){
            this.myLargePic.childNodes[2].childNodes[0].classList.remove("disabled");
        }
        if(this.myPhotograph.imgArray.length === 1){
            this.myLargePic.childNodes[2].childNodes[0].classList.add("disabled");
        } 
        if(this.myIndex === 0){
            this.myLargePic.childNodes[0].childNodes[0].classList.add("disabled");
        }
        else if(this.myIndex === this.myPhotograph.imgArray.length - 1){
            this.myLargePic.childNodes[2].childNodes[0].classList.add("disabled");
        }
        if(this.myPhotograph.cur_Idx !== this.myPhotograph.imgArray.length)
            this.myParent.childNodes[this.myPhotograph.cur_Idx].classList.remove("selected");
        this.myPhotograph.setcur_Idx(this.myIndex);
        this.myNode.classList.add("selected");
        this.myLargePic.childNodes[1].childNodes[0].src = this.myNode.src;
    }
    delete(){
        alert("Picture deleted!");
        countN--;
        document.getElementById("total").textContent = `目前總張數：${countN}`;
        for(let i = this.myIndex+1; i < this.myParent.childNodes.length; ++i){
            this.myPhotograph.SmallPictures[i].setMyIndex(i-1);
        }
        this.myPhotograph.SmallPictures.splice(this.myIndex, 1);
        this.myPhotograph.imgArray.splice(this.myIndex, 1);
        this.myParent.removeChild(this.myNode);
        if(this.myParent.childNodes.length === 1){
            this.myLargePic.childNodes[2].childNodes[0].classList.add("disabled");
        }
        document.getElementById("pnode1").textContent = `相簿總張數：${this.myPhotograph.imgArray.length}`;
        if(this.myParent.childNodes.length === 0){
            alert("The album is empty! Add some photos.");
            document.getElementById("pnode2").textContent = "";
            this.myLargePic.childNodes[1].childNodes[0].src = "";
            this.myPhotograph.setcur_Idx(0);
            return;
        }
        this.myPhotograph.SmallPictures[0].select();
    }
}
class photogrph{
    constructor(arr){
        countN += arr.length;
        this.node = document.createElement("div");
        this.node.classList.add("photograph");

        let add_pic = document.createElement("input");
        add_pic.name = "newpic";
        add_pic.id = "inputBox";
        add_pic.type = "text";
        add_pic.placeholder = "URL(請輸入正確網址！)";
        this.node.appendChild(add_pic);
        let submit = document.createElement("input");
        submit.type = "submit";
        submit.value = "add a new pic!";
        submit.addEventListener("click", this.addNewPic.bind(this));
        let deleteSign = document.createElement("p");
        deleteSign.textContent = "連續點擊預覽圖兩下可刪除該相片！";
        this.formNode = document.createElement("div");
        this.formNode.appendChild(deleteSign);
        this.formNode.appendChild(add_pic);
        this.formNode.appendChild(submit);
        let pnode1 = document.createElement("p");
        pnode1.textContent = `相簿總張數：${arr.length}`;
        let pnode2 = document.createElement("p");
        pnode2.textContent = "";
        pnode1.id = "pnode1";
        pnode2.id = "pnode2";
        this.formNode.appendChild(pnode1);
        this.formNode.appendChild(pnode2);
        this.node.appendChild(this.formNode);

        this.display_node = document.createElement("div");

        this.display_node.classList.add("display");
        this.node.appendChild(this.display_node);
        this.lNode = document.createElement("div");
        this.cNode = document.createElement("div"),
        this.rNode = document.createElement("div");

        this.lNode.innerHTML = '<img src="back.png" class="disabled"/>';
        this.cNode.innerHTML = '<img src="" id="large_size">';
        this.rNode.innerHTML = '<img src="next.png" class="disabled"/>';

        this.lNode.classList.add("image-viewer__button");
        this.rNode.classList.add("image-viewer__button");
        this.cNode.classList.add("image-viewer__display");


        this.display_node.appendChild(this.lNode);
        this.display_node.appendChild(this.cNode);
        this.display_node.appendChild(this.rNode);

        this.img_arr = arr;
        this.cur_idx = 0;
        if(arr.length > 0) 
            pnode2.textContent = `目前在瀏覽第${this.cur_idx+1}張`;
        this.previewNode = document.createElement("div");
        this.previewNode.classList.add("preview");
        this.node.appendChild(this.previewNode);
        this.smallPictures = [];
        if(this.img_arr.length > 0){
            this.cNode.childNodes[0].src = this.img_arr[0];
            for(let i = 0; i < this.img_arr.length; ++i){
                this.smallPictures[this.smallPictures.length] 
                    = new small_size_pic(this.previewNode, i, this.display_node, this.img_arr[i], this);
            }
            this.rNode.childNodes[0].classList.remove("disabled");
            this.previewNode.childNodes[0].classList.add("selected");
        }
        this.lNode.childNodes[0].addEventListener("click", this.toPrev.bind(this));
        this.rNode.childNodes[0].addEventListener("click", this.toNext.bind(this));
        album_list[album_list.length] = this;
    }
    get Node(){
        return this.node;
    }
    get imgArray(){
        return this.img_arr;
    }
    get LNode(){
        return this.lNode;
    }
    get RNode(){
        return this.rNode;
    }
    get CNode(){
        return this.cNode;
    }
    get cur_Idx(){
        return this.cur_idx;
    }
    get PreviewNode(){
        return this.previewNode;
    }
    get SmallPictures(){
        return this.smallPictures;
    }
    setcur_Idx(newVal){
        this.cur_idx = newVal;
    }
    toNext(){
        if(this.cur_Idx === this.imgArray.length - 1) return;
        this.setcur_Idx(this.cur_Idx+1);
        document.getElementById("pnode2").textContent = `目前在瀏覽第${this.cur_idx+1}張`;
        this.CNode.childNodes[0].src = this.imgArray[this.cur_Idx];
        this.PreviewNode.childNodes[this.cur_Idx].classList.add("selected");
        this.PreviewNode.childNodes[this.cur_Idx-1].classList.remove("selected");
        if(this.cur_Idx === this.imgArray.length - 1){
            this.RNode.childNodes[0].classList.add("disabled");
        }
        if(this.cur_Idx === 1){
            this.LNode.childNodes[0].classList.remove("disabled");
        }
    }
    toPrev(){
        if(this.cur_Idx === 0) return;
        this.setcur_Idx(this.cur_Idx-1);
        document.getElementById("pnode2").textContent = `目前在瀏覽第${this.cur_idx+1}張`;
        this.cNode.childNodes[0].src = this.imgArray[this.cur_Idx];
        this.PreviewNode.childNodes[this.cur_Idx].classList.add("selected");
        this.PreviewNode.childNodes[this.cur_Idx+1].classList.remove("selected");
        if(this.cur_Idx === 0){
            this.LNode.childNodes[0].classList.add("disabled");
        }
        if(this.cur_Idx === this.imgArray.length - 2){
            this.RNode.childNodes[0].classList.remove("disabled");
        }
    }
    addNewPic(){
        if(this.formNode.childNodes[1].value === '') return;
        if(this.img_arr.length === 0){
            document.getElementById("pnode2").textContent = "目前在瀏覽第1張";
        }
        console.log(this.formNode.childNodes[1].value);
        countN++;
        document.getElementById("total").textContent = `目前總張數：${countN}`;
        if(this.img_arr.length === 0){
            console.log("hello!");
            this.setcur_Idx(0);
            this.display_node.childNodes[1].childNodes[0].src = this.formNode.childNodes[1].value;
        }
        if(this.img_arr.length === 1){
            this.rNode.childNodes[0].classList.remove("disabled");
        }
        if(this.cur_idx === this.img_arr.length - 1){
            this.rNode.childNodes[0].classList.remove("disabled");
        }
        this.img_arr[this.img_arr.length] = this.formNode.childNodes[1].value;
        let newPic = new small_size_pic(this.previewNode, this.img_arr.length-1, this.display_node, this.formNode.childNodes[1].value, this);
        if(this.img_arr.length === 1) newPic.myNode.classList.add("selected");
        this.formNode.childNodes[1].value = '';
        this.smallPictures[this.smallPictures.length] = newPic;
        document.getElementById("pnode1").textContent = `相簿總張數：${this.img_arr.length}`;
    }
}
class selector{
    constructor(idx, pn, name){
        this.myIndex = idx;
        this.myParent = pn;
        this.myNode = document.createElement("input");
        this.myNode.value = name;
        this.myNode.type = "submit";
        document.getElementById("list").appendChild(this.myNode);
        this.myNode.addEventListener("click", this.select.bind(this));
        this.myNode.addEventListener("dblclick", this.delete.bind(this));
    }
    get MyNode(){
        return this.myNode;
    }
    setMyIndex(i){
        this.myIndex = i;
    }
    select(){
        if(this.myParent.cur_Idx === this.myIndex) return;
        if(album_list[this.myIndex].imgArray.length === 0){
            alert("The album is empty! Add some photos.");
        }
        if(this.myParent.cur_Idx !== -1)
            document.getElementById("list").childNodes[this.myParent.cur_Idx+3].classList.remove("selected");
        this.myNode.classList.add("selected");
        if(this.myParent.cur_Idx !== -1)
            document.body.removeChild(album_list[this.myParent.cur_Idx].Node);
        document.body.appendChild(album_list[this.myIndex].Node);
        this.myParent.setcur_Idx(this.myIndex);
    }
    delete(){
        alert("Album deleted!");
        countN -= album_list[this.myIndex].imgArray.length;
        document.getElementById("total").textContent = `目前總張數：${countN}`;
        this.myNode.classList.remove("selected");
        for(let i = this.myIndex+1; i < album_list.length; ++i){
            this.myParent.myArr[i].setMyIndex(i-1);
        }
        document.body.removeChild(album_list[this.myIndex].Node);
        album_list.splice(this.myIndex, 1);
        this.myParent.myArr.splice(this.myIndex, 1);
        this.myParent.setcur_Idx(-1);
        document.getElementById("list").removeChild(this.myNode);
        if(this.myParent.myArr.length === 0){
            return;
        }
        this.myParent.myArr[0].select();
    }
}
class list{
    constructor(){
        this.cur_idx = 0;
        document.body.appendChild(album_list[this.cur_idx].Node);
        this.arr = [];
        this.arr[0] = new selector(0, this, "電系");
        this.arr[0].MyNode.classList.add("selected");
        this.arr[1] = new selector(1, this, "火系");
        this.arr[2] = new selector(2, this, "水系");
        this.arr[3] = new selector(3, this, "神獸");
        this.arr[4] = new selector(4, this, "地面系");
        document.getElementById("enterNewAlbum").addEventListener("click", this.add.bind(this));
    }
    setcur_Idx(i){
        this.cur_idx = i;
    }
    get myArr(){
        return this.arr;
    }
    get cur_Idx(){
        return this.cur_idx;
    }
    add(){
        let newPhotograph = new photogrph([]);
        console.log(newPhotograph);
        this.arr[this.arr.length] = new selector(album_list.length-1, this, document.getElementById("newalbum").value);
        document.getElementById("newalbum").value = "";
    }
}
const init_img = ["https://i.imgur.com/V2CYNaw.png", "https://i.imgur.com/EXEhUNe.png", "https://i.imgur.com/Zx7jj8k.png", "https://i.imgur.com/kyiTbxx.png", 
                    "https://i.imgur.com/RsFWgwJ.png", "https://i.imgur.com/83md8Cl.png", "https://i.imgur.com/y6oLcqt.png", "https://i.imgur.com/nInlFSy.png", 
                    "https://i.imgur.com/FVEbBun.png", "https://i.imgur.com/VkMuDvm.png", "https://i.imgur.com/6Vk2eW3.png", "https://i.imgur.com/tRbNtJP.png",
                    "https://i.imgur.com/PsCnBsL.png"];
const init_img2 = ["https://i.imgur.com/3SHHdlF.png", "https://i.imgur.com/3ZmLjpN.png", "https://i.imgur.com/KxHU0EA.png", "https://i.imgur.com/tgLpnLZ.png", 
                    "https://i.imgur.com/F8WRLiC.png", "https://i.imgur.com/n7R3oWr.png", "https://i.imgur.com/vMLBFIj.png"];
const init_img3 = ["https://i.imgur.com/KqR2CJT.png", "https://i.imgur.com/EXZjgKy.png", "https://i.imgur.com/eUfPF41.png", "https://i.imgur.com/2GluQ1i.png", 
                    "https://i.imgur.com/AszSqpm.png", "https://i.imgur.com/qpBdULT.png"];
const init_img4 = ["https://i.imgur.com/nc9wnB2.png", "https://i.imgur.com/7xA4mGQ.png", "https://i.imgur.com/Tw1HXUt.png",  "https://i.imgur.com/DLgvk7l.png",
                    "https://i.imgur.com/eC2pDPx.png", "https://i.imgur.com/khEMFnn.png", "https://i.imgur.com/AmtVwoq.png"];
let init_album = new photogrph(init_img);
let init_album2 = new photogrph(init_img2);
let init_album3 = new photogrph(init_img3);
let init_album4 = new photogrph(init_img4);
let init_album5 = new photogrph([]);
let selector_list = new list();
document.getElementById("total").textContent = `目前總張數：${countN}`;