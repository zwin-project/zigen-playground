import { Vector3 } from "../../math/vector";
import Resource from "../resource";

class Cuboid implements Resource {
  constructor(private position: Vector3, private halfSize: Vector3) {}

  serialize = (): object => {
    return {
      type: "cuboid",
      position: this.position.toArray(),
      half_size: this.halfSize.toArray(),
    };
  };
}

export default Cuboid;
