// mui
import AppBar from "@mui/material/AppBar";
import Toolbar from "@mui/material/Toolbar";
import IconButton from "@mui/material/IconButton";
import Typography from "@mui/material/Typography";
import EventNoteIcon from "@mui/icons-material/EventNote";
import Button from "@mui/material/Button";

export default function Bar({ handleOpenCreateTaskModal }) {
  return (
    <>
      <AppBar
        position="static"
        sx={{
          backgroundColor: "#222",
        }}
      >
        <Toolbar>
          <IconButton
            size="large"
            edge="start"
            color="inherit"
            aria-label="menu"
            sx={{ mr: 2 }}
          >
            <EventNoteIcon />
          </IconButton>
          <Typography variant="h6" color="inherit" noWrap sx={{ flexGrow: 1 }}>
            Project Planner
          </Typography>
          <Button
            color="primary"
            variant="contained"
            onClick={handleOpenCreateTaskModal}
          >
            Create
          </Button>
        </Toolbar>
      </AppBar>
    </>
  );
}
