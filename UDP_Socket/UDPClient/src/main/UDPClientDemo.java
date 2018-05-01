package main;

import java.net.*;
import java.io.*;


public class UDPClientDemo 
{
	
	public static void main(String[] args) throws Exception
	{
		if (args.length == 2)
		{	
			UDPClient client = new UDPClient(args[0], Integer.parseInt(args[1]));
			client.display();
			
		}
		else
		{
			System.out.print("Invalid parampeters ");
			return;
		}

	}

}

class UDPClient
{
	// The port number of Server
	private int portNumber = 0;
	
	// Name of Server
	private String serverHostName = null;
	
	BufferedReader br;
	
	UDPClient(String hostName, int port)
	{
		serverHostName = hostName;
		portNumber = port;
		br=new BufferedReader(new InputStreamReader(System.in));
	}
	
	int sendData( String Strdata )
	{
		try{
			byte[] sendData = new byte[1024];
			
			// Creat the UDP socket
			DatagramSocket clientSocket = new DatagramSocket(); 
	  
			// Get the IP of the host.
			InetAddress IPAddress = InetAddress.getByName(serverHostName); 
			System.out.println ("Attemping to connect to " + IPAddress + " via UDP port: " + portNumber);
	  
			// Generate the data for sending...
			sendData = Strdata.getBytes();         
	        System.out.println ("Sending data to " + sendData.length + " bytes to server.");
			
	        // Generate the sendPack...
			DatagramPacket sendPacket = 
				new DatagramPacket(sendData, sendData.length, IPAddress, portNumber); 
	  
			// Begin to send...
			clientSocket.send(sendPacket); 
			
			// Close the Socket...
			clientSocket.close();
		}
		catch (UnknownHostException ex) { 
			System.err.println(ex);
	    }
		catch (IOException ex) {
			System.err.println(ex);
	    }
	    
		 
		return 0;
	}// int sendData
	
    public void display() throws Exception
    {
    	int choice;
        String strMsg;
        
    	while(true)
        {
            System.out.println("\n\n[ DISPLAY ]");
            System.out.println("1. Send a Message");
            System.out.println("2. Exit");
            System.out.print("\nEnter Choice :");
            
            choice=Integer.parseInt(br.readLine());
            if (choice == 1)
            {                
                System.out.print("Enter Message: ");
                strMsg=br.readLine();
                sendData (strMsg);
            }
            else if(choice==2)
            {
                System.exit(1);
            }
            else
            {
            	
            }

        }
    } //display
}