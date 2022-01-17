import moment from "moment";
// mui
import Typography from "@mui/material/Typography";
import IconButton from "@mui/material/IconButton";
import Grid from "@mui/material/Grid";
import Box from "@mui/material/Box";

import AccessTimeIcon from "@mui/icons-material/AccessTime";
import DeleteOutlineIcon from "@mui/icons-material/DeleteOutline";
// graphql
import { useMutation } from "@apollo/client";
import { GET_TASKS_QUERY, DELETE_TASK_MUTATION } from "../../graphql";

export default function Task({ id, title, content, dueDate }) {
  const [deleteTask] = useMutation(DELETE_TASK_MUTATION);

  const handleDelete = () => {
    deleteTask({
      variables: {
        id,
      },
      refetchQueries: [GET_TASKS_QUERY],
      onError: (err) => {
        console.log(err);
      },
    });
  };

  return (
    <Box
      sx={{
        display: "flex",
        width: "100%",
      }}
    >
      <Box
        sx={{
          flexGrow: 1,
        }}
      >
        <Typography variant="body1">
          <strong>{title}</strong>
        </Typography>
        <Typography variant="body1">{content}</Typography>

        <Typography
          variant="body2"
          color="textSecondary"
          sx={{
            display: "flex",
            alignItems: "center",
          }}
        >
          <AccessTimeIcon fontSize="small" />
          {moment(dueDate).fromNow()}
        </Typography>
      </Box>
      <Box>
        <IconButton onClick={handleDelete}>
          <DeleteOutlineIcon />
        </IconButton>
      </Box>
    </Box>
  );
}
