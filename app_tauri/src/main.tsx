import React from "react";
import ReactDOM from "react-dom/client";
import App from "./App";

import "@mantine/core/styles.css";

import { createTheme, MantineProvider } from "@mantine/core";

const theme = createTheme({
  fontFamily: 'Montserrat, sans-serif',
  defaultRadius: 'md',
});

ReactDOM.createRoot(document.getElementById("root") as HTMLElement).render(
  <React.StrictMode>
    <MantineProvider defaultColorScheme="dark" theme={theme}>
      <App />
    </MantineProvider>
  </React.StrictMode>
);
