import axios from 'axios'

const API_ROOT = 'http://localhost:4000/api'

const instance = axios.create({
  baseURL: API_ROOT,
  withCredentials: true
})

export default instance
