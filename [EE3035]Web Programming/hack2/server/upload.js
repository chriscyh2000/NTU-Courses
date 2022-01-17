import Post from './models/post'

const example = [
  {
    postId: '1b9d6bcd-bbfd-4b2d-9b5d-ab8dfbbd4bed',
    title: 'Hackathon 2 Title',
    content: 'Hope you guys have a nice experience while writing hackathon 2 !',
    timestamp: new Date('2021-12-02T05:51:04.360Z')
  },
  {
    postId: '7a01acle-a98f-4dbc-b39e-7e50c4999a05',
    title: 'Test 2 Title',
    content: 'Show testing 2 content here.',
    timestamp: new Date('2021-12-01T03:32:10.360Z')
  },
  {
    postId: '65c4a4d8-c0f9-4f07-86cb-d213d7b66243',
    title: 'No Idea How to Name This Title',
    content: 'Any suggestions ?',
    timestamp: new Date('2021-12-01T01:45:01.360Z')
  },
  {
    postId: '11f10e16-1721-4e0f-8080-827229198f06',
    title: '2021-1218 Referenda',
    content: 'Taiwan is going to hold referenda on December 18th.',
    timestamp: new Date('2021-12-02T04:13:24.360Z')
  }
]

const dataInit = async () => {
  const checkData = await Post.find()
  if (checkData.length !== 4) {
    await Post.deleteMany({})
    await Post.insertMany(example)
  }
}

export { dataInit }
