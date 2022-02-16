import { Vector3 } from '../math/vector'
import Client from '../model/client'

class UpdateRayEvent {
  constructor(
    private type: 'move' | 'destroy',
    private origin: Vector3,
    private target: Vector3
  ) {}

  send = (client: Client) => {
    const data = {
      action: 'update-ray',
      data: {
        clientId: client.getId(),
        type: this.type,
        origin: this.origin.toArray(),
        target: this.target.toArray(),
      },
    }
    const json = JSON.stringify(data)
    client.send(json)
  }
}

export default UpdateRayEvent
