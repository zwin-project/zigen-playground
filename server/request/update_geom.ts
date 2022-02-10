import { Vector3 } from '../math/vector'

class UpdateGeomRequest {
  constructor(seed: any) {
    this.id = (seed.data && seed.data.id) || -1
    this.r = (seed.data && seed.data.r) || 0.1
    this.type = (seed.data && seed.data.type) || 'sphere'
    const v: number[] = (seed.data && seed.data.position) || []
    if (v && v.length === 3) this.position = new Vector3(v[0], v[1], v[2])
    else this.position = new Vector3(0, 0, 0)
  }

  action = 'update-geom' as const

  id: number
  type: 'sphere' | 'cuboid'
  position: Vector3
  r: number
}

export default UpdateGeomRequest
