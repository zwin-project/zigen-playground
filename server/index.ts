import express from "express";
import http from "http";
import log4js from "log4js";
import { URL } from "url";
import { WebSocketServer } from "ws";
import { Vector3 } from "./math/vector";
import { WebSocketClient } from "./model/client";
import Playground from "./model/playground";
import Cuboid from "./model/resource/cuboid";
import Router from "./router";

const app = express();
const server = http.createServer(app);
const wss = new WebSocketServer({ noServer: true });
const router = new Router();

const PORT = Number(process.env["APP_PORT"]) || 3000;
const LOG_CONFIG = process.env["LOG_CONF"] || "./config/log4js.json";

log4js.configure(LOG_CONFIG);
app.use(log4js.connectLogger(log4js.getLogger("express"), {}));

wss.on("connection", (ws, req) => {
  const client = WebSocketClient.create(ws, req);
  if (client == null) return ws.close(4401, "Unauthorized");
  router.add(client);
});

server.on("upgrade", (req, socket, head) => {
  let url: URL;
  if (!req.url) {
    socket.destroy();
    return;
  }

  try {
    url = new URL(req.url, "http://noop.com");
  } catch (error) {
    socket.destroy();
    return;
  }

  if (url.pathname === "/ws") {
    wss.handleUpgrade(req, socket, head, (ws) => {
      wss.emit("connection", ws, req);
    });
  } else {
    socket.destroy();
  }
});

app.get("/", (req, res) => {
  res.send("<h1>Hello World</h1>");
});

const playground = Playground.get();

playground.resources.push(
  new Cuboid(new Vector3(0, 0, 0), new Vector3(0.25, 0.25, 0.25))
);

server.listen(PORT, () => console.log(`[*] Server listening on port ${PORT}`));
