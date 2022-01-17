// mui
import Paper from "@mui/material/Paper";
import Box from "@mui/material/Box";
import List from "@mui/material/List";
import ListItem from "@mui/material/ListItem";
import ListItemButton from "@mui/material/ListItemButton";
// components
import Task from "./Task";
import TaskListTitle from "./TaskListTitle";
import { Draggable, Droppable } from "react-beautiful-dnd";

export default function TaskList({ tasks = [], status }) {
  return (
    <Paper variant="outlined" sx={{ width: "100%", height: "100%" }}>
      <TaskListTitle title={status} count={tasks.length} />
      <Droppable droppableId={status}>
        {(provided, snapshot) => (
          <Box
            ref={provided.innerRef}
            sx={{
              height: "calc(100% - 52px)",
              overflow: "scroll",
            }}
          >
            <List disablePadding>
              {tasks.map((task, index) => (
                <Draggable key={task.id} draggableId={task.id} index={index}>
                  {(provided, snapshot) => (
                    <ListItem
                      ref={provided.innerRef}
                      {...provided.draggableProps}
                      {...provided.dragHandleProps}
                    >
                      <ListItemButton
                        sx={{
                          backgroundColor: "#f5f5f5",
                          borderRadius: "4px",
                        }}
                      >
                        <Task {...task} />
                      </ListItemButton>
                    </ListItem>
                  )}
                </Draggable>
              ))}
              {provided.placeholder}
            </List>
          </Box>
        )}
      </Droppable>
    </Paper>
  );
}
