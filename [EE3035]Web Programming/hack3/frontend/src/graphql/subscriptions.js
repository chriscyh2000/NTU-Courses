import { gql } from "@apollo/client";

export const TASK_CREATED_SUBSCRIPTION = gql`
  subscription OnTaskCreated {
    taskCreated {
      id
      title
      content
      dueDate
      status
    }
  }
`;

export const TASK_UPDATED_SUBSCRIPTION = gql`
  subscription OnTaskUpdated {
    taskUpdated {
      id
      status
    }
  }
`;

// TODO 6.4 Uncomment the following line and finish gql part
export const TASK_DELETED_SUBSCRIPTION = gql`
  subscription OnTaskDeleted {
    taskDeleted
  }
`;
