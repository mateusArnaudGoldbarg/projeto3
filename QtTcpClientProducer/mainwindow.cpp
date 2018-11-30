#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow){
  ui->setupUi(this);
  socket = new QTcpSocket(this);

  connect(ui->pushButton_Start,
          SIGNAL(clicked(bool)),
          this,
          SLOT(timerControl()));

  connect(ui->pushButton_Connect,
           SIGNAL(clicked(bool)),
           this,
           SLOT(tcpConnect()));
  connect(ui->pushButton_Disconnect,
           SIGNAL(clicked(bool)),
           this,
           SLOT(tcpDisconnect()));
  connect(ui->horizontalSlider_Max,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(mudaMax()));
  connect(ui->horizontalSlider_Min,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(mudaMin()));
  connect(ui->horizontalSlider_Time,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(mudaTempo(int)));
  ui->horizontalSlider_Time->setEnabled(false);
}

void MainWindow::tcpConnect(){
  socket->connectToHost(ui->lineEdit_ip->text(),1234);
  if(socket->waitForConnected(3000)){
    qDebug() << "Connected";
    statusBar()->showMessage("Connected");
  }
  else{
    qDebug() << "Disconnected";
  }
}

void MainWindow::tcpDisconnect()
{
   socket->disconnectFromHost();
   statusBar()->showMessage("Disconnected");
}

void MainWindow::mudaMax()
{
    if(ui->horizontalSlider_Max->value() < ui->horizontalSlider_Min->value())
        ui->horizontalSlider_Max->setValue(ui->horizontalSlider_Max->value()+1);
}

void MainWindow::mudaMin()
{
    if(ui->horizontalSlider_Min->value() > ui->horizontalSlider_Max->value())
        ui->horizontalSlider_Min->setValue(ui->horizontalSlider_Min->value()-1);
}

void MainWindow::mudaTempo(int _tempo)
{
    tempo = _tempo;
    killTimer(timerId);
    timerId = startTimer(1000*tempo);
}

void MainWindow::putData(){
  QDateTime datetime;
  QString str;
  qint64 msecdate;
  QStringList list;

  if(socket->state()== QAbstractSocket::ConnectedState){

    msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
    str = "set "+ QString::number(msecdate) + " " +
            QString::number(ui->horizontalSlider_Min->value() +
                            qrand()%(ui->horizontalSlider_Max->value() - ui->horizontalSlider_Min->value() +1))+"\r\n";
    socket->write(str.toStdString().c_str()) + " bytes written";


      list.append(str);
      if(socket->waitForBytesWritten(3000)){
      }
       ui->listWidget_lista->addItems(list);
  }
}


MainWindow::~MainWindow(){
  delete socket;
    delete ui;
}



void MainWindow::timerEvent(QTimerEvent *event)
{
    putData();
}

void MainWindow::timerControl()
{
    if(flag){
        timerId = startTimer(1000*tempo);
        flag = false;
        ui->pushButton_Start->setText("Stop");
        ui->horizontalSlider_Time->setEnabled(true);
    }else{
        killTimer(timerId);
        flag = true;
        ui->pushButton_Start->setText("Start");
        ui->horizontalSlider_Time->setEnabled(false);
    }
}
