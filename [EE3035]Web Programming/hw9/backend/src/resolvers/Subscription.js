const Subscription = {
  chatBox: {
    subscribe(parent, { name }, { db, pubsub }, info){
      const box = db.ChatBoxModel.findOne({ name });
      if(!box){
        throw new Error('ChatBox does not exist!');
      }
      return pubsub.asyncIterator(`ChatBox ${name}`);
    }
  },
}

export default Subscription;