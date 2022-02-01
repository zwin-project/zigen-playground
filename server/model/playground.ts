import Resource from './resource'

class Playground {
  get resources() {
    return this._resources
  }

  private _resources: Resource[] = []

  /* eslint-disable-next-line no-use-before-define */
  private static instance: Playground = new Playground()

  static get = (): Playground => Playground.instance
}

export default Playground
