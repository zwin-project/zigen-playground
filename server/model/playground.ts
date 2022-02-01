import Resource from "./resource";

class Playground {
  private static instance: Playground = new Playground();

  static get = (): Playground => Playground.instance;

  get resources() {
    return this._resources;
  }

  private _resources: Resource[] = [];

  private constructor() {}
}

export default Playground;
