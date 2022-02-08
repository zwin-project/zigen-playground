import { Vector3 } from '../math/vector'

class NewResourceRequest {
  constructor(seed: any) {
    this.type = (seed.data && seed.data.type) || 'cuboid'
    const p = seed.data && seed.data.position
    if (Array.isArray(p) && p.length === 3)
      this.position = new Vector3(p[0], p[1], p[2])
    else this.position = new Vector3(0, 0, 0)
  }

  action = 'new-resource' as const

  type: 'cuboid' | 'sphere'
  position: Vector3
}

export default NewResourceRequest
