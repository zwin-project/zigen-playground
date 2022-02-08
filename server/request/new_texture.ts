class NewTextureRequest {
  constructor(seed: any) {
    this.id = (seed.data && seed.data.id) || -1
    this.url = (seed.data && seed.data.url) || ''
  }

  action = 'new-texture' as const

  id: number
  url: string
}

export default NewTextureRequest
