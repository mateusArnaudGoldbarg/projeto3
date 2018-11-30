#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QDebug>
#include <QStringList>

#include "mythread.h"
#include "datastorage.h"


class MyServer : public QTcpServer
{
  Q_OBJECT
public:

  MyServer(QObject *parent = 0);

  void startServer();

  QStringList getIPList();

signals:
  void message(QString);

public slots:
  void receiveMsg(QString str);

protected:

  void incomingConnection(qintptr socketDescriptor);
private:
  int ok;

  DataStorage storage;
  QStringList iplist;
};

#endif // MYSERVER_H
