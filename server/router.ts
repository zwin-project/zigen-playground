import Client from './model/client'
import WebSocketRequest from './request/request'
import { syncController } from './controller/sync'

class Router {
  add = (client: Client) => {
    client.onRequest(this.onRequest)
  }

  onRequest = (client: Client, data: string) => {
    const request = WebSocketRequest.parse(data)

    if (request.action === 'sync') {
      syncController(client, request)
    } else if (request.action === 'invalid') {
      client.close(4400, 'Bad Request')
    } else {
      client.close(1011, 'Internal Server Error')
    }
  }
}

export default Router
