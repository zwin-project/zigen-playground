import Grid from '@mui/material/Grid'
import Box, { BoxProps } from '@mui/material/Box'
import React from 'react'
import DraggableTextureCard from './DraggableTextureCard'

// eslint-disable-next-line react/display-name
const Textures = React.forwardRef((props: BoxProps, ref) => (
  <Box ref={ref} {...props}>
    <Grid
      container
      spacing={2}
      justifyContent="center"
      sx={{ position: 'relative' }}
    >
      <Grid item xs={4}>
        <DraggableTextureCard url="/earth.jpg" title="Earth" />
      </Grid>
      <Grid item xs={4}>
        <DraggableTextureCard url="/night.jpg" title="Night" />
      </Grid>
      <Grid item xs={4}>
        <DraggableTextureCard url="/moon.jpg" title="Moon" />
      </Grid>
      <Grid item xs={4}>
        <DraggableTextureCard url="/sun.jpg" title="Sun" />
      </Grid>
      <Grid item xs={4}>
        <DraggableTextureCard url="/jupiter.jpg" title="Jupiter" />
      </Grid>
      <Grid item xs={4}>
        <DraggableTextureCard url="/mars.jpg" title="Mars" />
      </Grid>
      <Grid item xs={4}>
        <DraggableTextureCard url="/venus.jpg" title="Venus" />
      </Grid>
      <Grid item xs={4}>
        <DraggableTextureCard url="/neptune.jpg" title="Neptune" />
      </Grid>
    </Grid>
  </Box>
))

export default Textures
