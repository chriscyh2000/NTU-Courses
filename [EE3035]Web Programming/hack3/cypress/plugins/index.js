const mongoose = require("mongoose");
require("dotenv-defaults").config({ path: "backend/.env" });

const taskSchema = new mongoose.Schema(
  {
    id: { type: String, unique: true },
  },
  {
    collection: "task",
    strict: false,
  }
);

const model = mongoose.model("task", taskSchema);

const examples = [
  {
    id: "1",
    title: "This is task1",
    content: "This is the task's content",
    status: "TODO",
    dueDate: new Date("2021-12-02T05:51:04.360Z").getTime(),
  },
  {
    id: "4",
    title: "This is task4",
    content: "This is the task's content",
    status: "TODO",
    dueDate: new Date("2021-12-02T05:51:04.360Z").getTime(),
  },
  {
    id: "2",
    title: "This is task2",
    content: "This is the task's content",
    status: "INPROGRESS",
    dueDate: new Date("2021-12-01T03:32:10.360Z").getTime(),
  },
  {
    id: "5",
    title: "This is task5",
    content: "This is the task's content",
    status: "INPROGRESS",
    dueDate: new Date("2021-12-01T03:32:10.360Z").getTime(),
  },
  {
    id: "3",
    title: "This is task3",
    content: "This is the task's content",
    status: "DONE",
    dueDate: new Date("2021-12-01T03:32:10.360Z").getTime(),
  },
  {
    id: "6",
    title: "This is task6",
    content: "This is the task's content",
    status: "DONE",
    dueDate: new Date("2021-12-01T03:32:10.360Z").getTime(),
  },
];


module.exports = async (on, config) => {
  await mongoose.connect(process.env.MONGO_URL);

  on('task', {
    async initDB () {      
      const res = await model.deleteMany({});
      for (const example of examples) {
        const doc = new model(example);
        Object.keys(example).forEach(col => doc.markModified(col));
        await doc.save();
      }
      return null;
    },
  })
}