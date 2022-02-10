import { Vector3 } from '../../math/vector'
import Resource, { ResourceIdGenerator } from '../resource'

class Sphere implements Resource {
  private _id: number

  constructor(
    private position: Vector3,
    private r: number,
    private resolution: number,
    private texture?: string
  ) {
    this._id = ResourceIdGenerator.get()
  }

  serialize = () => ({
    type: 'sphere',
    id: this._id,
    position: this.position.toArray(),
    r: this.r,
    resolution: this.resolution,
    texture: this.texture || '',
  })

  id = () => this._id

  setTexture = (url: string) => (this.texture = url)

  setGeom = (position: Vector3, r: number) => {
    this.position = position
    this.r = r
  }
}

export default Sphere
