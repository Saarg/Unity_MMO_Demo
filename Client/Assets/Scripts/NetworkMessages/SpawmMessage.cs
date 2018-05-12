using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnMessage: NetworkMessage {


    public int prefabId = -1;
    public int objectId = -1;

    float[] position = new float[3];
    float[] rotation = new float[4];

    public SpawnMessage() {
        id = 2;
        size = 1 + 2 * sizeof(int) + 7 * sizeof(float);
    }

    public SpawnMessage(Transform t) {
        id = 2;
        size = 1 + 2 * sizeof(int) + 7 * sizeof(float);

        NetworkIdentity netId = t.GetComponent<NetworkIdentity>();
        if (netId == null)
            throw new NullReferenceException();

        position [0] = t.position.x;
        position [1] = t.position.y;
        position [2] = t.position.z;

        rotation [0] = t.rotation.w;
        rotation [1] = t.rotation.x;
        rotation [2] = t.rotation.y;
        rotation [3] = t.rotation.z;
    }

	public override void Deserialize(ref byte[] buffer) {
        int offset = sizeof(int) + 1;

        prefabId = BitConverter.ToInt32(buffer, offset);
        offset += sizeof(int);
        objectId = BitConverter.ToInt32(buffer, offset);
        offset += sizeof(int);

        position [0] = BitConverter.ToSingle(buffer, offset);
        offset += sizeof(float);
        position [1] = BitConverter.ToSingle(buffer, offset);
        offset += sizeof(float);
        position [2] = BitConverter.ToSingle(buffer, offset);
        offset += sizeof(float);

        rotation [0] = BitConverter.ToSingle(buffer, offset);
        offset += sizeof(float);
        rotation [1] = BitConverter.ToSingle(buffer, offset);
        offset += sizeof(float);
        rotation [2] = BitConverter.ToSingle(buffer, offset);
        offset += sizeof(float);
        rotation [3] = BitConverter.ToSingle(buffer, offset);
        offset += sizeof(float);
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

        Buffer.BlockCopy(position, 0, buffer, offset, position.Length*sizeof(float));
        offset += position.Length*sizeof(float);

        Buffer.BlockCopy(rotation, 0, buffer, offset, rotation.Length*sizeof(float));
        offset += rotation.Length*sizeof(float);
	}
}