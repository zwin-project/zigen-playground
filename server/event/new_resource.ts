import Client from '../model/client'
import Resource from '../model/resource'

class NewResourceEvent {
  constructor(private resource: Resource) {}

  send = (client: Client) => {
    const data = {
      action: 'new-resource',
      data: this.resource.serialize(),
    }
    const json = JSON.stringify(data)
    client.send(json)
  }
}

export default NewResourceEvent
