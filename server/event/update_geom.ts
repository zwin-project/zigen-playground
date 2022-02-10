import Client from '../model/client'
import Resource from '../model/resource'

class UpdateGeomEvent {
  constructor(private resource: Resource) {}

  send = (client: Client) => {
    const data = {
      action: 'update-geom',
      data: this.resource.serialize(),
    }
    const json = JSON.stringify(data)
    client.send(json)
  }
}

export default UpdateGeomEvent
