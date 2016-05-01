#include "widget.h"

Widget::Widget(QWidget *parent)
        : QWidget(parent)
{
    this->setGeometry(200,100,740, 680);
    messigeText1 = new  QTextEdit(this);
    messigeText1->setGeometry(10, 10, 550, 570 );
    messigeText1->setReadOnly(true);
    messigeText2 = new  QTextEdit(this);
    messigeText2->setGeometry(10, 600, 610, 70);
    ClientName = new QListWidget(this);
    ClientName->setGeometry(575, 10, 150, 570 );

    QPushButton *cend = new QPushButton(this);
    cend->setText("Send");
    cend->setGeometry(630, 600, 90,70);
    struct sockaddr_in addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock<0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7777);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(::connect(sock, (struct sockaddr *)&addr, sizeof(addr))   < 0)
    {
        perror("connect");
        exit(2);
    }
    connect(cend, SIGNAL(clicked()), this, SLOT(sendMessage() ));
    std::thread recvtrd(&Widget::recvMessage, this);
    recvtrd.detach();
    this->close();
}

Widget::~Widget()
{

}

void Widget::function(QListWidgetItem* item)
{
    messigeText2->setText(item->text() + ':');
}


void Widget::sendMessage()
{
    std::string str;
    str = "";
    str =  messigeText2->toPlainText().toStdString();
    send(sock, str.c_str(),sizeof(message),0);

    messigeText2->clear();
}

void Widget::recvMessage()
{
    while(1){
        bzero(buf,500);
        recv(sock, buf, 500, 0);
        if (strlen(buf)== 0){
           exit (3);
        }
        std::istringstream ss(buf);
        std::string token;

        if(buf[0]==' '){
            qDebug("dksajfkdsjk");
            memmove (buf, buf+1, strlen (buf+1));
            while(std::getline(ss, token, ':')){
                if(isspace(token[0])) {
                    token.erase(0,1);
                }
                if(token != "") {
                ClientName->addItem(token.c_str());
                connect(ClientName, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(function(QListWidgetItem*)));
                }
            }
        }else if(buf[0]=='-'){
              std::stringstream ss(buf);
              std::string str1;
              str1 = ss.str() ;
              str1.erase(str1.begin() + 0);
              str1.erase(str1.begin()+str1.find(':'), str1.end());
              for(int i = 0; i < ClientName->count(); ++i){
                  std::string str =  ClientName->item(i)->text().toStdString();
                  if(str == str1){
                     delete ClientName->item(i);
                  }
              }
        }else{
            messigeText1->append(buf);
        }
        bzero(buf, 500);
    }
}

