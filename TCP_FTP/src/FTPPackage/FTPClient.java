package FTPPackage;

import java.net.*;
import java.io.*;
/**
 *
 * @author Hamid Mcheick
 */

class FTPClient
{
    public static void main(String args[]) throws Exception
    {
        Socket soc=new Socket("127.0.0.1",6754);
        transferfileClient t=new transferfileClient(soc);
        t.displayMenu();
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

class transferfileClient
{
    Socket ClientSoc;

    DataInputStream din;
    DataOutputStream dout;
    BufferedReader br;
      /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/    
    transferfileClient(Socket soc)
    {
        try
        {
            ClientSoc=soc;
            din=new DataInputStream(ClientSoc.getInputStream());
            dout=new DataOutputStream(ClientSoc.getOutputStream());
            br=new BufferedReader(new InputStreamReader(System.in));
        }
        catch(Exception ex)
        {
        }
	}
    
      /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/    
    
    void SendFile() throws Exception
    {
        String filename;
        System.out.print("Enter File Name :");
        filename=br.readLine();
        
        File f=new File("Files_Of_Client/"+filename);
        if(!f.exists())
        {
            System.out.println("\n{{{File not exists at the client side... Please re-enter the correct name for the file...}}}");
            dout.writeUTF("File not found");
            return;
        }

        dout.writeUTF(filename);

        String msgFromServer=din.readUTF();
        String option="Y";
        if(msgFromServer.compareTo("File Already Exists")==0)
        {
            option="none";
            while(!option.equalsIgnoreCase("Y")&&!option.equalsIgnoreCase("N"))
            {
                System.out.println("File Already Exists. Want to OverWrite (Y/N) ?");
                option=br.readLine();
            }
            dout.writeUTF(option);
        }

        if(option.equalsIgnoreCase("Y"))
        {
            System.out.println("Sending File ...");
            FileInputStream fin=new FileInputStream(f);
            int ch;
            do
            {
        	ch=fin.read();
        	dout.writeUTF(String.valueOf(ch));
            }
            while(ch!=-1);
            fin.close();
        }
        System.out.println(din.readUTF());
    }
    
      /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    
    void ReceiveFile() throws Exception
    {
        String fileName;
        System.out.print("Enter File Name :");
        fileName=br.readLine();
        dout.writeUTF(fileName);
        String msgFromServer=din.readUTF();
        if(msgFromServer.compareTo("File Not Found")==0)
        {
            System.out.println("File not found on Server ...:(");
            return;
        }
        else if(msgFromServer.compareTo("READY")==0)
        {
            System.out.println("Receiving File ...");
            File f=new File("Files_Of_Client/"+fileName);
            //File f=new File("C:\\Users\\QIYAN\\workspace\\FTPClient\\bin\\Files_Of_Client\\abc.txt");
            if(f.exists())
            {
            	/*
                String Option="none";
                while(!Option.equalsIgnoreCase("Y")&&!Option.equalsIgnoreCase("N"))
                {
                    System.out.println("File Already Exists. Do you want to OverWrite (Y/N) ?");
                    Option=br.readLine();
                    if(Option.equalsIgnoreCase("N"))
                    {
                        dout.writeUTF("N");
                        return;
		            }
                    else
                        dout.writeUTF("Y");
                }
                */
            	
            }
            dout.writeUTF("Y");
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
            System.out.println(din.readUTF());
        }
    }
    
      /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    
    public void displayMenu() throws Exception
    {
        while(true)
        {
            System.out.println("\n\n[ MENU ]");
            System.out.println("1. Send a File");
            System.out.println("2. Receive a File");
            System.out.println("3. Exit");
            System.out.print("\nEnter Choice :");
            int choice;
            choice=Integer.parseInt(br.readLine());
            if(choice==1)
            {
                dout.writeUTF("SEND");
                SendFile();
            }
            else if(choice==2)
            {
                dout.writeUTF("GET");
                ReceiveFile();
            }
            else
            {
                dout.writeUTF("DISCONNECT");
                System.exit(1);
            }
        }
    }
      /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/    
}


        
        

