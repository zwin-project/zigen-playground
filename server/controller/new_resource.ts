import SyncEvent from '../event/sync'
import { Vector3 } from '../math/vector'
import Client from '../model/client'
import Playground from '../model/playground'
import Cuboid from '../model/resource/cuboid'
import Sphere from '../model/resource/sphere'
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
  } else if (request.type === 'sphere') {
    playground.resources.push(new Sphere(request.position, 0.02, 4))
  }

  playground.forEachClients((client) => {
    const syncEvent = new SyncEvent(playground)
    syncEvent.send(client)
  })
}
