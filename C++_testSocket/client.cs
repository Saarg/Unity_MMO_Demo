using System.Net;
using System.Net.Sockets;
using System.Text;

Console.WriteLine("Hello World!");

Socket s = null;
IPHostEntry hostEntry = null;
Socket serverSocket = null;

hostEntry = Dns.GetHostEntry("127.0.0.1");
foreach(IPAddress address in hostEntry.AddressList) {
    IPEndPoint ipe = new IPEndPoint(address, 1234);
    serverSocket = new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

    serverSocket.Connect(ipe);

    if(serverSocket.Connected)
    {
        print("Client connected to server!");
        break;
    }
    else
    {
        continue;
    }
}

byte[] msg = Encoding.UTF8.GetBytes("hello from client");
byte[] bytes = new byte[1024];

// Blocks until send returns.
int byteCount = serverSocket.Send(msg, SocketFlags.None);
Console.WriteLine("Sent {0} bytes.", byteCount);

// Get reply from the server.
byteCount = serverSocket.Receive(bytes, SocketFlags.None);
if (byteCount > 0)
    Console.WriteLine(Encoding.UTF8.GetString(bytes));