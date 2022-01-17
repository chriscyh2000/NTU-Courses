import { gql } from "@apollo/client";

export const UPDATE_TASK_MUTATION = gql`
  mutation UpdateTask($id: ID!, $status: Status!) {
    updateTask(id: $id, status: $status) {
      id
      title
      content
      dueDate
      status
    }
  }
`;

// TODO 4.1 Create Task Mutation.
// Uncomment the following lines and fill the gql part
export const CREATE_TASK_MUTATION = gql`
    mutation createTask(
        $input: CreateTaskInput!
    ){
        createTask(input: $input){
          id
          title
          content
          dueDate
          status
        }
    }
`;

export const DELETE_TASK_MUTATION = gql`
  mutation DeleteTask($id: ID!) {
    deleteTask(id: $id)
  }
`;
