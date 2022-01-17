import Store from '../component/Store'
import { useQuery } from '@apollo/client';
import { STORES_SUBSCRIPTION, STORES_QUERY } from '../graphql';
import { LoadingOutlined } from '@ant-design/icons'
import { Empty, Modal, Input } from 'antd';
import { useState, useEffect } from "react";
import '../App.css';
import PostBox from './PostBox';
import status from '../hook/status';

const StoreBox = (props) => {
    const { Search } = Input;
    const [search, setSearch] = useState("");

    let r = 0;
    const { data, loading, subscribeToMore, refetch } = useQuery(STORES_QUERY);
    refetch();

    const [res, setRes] = useState([]);

    // string similarity
    function similar(str1, str2, f) {
        if(!str1 || !str2){
            return 0;
        }

        var l = str1.length > str2.length ? str1.length : str2.length;
        var n = str1.length;
        var m = str2.length;
        var d = [];
        f = f || 3;

        var min = function(a, b, c) {
            return a < b ? (a < c ? a : c) : (b < c ? b : c);
        }

        var i, j, si, tj, cost;
        if (n === 0) return m;
        if (m === 0) return n;

        for (i = 0; i <= n; i++) {
            d[i] = [];
            d[i][0] = i;
        }
        for (j = 0; j <= m; j++) {
            d[0][j] = j;
        }

        for (i = 1; i <= n; i++) {
            si = str1.charAt(i - 1);
            for (j = 1; j <= m; j++) {
                tj = str2.charAt(j - 1)
                if (si === tj) {
                    cost = 0;
                } 
                else {
                    cost = 1;
                }
                d[i][j] = min(d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + cost);
            }
        }
        let res = (1 - d[n][m] / l)
        return res.toFixed(f)
    }

    let stores = [];
    function compare(store1, store2){
        const avg1 = store1.score / ((store1.postCount === 0) ? 1 : store1.postCount);
        const avg2 = store2.score / ((store2.postCount === 0) ? 1 : store2.postCount);
        if(avg1 > avg2) return -1;
        if(avg2 > avg1) return 1;
        return 0;
    }

    useEffect(() => {
        try{
            subscribeToMore({
                document: STORES_SUBSCRIPTION,
                updateQuery: (prev, { subscriptionData }) => {
                    if(!subscriptionData.data) return prev;
                    if(subscriptionData.mutation === 'CREATED' && props.type !== 'find'){
                        if(r === 0){
                            stores[0] = subscriptionData.data.store.data;
                            for(let i = 0; i < prev.stores.length; ++i){
                                stores[prev.stores.length-i] = prev.stores[i];
                            }
                            r = 1;
                        } 
                        else{
                            stores = [subscriptionData.data.store.data, ...prev.stores];
                        }
                    } 
                    if(subscriptionData.mutation === 'UPDATED' && props.type !== 'find'){
                        for(let i = 0; i < prev.stores.length; ++i){
                            if(prev.stores[i].id !== subscriptionData.data.store.data.id){
                                stores.push(prev.stores[i]);
                            }
                            else stores.push(subscriptionData.data.store.data);
                        }
                    }
                    
                    if(props.type === 'popular'){
                        stores.sort(compare);
                    }
                    if(props.type !== 'find'){
                        return {
                            stores: stores
                        };
                    }
                    else{
                        let tmp = [];
                        stores = [];
                        for(let i = 0; i < prev.stores.length; ++i){
                            tmp.push(prev.stores[i]);
                            if(similar(search, prev.store[i].name, 3) > 0.15){
                                stores.push(prev.store[i]);
                            }
                        }
                        tmp[tmp.length] = subscriptionData.data.store.data;
                        if(similar(search, subscriptionData.data.store.data.name, 3) > 0.2){
                            stores.push(subscriptionData.data.store.data);
                        }
                        setRes(stores);
                        return{
                            stores: tmp
                        }
                    }
                }
            })
        }
        catch(e){console.log(`error ${e}`)}
    }, [subscribeToMore])

    if(loading && data === undefined){
        return(
            <div className="storeBox">
                <LoadingOutlined style={{ fontSize: "70px" }}/>
            </div>
        )
    }
    
    const query = () => {
        for(let i = 0; i < data.stores.length; ++i){
            if(parseFloat(similar(search, data.stores[i].name, 3)) > 0.15){
                stores.push(data.stores[i]);
            }
        }
        setRes(stores);
    }
    if(props.type !== 'find'){
        for(let i = 0; i < data.stores.length; ++i){
            stores.push(data.stores[i]);
        }
    }
    if(props.type === 'popular' || props.type === 'find'){
        stores.sort(compare);
    }
    else if(props.type === 'recent'){
        for(let i = 0; i < data.stores.length; ++i){
            stores[data.stores.length-1-i] = data.stores[i];
        }
    }

    return (
        <div className="storeBox">
            {
                (stores.length === 0 && props.type !== 'find') ? (
                    <>
                        <br/>
                        <br/>
                        <Empty/>
                    </>
                    ) : (
                        <div>
                            {(props.type === 'find') ? 
                            <>
                                <p>&nbsp;</p>
                                <Search 
                                    className='searchBar' 
                                    placeholder="input search text" 
                                    onChange={(e) => {setSearch(e.target.value)}}  
                                    onSearch={() => {
                                        if(search === ''){
                                            status.display({
                                                type: 'error',
                                                msg: 'Enter your query'
                                            })
                                        }
                                        query();
                                    }}
                                    enterButton 
                                    style={{ width: 400 }} 
                                    size="large"
                                />
                                <p>&nbsp;</p>

                            </> : <></>}

                            {(props.type !== 'find') ? 
                                ((props.userFavs !== null) ? 
                                    stores.map((store) => {
                                        for(let i = 0 ; i < props.userFavs.length; ++i){
                                            if(props.userFavs[i].id === store.id){
                                                return <Store initColor="red" refetch={props.refetch} store={store} uid={props.uid}></Store>
                                            }
                                        }
                                        return <Store initColor="" refetch={props.refetch} store={store} uid={props.uid}></Store>
                                    }) : 
                                    stores.map((store) => {
                                        return <Store initColor="" refetch={props.refetch} store={store} uid={props.uid}></Store>
                                    })) : 
                                ((props.userFavs !== null) ? 
                                    res.map((store) => {
                                        for(let i = 0 ; i < props.userFavs.length; ++i){
                                            if(props.userFavs[i].id === store.id){
                                                return <Store initColor="red" refetch={props.refetch} store={store} uid={props.uid}></Store>
                                            }
                                        }
                                        return <Store initColor="" refetch={props.refetch} store={store} uid={props.uid}></Store>
                                    }) :
                                    res.map((store) => {
                                        return <Store initColor="" refetch={props.refetch} store={store} uid={props.uid}></Store>
                                    })
                                )
                            }
                        </div>
                    )
            }
        </div>
    )
}

export default StoreBox;