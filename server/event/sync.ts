import Client from '../model/client'
import Playground from '../model/playground'

class SyncEvent {
  constructor(private playground: Playground) {}

  send = (client: Client) => {
    const data = {
      action: 'sync',
      data: this.playground.resources.map((resource) => resource.serialize()),
    }
    const json = JSON.stringify(data)
    client.send(json)
  }
}

export default SyncEvent
