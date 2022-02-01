import Playground from '../model/playground'
import Client from '../model/client'
import SyncMessage from '../message/sync'
import AllObjectsEvent from '../event/all-resources'

// eslint-disable-next-line @typescript-eslint/no-unused-vars
export const syncController = (client: Client, message: SyncMessage) => {
  const playground = Playground.get()
  const event = new AllObjectsEvent(playground)
  event.send(client)
}
