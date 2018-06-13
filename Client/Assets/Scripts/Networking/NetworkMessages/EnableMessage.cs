using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnableMessage: NetworkMessage {

    public int objectId = -1;
    public bool toEnable = false;

    public float[] position = new float[3];
    public float[] rotation = new float[4];

    public EnableMessage() {
        id = 4;
        size = 1 + 8*sizeof(int) + sizeof(bool);
    }

	public override void Deserialize(ref byte[] buffer) {
        if (buffer[0] != id) {
            Debug.LogError("Deserializing wrong packet id found!");
            return;
        }

        int offset = 1;

        objectId = BitConverter.ToInt32(buffer, offset);
        offset += sizeof(int);
        toEnable = BitConverter.ToBoolean(buffer, offset);
        offset += sizeof(bool);

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

        Buffer.BlockCopy(BitConverter.GetBytes(objectId), 0, buffer, offset, sizeof(int));
        offset += sizeof(int);   
        Buffer.BlockCopy(BitConverter.GetBytes(toEnable), 0, buffer, offset, sizeof(bool));
        offset += sizeof(bool);

        Buffer.BlockCopy(position, 0, buffer, offset, position.Length*sizeof(float));
        offset += position.Length*sizeof(float);

        Buffer.BlockCopy(rotation, 0, buffer, offset, rotation.Length*sizeof(float));
        offset += rotation.Length*sizeof(float);
	}
}