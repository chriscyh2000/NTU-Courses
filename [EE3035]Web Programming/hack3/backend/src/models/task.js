import mongoose from "mongoose";

/**
 * Task Schema for MongoDB
 */
const taskSchema = new mongoose.Schema(
  {
    id: { type: String, unique: true },
    title: String,
    content: String,
    dueDate: Date,
    status: String,
  },
  {
    collection: "task",
  }
);

export default mongoose.model("task", taskSchema);
