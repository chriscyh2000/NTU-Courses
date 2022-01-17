const ChatBox = {
  messages(parent, args, context, info){
    const { db } = context;
    // chatBox schema is constructed with 3 property, one of them is 'messages'
    // the type of an element in 'messages' is Id
    // when you use Query->ChatBox(name), you will get the corresponding chatbox
    // and the chatbox will become the parent here
    const msgs = parent.messages.map(async(mId) => {
        return await db.MessageModel.findById(mId);
    });
    return msgs;
    // wait until all the operations are finished then return
  }
}

export default ChatBox;