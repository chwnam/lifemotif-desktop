#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QSslSocket>
#include <QString>

#include "tag.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_action_connect_triggered();
  void on_action_disconnect_triggered();

  void socketConnected();
  void socketDisconnected();
  void socketStateChanged(QAbstractSocket::SocketState state);
  void socketEncrypted();
  void socketReadyRead();
  void sslErrors(const QList<QSslError> &errors);

  void on_action_exit_triggered();
  void on_submitButton_clicked();
  void on_commandEdit_returnPressed();

  void on_action_advance_tag_counter_triggered();

  void on_action_reset_tag_triggered();

private:
  void updateUI();
  void submit();
  void disconnect();
  void appendString(const QString& string);

private:
  Ui::MainWindow *ui;
  QSslSocket* socket;
  Tag         tag;
};

#endif // MAIN_WINDOW_H
