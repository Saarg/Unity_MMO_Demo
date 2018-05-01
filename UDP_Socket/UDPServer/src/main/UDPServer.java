package main;

import java.net.*;

public class UDPServer 
{

	/**
	 * @param args
	 */
	public static void main(String[] args) throws Exception
	{
    	// The port number of UDP Server for listening the incoming packets.	 
	   	int portNumber;
	   	
		if (args.length == 1)
		{	
			portNumber = Integer.parseInt(args[0]);
						
		}
		else
		{
			System.out.print("Invalid parampeters ");
			return;
		} 
		
		try
	    { 
		   	// Create the UDP Socket.
		   	DatagramSocket serverSocket = new DatagramSocket( portNumber ); 
		  
		    byte[] receiveData = new byte[1024]; 
		    System.out.println ("Server begins waiting for messages:......");
		  
		    while(true) 
		    { 
				 receiveData = new byte[1024]; 
				
				  DatagramPacket receivePacket = 
				         new DatagramPacket(receiveData, receiveData.length); 
				
				  // Wait for incoming data...
				  serverSocket.receive(receivePacket); 
				
				  // Get the incoming data, and abstract information from data.
		          String sentence = new String(receivePacket.getData()); 
		          int rxSize = receivePacket.getLength();
		            
		          sentence = sentence.substring(0, rxSize);
		          
				  System.out.println ("Received Message: " + sentence);
				  
		    }
	     }
	     catch (SocketException ex) 
	     {
	    	System.out.println("UDP Port is occupied.");
		 	System.exit(1);
		 }		

	}

}
