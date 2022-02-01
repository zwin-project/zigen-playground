import http from "http";
import { URL } from "url";

class User {
  static fromRequest = (req: http.IncomingMessage): User | null => {
    let url: URL;

    if (!req.url) return null;

    try {
      url = new URL(req.url, "http://noop.com");
    } catch (error) {
      return null;
    }

    const id = parseInt(url.searchParams.get("id") as string);
    if (isNaN(id)) return null;

    return new User(id);
  };

  private constructor(public id: number) {}
}

export default User;
