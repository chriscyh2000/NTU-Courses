import mongoose from 'mongoose'

const Schema = mongoose.Schema

const PostSchema = Schema({
  postId: { type: String, unique: true },
  title: String,
  content: String,
  timestamp: Date
}, {
  collection: 'post',
  timestamps: { createdAt: 'created_at', updatedAt: 'updated_at' }
})

const exportSchema = mongoose.model('post', PostSchema)

export default exportSchema
