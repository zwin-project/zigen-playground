import Client from '../model/client'
import { EventEmitter } from 'events'
import { Vector3 } from '../math/vector'

class TestClient implements Client {
  static nextId = 1

  private emitter = new EventEmitter()

  alive = true
  closedCode?: number
  closedData?: string
  sendMessage = ''
  sendCount = 0
  rayOrigin?: Vector3
  rayTarget?: Vector3
  id: number

  constructor() {
    this.id = TestClient.nextId
    TestClient.nextId += 1
  }

  onRequest = (func: (client: Client, message: string) => void) => {
    this.emitter.on('message', func)
  }

  emitMessage(message: string) {
    this.emitter.emit('message', this, message)
  }

  close = (code?: number, data?: string) => {
    this.closedCode = code
    this.closedData = data
  }

  send = (msg: string) => {
    this.sendMessage = msg
    this.sendCount += 1
  }

  setRay = (origin: Vector3, target: Vector3): void => {
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

export default TestClient
