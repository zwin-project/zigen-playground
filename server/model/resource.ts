interface Resource {
  serialize: () => { id: number } & { [key: string]: object | string | number }
  id: () => number
}

export class ResourceIdGenerator {
  static nextId = 1
  static get = (): number => {
    const id = ResourceIdGenerator.nextId
    ResourceIdGenerator.nextId += 1
    return id
  }
}

export default Resource
