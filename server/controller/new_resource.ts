import NewResourceEvent from '../event/new_resource'
import { Vector3 } from '../math/vector'
import Client from '../model/client'
import Playground from '../model/playground'
import Resource from '../model/resource'
import Cuboid from '../model/resource/cuboid'
import Sphere from '../model/resource/sphere'
import NewResourceRequest from '../request/new_resource'

export const newResourceController = (
  client: Client,
  request: NewResourceRequest
) => {
  const playground = Playground.get()

  let resource: Resource

  if (request.type === 'cuboid') {
    resource = new Cuboid(request.position, new Vector3(0.01, 0.01, 0.01))
  } else if (request.type === 'sphere') {
    resource = new Sphere(request.position, 0.05, 10)
  } else {
    return
  }

  playground.resources.push(resource)

  playground.forEachClients((client) => {
    const newResourceEvent = new NewResourceEvent(resource)
    newResourceEvent.send(client)
  })
}
