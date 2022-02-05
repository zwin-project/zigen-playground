import { expect } from 'chai'
import { Vector3 } from '../math/vector'
import Playground from '../model/playground'
import Cuboid from '../model/resource/cuboid'
import Router from '../router'
import TestClient from './client'

describe('sync message', () => {
  it('returns all resources', () => {
    const playground = Playground.get()
    const cuboid1 = new Cuboid(new Vector3(1, 1, 1), new Vector3(2, 2, 2))
    const cuboid2 = new Cuboid(new Vector3(3, 3, 3), new Vector3(4, 4, 4))
    playground.resources.push(cuboid1)
    playground.resources.push(cuboid2)

    const router = new Router()
    const client = new TestClient()
    router.add(client)
    client.emitMessage(JSON.stringify({ action: 'sync' }))
    expect(client.sendCount).to.equal(1)
    const sendData = JSON.parse(client.sendMessage)

    expect(sendData.action).to.equal('sync')
    expect(sendData.data).length(2)

    expect(sendData.data[0]).deep.equal(cuboid1.serialize())
  })
})
