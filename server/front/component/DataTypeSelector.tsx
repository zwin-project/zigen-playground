import Box, { BoxProps } from '@mui/material/Box'
import MenuItem from '@mui/material/MenuItem'
import Select from '@mui/material/Select'
import React from 'react'
import { styled } from '@mui/material/styles'
import { SelectInputProps } from '@mui/material/Select/SelectInput'

const StyledSelect = styled(Select)(({ theme }) => ({
  fontSize: '2rem',
  paddingLeft: 30,
  paddingRight: 30,
}))

export enum DataType {
  primitives,
  textures,
}

type Props = {
  onChangeDataType?: (dataType: DataType) => void
  value: DataType
} & BoxProps

const DataTypeSelector: React.FC<Props> = ({
  onChangeDataType,
  value,
  ...boxProps
}) => {
  const onChange: SelectInputProps['onChange'] = (ev) =>
    onChangeDataType && onChangeDataType(ev.target.value as DataType)

  return (
    <Box {...boxProps}>
      <StyledSelect value={value} variant="standard" onChange={onChange}>
        <MenuItem value={DataType.primitives}>Primitives</MenuItem>
        <MenuItem value={DataType.textures}>Textures</MenuItem>
      </StyledSelect>
    </Box>
  )
}

export default DataTypeSelector
