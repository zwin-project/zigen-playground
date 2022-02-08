import { Vector3 } from '../../math/vector'
import Resource, { ResourceIdGenerator } from '../resource'

class Cuboid implements Resource {
  private _id: number

  constructor(private position: Vector3, private halfSize: Vector3) {
    this._id = ResourceIdGenerator.get()
  }

  serialize = () => ({
    type: 'cuboid',
    id: this._id,
    position: this.position.toArray(),
    half_size: this.halfSize.toArray(),
  })

  id = () => this._id
}

export default Cuboid
