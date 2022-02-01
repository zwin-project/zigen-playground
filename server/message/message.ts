import SyncMessage from './sync'

class InvalidMessage {
  type = 'invalid' as const
}

class Message {
  static parse = (msg: string): SyncMessage | InvalidMessage => {
    let json: { type?: string }
    try {
      json = JSON.parse(msg)
    } catch (error) {
      return new InvalidMessage()
    }

    if (json.type === 'sync') {
      return new SyncMessage()
    } else {
      return new InvalidMessage()
    }
  }
}

export default Message
