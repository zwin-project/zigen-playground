import http from 'http'
import User from './user'
import { WebSocket } from 'ws'

interface Client {
  onRequest: (func: (client: Client, message: string) => void) => void

  close: (code?: number, data?: string) => void

  send: (msg: string) => void
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

  private constructor(public user: User, private ws: WebSocket) {
    ws.on('close', this.onClose)
  }

  // eslint-disable-next-line @typescript-eslint/no-unused-vars
  private onClose = (code: number, reason: Buffer) => {
    // TODO:
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
}

export default Client
