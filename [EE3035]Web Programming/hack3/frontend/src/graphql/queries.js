import { gql } from "@apollo/client";

export const GET_TASKS_QUERY = gql`
  query GetTasksQuery {
    tasks {
      # TODO 2 Please modify the query to get more properties
      dueDate
      id
      status
      title
      content
    }
  }
`;
