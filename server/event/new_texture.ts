import Client from '../model/client'

class NewTextureEvent {
  constructor(private resourceId: number, private url: string) {}

  send = (client: Client) => {
    const data = {
      action: 'new-texture',
      data: {
        id: this.resourceId,
        url: this.url,
      },
    }
    const json = JSON.stringify(data)
    client.send(json)
  }
}

export default NewTextureEvent
