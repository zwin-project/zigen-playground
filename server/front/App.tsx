import React, { useState } from 'react'
import CssBaseline from '@mui/material/CssBaseline'
import Header from './component/Header'
import Container from '@mui/material/Container'
import DataTypeSelector, { DataType } from './component/DataTypeSelector'
import Primitives from './component/Primitives'
import Textures from './component/Textures'
import Fade from '@mui/material/Fade'
import Box from '@mui/material/Box'

const App: React.FC = () => {
  const [dataType, setDataType] = useState<DataType>(DataType.textures)

  return (
    <React.Fragment>
      <CssBaseline />
      <Header />
      <Container maxWidth="lg" sx={{ marginTop: '40px' }}>
        <DataTypeSelector
          marginBottom={8}
          onChangeDataType={setDataType}
          value={dataType}
        />
      </Container>
      <Box sx={{ position: 'absolute', width: '100%' }}>
        <Container maxWidth="lg">
          <Fade in={dataType === DataType.primitives} unmountOnExit>
            <Primitives />
          </Fade>
        </Container>
      </Box>
      <Box sx={{ position: 'absolute', width: '100%' }}>
        <Container maxWidth="lg">
          <Fade in={dataType === DataType.textures} unmountOnExit>
            <Textures />
          </Fade>
        </Container>
      </Box>
    </React.Fragment>
  )
}

export default App
