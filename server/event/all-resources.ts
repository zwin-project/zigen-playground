import Client from "../model/client";
import Playground from "../model/playground";

class AllObjectsEvent {
  constructor(private playground: Playground) {}

  send = (client: Client) => {
    const data: any = {
      type: "all-resources",
      data: this.playground.resources.map((resource) => resource.serialize()),
    };
    const json = JSON.stringify(data);
    client.send(json);
  };
}

export default AllObjectsEvent;
