import Client from "./model/client";
import Message from "./message/message";
import { sync_controller } from "./controller/sync";

class Router {
  add = (client: Client) => {
    client.onMessage(this.onMessage);
  };

  onMessage = (client: Client, data: string) => {
    const message = Message.parse(data);

    if (message.type === "sync") {
      sync_controller(client, message);
    } else if (message.type == "invalid") {
      client.close(4400, "Bad Request");
    } else {
      client.close(1011, "Internal Server Error");
    }
  };
}

export default Router;
