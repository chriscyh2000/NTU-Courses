const nativeInputValueSetter = Object.getOwnPropertyDescriptor(
    window.HTMLInputElement.prototype,
    'value'
  ).set
const changeRangeInputValue = $range => value => {
    nativeInputValueSetter.call($range[0], value)
    $range[0].dispatchEvent(new Event('change', { value, bubbles: true }))
}

describe('Todo6: HomePage Control', () => {
    // 1st test
    it('Correct HTML relationship', () => {
      //container exists
      cy.visit('/')
      cy.get('div').should('have.class', 'HomeWrapper').children().should('have.class','controlContainer')
      cy.get('div').should('have.class', 'controlContainer').children().should('have.class','btn').contains('Difficulty Adjustment')
      //Control container exists
      cy.get('button').contains('Difficulty Adjustment').click()
      cy.get('div').should('have.class', 'controlContainer').children().should('have.class','controlWrapper')
      cy.get('div').should('have.class','controlWrapper').children().should('have.class','controlPanel')
      //Mines Number
      cy.get('.controlPanel > div:nth-child(1)').should('have.class','controlCol')
      cy.get('.controlCol:nth-child(1) > .controlTitle:nth-child(1)').contains('Mines Number')
      
      cy.get('.controlCol:nth-child(1) > input')
        .invoke('attr', 'type')
        .should('equal', 'range')
      cy.get('.controlCol:nth-child(1) > input')
        .invoke('attr', 'min')
        .should('equal', '1')
      cy.get('.controlCol:nth-child(1) > input')
        .invoke('attr', 'step')
        .should('equal', '1')
      cy.get('.controlCol:nth-child(1) > input')
        .invoke('attr', 'max')
        .should('equal', '50')
      cy.get('.controlCol:nth-child(1) > input')
        .invoke('attr', 'value')
        .should('equal', '10')
      
      cy.get('.controlCol:nth-child(1) > .controlNum:nth-child(3)')
      //Size
      cy.get('.controlPanel > div:nth-child(2)').should('have.class','controlCol')
      cy.get('.controlCol:nth-child(2) > .controlTitle:nth-child(1)').contains('Board Size (n×n)')
      
      cy.get('.controlCol:nth-child(2) > input')
        .invoke('attr', 'type')
        .should('equal', 'range')
      cy.get('.controlCol:nth-child(2) > input')
        .invoke('attr', 'max')
        .should('equal', '20')
      cy.get('.controlCol:nth-child(2) > input')
        .invoke('attr', 'min')
        .should('equal', '1')
      cy.get('.controlCol:nth-child(2) > input')
        .invoke('attr', 'step')
        .should('equal', '1')
      cy.get('.controlCol:nth-child(2) > input')
        .invoke('attr', 'value')
        .should('equal', '8')
      cy.get('.controlCol:nth-child(2) > .controlNum:nth-child(3)')
    
    })
    
    it('Input value update', () => {
        
        cy.get('.controlCol:nth-child(1) > input')
            .then(input => changeRangeInputValue(input)(20))
        cy.get('.controlCol:nth-child(1) > p:nth-child(3)').should('have.text', '20')
        cy.get('.controlCol:nth-child(2) > input')
            .then(input => changeRangeInputValue(input)(10))
        cy.get('.controlCol:nth-child(2) > p:nth-child(3)').should('have.text', '10')

        cy.get('.controlCol:nth-child(1) > input')
            .then(input => changeRangeInputValue(input)(30))
        cy.get('.controlCol:nth-child(1) > p:nth-child(3)').should('have.text', '30')
        cy.get('.controlCol:nth-child(2) > input')
            .then(input => changeRangeInputValue(input)(5))
        cy.get('.controlCol:nth-child(2) > p:nth-child(3)').should('have.text', '5')
        cy.get('.controlCol:nth-child(2) > input')
            .then(input => changeRangeInputValue(input)(50))
        cy.get('.controlCol:nth-child(2) > p:nth-child(3)').should('have.text', '20')
    })
    it('Error checking and prevent starting the game', () => {
        cy.get('.controlCol:nth-child(1) > input')
            .then(input => changeRangeInputValue(input)(40))
        cy.get('.controlCol:nth-child(2) > input')
            .then(input => changeRangeInputValue(input)(6))
        cy.get('.controlWrapper > div:nth-child(1)').should('have.css','color','rgb(136, 0, 0)').should('have.text', 'ERROR: Mines number and board size are invalid!')

    })
})
