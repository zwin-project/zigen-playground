import * as React from 'react'
import * as ReactDOM from 'react-dom'
import { createTheme, ThemeProvider } from '@mui/material'
import App from './App'

const theme = createTheme({})

ReactDOM.render(
  <ThemeProvider theme={theme}>
    <App />
  </ThemeProvider>,
  document.getElementById('App')
)
