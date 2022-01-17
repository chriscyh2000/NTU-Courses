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
describe('Todo3: Board', () => {
    // 1st test
    it('Implement the Board with Cell (html)', () => {
      //container exists
      cy.visit('/')
      cy.get('button').contains('Start Game').click()
      cy.get('.mineSweeper > div:nth-child(1)').should('have.class','boardPage')
      cy.get('.boardPage > div:nth-child(1)').should('have.class','boardWrapper')
      cy.get('.boardWrapper > div:nth-child(1)').should('have.class','boardContainer')
      cy.get('.boardContainer > div:nth-child(1)').should('have.class','dashBoard')
      cy.get('.boardContainer').children().should('have.length', 9);  
      
      cy.get('.boardContainer > div').each(($el, index) => {
          if (index===0){
            cy.wrap($el).children().should('have.length', 2);
          }
          else{
            cy.wrap($el).should('have.id',`row${index-1}`)
            cy.wrap($el).should('have.css', 'display', 'flex')
            cy.wrap($el).children().should('have.length', 8);
            cy.wrap($el).children().each( ($cell, colIdx)=>{
                cy.wrap($cell).should('have.id',`${index-1}-${colIdx}`)
                cy.wrap($cell).should('have.value','')
              }
            )
          }
        }
      )
    })
    it('Value Correct',() => {
      cy.get('.boardContainer > div').each(($el, index) => {
        if (index===0){
          
        }
        else{
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
      }
    )
    })

    it('Dash Board with Flags',() => {
      cy.get('.dashBoard > div:nth-child(1)').should('have.id','dashBoard_col1')
      cy.get('div:nth-child(1) > .dashBoard_col').contains(10)
    }) 

    it('Update flags',() => {
      cy.reload()
      cy.visit('/')
      cy.get('button').contains('Start Game').click()
      cy.get('div:nth-child(1) > .dashBoard_col').contains(10)
      cy.get('[id="0-0"]').click()
      cy.get('[id="6-6"]').rightclick()
      cy.get('div:nth-child(1) > .dashBoard_col').contains(9)
      cy.get('[id="0-7"]').rightclick()
      cy.get('div:nth-child(1) > .dashBoard_col').contains(8)
    }) 
})
