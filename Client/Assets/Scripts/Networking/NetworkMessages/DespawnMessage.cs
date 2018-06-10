using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DespawnMessage: NetworkMessage {

    public int objectId = -1;

    public DespawnMessage() {
        id = 3;
        size = 1 + sizeof(int);
    }

	public override void Deserialize(ref byte[] buffer) {
        if (buffer[0] != id) {
            Debug.LogError("Deserializing wrong packet id found!");
            return;
        }

        int offset = 1;

        objectId = BitConverter.ToInt32(buffer, offset);
        offset += sizeof(int);

	}

	public override void Serialize(out byte[] buffer) {
		buffer = new byte[sizeof(int) + size];
        Buffer.BlockCopy(BitConverter.GetBytes(size), 0, buffer, 0, sizeof(int));
        int offset = sizeof(int); 

        buffer[offset++] = id;

        Buffer.BlockCopy(BitConverter.GetBytes(objectId), 0, buffer, offset, sizeof(int));
        offset += sizeof(int);
	}
}