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
#include <iostream>
#include <sys/ipc.h> 
#include <sys/msg.h>
using namespace std;

//This take a request from user and send it to another process
typedef struct message_buffer{
    long message_type;
    char message_text[100];
};

int main()  {
 int fd, id;
 char * myfifo = new char [12];
 char *mymessage = new char[20];
 strcpy(myfifo, "../user_request");
//  strcpy(mymessage, "../msgid");

 /* create the FIFO (named pipe) */
 mkfifo(myfifo, 0666);
 fd = open("../user_request", O_WRONLY ); //open(myfifo, O_WRONLY | O_NONBLOCK);
 //create the msgid pipe for communication between procces for receiving RunTimeValue
  mkfifo(mymessage, 0666);
 id = open("../user_request", O_WRONLY ); //open(myfifo, O_WRONLY | O_NONBLOCK);
 if (fd == -1) {
     perror("open");
     return EXIT_FAILURE;
 }
 if(id == -1){
     perror("open");
     return EXIT_FAILURE;
 }
 printf("File open\n");
 while(1) {
     string request;
    cout << "User Request: ";
    cin >> request;
    //send request to second thread
    std::string send = request;
    message_buffer message;
    key_t key = ftok("progfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    message.message_type = 1;
    strcpy(message.message_text, send.c_str());
    //message.message_text = request;
    msgsnd(msgid, &message, sizeof(message), 0);
    // write(id, msgid, )
    printf("Data sent is : %s \n", message.message_text);
    write(fd, request.c_str(), request.size());
    sleep(1);

 }
 close(fd);
 printf("Connection closed\n");
 /* remove the FIFO */
 unlink(myfifo);
 return 0;
}