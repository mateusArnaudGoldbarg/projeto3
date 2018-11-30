#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

private:

    int tempo = 1;

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void timerEvent(QTimerEvent *event);

  int timerId;

  bool flag = true;

  void putData();
public slots:

  void timerControl();

  void tcpConnect();

  void tcpDisconnect();

  void mudaMax();

  void mudaMin();

  void mudaTempo(int _tempo);

private:

  Ui::MainWindow *ui;

  QTcpSocket *socket;
};

#endif // MAINWINDOW_H
