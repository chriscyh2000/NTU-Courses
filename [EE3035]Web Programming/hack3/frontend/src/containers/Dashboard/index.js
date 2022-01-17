import { useState, useEffect } from "react";
import { DragDropContext } from "react-beautiful-dnd";
// mui
import Container from "@mui/material/Container";
import Stack from "@mui/material/Stack";
// components
import TaskList from "../../components/TaskList";
// graphql
import {
  GET_TASKS_QUERY,
  UPDATE_TASK_MUTATION,
  TASK_CREATED_SUBSCRIPTION,
  TASK_UPDATED_SUBSCRIPTION,
  // TODO 6.5 Uncomment the following line
  TASK_DELETED_SUBSCRIPTION,
} from "../../graphql";
import { useQuery, useMutation } from "@apollo/client";
// constants
import { TODO, INPROGRESS, DONE } from "../../constants";

const initialTasks = {
  [TODO]: [],
  [INPROGRESS]: [],
  [DONE]: [],
};

export default function Dashboard() {
  // all the tasks
  const [tasks, setTasks] = useState(initialTasks);

  // fetch all the tasks with graphql
  const { data, subscribeToMore } = useQuery(GET_TASKS_QUERY);
  console.log(data);
  useEffect(() => {
    if (!data) return;
    // split the tasks into three different array according to their status
    console.log("data: ");
    console.log(data);
    setTasks({
      [TODO]: data.tasks.filter(({ status }) => status === TODO),
      [INPROGRESS]: data.tasks.filter(({ status }) => status === INPROGRESS),
      [DONE]: data.tasks.filter(({ status }) => status === DONE),
    });
  }, [data]);

  // subscription to task created
  useEffect(() => {
    subscribeToMore({
      document: TASK_CREATED_SUBSCRIPTION,
      updateQuery: (prev, { subscriptionData }) => {
        if (!subscriptionData.data) return prev;
        return {
          tasks: [...prev.tasks, subscriptionData.data.taskCreated],
        };
      },
    });
  }, [subscribeToMore]);

  // subscription to task updated
  useEffect(() => {
    subscribeToMore({
      document: TASK_UPDATED_SUBSCRIPTION,
      updateQuery: (prev, { subscriptionData }) => {
        if (!subscriptionData.data) return prev;
        const {
          data: { taskUpdated },
        } = subscriptionData;
        return {
          tasks: prev.tasks.map((task) => {
            if (task.id !== taskUpdated.id) return task;
            return {
              ...task,
              status: taskUpdated.status,
            };
          }),
        };
      },
    });
  }, [subscribeToMore]);

  // subscription to task deleted
  useEffect(() => {
    // TODO 6.5 subscription logic
    subscribeToMore({
      document: TASK_DELETED_SUBSCRIPTION,
      updateQuery: (prev, { subscriptionData }) => {
        if (!subscriptionData.data) return prev;
        console.log(prev.tasks);
        console.log(subscriptionData);
        const newTask = prev.tasks.filter((task) => {
          return task.id !== subscriptionData.data.taskDeleted
        })
        console.log("prev: ");
        console.log(prev);
        console.log("data: ");
        console.log(data);
        console.log("subscriptionData: ");
        console.log(subscriptionData);
        return {
          tasks: newTask
        }
      }
    })
  }, [subscribeToMore]);

  // graphql mutation function
  const [updateTask] = useMutation(UPDATE_TASK_MUTATION);
  /**
   * This function will be called on any status change
   * @param {ID} id - the id of the task which should be changed
   * @param {Status} status - the new status for the task
   */
  const handleUpdateTask = (id, status) => {
    // Call graphql mutation function
    // TODO 3 Use `updateTask` and pass the correct variables
    updateTask({
      variables: {
        id: id,
        status: status
      }
    });
  };

  // drag and drop
  const onDragEnd = ({ source, destination }) => {
    // dropped outside the list
    if (!destination) {
      return;
    }
    const sourceId = source.droppableId;
    const destId = destination.droppableId;

    if (sourceId === destId) {
      const items = reorder(tasks[sourceId], source.index, destination.index);
      const newTasks = JSON.parse(JSON.stringify(tasks));
      newTasks[sourceId] = items;
      setTasks(newTasks);
    } else {
      const result = move(tasks[sourceId], tasks[destId], source, destination);
      const newTasks = JSON.parse(JSON.stringify(tasks));
      newTasks[sourceId] = result[sourceId];
      newTasks[destId] = result[destId];
      setTasks(newTasks);
      handleUpdateTask(result.removed.id, destId);
    }
  };

  return (
    <Container maxWidth="lg" sx={{ height: "80vh" }}>
      <DragDropContext onDragEnd={onDragEnd}>
        <Stack mt={4} direction="row" spacing={2} sx={{ height: "100%" }}>
          <TaskList tasks={tasks[TODO]} status={TODO} />
          <TaskList tasks={tasks[INPROGRESS]} status={INPROGRESS} />
          <TaskList tasks={tasks[DONE]} status={DONE} />
        </Stack>
      </DragDropContext>
    </Container>
  );
}

/**
 * Reorder the items in one list.
 */
const reorder = (list, startIndex, endIndex) => {
  const result = Array.from(list);
  const [removed] = result.splice(startIndex, 1);
  result.splice(endIndex, 0, removed);

  return result;
};

/**
 * Moves an item from one list to another list.
 */
const move = (source, destination, droppableSource, droppableDestination) => {
  const sourceClone = Array.from(source);
  const destClone = Array.from(destination);
  const [removed] = sourceClone.splice(droppableSource.index, 1);

  destClone.splice(droppableDestination.index, 0, removed);

  const result = {};
  result[droppableSource.droppableId] = sourceClone;
  result[droppableDestination.droppableId] = destClone;
  result.removed = removed;

  return result;
};

