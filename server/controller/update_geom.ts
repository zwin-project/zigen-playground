import UpdateGeomEvent from '../event/update_geom'
import Client from '../model/client'
import Playground from '../model/playground'
import Sphere from '../model/resource/sphere'
import UpdateGeomRequest from '../request/update_geom'

export const updateGeomController = (
  client: Client,
  request: UpdateGeomRequest
) => {
  const playground = Playground.get()
  playground.resources
    .filter((resource) => resource.id() === request.id)
    .forEach((resource) => {
      if (request.type === 'sphere' && resource instanceof Sphere) {
        resource.setGeom(request.position, request.r)

        playground.forEachClients((c) => {
          if (c === client) return
          const updateGeomEvent = new UpdateGeomEvent(resource)
          updateGeomEvent.send(c)
        })
      }
    })
}
