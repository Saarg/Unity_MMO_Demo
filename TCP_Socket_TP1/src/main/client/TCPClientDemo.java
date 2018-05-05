package main.client;

import java.io.*;
import java.net.Socket;

import main.common.*;

public class TCPClientDemo 
{

	public static void main(String[] args) throws Exception
	{
		if (args.length == 2)
		{	
			TCPClient client = new TCPClient(args[0], Integer.parseInt(args[1]));
			
			client.display();
			
		}
		else
		{
			System.out.print("Invalid parampeters ");
			return;
		}
	}
}


class TCPClient
{
    Socket ClientSoc;
   
    DataInputStream din;
    DataOutputStream dout;
    BufferedReader br;
       
    TCPClient(String hostName, int port)
    {
        try
        {
            ClientSoc = new Socket(hostName,port);
            din=new DataInputStream(ClientSoc.getInputStream());
            dout=new DataOutputStream(ClientSoc.getOutputStream());
            br=new BufferedReader(new InputStreamReader(System.in));
        }
        catch(Exception ex)
        {
        }
	}
    
    
    public void display() throws Exception
    {
    	String strMsg;
    	String msgFromServer;
    	
    	int choice;
    	
        while(true)
        {
            System.out.println("\n\n[ DISPLAY ]");
            System.out.println("1. (OBJECTColl) Ask server to add");
            System.out.println("2. (BYTEColl) Ask server to add");
            System.out.println("3. Exit");
            System.out.print("\nEnter Choice :");
            
            choice = Integer.parseInt(br.readLine());
            if(choice == 1)
            {
                dout.writeInt(0);
            }
            else if(choice == 2)
            {
                try {
                    dout.writeInt(1);

                    File file = new File("./main/common/Calc.class");
                    dout.writeUTF("./main/common/Calc.class");
                    ByteStream.toStream(dout, file);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            else if(choice == 3)
            {
                dout.writeInt(2);
                dout.close();
                System.exit(1);
            }
            else
            {
                System.out.print("\nAre you even trying? :");
            }

            if (choice != 3) {
                Calc c = new Calc();

                ObjectOutputStream  oos = null;
                
                System.out.print("\nFirst number :");                
                c.a = Integer.parseInt(br.readLine());
                System.out.print("\nSecond number :");                                
                c.b = Integer.parseInt(br.readLine());

                try {
                    oos = new ObjectOutputStream(dout);
                    oos.writeObject(c);
                    
                    ObjectInputStream ois = new ObjectInputStream(din);
                    Object result = ois.readObject();
                    
                    System.out.println("\nServer said: " + c.a + " + " + c.b + " = " + result.toString());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}

