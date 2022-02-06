import SyncEvent from '../event/sync'
import { Vector3 } from '../math/vector'
import Client from '../model/client'
import Playground from '../model/playground'
import Cuboid from '../model/resource/cuboid'
import NewResourceRequest from '../request/new_resource'

export const newResourceController = (
  client: Client,
  request: NewResourceRequest
) => {
  const playground = Playground.get()

  if (request.type === 'cuboid') {
    playground.resources.push(
      new Cuboid(request.position, new Vector3(0.01, 0.01, 0.01))
    )
  }

  playground.forEachClients((client) => {
    const syncEvent = new SyncEvent(playground)
    syncEvent.send(client)
  })
}
