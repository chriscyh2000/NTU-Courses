import { useState } from "react";
import { v4 as uuidv4 } from "uuid";
// mui
import AdapterMoment from "@mui/lab/AdapterMoment";
import DateTimePicker from "@mui/lab/DateTimePicker";
import LocalizationProvider from "@mui/lab/LocalizationProvider";
import Button from "@mui/material/Button";
import TextField from "@mui/material/TextField";
import Dialog from "@mui/material/Dialog";
import DialogActions from "@mui/material/DialogActions";
import DialogContent from "@mui/material/DialogContent";
import DialogTitle from "@mui/material/DialogTitle";
// constants
import { TODO } from "../../constants";
// graphql
import { useMutation } from "@apollo/react-hooks";
// TODO 4.2 Uncomment the following line
import { GET_TASKS_QUERY, CREATE_TASK_MUTATION } from "../../graphql";

const TITLE = "title";
const CONTENT = "content";
const DUEDATE = "dueDate";

const initialFormData = {
  [TITLE]: "",
  [CONTENT]: "",
  [DUEDATE]: "",
};

export default function CreateTaskModal({ open, handleCloseCreateTaskModal }) {
  // form data control
  const [formData, setFormData] = useState(initialFormData);
  const [displayError, setDisplayError] = useState(false);

  const handleChangeFormData = (key, value) => {
    setDisplayError(false);
    setFormData({
      ...formData,
      [key]: value,
    });
  };

  // TODO 4.2 Uncomment the following lines
  const [createTask] = useMutation(CREATE_TASK_MUTATION);
  const handleCreate = () => {
    if (Object.values(formData).some((v) => !v)) {
      setDisplayError(true);
      return;
    }
    // TODO 4.2 Uncomment the following lines
    createTask({
      variables: {
        input: {
          id: uuidv4(),
          title: formData.title,
          content: formData.content,
          dueDate: parseInt(formData.dueDate.format("x")),
          status: TODO,
        },
      },
      refetchQueries: [GET_TASKS_QUERY], // refetch after createTask
      onCompleted: () => {
        handleClose();
      },
    });
  };

  const handleClose = () => {
    // reset data
    setFormData(initialFormData);
    handleCloseCreateTaskModal();
  };

  return (
    <Dialog open={open} onClose={handleClose} fullWidth>
      <DialogTitle>Create a new task</DialogTitle>
      <DialogContent>
        <TextField
          error={displayError && !formData[TITLE]}
          autoFocus
          margin="dense"
          label="Title"
          fullWidth
          variant="standard"
          value={formData[TITLE]}
          onChange={(e) => handleChangeFormData(TITLE, e.target.value)}
          helperText={displayError && "The field can't be empty!"}
        />
        <TextField
          error={displayError && !formData[CONTENT]}
          margin="dense"
          label="Content"
          fullWidth
          variant="standard"
          value={formData[CONTENT]}
          onChange={(e) => handleChangeFormData(CONTENT, e.target.value)}
          helperText={displayError && "The field can't be empty!"}
          multiline
        />
        <LocalizationProvider dateAdapter={AdapterMoment}>
          <DateTimePicker
            label="Due Date"
            value={formData[DUEDATE]}
            onChange={(v) => handleChangeFormData(DUEDATE, v)}
            renderInput={(params) => (
              <TextField
                {...params}
                variant="standard"
                fullWidth
                margin="dense"
                error={displayError && !formData[DUEDATE]}
                helperText={displayError && "The field can't be empty!"}
              />
            )}
          />
        </LocalizationProvider>
      </DialogContent>
      <DialogActions>
        <Button onClick={handleClose}>Cancel</Button>
        <Button onClick={handleCreate}>Create</Button>
      </DialogActions>
    </Dialog>
  );
}
