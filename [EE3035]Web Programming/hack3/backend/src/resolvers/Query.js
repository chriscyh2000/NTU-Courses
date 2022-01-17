const Query = {
  /**
   * Get all tasks
   */
  tasks: async (parent, args, { taskModel }) => {
    const tasks = await taskModel.find().sort({ dueDate: -1 });
    return tasks;
  },
};

export default Query;
