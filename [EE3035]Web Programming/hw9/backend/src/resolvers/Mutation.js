import uuidv4 from 'uuid/v4';
function make_name(name1, name2){
  const arr = [name1, name2];
  return arr.sort().join('_');
}
const checkUser = async (DB, name) => {
  const user = await DB.UserModel.findOne({ name: name });
  return user;
}
const checkChatBox = async (DB, name) => {
  const chatBox =  await DB.ChatBoxModel.findOne({ name: name });
  return chatBox;
}

const newUser = async (DB, name, password) => {
  const nu = await new DB.UserModel({
    id: uuidv4(),
    name: name,
    password: password 
  });
  await nu.save();
  return nu;
}
const newChatBox = async (DB, name) => {
  const ncb = await new DB.ChatBoxModel({
    id: uuidv4(),
    name: name,
    messages: []
  });
  await ncb.save();
  return ncb;
}

const Mutation = {
  async createChatBox(parent, args, context, info){
    const { name1, name2 } = args;
    const { db } = context;

    if(!name1 || !name2){
      throw new Error("Missing chatbox name for createChatBox!");
    }

    if(!(await checkUser(db, name1))){
      console.log("user does not exist for createChatBox!");
      await newUser(db, name1, '-1'); // the user has not been signed up
    }

    if(!(await checkUser(db, name2))){
      console.log("user does not exist for createChatBox!");
      await newUser(db, name2, '-1');
    }

    const chatBoxName = make_name(name1, name2);
    let chatBox = await checkChatBox(db, chatBoxName);
    if(!chatBox) chatBox =  await newChatBox(db, chatBoxName);

    return chatBox;
  },

  async createUser(parent, args, context, info){
    const { name, password } = args;
    const { db } = context;
    let usr = await checkUser(db, name);
    if(!usr){
      usr = await newUser(db, name, password);
    }
    else if(usr.password === '-1'){
        usr.password = password;
        await usr.save();
    }
    else{
      throw new Error("existing");
    } 
    return usr;
  },

  async createMessage(parent, args, context, info){
    const { sender, receiver, body } = args;
    const { db, pubsub } = context;

    const chatBoxName = [sender, receiver].sort().join('_');
    const chatBox = await checkChatBox(db, chatBoxName);
    const Sender= await checkUser(db, sender);

    const id = uuidv4();
    let newMessage = await new db.MessageModel({
        id: id,
        sender: Sender,
        body: body
    }).save();


    // chatBox.messages = []
    chatBox.messages.push(newMessage);

    await chatBox.save();
    await pubsub.publish(`ChatBox ${chatBoxName}`, {
      chatBox: {
        mutation: 'CREATED',
        data: {
          id: id,
          sender: Sender.id,
          body: body
        }
      }
    })
    return newMessage;
  },

  async login(parent, { name, password }, { db }, info){
    const USR = await checkUser(db, name);
    if(!USR || USR.password === '-1'){
      return {
        message: "User not found.",
        password: null
      }
    }
    return {
      message: "User exist.",
      password: USR.password
    }
  },

  async signup(parent, { name, password }, { db }, info){
    const USR = await checkUser(db, name);
    if(!USR){
      newUser(db, name, password);
    }
    else if(USR.password === '-1'){
      USR.password = password;
      await USR.save();
    }
    else{
      return {
        message: "User exist.",
        password: null
      }
    }
    return {
      message: "Sign-up succeeded.",
      password: null
    }
  }
}

export default Mutation;