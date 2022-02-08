import CardMedia from '@mui/material/CardMedia'
import Typography from '@mui/material/Typography'
import Card from '@mui/material/Card'
import CardContent from '@mui/material/CardContent'
import React, { useState } from 'react'
import palette from '../palette'

type Props = {
  url: string
  title: string
}

const urlToDndRequestBody = (url: string) =>
  JSON.stringify({
    action: 'new-texture',
    data: {
      url,
    },
  })

const DraggableTextureCard: React.FC<Props> = ({ url, title }) => {
  const [borderColor, setBorderColor] = useState('default')
  const [dragging, setDragging] = useState(false)

  const onDragStart: React.DragEventHandler<HTMLDivElement> = (ev) => {
    setDragging(true)
    ev.dataTransfer.setData('text/plain', urlToDndRequestBody(url))
  }

  return (
    <Card
      draggable
      variant="outlined"
      onDragStart={onDragStart}
      onDragEnd={() => setDragging(false)}
      onMouseOver={() => setBorderColor(palette.first)}
      onMouseLeave={() => setBorderColor('default')}
      sx={{
        cursor: 'grab',
        borderColor: borderColor,
        backgroundColor: dragging ? 'black' : 'default',
        color: dragging ? 'white' : 'default',
      }}
    >
      <CardContent>
        <Typography variant="h5">{title}</Typography>
      </CardContent>
      <CardMedia component="img" image={url} draggable="false"></CardMedia>
    </Card>
  )
}

export default DraggableTextureCard
