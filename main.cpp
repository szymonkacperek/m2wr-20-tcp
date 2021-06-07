#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <bitset>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include <fcntl.h>

using namespace std;

void SendSerial(unsigned char c){
    /* std::cout<< (int)c <<std::endl; */
    std::bitset<8> x(c);
    std::cout<< x <<std::endl;
}

void BitsetDouble(double c){
    std::bitset<64> x(c);
    std::cout<< x << \
        "\n string: "<< x.to_string() << \
        "\n ulong: "<< x.to_ulong() << \
        "\n converted: " << stod(x.to_string()) << \
        std::endl;
}


int main(){
    /* Creating a socket */
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1){
        cerr<<"Can't create a socket!";
        return -1;
    } else {
        cout << "Socket created. Value of 'listening': " << listening << endl;
    }

    /* Bind the socket to IP / port */
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (sockaddr*) &hint, sizeof(hint)) == -1){
        cerr << "Can't bind to IP/port";
        return -2;
    }

    /* Mark the socket for listening in */ 
    if (listen(listening, SOMAXCONN) == -1){
        cerr << "Can't listen"; 
        return -3;

    }

    /* Accept a call */
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    /* Create a socket and turn into non-blocking */
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    if (clientSocket == -1){
        cerr<<"Problem with client connecting";
        return -4;
    }
    /* fcntl(clientSocket, F_SETFL, O_NONBLOCK); */

    /* Close the listening socket */
    close(listening); 
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

    if (result){
        cout << host << " connected on " << svc << endl;
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on " << ntohs(client.sin_port) << endl;
    }

    /* Create robot model, store state into variables */
    RobotModel robot_model_1(10.0, 10.0, 3.1416/3.0);
    double x, y, theta, v, w;
    TPacket r_packet;

    /* Infinite loop */
    while(1){
         /* Wait for the message and store into TPacket struct */
        int bytes_received = recv(clientSocket, &r_packet, 4096, 0);
        if (bytes_received == -1){
            cerr << "Connection issue" << endl;
            return -4;
        }

        /* Stopping the robot */
        if (bytes_received == 0){
            cout << "[WARN] Client disconnected. Stopping the robot..." << endl;
            while(r_packet.AppData.velocity.v >= 0){
                r_packet.AppData.velocity.v -= 0.1;
                r_packet.AppData.velocity.w -= 0.1;
                robot_model_1.Update(r_packet.AppData.velocity.v, r_packet.AppData.velocity.w);
                robot_model_1.GetState(&x, &y, &theta, &v, &w);
                cout << "out[x, y, theta, v, w] = [" << x << ", " \
                    << y << ", " << theta <<  ", " << v << ", " << w << "]" << endl;
                if(r_packet.AppData.velocity.v < 0.2){
                    r_packet.AppData.velocity.v = 0;
                }
                usleep(1000000);
            }
            
            return -5;
        }
       
        /* Update robot state and display incoming [v, w] values */
        usleep(1000000);
        robot_model_1.Update(r_packet.AppData.velocity.v, r_packet.AppData.velocity.w);
        robot_model_1.GetState(&x, &y, &theta, &v, &w);
        cout << "out[x, y, theta, v, w] = [" << x << ", " << \
            y << ", " << theta <<  ", " << v << ", " << w << "]" << endl;
    }

    /* Close the socket */
    close(clientSocket);

    return 0;
}


/* Split the values into bytes */

        /* cout << "Received: " << string(buf, 0, bytes_received) << endl; */
        /* for (int i=0; i<24; i++){ */
        /*     cout<<"Received on "<< i <<"B: "; */
        /*     SendSerial(buf[i]); */
        /* } */ 
        /* r_packet.i_ver = buf[0] + buf[1] + buf[2] + buf[3]; */
        /* r_packet.i_content_id = buf[4] + buf[5] + buf[6] + buf[7]; */
        /* r_packet.AppData.velocity.v =  (uint32_t)buf[8]           | \ */
        /*                             ((uint32_t)buf[9]  << 8)     | \ */
        /*                             ((uint64_t)buf[10] << 16)   | \ */
        /*                             ((uint64_t)buf[11] << 24)   | \ */
        /*                             ((uint64_t)buf[12] << 32)   | \ */
        /*                             ((uint64_t)buf[13] << 40)   | \ */
        /*                             ((uint64_t)buf[14] << 48)  | \ */
        /*                             ((uint64_t)buf[15] << 56); */
        /* cout << "bit_velocity: "; */
        /* BitsetDouble(r_packet.AppData.velocity.v); */
        /* cout << "sizeof char buf: " << sizeof(buf) << endl; */
