import { Vector3 } from '../../math/vector'
import Resource from '../resource'

class Sphere implements Resource {
  constructor(
    private position: Vector3,
    private r: number,
    private resolution: number
  ) {}

  serialize = (): object => ({
    type: 'sphere',
    position: this.position.toArray(),
    r: this.r,
    resolution: this.resolution,
  })
}

export default Sphere
