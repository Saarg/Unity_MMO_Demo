package main.common;

import java.io.*;

public class Calc implements Serializable {

    public int a = 0;
    public int b = 0;

    private void writeObject(java.io.ObjectOutputStream out)
    throws IOException {
        out.writeInt(a);
        out.writeInt(b);
    }

    private void readObject(java.io.ObjectInputStream in)
    throws IOException, ClassNotFoundException {
        a = in.readInt();
        b = in.readInt();
    }

    public int add(int a, int b){
        return a + b;
    }

    public int addInternal(){
        return this.a + this.b;
    }
}