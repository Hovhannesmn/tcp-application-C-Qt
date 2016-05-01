#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <QWidget>
#include <QDialog>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <iostream>

#include <string>
#include <thread>
#include <string.h>
#include  <sstream>
#include <QTextEdit>
#include <QString>
#include <QRadioButton>
#include<QListWidget>
class Widget : public QWidget
{
    Q_OBJECT
private:
    QTextEdit *messigeText1; //monitor
    QTextEdit *messigeText2;
    QWidget* main;
    QListWidget * ClientName;
char message [500];
char buf[500];

private slots:
        void sendMessage();
        void function(QListWidgetItem* item);

public:
    int sock;
    void recvMessage();
    Widget(QWidget *parent = 0);
     ~Widget();
};
#endif // WIDGET_H
