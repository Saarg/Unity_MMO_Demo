using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnMessage: NetworkMessage {


    public int prefabId = -1;
    public int objectId = -1;
    public bool hasAuthority = false;
    
    public SpawnMessage() {
        id = 2;
        size = 1 + 2 * sizeof(int) + sizeof(bool);
    }

	public override void Deserialize(ref byte[] buffer) {
        if (buffer[0] != id) {
            Debug.LogError("Deserializing wrong packet id found!");
            return;
        }

        int offset = 1;

        prefabId = BitConverter.ToInt32(buffer, offset);
        offset += sizeof(int);
        objectId = BitConverter.ToInt32(buffer, offset);
        offset += sizeof(int);
        hasAuthority = BitConverter.ToBoolean(buffer, offset);
        offset += sizeof(bool);
	}

	public override void Serialize(out byte[] buffer) {
		buffer = new byte[sizeof(int) + size];
        Buffer.BlockCopy(BitConverter.GetBytes(size), 0, buffer, 0, sizeof(int));
        int offset = sizeof(int); 

        buffer[offset++] = id;

        Buffer.BlockCopy(BitConverter.GetBytes(prefabId), 0, buffer, offset, sizeof(int));
        offset += sizeof(int); 
        Buffer.BlockCopy(BitConverter.GetBytes(objectId), 0, buffer, offset, sizeof(int));
        offset += sizeof(int);      
        Buffer.BlockCopy(BitConverter.GetBytes(hasAuthority), 0, buffer, offset, sizeof(bool));
        offset += sizeof(bool);
	}
}