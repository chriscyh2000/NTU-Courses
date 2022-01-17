import { Menu } from 'antd';
import { AppstoreOutlined, SettingOutlined, StarOutlined, HeartOutlined, SmileOutlined, EnvironmentOutlined, HomeOutlined, PlusSquareFilled, PlusSquareOutlined, HistoryOutlined, MessageOutlined } from '@ant-design/icons';

export default function LeftMenuModal(props) {
    const { theme, page, menu } = props;
    const { SubMenu } = Menu;
    const handleClick = (e) => {
        menu(false);
        page(e.key);
    }

    return (
        <Menu
            theme={theme}
            onClick={handleClick}
            style={{ width: 256 }}
            // defaultOpenKeys={['sub1']}
            // selectedKeys={[this.state.current]}
            mode="inline"
        >
            <Menu.Item key="home" icon={<HomeOutlined />}>home</Menu.Item>
            {/* <Menu.Item key="upgrade" icon={<StarOutlined />}>upgrade Pro</Menu.Item> */}
            <Menu.Item key="create" icon={<PlusSquareOutlined />}>create post</Menu.Item>
            <Menu.Item key="my" icon={<HistoryOutlined />}>my post</Menu.Item>
            <Menu.Item key="favorite" icon={<HeartOutlined />}>favorite</Menu.Item>
            {/* <Menu.Item key="location" icon={<EnvironmentOutlined />}>location</Menu.Item> */}
            <Menu.Item key="profile" icon={<SmileOutlined />}>profile</Menu.Item>
            <Menu.Item key="contact" icon={<MessageOutlined />}>contact us</Menu.Item>
            {/* <Menu.Item key="setting" icon={<SettingOutlined />}>setting</Menu.Item> */}

        </Menu>
    );
}