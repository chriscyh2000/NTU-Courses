import { Modal, Input, DatePicker } from 'antd';
import { useState, useEffect } from 'react';
import moment from 'moment';

function UpgradeProModal(props){
    const monthFormat = 'YYYY/MM';

    return (
        <Modal
            title="Upgrade Pro"
            visible={props.visible}
            onOk={props.onOk}
            onCancel={props.onCancel}
            onText="Upgrade Pro"
        >
            <p>Credit Card Number</p>
            <Input placeholder="XXXX-XXXX-XXXX-XXXX" allowClear="true" ref={props.CCNRef}></Input>
            <p>&nbsp;</p>
            <p>Card Security Code</p>
            <Input placeholder="XXX" allowClear="true" ref={props.CSCRef} />
            <p>&nbsp;</p>
            <p>Valid Dates</p>
            <DatePicker format={monthFormat} picker="month" />
        </Modal>
    )
}


function UpgradeProMaxModal(props){
    return (
        <Modal
            title="Upgrade Pro"
            visible={props.visible}
            onOk={props.onSignUp}
            onCancel={props.onCancel}
            onText="Upgrade Pro"
        >
            <Input placeholder="Credit Card Number" ref={props.CCNRef}></Input>
            <Input placeholder="Card Security Code" ref={props.CSCRef} />
        </Modal>
    )
}

export { UpgradeProModal, UpgradeProMaxModal };