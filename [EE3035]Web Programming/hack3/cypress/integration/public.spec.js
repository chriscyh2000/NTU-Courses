import { 
  WAIT_INTERVAL,
  FRONTEND_URL,
  BACKEND_URL,
  GET_TASKS_QUERY,
  DELETE_TASK_MUTATION,
  fetch,
  addDnDCommand,
} from "./utils.js";

addDnDCommand();

const publicExamples = [
  {
    id: "1",
    title: "This is task1",
    content: "This is the task's content",
    status: "TODO",
    dueDate: new Date("2021-12-02T05:51:04.360Z").getTime(),
  },
  {
    id: "4",
    title: "This is task4",
    content: "This is the task's content",
    status: "TODO",
    dueDate: new Date("2021-12-02T05:51:04.360Z").getTime(),
  },
  {
    id: "2",
    title: "This is task2",
    content: "This is the task's content",
    status: "INPROGRESS",
    dueDate: new Date("2021-12-01T03:32:10.360Z").getTime(),
  },
  {
    id: "5",
    title: "This is task5",
    content: "This is the task's content",
    status: "INPROGRESS",
    dueDate: new Date("2021-12-01T03:32:10.360Z").getTime(),
  },
  {
    id: "3",
    title: "This is task3",
    content: "This is the task's content",
    status: "DONE",
    dueDate: new Date("2021-12-01T03:32:10.360Z").getTime(),
  },
  {
    id: "6",
    title: "This is task6",
    content: "This is the task's content",
    status: "DONE",
    dueDate: new Date("2021-12-01T03:32:10.360Z").getTime(),
  },
];

describe("Public Tests", () => {
  beforeEach(() => {
    cy.task("initDB", publicExamples);
  })

  it("1.1", () => {
    fetch(BACKEND_URL, GET_TASKS_QUERY)
    .then((res) => {
      expect(res?.body?.data?.tasks).to.have.lengthOf(publicExamples.length);
    })
  })

  it("1.2", () => {
    cy.visit(FRONTEND_URL);
    cy.get('body')
    .find("[data-rbd-draggable-id] > .MuiListItemButton-root")
    .should('have.length', publicExamples.length);
  })

  it("2.", () => {
    cy.visit(FRONTEND_URL);
    
    fetch(BACKEND_URL, GET_TASKS_QUERY)
    .then((res) => {
      const tasks = res.body.data.tasks;
      for (const task of tasks) {
        cy.contains(task.title);
        cy.contains(task.content);
      }          
    })
  })

  it("3.", () => {
    cy.visit(FRONTEND_URL);

    // drap one from TODO to INPROGRESS
    cy.dragAndDrop('[data-rbd-draggable-id="1"]', ':nth-child(2) > .css-jsccjo');
    cy.wait(WAIT_INTERVAL);

    fetch(BACKEND_URL, GET_TASKS_QUERY)
    .then((res) => {
      const tasks = res.body.data.tasks;
      const todo = tasks.filter(task => task.status === "TODO");
      const inprogress = tasks.filter(task => task.status === "INPROGRESS");
      const done = tasks.filter(task => task.status === "DONE");

      expect([todo.length, inprogress.length, done.length]).deep.eq([1, 3, 2]);
    })
  })

  it("4.", () => {
    cy.visit(FRONTEND_URL);
    cy.get(".MuiButton-root").click();
    
    cy.get('.MuiDialogContent-root > :nth-child(1)').type("New Task");
    cy.get('.MuiDialogContent-root > :nth-child(2)').type("Content of the new task");
    cy.get('.MuiDialogContent-root > :nth-child(3)').type("12/01/2021 06:41 AM");

    cy.get('.MuiDialogActions-root > :nth-child(2)').click({ multiple: true, force: true })

    cy.wait(WAIT_INTERVAL);
    fetch(BACKEND_URL, GET_TASKS_QUERY)
    .then((res) => {
      const tasks = res.body.data.tasks;
      const todo = tasks.filter(task => task.status === "TODO");
      const inprogress = tasks.filter(task => task.status === "INPROGRESS");
      const done = tasks.filter(task => task.status === "DONE");

      expect([todo.length, inprogress.length, done.length]).deep.eq([3, 2, 2]);
      expect(tasks.filter(task => task.status === "TODO" && task.title === "New Task"))
      .to.have.lengthOf(1);
    });
  })

  it("5.", () => {
    fetch(BACKEND_URL, DELETE_TASK_MUTATION, { id: "1" })
    .then(res => {
      expect(res.body.data).deep.eq({ deleteTask: "1" });
    })
    .then(() => cy.wait(WAIT_INTERVAL))
    .then(() => fetch(BACKEND_URL, GET_TASKS_QUERY))
    .then(res => {
      const tasks = res.body.data.tasks;
      const todo = tasks.filter(task => task.status === "TODO");
      const inprogress = tasks.filter(task => task.status === "INPROGRESS");
      const done = tasks.filter(task => task.status === "DONE");

      expect([todo.length, inprogress.length, done.length]).deep.eq([1, 2, 2]);
      expect(tasks.filter(task => task.status === "TODO" && task.title === "This is task1"))
      .to.be.empty;
    })
  })

  it("6.", () => {
    cy.visit(FRONTEND_URL);
    cy.wait(WAIT_INTERVAL);

    fetch(BACKEND_URL, DELETE_TASK_MUTATION, { id: "1" })
    .then(() => {
      cy.wait(WAIT_INTERVAL);
      cy.get(':nth-child(1) > .css-jsccjo')
      .find('[data-rbd-draggable-id]')
      .should('have.length', 1);
    })
  })
})