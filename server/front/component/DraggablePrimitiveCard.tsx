import React, { useState } from 'react'
import Card from '@mui/material/Card'
import TypoGraphy from '@mui/material/Typography'
import CardContent from '@mui/material/CardContent'
import CardMedia from '@mui/material/CardMedia'
import palette from '../palette'

export enum PrimitiveType {
  cuboid = 'cuboid',
  sphere = 'sphere',
}

type Props = { type: PrimitiveType }

const primitiveTypeToDndRequestBody = (type: PrimitiveType) =>
  JSON.stringify({
    action: 'new-resource',
    data: { type: type },
  })

const DraggablePrimitiveCard: React.FC<Props> = ({ type }) => {
  const [borderColor, setBorderColor] = useState('default')
  const [dragging, setDragging] = useState(false)
  const onDragStart: React.DragEventHandler<HTMLDivElement> = (ev) => {
    setDragging(true)
    ev.dataTransfer.setData('text/plain', primitiveTypeToDndRequestBody(type))
  }

  let img = ''
  let title = ''

  switch (type) {
    case PrimitiveType.cuboid:
      img = dragging ? '/cuboid-gb.png' : '/cuboid-black.png'
      title = 'Cuboid'
      break

    case PrimitiveType.sphere:
      img = dragging ? '/sphere-gb.png' : '/sphere-black.png'
      title = 'Sphere'
      break
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
        <TypoGraphy variant="h5">{title}</TypoGraphy>
      </CardContent>
      <CardMedia
        component="img"
        height="140"
        image={img}
        draggable="false"
        sx={{
          objectFit: 'contain',
          paddingY: '10px',
          userSelect: 'none',
        }}
      />
    </Card>
  )
}

export default DraggablePrimitiveCard
