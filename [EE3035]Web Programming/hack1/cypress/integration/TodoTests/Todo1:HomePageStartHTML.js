describe('Todo1: HomePage Start HTML', () => {
    // 1st test
    it('HomeWrapper, title and start button exists exist', () => {
      cy.visit('/')
      cy.get('div').should('have.class', 'mineSweeper')
      cy.get('div').should('have.class', 'HomeWrapper')
      cy.get('p').should('have.class', 'title').contains('MineSweeper')
      cy.get('button').should('have.class', 'btn').contains('Start Game')
    })
    it('Correct Relationship', () => {
        cy.visit('/')
        cy.get('div[class="mineSweeper"]')
            .find('div:last-child')     // use find here to restrict search to previous subject
            .should('have.class', 'HomeWrapper')
        cy.get('div[class="HomeWrapper"]')
            .children().should('have.class','title')
    })
    it('tartGameOnClick hook function and switching from Home to Board  (5%)', () => {
        cy.visit('/')
        cy.get('button').contains('Start Game').click()
        cy.get('div').should('have.class', 'boardWrapper')
    })
})
