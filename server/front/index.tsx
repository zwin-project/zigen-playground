import * as React from 'react'
import * as ReactDOM from 'react-dom'
import { createTheme, ThemeProvider } from '@mui/material'
import App from './App'

const theme = createTheme({
  components: {
    MuiAppBar: {
      styleOverrides: {
        colorDefault: {
          backgroundColor: '#f2f4f7',
          fontFamily: "'M PLUS 1p', sans-serif;",
        },
      },
    },
  },
})

ReactDOM.render(
  <ThemeProvider theme={theme}>
    <App />
  </ThemeProvider>,
  document.getElementById('App')
)
