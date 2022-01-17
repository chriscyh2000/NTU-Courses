const board = {
  '0-0':'','0-1':'','0-2':'','0-3':1,'0-4':2,'0-5':2,'0-6':1,'0-7':'',
  '1-0':'','1-1':'','1-2':'','1-3':1,'1-4':'💣','1-5':'💣','1-6':1,'1-7':'',
  '2-0':'','2-1':'','2-2':'','2-3':2,'2-4':4,'2-5':4,'2-6':2,'2-7':'',
  '3-0':'','3-1':'','3-2':'','3-3':1,'3-4':'💣','3-5':'💣','3-6':1,'3-7':'',
  '4-0':1,'4-1':1,'4-2':'','4-3':1,'4-4':2,'4-5':3,'4-6':2,'4-7':1,
  '5-0':'💣','5-1':1,'5-2':'','5-3':1,'5-4':1,'5-5':3,'5-6':'💣','5-7':3,
  '6-0':1,'6-1':2,'6-2':1,'6-3':2,'6-4':'💣','6-5':3,'6-6':'💣','6-7':'💣',
  '7-0':'','7-1':1,'7-2':'💣','7-3':2,'7-4':1,'7-5':2,'7-6':2,'7-7':2
}
describe('Todo5: Modal', () => {
    it('Correct HTML relationship', () => {
      //container exists
      cy.visit('/')
      cy.get('button').contains('Start Game').click()
      cy.get('[id="6-6"]').click()
      cy.wait(2000)
      cy.get('.boardWrapper > .modal:nth-child(1)').should('have.css','opacity','1')
      cy.get('.modal > .modalWrapper:nth-child(1)')
      cy.get('.modal > .modalWrapper:nth-child(3)')
      cy.get('.modal > .modalContent:nth-child(2)')
      cy.get('.modalContent:nth-child(2) > div:nth-child(1)').should('have.class','modalResult')
      cy.get('.modalContent:nth-child(2) > div:nth-child(2)').should('have.class','modalBtnWrapper')
      cy.get('.modalBtnWrapper:nth-child(2) > div:nth-child(1)').should('have.class','modalBtn')
      cy.get('.modalBtnWrapper:nth-child(2) > div:nth-child(2)').should('have.class','modalBtn')
    })
    it('Show the difference between Win and Lost (3%)', () => {
      //container exists
      cy.get('.modalContent:nth-child(2) > div:nth-child(1)').should('have.class','modalResult').contains('Game Over')
      cy.get('.modalBtnWrapper:nth-child(2) > div:nth-child(1)').should('have.class','modalBtn').contains('Try Again')
      cy.get('.modalBtnWrapper:nth-child(2) > div:nth-child(2)').should('have.class','modalBtn').contains('Back to Home')

      cy.reload()
      cy.visit('/')
      cy.get('button').contains('Start Game').click()

      cy.get('.boardContainer > div').each(($el, index) => {
        if (index > 0){
          cy.wrap($el).children().each( ($cell, colIdx)=>{
              const stringId = (index-1).toString() + '-' + colIdx.toString()
              const value = board[stringId]
              if (index >= 1){
                if( value !== '💣'){
                  cy.wrap($cell).click()
                  if(value == '')
                    cy.wrap($cell).should('have.value', value);
                  else
                    cy.wrap($cell).contains(value);
                }
              }
            }
          )
        }
      })
      cy.wait(2000)
      cy.get('.modalContent:nth-child(2) > div:nth-child(1)').should('have.class','modalResult').contains('WIN')
      cy.get('.modalBtnWrapper:nth-child(2) > div:nth-child(1)').should('have.class','modalBtn').contains('New Game')
      cy.get('.modalBtnWrapper:nth-child(2) > div:nth-child(2)').should('have.class','modalBtn').contains('Back to Home')
    })
    it('Restart Game and Back to Home', () => {
      cy.get('.modalBtnWrapper:nth-child(2) > div:nth-child(1)').should('have.class','modalBtn').contains('New Game').click()
      cy.get('.mineSweeper > div:nth-child(1)').should('have.class','boardPage')
      cy.get('.boardPage > div:nth-child(1)').should('have.class','boardWrapper')
      cy.get('.boardWrapper > div:nth-child(1)').should('have.class','boardContainer')
      cy.get('.boardContainer > div:nth-child(1)').should('have.class','dashBoard')
      cy.get('.boardContainer').children().should('have.length', 9); 
      cy.get('[id="2-1"]').click()
      cy.get('.modalBtnWrapper:nth-child(2) > div:nth-child(2)').should('have.class','modalBtn').contains('Back to Home').click()
      cy.get('.HomeWrapper')
    })
  
      
})
