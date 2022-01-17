export const WAIT_INTERVAL = 500;
export const FRONTEND_URL = "localhost:3000/";
export const BACKEND_URL = "localhost:5000/";

export const GET_TASKS_QUERY = `
query GetTasksQuery {
  tasks {
    title
    content
    dueDate
    id
    status
  }
}
`;

export const DELETE_TASK_MUTATION = `
mutation DeleteTask($id: ID!) {
  deleteTask(id: $id)
}
`

export function fetch(url, query, variables) {
  return cy.request({
    method: "POST",
    url,
    headers: {
      'content-type': 'application/json',
      'accept': 'application/json',
    },
    body: JSON.stringify({
      query,
      variables,
    }),
    failOnStatusCode: false,
    timeout: 1000
  });
}

export function addDnDCommand() {
  Cypress.Commands.add('dragAndDrop', (subject, target) => {
    Cypress.log({
      name: 'DRAGNDROP',
      message: `Dragging element ${subject} to ${target}`,
      consoleProps: () => {
        return {
          subject: subject,
          target: target
        };
      }
    });
    const BUTTON_INDEX = 0;
    const SLOPPY_CLICK_THRESHOLD = 10;
    cy.get(target)
    .first()
    .then($target => {
      let coordsDrop = $target[0].getBoundingClientRect();
      cy.get(subject)
      .first()
      .then(subject => {
        const coordsDrag = subject[0].getBoundingClientRect();
        cy.wrap(subject)
        .trigger('mousedown', {
          button: BUTTON_INDEX,
          clientX: coordsDrag.x,
          clientY: coordsDrag.y,
          force: true
        })
        .trigger('mousemove', {
          button: BUTTON_INDEX,
          clientX: coordsDrag.x + SLOPPY_CLICK_THRESHOLD,
          clientY: coordsDrag.y,
          force: true
        });
        cy.get('body')
        .trigger('mousemove', {
          button: BUTTON_INDEX,
          clientX: coordsDrop.x,
          clientY: coordsDrop.y,
          force: true            
        })
        .trigger('mouseup');
      });
    });
  });  
}