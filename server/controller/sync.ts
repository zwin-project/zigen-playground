import Playground from "../model/playground";
import Client from "../model/client";
import SyncMessage from "../message/sync";
import AllObjectsEvent from "../event/all-resources";

export const sync_controller = (client: Client, message: SyncMessage) => {
  const playground = Playground.get();
  const event = new AllObjectsEvent(playground);
  event.send(client);
};
