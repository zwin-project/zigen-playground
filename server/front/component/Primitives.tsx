import Grid from '@mui/material/Grid'
import Box, { BoxProps } from '@mui/material/Box'
import React from 'react'
import DraggablePrimitiveCard, { PrimitiveType } from './DraggablePrimitiveCard'

// eslint-disable-next-line react/display-name
const Primitives = React.forwardRef((props: BoxProps, ref) => (
  <Box ref={ref} {...props}>
    <Grid
      container
      spacing={2}
      justifyContent="center"
      sx={{ position: 'relative' }}
    >
      <Grid item xs={4}>
        <DraggablePrimitiveCard type={PrimitiveType.cuboid} />
      </Grid>
      <Grid item xs={4}>
        <DraggablePrimitiveCard type={PrimitiveType.sphere} />
      </Grid>
    </Grid>
  </Box>
))

export default Primitives
