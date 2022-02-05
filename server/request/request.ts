import SyncRequest from './sync'

class InvalidRequest {
  action = 'invalid' as const
}

class WebSocketRequest {
  static parse = (msg: string): SyncRequest | InvalidRequest => {
    let json: { action?: string }
    try {
      json = JSON.parse(msg)
    } catch (error) {
      return new InvalidRequest()
    }

    if (json.action === 'sync') {
      return new SyncRequest()
    } else {
      return new InvalidRequest()
    }
  }
}

export default WebSocketRequest
