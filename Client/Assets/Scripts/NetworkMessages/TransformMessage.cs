using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TransformMessage: NetworkMessage {

    float[] position = new float[3];
    float[] rotation = new float[4];

    public TransformMessage() {
        id = 1;
        size = 1 + 7 * sizeof(float);
    }

    public TransformMessage(Transform t) {
        id = 1;
        size = 1 + 7 * sizeof(float);

        position [0] = t.position.x;
        position [1] = t.position.y;
        position [2] = t.position.z;

        rotation [0] = t.rotation.w;
        rotation [1] = t.rotation.x;
        rotation [2] = t.rotation.y;
        rotation [3] = t.rotation.z;
    }

    public void Apply(Transform t) {
        t.position = new Vector3(position[0], position[1], position[2]);
        t.rotation = new Quaternion(rotation[0], rotation[1], rotation[2], rotation[3]);
    }

	public override void Deserialize(ref byte[] buffer) {
        int offset = 1;

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
		buffer = new byte[size];
        buffer[0] = id;
        int offset = 1;

        Buffer.BlockCopy(position, 0, buffer, offset, position.Length*sizeof(float));
        offset += position.Length*sizeof(float);

        Buffer.BlockCopy(rotation, 0, buffer, offset, rotation.Length*sizeof(float));
        offset += rotation.Length*sizeof(float);
	}
}