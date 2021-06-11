#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include "main.h"
#include <ncurses.h>
#include <curses.h>

using namespace std;

/* void BitsetDouble(double c){ */
/*     /1* std::cout<< (int)c <<std::endl; *1/ */
/*     std::bitset<64> x(c); */
/*     std::cout<< x << \ */
/*         "\n string: "<< x.to_string() << \ */
/*         "\n ulong: "<< x.to_ulong() << \ */
/*         "\n converted: " << stod(x.to_string()) << \ */
/*         std::endl; */
/* } */


int main(){
    initscr();
    echo();
    keypad(stdscr, true);
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

    /* Variables */
    TPacket packet;
    char char_vel[20] = "0.0";
    char char_ang[20] = "0.0";
    int ch;

    /* Configuring GUI and staring infinite loop, navigating the menu with @ch */
    printw("[INFO] Initialising screen.\n");
    timeout(-1);
    noecho();
    while(1){
        printw("[INFO] Entering the main menu...\n");
        printw("[INFO] Press: \t 'i' to input values, \n \
                'q' to quit, \n \
                'l' to send in loop, \n \
                's' to show current [v, w] values.\n");
        ch = getch();
        
        /* @ch: Exit loop */
        if (ch == 'q') break;

        /* @ch: Input values and create a packet */
        if (ch == 'i'){
            timeout(-1);
            printw("[INFO] Input velocities: \n[v]> "); 
            echo();
            getstr(char_vel);
            printw("[w]> ");
            getstr(char_ang);;
            noecho();
               
            packet.i_ver = 9;
            packet.i_content_id = 17;
            packet.AppData.velocity.v = std::stod(char_vel);
            packet.AppData.velocity.w = std::stod(char_ang);
            printw("[INFO] Entered velocities: [%f, %f].\n", \
                    std::stod(char_vel), std::stod(char_ang));
        }

        /* @ch: Continuous loop */
        if (ch == 'l'){
            printw("[INFO] Sending data with frequency 1Hz.\n");
            while(1){
                nodelay(stdscr, TRUE);
                usleep(1000000);
                if (send(sock, &packet, sizeof(packet)+1, 0) == -1){
                        printw("[INFO] Packet transfer error. Aborting...\n");
                        continue;
                }
                printw("[INFO] Packet sent. Press 'q' to exit the loop and enter menu.\n");
                ch = getch();
                if (ch == 'q') break;
            }
        }
        
        /* @ch: Show current values */
        if (ch == 's') printw("[INFO] Current values u[w, v]: [%f, %f]\n", \
                    std::stod(char_vel), std::stod(char_ang));
        
    }

    /* Close the socket */
    printw("[INFO] Closing the socket.\n");
    endwin();
    close(sock);
    getch();
    
    return 0;
}
