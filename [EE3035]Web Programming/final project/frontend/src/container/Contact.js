import { Input, Button } from 'antd';
import { useRef, useState } from 'react';
import status from '../hook/status';
import { EnvironmentOutlined, MailOutlined } from '@ant-design/icons'
import "../App.css";
import { UpgradeProModal, UpgradeProMaxModal } from './UpgradeProModal'; 

export default function Contact(props) {

    return (
        <>
        <div className='contactWrapper'>
            <div className='contact'>
                <h1 style={ { marginTop: "10px" } }>B07902056 資工四 郭瑋喆💩</h1>
                <div className='inlineParent' style={{ margin: "0px 15px" }}>
                    <MailOutlined className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;b07902056@ntu.edu.tw</h2>
                </div>
                <div className='inlineParent' style={{ margin: "0px 15px" }}>
                    <EnvironmentOutlined className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;男八</h2>
                </div>
            </div>
            <div className='contact'>
                <h1 style={ { marginTop: "10px" } }>B07902142 資工四 許庭維⚡</h1>
                <div className='inlineParent' style={{ margin: "0px 15px" }}>
                    <MailOutlined className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;b07902142@ntu.edu.tw</h2>
                </div>
                <div className='inlineParent' style={{ margin: "0px 15px" }}>
                    <EnvironmentOutlined className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;水源BOT</h2>
                </div>
            </div>
            <div className='contact'>
                <h1 style={ { marginTop: "10px" } }>B08902149 資工三 徐晨祐🌟</h1>
                <div className='inlineParent' style={{ margin: "0px 15px" }}>
                    <MailOutlined className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;b08902149@ntu.edu.tw</h2>
                </div>
                <div className='inlineParent' style={{ margin: "0px 15px" }}>
                    <EnvironmentOutlined className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;水源BOT</h2>
                </div>
            </div>
        </div>
        </>
    )
}