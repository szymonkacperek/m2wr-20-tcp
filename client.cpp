#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include "main.h"

using namespace std;

void BitsetDouble(double c){
    /* std::cout<< (int)c <<std::endl; */
    std::bitset<64> x(c);
    std::cout<< x << \
        "\n string: "<< x.to_string() << \
        "\n ulong: "<< x.to_ulong() << \
        "\n converted: " << stod(x.to_string()) << \
        std::endl;
}


int main(){
    /* Create a socket */
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        return 1;
    }
   
    /* Create a hint structure for the server we're connecting with */
    int port = 54000;
    string ip_address = "127.0.0.1";
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ip_address.c_str(), &hint.sin_addr);

    /* Connect to the server on the socket */
    int connect_result = connect(sock, (sockaddr*) &hint, sizeof(hint));
    if (connect_result == -1){
        return 1;
    }

    /* While loop */
    string velocity;
    string angular_acc;
    char buf[4096];

    while(1){
        /* Enter values */
        cout << "Input velocities:\n[v]> ";
        getline(cin, velocity);
        cout << "[omega]> ";
        getline(cin, angular_acc);
           
        /* Create a packet */   
        TPacket packet;
        packet.i_ver = 9;
        packet.i_content_id = 17;
        packet.AppData.velocity.v = std::stod(velocity);
        packet.AppData.velocity.w = std::stod(angular_acc);
        cout << "Size of 'packet' created: " << sizeof(packet) <<"B"<<endl;
        /* std::cout << "Velocity sent: " << packet.AppData.velocity.v << std::endl; */
        /* std::cout << "Velocity bin: "; */
        /* BitsetDouble(packet.AppData.velocity.v); */

        /* Send to server */
        if (send(sock, &packet, sizeof(packet)+1, 0) == -1){
            cout << "Packet transfer error.";
            continue;
        }

        /* Wait for response */
        /* memset(buf, 0, 4096); */
        /* int bytes_received = recv(sock, buf, 4096, 0); */

        /* Display response */
        /* cout << "SERVER> " << string(buf, bytes_received) << "\r\n"; */
    }

    /* Close the socket */
    close(sock);
    
    return 0;
}
