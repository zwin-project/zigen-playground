import Grid from '@mui/material/Grid'
import Box, { BoxProps } from '@mui/material/Box'
import React from 'react'
import Typography from '@mui/material/Typography'

// eslint-disable-next-line react/display-name
const Textures = React.forwardRef((props: BoxProps, ref) => (
  <Box ref={ref} {...props}>
    <Grid container spacing={2} justifyContent="center">
      <Typography>Textures</Typography>
    </Grid>
  </Box>
))

export default Textures
