const Subscription = {
  /**
   * Subscribe for task creation
   */
  taskCreated: {
    subscribe: (parent, args, { pubSub }) => {
      return pubSub.asyncIterator("TASK_CREATED");
    },
  },
  /**
   * Subscribe for task update
   */
  taskUpdated: {
    subscribe: (parent, args, { pubSub }) => {
      return pubSub.asyncIterator("TASK_UPDATED");
    },
  },
  /**
   * Subscribe for task deletion
   */
  // TODO 6.2 Add taskDeleted resolver
  taskDeleted: {
    subscribe: (parent, args, { pubSub }, info) => {
      return pubSub.asyncIterator("TASK_DELETED");
    },
  }
};

export default Subscription;
