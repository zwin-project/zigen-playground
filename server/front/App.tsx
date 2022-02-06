import React from 'react'
import Paper from '@mui/material/Paper'
import Grid from '@mui/material/Grid'
import Container from '@mui/material/Container'
import CssBaseline from '@mui/material/CssBaseline'
import { styled } from '@mui/material/styles'

const StyledContainer = styled(Container)(({ theme }) => ({
  marginTop: theme.spacing(4),
}))

const StyledPaper = styled(Paper)(({ theme }) => ({
  ...theme.typography.h4,
  padding: theme.spacing(1),
  height: 200,
  lineHeight: '200px',
  textAlign: 'center',
  color: theme.palette.text.secondary,
}))

type DraggablePaperProps = {
  data: string
}

const DraggablePaper: React.FC<DraggablePaperProps> = ({ data, children }) => {
  const onDragStart: React.DragEventHandler<HTMLDivElement> = (ev) => {
    ev.dataTransfer.setData('text/plain', data)
  }
  return (
    <StyledPaper draggable onDragStart={onDragStart}>
      {children}
    </StyledPaper>
  )
}

const App: React.FC = () => {
  return (
    <React.Fragment>
      <CssBaseline />
      <StyledContainer maxWidth="md">
        <Grid container spacing={2}>
          <Grid item xs={4}>
            <DraggablePaper
              data={JSON.stringify({
                action: 'new-resource',
                data: { type: 'cuboid' },
              })}
            >
              Cuboid
            </DraggablePaper>
          </Grid>
          <Grid item xs={4}>
            <DraggablePaper
              data={JSON.stringify({
                action: 'new-resource',
                data: { type: 'sphere' },
              })}
            >
              Sphere
            </DraggablePaper>
          </Grid>
        </Grid>
      </StyledContainer>
    </React.Fragment>
  )
}

export default App
