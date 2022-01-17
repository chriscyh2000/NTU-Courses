import { Input, Button } from 'antd';
import { useRef, useState } from 'react';
import status from '../hook/status';
import { CheckSquareTwoTone, CloseSquareTwoTone } from '@ant-design/icons'
import "../App.css";
import { UpgradeProModal, UpgradeProMaxModal } from './UpgradeProModal'; 

export default function Pro(props) {

    const [buyProVisible, setbuyProVisible] = useState(false);
    const [buyProMaxVisible, setbuyProMaxVisible] = useState(false);
    const CCNRef = useRef("0000-0000-0000-0000");
    const CSCRef = useRef("123");
    
    const handleCancel = () => {
        setbuyProVisible(false);
        setbuyProMaxVisible(false);
    }

    const handleCreate = async () => {
        status.display({
            type: "success", msg: "Upgrade successfully"
        })
        setbuyProVisible(false);
        setbuyProMaxVisible(false);
    }

    return (
        <>
        <div className='pro'>
            <div className='proOption'>
                <h1 style={ { marginTop: "10px" } }>Free</h1>
                <div className='inlineParent'>
                    <CheckSquareTwoTone className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;1 post per day</h2>
                </div>
                <div className='inlineParent'>
                    <CheckSquareTwoTone className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;max 30 favorites</h2>
                </div>
                <div className='inlineParent'>
                    <CloseSquareTwoTone className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#ff0000'/>
                    <h2 className='inlineChild'>&nbsp;no coupon</h2>
                </div>
                <div className='inlineParent'>
                    <CloseSquareTwoTone className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#ff0000'/>
                    <h2 className='inlineChild'>&nbsp;no event</h2>
                </div>
                <br/>
                <h2 style={ { fontWeight: "700" } }>Free</h2>
                <Button disabled size="large" shape="round">Default</Button>
                <p>&nbsp;</p>
            </div>
            <div className='proOption'>
                <h1 style={ { marginTop: "10px" } }>Pro</h1>
                <div className='inlineParent'>
                    <CheckSquareTwoTone className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;5 posts per day</h2>
                </div>
                <div className='inlineParent'>
                    <CheckSquareTwoTone className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;max 100 favorites</h2>
                </div>
                <div className='inlineParent'>
                    <CheckSquareTwoTone className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;coupon</h2>
                </div>
                <div className='inlineParent'>
                    <CloseSquareTwoTone className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#ff0000'/>
                    <h2 className='inlineChild'>&nbsp;no event</h2>
                </div>
                <br/>
                <h2 style={ { fontWeight: "700" } }>99 NTD / month</h2>
                <Button type="primary" size="large" shape="round" onClick={() => setbuyProVisible(true)}>Buy now</Button>
                <p>&nbsp;</p>
            </div>
            <div className='proOption'>
                <h1 style={ { marginTop: "10px" } }>Pro Max</h1>
                <div className='inlineParent'>
                    <CheckSquareTwoTone className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;unlimited posts</h2>
                </div>
                <div className='inlineParent'>
                    <CheckSquareTwoTone className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;unlimited favorites</h2>
                </div>
                <div className='inlineParent'>
                    <CheckSquareTwoTone className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;coupon</h2>
                </div>
                <div className='inlineParent'>
                    <CheckSquareTwoTone className='inlineChild' style={{ fontSize: '24px' }} twoToneColor='#00ab00'/>
                    <h2 className='inlineChild'>&nbsp;pro max user events</h2>
                </div>
                <br/>
                <h2 style={ { fontWeight: "700" } }>199 NTD / month</h2>
                <Button type="primary" size="large" shape="round" onClick={() => setbuyProMaxVisible(true)}>Buy now</Button>
                <p>&nbsp;</p>
            </div>
        </div>
        <UpgradeProModal 
            visible={buyProVisible}
            onCancel={handleCancel}
            onOk={handleCreate}
            CCNRef={CCNRef}
            CSCRef={CSCRef}
        />
        <UpgradeProMaxModal 
            visible={buyProMaxVisible}
            onCancel={handleCancel}
            onOk={handleCreate}
            CCNRef={CCNRef}
            CSCRef={CSCRef}
        />
        </>
    )
}