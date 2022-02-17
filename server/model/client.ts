import http from 'http'
import User from './user'
import { WebSocket } from 'ws'
import Playground from './playground'
import { Vector3 } from '../math/vector'
import UpdateRayEvent from '../event/update_ray'

interface Client {
  onRequest: (func: (client: Client, message: string) => void) => void

  close: (code?: number, data?: string) => void

  send: (msg: string) => void

  setRay: (origin: Vector3, target: Vector3) => void

  getRay: () => { origin: Vector3; target: Vector3 } | null

  getId: () => number
}

export class WebSocketClient implements Client {
  static create = (
    ws: WebSocket,
    req: http.IncomingMessage
  ): WebSocketClient | null => {
    const user = User.fromRequest(req)
    if (!user) return null
    return new WebSocketClient(user, ws)
  }

  static nextId = 1

  rayOrigin?: Vector3
  rayTarget?: Vector3
  id: number

  private constructor(public user: User, private ws: WebSocket) {
    ws.on('close', this.onClose)
    const playground = Playground.get()
    this.id = WebSocketClient.nextId
    WebSocketClient.nextId += 1
    playground.pushClient(this)
  }

  // eslint-disable-next-line @typescript-eslint/no-unused-vars
  private onClose = (code: number, reason: Buffer) => {
    const playground = Playground.get()
    playground.removeClient(this)
    playground.forEachClients((c) => {
      const event = new UpdateRayEvent(
        'destroy',
        new Vector3(0, 0, 0),
        new Vector3(0, 0, 0)
      )
      event.send(c)
    })
  }

  onRequest = (func: (client: Client, message: string) => void) => {
    this.ws.on('message', (message, isBinary) => {
      if (isBinary) return this.close(4400, 'Bad Request')
      func(this, message.toString())
    })
  }

  close = (code?: number, data?: string) => {
    this.ws.close(code, data)
  }

  send = (msg: string) => {
    this.ws.send(msg)
  }

  setRay = (origin: Vector3, target: Vector3) => {
    this.rayOrigin = origin
    this.rayTarget = target
  }

  getRay = () => {
    if (this.rayOrigin && this.rayTarget) {
      return { origin: this.rayOrigin, target: this.rayTarget }
    } else {
      return null
    }
  }

  getId = () => this.id
}

export default Client
