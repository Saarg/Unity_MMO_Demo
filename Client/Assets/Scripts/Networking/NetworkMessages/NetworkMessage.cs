using System;
using System.Collections;
using System.Collections.Generic;

enum MessageId {None, Transform, Spawn, Despawn, Enable}

public class NetworkMessage {

	protected static byte id = 0;
	public int Id { get { return id; } }
	protected static int size = 1024;
	public int Size { get { return size; } }

	public NetworkMessage() {

	}

	public virtual void Deserialize(ref byte[] buffer) {

	}

	public virtual void Serialize(out byte[] buffer) {
		buffer = new byte[1024];

	}
}
