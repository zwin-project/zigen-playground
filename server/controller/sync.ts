import Playground from '../model/playground'
import Client from '../model/client'
import SyncRequest from '../request/sync'
import SyncEvent from '../event/sync'

// eslint-disable-next-line @typescript-eslint/no-unused-vars
export const syncController = (client: Client, request: SyncRequest) => {
  const playground = Playground.get()
  const event = new SyncEvent(playground)
  event.send(client)
}
