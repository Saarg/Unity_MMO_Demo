package main;

import java.net.*;
import java.util.Date;
import java.io.*;
import java.lang.reflect.*;
import javax.tools.*;

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
        int instruction;
    	String msg;
    	
        while(true)
        {
            try
            {
                instruction = din.readInt();
                
                int a;
                int b;
                Method method;
                Object result;
                switch (instruction) {
                    case 0:
                        System.out.println("received instruction id: " + instruction);
                        ObjectInputStream ois = new ObjectInputStream(din);
                        Object object = ois.readObject();
                        System.out.println("Object " + object.getClass().getName() + " succesfully received! ");
                        
                        method = object.getClass().getMethod("add", int.class, int.class);

                        a = din.readInt();
                        b = din.readInt();

                        result = method.invoke(object, a, b);

                        System.out.println(a + " + " + b + " = " + result.toString());                

                        dout.writeInt((int) result);
                        break;
                    case 1:
                        File file = new File("./main/Calc.class");
                        ByteStream.toFile(din, file);

                        a = din.readInt();
                        b = din.readInt();

                        ClassLoader classLoader = new URLClassLoader(new URL[]{file.toURI().toURL()});
                        Class classe = classLoader.loadClass("main." + file.getName().substring(0, file.getName().lastIndexOf(".")));

                        System.out.println("Class " + classe.getName() + " was loaded successfully.");

                        method = classe.getDeclaredMethod("add", int.class, int.class);

                        result = method.invoke(classe.getDeclaredConstructor().newInstance(), a, b);

                        System.out.println(a + " + " + b + " = " + result.toString());
                        
                        dout.writeInt((int) result);                        
                        break;
                }
            }
            catch(Exception ex)
            {
                ex.printStackTrace();
            }
        }
    }
}

