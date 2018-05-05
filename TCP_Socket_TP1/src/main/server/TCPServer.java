package main.server;

import java.net.*;
import java.util.Date;
import java.io.*;
import java.lang.reflect.*;
import javax.tools.*;

import main.common.*;

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
            ex.printStackTrace();
        }
    }
        
        
    @Override
    public void run()
    {
        int instruction;
    	
        while(true)
        {
            try
            {
                // Read method
                instruction = din.readInt();
                
                // Objects and methods
                Object object;
                Method method;
                Object result;
                
                // OBJECTColl 
                if (instruction == 0) {
                    // Nothing specific to do if OBJECTColl
                }
                // BYTEColl
                else if (instruction == 1) {
                    // Read filepath
                    String filePath = din.readUTF();
                    // Open file
                    File file = new File(filePath);
                    // Read class file from stream
                    ByteStream.toFile(din, file);

                    // Load downloaded class
                    ClassLoader classLoader = new URLClassLoader(new URL[]{file.toURI().toURL()});
                    Class classe = classLoader.loadClass("main.common." + file.getName().substring(0, file.getName().lastIndexOf(".")));
                    object = classe.getDeclaredConstructor().newInstance();
                    System.out.println("Class " + classe.getName() + " was loaded successfully.");
                }
                // EXIT
                else if (instruction == 2) {
                    dout.close();
                    System.exit(1);
                }
                
                // Read serialized object
                ObjectInputStream ois = new ObjectInputStream(din);
                object = ois.readObject();
                System.out.println("Object " + object.getClass().getName() + " succesfully received! ");

                // Find method
                method = object.getClass().getMethod("addInternal");
                // Invoke method
                result = method.invoke(object);

                System.out.println("result: " + result.toString());
                
                // Serialize and send result
                ObjectOutputStream oos = new ObjectOutputStream(dout);
                oos.writeObject(result);
            }
            catch(Exception ex)
            {
                ex.printStackTrace();
            }
        }
    }
}

