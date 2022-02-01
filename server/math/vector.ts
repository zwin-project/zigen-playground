export class Vector3 {
  constructor(public x: number, public y: number, public z: number) {}

  toArray = (): [x: number, y: number, z: number] => [this.x, this.y, this.z]
}
