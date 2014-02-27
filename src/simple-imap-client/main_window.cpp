#include "main_window.h"
#include "ui_main_window.h"
#include "connect_dialog.h"

#include <QDebug>
#include <QScrollBar>
#include <QSslCipher>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  socket(NULL),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  disconnect();
  delete ui;
}

void MainWindow::on_action_connect_triggered()
{
  ConnectDialog dlg(this);

  if (QDialog::Accepted == dlg.exec()) {
    const QString host = dlg.host();
    const quint16 port = dlg.port();
    const bool    use_ssl = dlg.use_ssl();

    qDebug() << "connecting" << host << port << "using ssl = " << use_ssl;

    if (socket == NULL) {
      socket = new QSslSocket(this);

      /* connected */
      connect(socket, SIGNAL(connected()),
              this, SLOT(socketConnected()));

      /* disconnected */
      connect(socket, SIGNAL(disconnected()),
              this, SLOT(socketDisconnected()));

      /* state changed */
      connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
              this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));

      /* encrypted */
      connect(socket, SIGNAL(encrypted()),
              this, SLOT(socketEncrypted()));

      /* ssl errors */
      connect(socket, SIGNAL(sslErrors(QList<QSslError>)),
              this, SLOT(sslErrors(QList<QSslError>)));

      /* ready read */
      connect(socket, SIGNAL(readyRead()),
              this, SLOT(socketReadyRead()));
    }
    socket->connectToHostEncrypted(host, port);
  }
}

void MainWindow::on_action_disconnect_triggered()
{
  disconnect();
}

void MainWindow::socketConnected()
{
  qDebug() << "connected.";
}

void MainWindow::socketDisconnected()
{
  qDebug() << "disconnected.";

  if (socket) {
    /* disconnected */
    QObject::disconnect(socket, SIGNAL(disconnected()),
            this, SLOT(socketDisconnected()));
  }
}

void MainWindow::socketStateChanged(QAbstractSocket::SocketState state)
{
  qDebug() << "Socket state changed:" << state;
}

void MainWindow::socketEncrypted()
{
  if (socket == NULL) {
    return;
  }

  QSslCipher cipher = socket->sessionCipher();
  QString cipherText
      = QString("%1, %2 (%3/%4)").arg(cipher.authenticationMethod())
      .arg(cipher.name()).arg(cipher.usedBits()).arg(cipher.supportedBits());

  qDebug() << "Cipher data:" << cipherText.toStdString().c_str();
}

void MainWindow::socketReadyRead()
{
  appendString(QString::fromUtf8(socket->readAll()));
}

void MainWindow::sslErrors(const QList<QSslError> &errors)
{
  qDebug() << "SSL error: ";
  for(QList<QSslError>::const_iterator it = errors.cbegin();
      it != errors.cend();
      ++it) {
    qDebug() << "\t" << *it;
  }
  qDebug() << "";
}

void MainWindow::appendString(const QString& string)
{
  QTextCursor cursor(ui->textEdit->textCursor());
  cursor.movePosition(QTextCursor::End);
  cursor.insertText(string);
  cursor.insertText(QChar('\n'));

  QScrollBar* bar = ui->textEdit->verticalScrollBar();
  if (bar) {
    bar->setValue(bar->maximum());
  }
}

void MainWindow::disconnect()
{
  if (socket && socket->state() == QAbstractSocket::ConnectedState) {

    /* connected */
    QObject::disconnect(socket, SIGNAL(connected()),
            this, SLOT(socketConnected()));

    /* state changed */
    QObject::disconnect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));

    /* encrypted */
    QObject::disconnect(socket, SIGNAL(encrypted()),
            this, SLOT(socketEncrypted()));

    /* ssl errors */
    QObject::disconnect(socket, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(sslErrors(QList<QSslError>)));

    /* ready read */
    QObject::disconnect(socket, SIGNAL(readyRead()),
            this, SLOT(socketReadyRead()));

    socket->close();
    socket->deleteLater();
    socket = NULL;
  }

  appendString(QString("<<< Disconnected from server >>>"));
}

void MainWindow::on_action_exit_triggered()
{
  close();
}

void MainWindow::on_submitButton_clicked()
{
  const QString& text = ui->commandEdit->text();

  qDebug() << "Text: " << text;

  submit(text);
}

void MainWindow::submit(const QString& text)
{
  if (socket) {
    appendString(text);
    socket->write(text.toUtf8() + "\r\n");
  }
}
