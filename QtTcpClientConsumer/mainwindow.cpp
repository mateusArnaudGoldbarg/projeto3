#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <vector>
#include <algorithm>
#include "plotter.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  socket = new QTcpSocket(this);

  connect(ui->pushButton_Connect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(tcpConnect()));

  connect(ui->pushButton_Disconnect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(tcpDisconnect()));

  connect(ui->pushButton_Update,
          SIGNAL(clicked(bool)),
          this,
          SLOT(getHost()));

  connect(ui->pushButton_Start,
          SIGNAL(clicked(bool)),
          this,
          SLOT(timerControl()));
  connect(ui->horizontalSlider_Time,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(mudaTempo(int)));
  connect(this,
            SIGNAL(emiteDados(vector<float>,vector<float>)),
            ui->widget,
            SLOT(armazenaPontos(vector<float>,vector<float>)));

  ui->horizontalSlider_Time->setEnabled(false);
  ui->pushButton_Start->setEnabled(false);

}

void MainWindow::tcpConnect(){
  socket->connectToHost(ui->lineEdit_Ip->text(),1234);
  if(socket->waitForConnected(300)){
    qDebug() << "Connected";
    statusBar()->showMessage("Connected");
  }
  else{
    statusBar()->showMessage("Disconnected");
  }
}

void MainWindow::tcpDisconnect(){
    socket->disconnectFromHost();
    if(socket->waitForDisconnected(300)){
        statusBar()->showMessage("Disconnected");
    }
}

void MainWindow::getData(){
  QString str;
  QByteArray array;
  QStringList list;
  qint64 thetime;
  float thevalue;
  vector<qint64> tempos;
  vector<float> valores;


  qDebug() << "to get data...";
  if(socket->state() == QAbstractSocket::ConnectedState){
    if(socket->isOpen()){

        if(ui->listWidget->count() == 0 || ui->listWidget->currentItem()->isSelected() == false)
          return;

      qDebug() << "reading...";
      str = "get "+ui->listWidget->currentItem()->text()+" 30\r\n";
      socket->write(str.toStdString().c_str());
      socket->waitForBytesWritten(3000);
      socket->waitForReadyRead(3000);
      qDebug() << socket->bytesAvailable();
      while(socket->bytesAvailable()){
        str = socket->readLine().replace("\n","").replace("\r","");
        list = str.split(" ");
        if(list.size() == 2){
          bool ok;
          str = list.at(0);
          thetime = str.toLongLong(&ok);
          tempos.push_back(thetime);
          str = list.at(1);
          thevalue = str.toFloat(&ok);
          valores.push_back(thevalue);
         }

      }

      for(int i=0; i<tempos.size(); i++){
          qint64 tempo = tempos[i] - tempos[0];
          tempoNorm.push_back(tempo);
      }

      for(int i=0; i<tempos.size(); i++){
          tempoNorm[i] = tempoNorm[i]/ tempoNorm[tempos.size()-1];

          qDebug() << "Tempo " << tempoNorm[i] << " Valores " << valores[i];
      }

    }

    emit emiteDados(tempoNorm, valores);
  }
}

void MainWindow::getHost(){
  QString str;

  qDebug() << "to get host...";
  ui->listWidget->clear();
  if(socket->state() == QAbstractSocket::ConnectedState){
    if(socket->isOpen()){
      qDebug() << "reading...";
      socket->write("list\r\n");
      socket->waitForBytesWritten(3000);
      socket->waitForReadyRead(3000);
      qDebug() << socket->bytesAvailable();
      while(socket->bytesAvailable()){
        str = socket->readLine().replace("\n","").replace("\r","");
        ui->listWidget->addItem(str);
      }
    }
  }

  if(ui->listWidget->count() != 0 || ui->listWidget->currentItem()->isSelected() == true)
    ui->pushButton_Start->setEnabled(true);
}

MainWindow::~MainWindow()
{
  delete socket;
  delete ui;
}


void MainWindow::timerEvent(QTimerEvent *event)
{
    getData();
}

void MainWindow::timerControl()
{
    if(flag){
        timerId = startTimer(1000*tempo);
        flag = false;
        ui->pushButton_Start->setText("Stop");
        ui->horizontalSlider_Time->setEnabled(true);
        getData();
    }else{
        killTimer(timerId);
        flag = true;
        ui->pushButton_Start->setText("Start");
        ui->horizontalSlider_Time->setEnabled(false);
    }
}

void MainWindow::mudaTempo(int _tempo)
{
    tempo = _tempo;
    killTimer(timerId);
    timerId = startTimer(1000*tempo);
}
