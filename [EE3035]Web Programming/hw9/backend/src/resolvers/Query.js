const Query = {
  async chatbox(parent, { name }, { db }, info){
    let chatBox = await db.ChatBoxModel.findOne({ name: name });
    return chatBox.populate('users')
                  .populate({ path: 'messages', populate: 'sender' })
                  .execPopulate();;
  },

  async user(parent, { name }, { db }, info){
    let User = await db.UserModel.findOne({ name });
    return User;
  }
}
export default Query;