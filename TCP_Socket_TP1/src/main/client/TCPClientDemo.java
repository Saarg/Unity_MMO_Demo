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
            // Ask what method to test
            System.out.println("\n\n[ DISPLAY ]");
            System.out.println("1. (OBJECTColl) Ask server to add");
            System.out.println("2. (BYTEColl) Ask server to add");
            System.out.println("3. Exit");
            System.out.print("\nEnter Choice :");
            
            // Note method choise
            choice = Integer.parseInt(br.readLine());

            // If OBJECTColl
            if(choice == 1)
            {
                // Send method id
                dout.writeInt(0);
            }
            // If BYTEColl
            else if(choice == 2)
            {
                try {
                    // Send method id
                    dout.writeInt(1);
                    
                    // Load class file
                    File file = new File("./main/common/Calc.class");
                    // Send the file path to stay consistent between client and server
                    dout.writeUTF("./main/common/Calc.class");
                    // Send the class file to the server
                    ByteStream.toStream(dout, file);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            else if(choice == 3)
            {
                // Handle exit
                dout.writeInt(2);
                dout.close();
                System.exit(1);
            }
            else
            {
                // If the choice isn't a known choice just tell the user
                System.out.print("\nAre you even trying? :");
            }
            
            // This part is common to both methods
            if (choice == 1 || choice == 2) {
                // Create the object
                Calc c = new Calc();
                
                // Ask user for the params
                System.out.print("\nFirst number :");                
                c.a = Integer.parseInt(br.readLine());
                System.out.print("\nSecond number :");                                
                c.b = Integer.parseInt(br.readLine());

                try {
                    // Send serialized object
                    ObjectOutputStream oos = new ObjectOutputStream(dout);
                    oos.writeObject(c);
                    // Read serialize object result
                    ObjectInputStream ois = new ObjectInputStream(din);
                    Object result = ois.readObject();
                    
                    // Tell the user
                    System.out.println("\nServer said: " + c.a + " + " + c.b + " = " + result.toString());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}

