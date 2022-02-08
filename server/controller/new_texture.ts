import Client from '../model/client'
import Playground from '../model/playground'
import Sphere from '../model/resource/sphere'
import NewTextureRequest from '../request/new_texture'
import NewTextureEvent from '../event/new_texture'

export const newTextureController = (
  client: Client,
  request: NewTextureRequest
) => {
  const playground = Playground.get()
  playground.resources
    .filter((resource) => resource.id() === request.id)
    .forEach((resource) => {
      if (resource instanceof Sphere) {
        resource.setTexture(request.url)
      }
    })

  playground.forEachClients((client) => {
    const newTextureEvent = new NewTextureEvent(request.id, request.url)
    newTextureEvent.send(client)
  })
}
