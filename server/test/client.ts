import Client from "../model/client";
import { EventEmitter } from "events";

class TestClient implements Client {
  private emitter = new EventEmitter();

  alive: boolean = true;
  closedCode?: number;
  closedData?: string;
  sendMessage: string = "";
  sendCount = 0;

  onMessage = (func: (client: Client, message: string) => void) => {
    this.emitter.on("message", func);
  };

  emitMessage(message: string) {
    this.emitter.emit("message", this, message);
  }

  close = (code?: number, data?: string) => {
    this.closedCode = code;
    this.closedData = data;
  };

  send = (msg: string) => {
    this.sendMessage = msg;
    this.sendCount += 1;
  };
}

export default TestClient;
