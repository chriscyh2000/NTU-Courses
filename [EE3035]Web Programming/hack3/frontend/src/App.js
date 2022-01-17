import { useState } from "react";
// mui
import CssBaseline from "@mui/material/CssBaseline";
// components
import Bar from "./components/Bar";
// containers
import Dashboard from "./containers/Dashboard";
import CreateTaskModal from "./containers/CreateTaskModal";

export default function App() {
  // CreateTaskModal control
  const [openCreateTaskModal, setOpenCreateTaskModal] = useState(false);
  const handleOpenCreateTaskModal = () => {
    setOpenCreateTaskModal(true);
  };
  const handleCloseCreateTaskModal = () => {
    setOpenCreateTaskModal(false);
  };

  return (
    <div>
      <CssBaseline />
      <div style={{ backgroundColor: "#f5f5f5", height: "100vh" }}>
        <Bar handleOpenCreateTaskModal={handleOpenCreateTaskModal} />
        <Dashboard />
        <CreateTaskModal
          open={openCreateTaskModal}
          handleCloseCreateTaskModal={handleCloseCreateTaskModal}
        />
      </div>
    </div>
  );
}
