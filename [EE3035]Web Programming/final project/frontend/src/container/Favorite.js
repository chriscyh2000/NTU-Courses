import Store from '../component/Store'
import { useQuery } from '@apollo/client';
import { STORES_SUBSCRIPTION, STORES_QUERY } from '../graphql';
import { LoadingOutlined } from '@ant-design/icons'
import { Empty, Modal, Input } from 'antd';
import { useState, useEffect } from "react";
import '../App.css';
import PostBox from './PostBox';
import status from '../hook/status';

const Favorite = (props) => {
    const { data, loading, subscribeToMore, refetch } = useQuery(STORES_QUERY);
    if(props.user === null){
        return(
            <div className="storeBox">
                <>
                    <h3>Please sign in first.</h3>
                    <br/>
                    <br/>
                    <Empty/>
                </>
            </div>
        )
    }

    const { name, id, favs } = props.user;
    let r = 0;
    refetch();

    let stores = favs;
    function compare(store1, store2){
        const avg1 = store1.score / ((store1.postCount === 0) ? 1 : store1.postCount);
        const avg2 = store2.score / ((store2.postCount === 0) ? 1 : store2.postCount);
        if(avg1 > avg2) return -1;
        if(avg2 > avg1) return 1;
        return 0;
    }


    if(loading && data === undefined){
        return(
            <div className="storeBox">
                <LoadingOutlined style={{ fontSize: "70px" }}/>
            </div>
        )
    }

    stores.sort(compare);
    return (
        <div className="storeBox">
            {
                (stores.length === 0) ? (
                    <>
                        <h3>My favorite</h3>
                        <br/>
                        <br/>
                        <Empty/>
                    </>
                    ) : (
                        <div>
                            <h3>My favorite</h3>
                            {
                                favs.map((store) => {
                                    return <Store initColor="red" refetch={props.refetch} store={store} uid={id}></Store>
                                })
                            }
                        </div>
                    )
            }
        </div>
    )
}

export default Favorite;