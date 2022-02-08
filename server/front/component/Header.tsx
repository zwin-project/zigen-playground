import AppBar from '@mui/material/AppBar'
import React from 'react'
import Typography from '@mui/material/Typography'
import { styled } from '@mui/material/styles'
import Toolbar from '@mui/material/Toolbar'

const Icon = styled('img')({
  display: 'inline-block',
  height: 80,
  marginRight: 20,
})

const Header: React.FC = () => (
  <AppBar position="static" color="default">
    <Toolbar>
      <Icon src="/logo.svg" />
      <Typography variant="h4" fontFamily="inherit">
        zigen playground
      </Typography>
    </Toolbar>
  </AppBar>
)

export default Header
