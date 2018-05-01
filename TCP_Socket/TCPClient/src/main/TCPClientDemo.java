package main;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
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
            System.out.println("1. Send a Message");
            System.out.println("2. Exit");
            System.out.print("\nEnter Choice :");
            
            choice=Integer.parseInt(br.readLine());
            if(choice == 1)
            {
                System.out.print("[Client] Enter Message: ");
                strMsg=br.readLine();
                
                //Send the message to server...
                dout.writeUTF( strMsg );
                
                // Wait for the echo from server...
                msgFromServer=din.readUTF();
                System.out.print("[Server] Echo: " + msgFromServer + "\n");
            }
            else if(choice == 2)
            {
                dout.close();
                System.exit(1);
            }
            else
            {

            }
        }
    }
      /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/    
}

