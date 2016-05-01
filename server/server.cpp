#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <thread>
#include <vector>
#include <string.h>
#include <sstream>
char buf[500];
std::vector<int> sockVect;
std::vector<std::string> clientname;


void wraitMessageName(int sockI)
{ 	

    std::string str = " ";
    std::string strname = "";
    char namesStr[500] = {0};
    if(sockI < 0){
        str = "-";
        sockI = sockI * (-1);
 
        for(int i = 0; i < sockVect.size(); ++i){
           if(sockVect[i] == sockI){
                strname.append(str);
                strname.append(clientname[i]);
                std::copy(strname.begin(),strname.end(),namesStr);


                for(int j = 0; j < sockVect.size(); ++j){
                    if(i == j)
                        continue;
                    else{


                        std::cout << namesStr<< "\n";
                        send(sockVect[j],namesStr,strlen(namesStr),0);
                        std::cout<<"sockVect[j]"<<sockVect[j]<<std::endl;
                    }
                }
           }
       }
    }else{
        strname.append(str); 
        std::string addName = "";
        char addName1[50] = {0};
        for(int i =0; i<clientname.size()-1; ++i){ 
            strname.append(clientname[i]);
        }
        std::copy(strname.begin(),strname.end(), namesStr);

		send(sockI, namesStr, strlen(namesStr), 0);
        addName.append(str);
        addName.append(clientname[clientname.size() - 1]);
        std::copy(addName.begin(),addName.end(),addName1);
        for(int i = 0; i < sockVect.size(); ++i){
         send(sockVect[i],addName1,strlen(addName1),0);
        

       }
    }
 }




void sendMessage(int sockId,char * message,int recverId)
{ 

			
	if(sockId == 0){
		for(int i = 0; i < sockVect.size(); ++i){
            if(sockId == recverId)
                continue;
            else        
                send(sockVect[i], message,strlen(message), 0 );	
		}
	}else{

		send(sockId,message,strlen(message),0); 
}	

}



void recvMessage(int sockId)
{	
	while(1){
		int sock;
		std::string name = "";
		std::string messegeAndName = "";
		char message[500] = {};
        bzero(buf,500);
		recv(sockId, buf, sizeof(buf), 0);
		if (strlen(buf)== 0) {
          for(int i = 0; i < sockVect.size(); ++i){
              if(sockVect[i] == sockId){
                sockId = sockId * (-1);
                wraitMessageName(sockId);
                sockVect.erase(sockVect.begin() + i);
              
                clientname.erase(clientname.begin()+ i) ;
               }
          }
		
            
            return;
        
        }
		for(int i = 0; i < sockVect.size(); i++){
			if(sockId == sockVect[i] )
				name= clientname[i];
         //WrdMessageClient( sock, message    );
		}

		std::string strmass(buf);

		strmass.erase (strmass.begin()+ strmass.find(":")+1, strmass.end());

		for(int i = 0; i< sockVect.size(); i++){
			if(strmass==clientname[i]){
				sock = sockVect[i];
				break;
			}else{ 
				sock =0;
			}
		}
        if(sock != 0){
            std::cout << "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n";
            sendMessage(sockId,buf,sockId);
        }
		std::string mess(buf);
		mess.erase(0,mess.find(":") + 1);
		messegeAndName.append(name);
		messegeAndName.append(mess);
		std::copy(messegeAndName.begin(),messegeAndName.end(),message);
		sendMessage(sock,message,sockId);
		bzero(message,500);        
		bzero(buf, 500); 
	}
}


int main(int argc, char *argv[])
{
	int sock, port, listener;
	struct sockaddr_in addr;
	listener = socket(AF_INET, SOCK_STREAM, 0);

	if(argc < 2)
	{
		perror("socket");
		exit(1);
	}
	bzero((char*) &addr, sizeof(addr));
	port = atoi(argv[1]);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr))<  0)
	{
		perror("bind");
		exit(2);
	}

	listen(listener, 0);
	std::string str1 = "client";
	int i = 1;
	while(1){
		sock = accept(listener, NULL, NULL );
		if(sock<0)
		{
			perror("accept");
			exit(3);
		}
		std::stringstream str;
		str << i;
		std::string str3 =":";
		std::string str2 = str.str();
		std::string client;
		client.append(str1);
		client.append(str2);
		client.append(str3);	
		clientname.push_back(client);
		++i;
        wraitMessageName(sock);

		std::thread* recvtrd=new std::thread(recvMessage,sock);
		sockVect.push_back(sock);

		recvtrd->detach();
	}
	close(sock);
	close(listener);
}


