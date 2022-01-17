describe('Hackathon 2 Public Test', () => {
  it('1 (20%)', () => {
    cy.visit({ url: 'localhost:4000', failOnStatusCode: false })
  })
  it('2-(1) (10%)', () => {
    cy.request('localhost:4000/api/allPosts')
    .then((response) => {
      expect(response.status).to.eq(200)
      expect(response.body).to.have.property('message')
      expect(response.body).to.have.property('data')
      expect(response.body.data).to.have.length(4)
    })
  })
  it('2-(2) (10%)', () => {
    cy.visit('localhost:3000')
    cy.get('#pid-0-title').should('contain', 'Hackathon 2 Title')
    cy.get('#pid-0-time').should('contain', '2021-12-02')
    cy.get('#pid-1-title').should('contain', '2021-1218 Referenda')
    cy.get('#pid-1-time').should('contain', '2021-12-02')
    cy.get('#pid-2-title').should('contain', 'Test 2 Title')
    cy.get('#pid-2-time').should('contain', '2021-12-01')
    cy.get('#pid-3-title').should('contain', 'No Idea How to Name This Title')
    cy.get('#pid-3-time').should('contain', '2021-12-01')
  })
  it('3-(1) (10%)', () => {
    cy.request('localhost:4000/api/postDetail?pid=1b9d6bcd-bbfd-4b2d-9b5d-ab8dfbbd4bed')
    .then((response) => {
      expect(response.status).to.eq(200)
      expect(response.body).to.have.property('message')
      expect(response.body).to.have.property('post')
      expect(response.body.post).to.have.property('postId')
      expect(response.body.post).to.have.property('title')
      expect(response.body.post).to.have.property('content')
      expect(response.body.post).to.have.property('timestamp')
    })
  })
  it('3-(2) (10%)', () => {
    cy.visit('localhost:3000')
    cy.get('#pid-0-title').click()
    cy.get('#pid-detail-title').should('contain', 'Hackathon 2 Title')
    cy.get('#pid-detail-content').should('contain', 'Hope you guys have a nice experience while writing hackathon 2 !')
    cy.get('#pid-detail-time').should('contain', '2021-12-02 13:51:04')
    cy.get('#goback-reply-btn').click()
    cy.get('#pid-3-title').click()
    cy.get('#pid-detail-title').should('contain', 'No Idea How to Name This Title')
    cy.get('#pid-detail-content').should('contain', 'Any suggestions ?')
    cy.get('#pid-detail-time').should('contain', '2021-12-01 09:45:01')
  })
  it('4-(1) (10%)', () => {
    cy.request({
      method: 'POST',
      url: 'localhost:4000/api/newPost',
      failOnStatusCode: false,
      body: {
        postId: '228d96c4-fc74-4a7a-86ea-aeeb7f574d69',
        title: 'Public Test',
        content: 'public test',
        timestamp: new Date('2021-12-02T06:50:03.360Z')
      }
    })
    .then((response) => {
      expect(response.status).to.eq(200)
      expect(response.body).to.have.property('message')
    })

    cy.request('localhost:4000/api/allPosts')
    .then((response) => {
      expect(response.status).to.eq(200)
      expect(response.body).to.have.property('message')
      expect(response.body).to.have.property('data')
      expect(response.body.data).to.have.length(5)
    })
  })
  it('4-(2) (10%)', () => {
    cy.visit('localhost:3000')
    cy.get('#pid-0-title').should('contain', 'Public Test')
    cy.get('#pid-0-time').should('contain', '2021-12-02')
    cy.get('#pid-post-btn').click()
    cy.get('#pid-create-title').type('Public Test 2')
    cy.get('#pid-create-content').type('public test 2')
    cy.get('#pid-create-submit-btn').click()
    cy.get('#pid-0-title').should('contain', 'Public Test 2')
    cy.get('#pid-1-title').should('contain', 'Public Test')
  })
  it('5-(1) (10%)', () => {
    cy.request({
      method: 'DELETE',
      url: 'localhost:4000/api/post?pid=228d96c4-fc74-4a7a-86ea-aeeb7f574d69',
      failOnStatusCode: false
    })
    .then((response) => {
      expect(response.status).to.eq(200)
      expect(response.body).to.have.property('message')
    })

    cy.request('localhost:4000/api/allPosts')
    .then((response) => {
      expect(response.status).to.eq(200)
      expect(response.body).to.have.property('message')
      expect(response.body).to.have.property('data')
      expect(response.body.data).to.have.length(5)
    })
  })
  it('5-(2) (10%)', () => {
    cy.visit('localhost:3000')
    cy.get('#pid-0-title').should('contain', 'Public Test 2')
    cy.get('#pid-0-title').click()
    cy.get('#pid-detail-del-btn').click()
    cy.get('#pid-0-title').should('contain', 'Hackathon 2 Title')
    cy.get('#pid-1-title').should('contain', '2021-1218 Referenda')
  })
})
