compilation:
javac -Xlint -d ./bin $(find ./src/main/ -name "*.java")

server side:
java main.server.TCPServer 1234

client side:
java main.client.TCPClientDemo 127.0.0.1 1234

MILJ13109509