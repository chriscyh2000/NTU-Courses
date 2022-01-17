// mui
import Typography from "@mui/material/Typography";
import Box from "@mui/material/Box";
import Stack from "@mui/material/Stack";

export default function TaskListTitle({ title, count }) {
  return (
    <>
      <Box paddingX={2} paddingY={1} borderBottom="1px solid #ddd">
        <Stack direction="row" alignItems="center" spacing={1}>
          <Typography variant="h6">{title}</Typography>
          <Typography variant="body1" color="textSecondary">
            ({count})
          </Typography>
        </Stack>
      </Box>
    </>
  );
}
