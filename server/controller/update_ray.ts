import UpdateRayEvent from '../event/update_ray'
import Client from '../model/client'
import Playground from '../model/playground'
import UpdateRayRequest from '../request/update_ray'

export const updateRayController = (
  client: Client,
  request: UpdateRayRequest
) => {
  client.setRay(request.origin, request.target)
  const playground = Playground.get()

  playground.forEachClients((c) => {
    if (client === c) return
    const event = new UpdateRayEvent('move', request.origin, request.target)
    event.send(c)
  })
}
