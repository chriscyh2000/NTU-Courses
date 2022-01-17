const Message = {
  async sender(parent, args, context, info){
    const { db } = context;
    return await db.UserModel.findById(parent.sender);
  },
  body(parent, args, context, info){
    return parent.body;
  }
}
export { Message as default };
