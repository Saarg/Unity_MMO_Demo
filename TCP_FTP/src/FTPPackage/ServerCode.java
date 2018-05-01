package FTPPackage;


import java.net.*;
import java.io.*;
/**
 *
 * @author Hamid Mcheick
 */

public class ServerCode
{
    public static void main(String args[]) throws Exception
    {
        ServerSocket soc=new ServerSocket(6754);
        System.out.println("\n\nFTP Server Started on Port Number 6754");
        while(true)
        {
            System.out.println("Waiting for Connection ...");
            transferfile t=new transferfile(soc.accept());
        }
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

class transferfile extends Thread
{
    Socket ClientSoc;
    DataInputStream din;
    DataOutputStream dout;
      /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        //the constructor:
    transferfile(Socket soc)
    {
        try
        {
            ClientSoc = soc;
            din = new DataInputStream(ClientSoc.getInputStream());
            dout = new DataOutputStream(ClientSoc.getOutputStream());
            System.out.println("FTP Client Connected ...");
            start();
        }
        catch(Exception ex)
        {
        }
    }
        
      /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/        
	
    void SendFile() throws Exception
    {
        String filename=din.readUTF();
        System.out.println("File Name is:"+ filename);
        String fn = "Files_Of_Server/"+filename;
          
        System.out.println(fn);
        
        File f=new File(fn);
        if(!f.exists())
        {
            dout.writeUTF("File Not Found");
            return;
        }
        else
        {
            dout.writeUTF("READY");
            String option=din.readUTF();
            if(option.equalsIgnoreCase("N")){
                dout.writeUTF("<<<<<<<<<<<<<<<<<<<<<< wasn't transfered... >>>>>>>>>>>>>>>>>>>>>>");
                return;
            }
            FileInputStream fin=new FileInputStream(f);
            int ch;
            do
            {
                ch=fin.read();
                dout.writeUTF(String.valueOf(ch));
            }
            while(ch!=-1);
            fin.close();
            dout.writeUTF("<<<<<<<<<<<<<<<<<<<<<< File Receive Successfully >>>>>>>>>>>>>>>>>>>>>>");
        }
    }
    
      /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
       
    void ReceiveFile() throws Exception
    {
        String filename=din.readUTF();
	if(filename.compareTo("File not found")==0)
        {
            return;
        }
        File f=new File("Files_Of_Server/"+filename);
        String option;

        if(f.exists())
        {
            dout.writeUTF("File Already Exists");
            option=din.readUTF();
        }
        else
        {
            dout.writeUTF("SendFile");
            option="Y";
        }

        if(option.equalsIgnoreCase("Y"))
        {
            FileOutputStream fout=new FileOutputStream(f);
            int ch;
            String temp;
            do
            {
                temp=din.readUTF();
                ch=Integer.parseInt(temp);
                if(ch!=-1)
                {
                    fout.write(ch);
                }
            }while(ch!=-1);
            fout.close();
            dout.writeUTF("<<<<<<<<<<<<<<<<<<<<<< File Send Successfully >>>>>>>>>>>>>>>>>>>>>>");
        }
        else
        {
            dout.writeUTF("<<<<<<<<<<<<<<<<<<<<<< File wasn't sent >>>>>>>>>>>>>>>>>>>>>>");
            return;
        }
    }

      /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        
    @Override
    public void run()
    {
        while(true)
        {
            try
            {
                System.out.println("Waiting for Command ...");
                String Command=din.readUTF();
                if(Command.compareTo("GET")==0)
                {
                    System.out.println("\tGET Command Received ...\nSo the server must send the file to the client..");
                    SendFile();
                    continue;
                }
                else if(Command.compareTo("SEND")==0)
                {
                    System.out.println("\tSEND Command Receiced ...\nSo the server must receive the file from the client..");
                    ReceiveFile();
                    continue;
                }
                else if(Command.compareTo("DISCONNECT")==0)
                {
                    System.out.println("\tDisconnect Command Received ...(So the server must close the connection with the client..)");
                    System.exit(1);
                }
            }
            catch(Exception ex)
            {
            }
        }
    }
          /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
}

/**********************  The end of Server program..  ***********************************/


