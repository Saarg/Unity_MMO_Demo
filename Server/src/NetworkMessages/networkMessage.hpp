#ifndef NETWORK_MESSAGE_HPP
#define NETWORK_MESSAGE_HPP

class NetworkMessage {
    public:
        NetworkMessage(int id, int size): id(id), size(size) {
            buffer = new char[size];
        }

        ~NetworkMessage() {
            delete[] buffer;
        }

        virtual char* Serialize() = 0;
        virtual void Deserialize(char* buffer) = 0;

        inline void Send(int socket) { send(socket, Serialize(), size + sizeof(int), 0); }

        inline int Id() { return id; }
        inline int Size() { return size; }

    protected:
        int id = 0;
        int size = 1;

        char* buffer;
};

#endif