package main;

import java.net.*;
import java.util.Date;
import java.io.*;


public class TCPServer
{

	public static void main(String args[]) throws Exception
    {
		Socket clientSoc;
		ServerSocket serverSoc;
        
	   	int portNumber;
	   	
		if (args.length == 1)
		{	
			portNumber = Integer.parseInt(args[0]);
			serverSoc = new ServerSocket(portNumber);
							
		}
		else
		{
			System.out.print("Invalid parampeters ");
			return;
		} 
		
		System.out.println("\nTCP Server Started on Port Number: " + portNumber);
        
        while(true)
        {
            System.out.println("Waiting for Connection ...");
            clientSoc = serverSoc.accept();
            TCPServerThread serverThread = new TCPServerThread(clientSoc);
        }
    }
}


class TCPServerThread extends Thread
{
    Socket ClientSoc;
    DataInputStream din;
    DataOutputStream dout;

    TCPServerThread(Socket soc)
    {
        try
        {
            ClientSoc = soc;
            
            din = new DataInputStream(ClientSoc.getInputStream());
            dout = new DataOutputStream(ClientSoc.getOutputStream());
            
            System.out.println("TPC Client Connected ...");
            start();
        }
        catch(Exception ex)
        {
        }
    }
        
        
    @Override
    public void run()
    {
    	String msg;
    	
        while(true)
        {
            try
            {
              
                // In...
                msg = din.readUTF();
                System.out.println("----> " + msg);
                
                //Out: received message + current time of server
                dout.writeUTF( msg + "  ["+(new Date().toString())+ "]");
            }
            catch(Exception ex)
            {
            }
        }
    }
}

