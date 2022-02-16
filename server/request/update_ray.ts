import { Vector3 } from '../math/vector'

class UpdateRayRequest {
  constructor(seed: any) {
    let v: number[] = (seed.data && seed.data.origin) || []
    if (v && v.length === 3) this.origin = new Vector3(v[0], v[1], v[2])
    else this.origin = new Vector3(0, 0, 0)

    v = (seed.data && seed.data.target) || []
    if (v && v.length === 3) this.target = new Vector3(v[0], v[1], v[2])
    else this.target = new Vector3(0, 0, 0)
  }

  action = 'update-ray' as const

  origin: Vector3
  target: Vector3
}

export default UpdateRayRequest
