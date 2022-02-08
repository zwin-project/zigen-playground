import React, { useState } from 'react'
import Container from '@mui/material/Container'
import CssBaseline from '@mui/material/CssBaseline'
import { styled } from '@mui/material/styles'
import Header from './component/Header'
import DataTypeSelector, { DataType } from './component/DataTypeSelector'
import Primitives from './component/Primitives'
import Textures from './component/Textures'
import Fade from '@mui/material/Fade'
import Box from '@mui/material/Box'

const StyledContainer = styled(Container)(({ theme }) => ({
  marginTop: theme.spacing(4),
}))

const App: React.FC = () => {
  const [dataType, setDataType] = useState<DataType>(DataType.primitives)

  return (
    <React.Fragment>
      <CssBaseline />
      <Header />
      <StyledContainer maxWidth="md">
        <DataTypeSelector
          marginBottom={8}
          onChangeDataType={setDataType}
          value={dataType}
        />
      </StyledContainer>
      <Box sx={{ position: 'absolute', width: '100%' }}>
        <StyledContainer maxWidth="md">
          <Fade in={dataType === DataType.primitives}>
            <Primitives />
          </Fade>
        </StyledContainer>
      </Box>
      <Box sx={{ position: 'absolute', width: '100%' }}>
        <Fade in={dataType === DataType.textures}>
          <Textures />
        </Fade>
      </Box>
    </React.Fragment>
  )
}

export default App
