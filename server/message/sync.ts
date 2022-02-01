class SyncMessage {
  type = "sync" as const;

  constructor(json: any) {}
}

export default SyncMessage;
