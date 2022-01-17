import { message } from 'antd'

const status_display = (payload) => {
    if(payload.msg){
        const { type, msg } = payload;
        const content = { content: msg, duration: 1.5 };
        switch(type){
            case 'success':{
                message.success(content);
                break;
            }
            case 'info':{
                message.info(content);
                break;
            }
            default: {
                message.error(content);
                break;
            }
        }
    }
}
const status = { display: status_display };
export default status;