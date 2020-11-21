#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <chrono>
#include <string.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <stdlib.h>     /* srand, rand */
#include <vector>
#include <string>
#include <algorithm>    // std::sort
#include <fstream>
#include <thread>
#include <sys/ipc.h> 
#include <sys/msg.h>
#include "user_interface.h"
using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

class MeasureFuncTime {
    string name;

public:
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::duration<double> time_span;
    MeasureFuncTime(string _name) :
        name(_name),
        start(std::chrono::high_resolution_clock::now()) {}
    ~MeasureFuncTime()
    {
        time_span =
            std::chrono::duration_cast<std::chrono::duration<double>>(
                std::chrono::high_resolution_clock::now() - start);
        cout << name << " time: " << time_span.count() << endl;
    }
};
//average of the runtimeValues
std::chrono::duration<double> average = std::chrono::duration<double>{0};
//remove Duplicates
void removeDuplicates(std::vector<unsigned int>& randomMillion);
void sort1000randoms(bool writeInFile)
{
        //MeasureFuncTime func_time("RuntimeValue");
         std::vector<unsigned int> randomMillion(1000000);
         //generator
         std::generate(randomMillion.begin(),randomMillion.end(),[](){
                    return rand() % 1000000;
         });
         //labmda function for gerenator
         const auto sortComp = [](const unsigned int lhs,
                                  const unsigned int rhs) noexcept -> bool {
                  return (lhs < rhs);
         };
         std::sort(randomMillion.begin(), randomMillion.end(), sortComp);
         removeDuplicates(randomMillion);
         if(writeInFile)
         {
            ofstream myfile;
            myfile.open("MillionRandSorted.txt", std::ofstream::out);
            if (myfile.is_open()) {

                    std::for_each(randomMillion.begin(), randomMillion.end(), [&myfile](unsigned int value)
                    {
                    myfile << value << endl;
                    });
                    myfile.close();
         }
         }
         else
         {
            MeasureFuncTime func_time("RuntimeValue");
            std::chrono::duration<double> time_to_send = func_time.time_span;

            average += time_to_send;
            int fd;
            char *runTimeValuePipe = new char[20];
            string to_send = to_string(time_to_send.count()) + 's';
            strcpy(runTimeValuePipe, "../runTimeValue");
            /* create the FIFO (named pipe) */
            mkfifo(runTimeValuePipe, 0666);
            fd = open("../runTimeValue", O_WRONLY ); //open(myfifo, O_WRONLY | O_NONBLOCK); 
            mkfifo(to_send.c_str(), 0666);  
            // sends to process (1) the runtime value at the end of each execution
            if (fd == -1) {
                perror("open");
                return;
            }
            //write throw the pipe
            write(fd, to_send.c_str(), to_send.size());
            std::cout << "RunTimeValue Write to the pipe is: " << to_send.c_str();
         }
}

void recordRuntimeValue(int id)
{   
    MeasureFuncTime *funct_time = new MeasureFuncTime("Runtime");
    std::cout << "Threadul separat a pornit" << std::endl;
    UserInterface user;
    //procesul prin care se citeste RunTimeValue  si se ruleaza de 1000 de ori sort1000randoms
    user.second_proces();

    // int fd; 

    // char buffer[1024];

    // fd = open("../user_request", O_RDONLY | O_NONBLOCK);
    // if (fd == -1) {
    //     perror("open");
    //     return ;
    // }

    // ssize_t bytes;
    
    // for(;;){
    //     if((bytes = read(fd, buffer, sizeof(buffer))) > 0){
    //         buffer[bytes] = 0;
    //         std::string message(buffer);
    //         UserInterface::user_request_t valid = user.check_new_user_request(message);
    //         printf("User request is: %s\n", message.c_str());
    //         cout << message <<"Message is";
    //             if(valid == UserInterface::AVERAGE_OF_1000_RUNTIMES)
    //             {
    //                 for(int i = 0; i <1;i++)
    //                     {
    //                         sort1000randoms(false);
    //                         // auto t_final_iteratie = std::chrono::high_resolution_clock::now();
    //                         // auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t_final_iteratie - t_inceput ).count();
                            
    //                         // cout << "Runtime Value for rulation " << i << " is "<< duration;
    //                         //trimite runtimevalue catre pipe pentru p1
    //                         // delete funct_time;
    //                     }
    //                 cout<<"avg 1000 runtimes = Message is"<<message<<endl;

    //             }
    //             else
    //             {
    //                 cout << "wrong command"<<endl;
    //                 //return;
    //             }
            
    //         }
    //         sleep(1); 
    // }
}   

std::thread th(recordRuntimeValue, 1);

int main()  {
        cout << "new listener."<<endl;
       th.detach();
        if (th.joinable())
          th.join();

        while(!UserInterface::getInstance().join_listener())
        {
               sleep_for(seconds(1));
        }
        cout << "exit listener."<<endl;
 	return 0;
}
//this function remove the duplicates
void removeDuplicates(std::vector<unsigned int>& randomMillion)
{
    std::vector<unsigned int>::iterator it;
    // using unique() method to remove duplicates
    it = std::unique(randomMillion.begin(), randomMillion.end());
    // resize the new vector
    randomMillion.resize(std::distance(randomMillion.begin(), it));
}


//procesul 1 trimite comanda avg 
//procesul 2 citeste din pipe comanda intr-un tread separat
//thread executa de 1000 ori functia
//de fiecare data trimite catre procesul 1 runtimevalue
//si acesta il afiseaza
//si face media aritmetica si il afiseaza