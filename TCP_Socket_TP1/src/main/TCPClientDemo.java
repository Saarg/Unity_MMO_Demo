package main;

import java.io.*;
import java.net.Socket;

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
                Calc c = new Calc();

                ObjectOutputStream  oos = null;
                
                System.out.print("\nFirst number :");                
                int a = Integer.parseInt(br.readLine());
                System.out.print("\nSecond number :");                                
                int b = Integer.parseInt(br.readLine());

                try {
                    dout.writeInt(0);
                    oos = new ObjectOutputStream(dout);
                    oos.writeObject(c);

                    dout.writeInt(a);
                    dout.writeInt(b);
                    
                    int result = din.readInt();
                    
                    System.out.println("\nServer said: " + a + " + " + b + " = " + result);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            else if(choice == 2)
            {
                try {
                    dout.writeInt(1);
                    System.out.print("\nFirst number :");                
                    int a = Integer.parseInt(br.readLine());
                    System.out.print("\nSecond number :");                                
                    int b = Integer.parseInt(br.readLine());

                    File file = new File("./main/Calc.class");
                    ByteStream.toStream(dout, file);

                    dout.writeInt(a);
                    dout.writeInt(b);
                    
                    int result = din.readInt();
                    
                    System.out.println("\nServer said: " + a + " + " + b + " = " + result);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            else if(choice == 3)
            {
                dout.close();
                System.exit(1);
            }
            else
            {
                System.out.print("\nAre you even trying? :");
            }
        }
    }
}

