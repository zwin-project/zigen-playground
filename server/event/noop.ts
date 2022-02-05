import Client from '../model/client'

class NoopEvent {
  send = (client: Client) => {
    const data = {
      action: 'noop',
    }

    const json = JSON.stringify(data)
    client.send(json)
  }
}

export default NoopEvent
