#include "user_interface.h"
#include <iostream>
#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
extern  void sort1000randoms(bool writeInFile);
UserInterface::UserInterface():
_thread_user_request_process()
{


    new (&_thread_user_request_process) thread(&UserInterface::monitor_user_request_thread_proc, this);
}
// UserInterface::
// _second_thread()
// {


//     new (& _second_thread) thread(&UserInterface::second_proces, 1);
// }
// std::share_ptr <std::thread> second thread = std::make_shared<std::thread>(&UserInterface::monitor_user_request_thread_proc, 1);
UserInterface::user_request_t UserInterface::check_new_user_request(string request)
{
    if(request == "avg1000runtimes") {
        return AVERAGE_OF_1000_RUNTIMES;
    }
    else if(request == "quit") {
        return QUIT;
    }
    return ERROR_USER_COMMAND;
}

void UserInterface::monitor_user_request_thread_proc() {
    
    int fd; 

    char buffer[1024];

    fd = open("../user_request", O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open");
        return ;
    } 
    ssize_t bytes;
    
    for(;;){
        if((bytes = read(fd, buffer, sizeof(buffer))) > 0){
            buffer[bytes] = 0;
            std::string message(buffer);
            
            printf("User request: %s\n", message.c_str());
            user_request_t validValue = check_new_user_request(message);
            switch(validValue) {
                case ERROR_USER_COMMAND:
                    // print command options
                    cout << "wrong command"<<endl;
                break;
                case AVERAGE_OF_1000_RUNTIMES:
                     sort1000randoms(true);
                    cout<<"avg 1000 runtimes= "<<message<<endl;
                break;
                case QUIT:
                    cout<<"quit= "<<message<<endl;
                    return;
            }
        }else{
            if (errno == EWOULDBLOCK) {
                continue;
            }
            sleep(1);
        } 
    }
}

void UserInterface::second_proces()
{
    //trimite acel signal and execute avg 1000 times
     int fd; 

    char buffer[1024];

    fd = open("../user_request", O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open");
        return ;
    } 
    ssize_t bytes;
    
    for(;;){
        if((bytes = read(fd, buffer, sizeof(buffer))) > 0){
            buffer[bytes] = 0;
            std::string message(buffer);
            
            printf("User request: %s\n", message.c_str());
            user_request_t validValue = check_new_user_request(message);
            switch(validValue) {
                case ERROR_USER_COMMAND:
                    // print command options
                    cout << "wrong command"<<endl;
                break;
                case AVERAGE_OF_1000_RUNTIMES:
                    for(int i = 0; i <1000;i++)
                    {
                        sort1000randoms(false);
                    }
                    cout<<"avg 1000 runtimes= "<<message<<endl;
                    cout<<"Sunt in seccond procces "<<message<<endl;
                break;
                case QUIT:
                    cout<<"quit= "<<message<<endl;
                    return;
            }
        }else{
            if (errno == EWOULDBLOCK) {
                continue;
            }
            sleep(1);
        } 
    }
}

//for (2) procces we need a aditional thread care asteapta listener 
//thread sa trmita un semnal cand avg command este primit
//