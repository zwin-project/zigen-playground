import Client from './client'
import Resource from './resource'

class Playground {
  get resources() {
    return this._resources
  }

  pushClient = (client: Client) => {
    if (this._clients.includes(client)) return
    this._clients.push(client)
  }

  removeClient = (client: Client) => {
    this._clients.filter((c) => c !== client)
  }

  forEachClients = (f: (client: Client, index?: number) => void) => {
    this._clients.forEach(f)
  }

  private _resources: Resource[] = []
  private _clients: Client[] = []

  /* eslint-disable-next-line no-use-before-define */
  private static instance: Playground = new Playground()

  static get = (): Playground => Playground.instance
}

export default Playground
