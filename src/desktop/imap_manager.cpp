#include "imap_manager.h"

#include <QDebug>
#include <QByteArray>
#include <QSslCipher>


namespace LifeMotif {

ImapManager::ImapManager(QObject *parent) :
  QObject(parent),
  socket(0)
{
}

void ImapManager::Connect(
    const QString& host,
    const quint16  port,
    const bool     useSsl)
{
  if (socket == NULL) {
    socket = new QSslSocket(this);
    ConnectSlots();

    if (useSsl) {
      socket->connectToHostEncrypted(host, port);
    }
    else {
      socket->connectToHost(host, port);
    }
  }
}

void ImapManager::Disconnect()
{
  if (socket) {
    socket->disconnectFromHost();
    DisconnectSlots();
  }
}

void ImapManager::Send(const QByteArray& data)
{
  if (socket) {
    // append tag
    QByteArray dataWithTag = tag.Issue() + ' ' + data;
    socket->write(dataWithTag + "\r\n");
    tag.Advance();

    // emit signal.
  }
}

/* * * * * * * * * * * * * * * * * * Slots * * * * * * * * * * * * * * * * * */
void ImapManager::ConnectSlots()
{
  /* connected */
  QObject::connect(socket, SIGNAL(onnected()),
          this, SLOT(SocketConnected()));

  /* disconnected */
  QObject::connect(socket, SIGNAL(disconnected()),
          this, SLOT(SocketDisconnected()));

  /* state changed */
  QObject::connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
          this, SLOT(SocketStateChanged(QAbstractSocket::SocketState)));

  /* encrypted */
  QObject::connect(socket, SIGNAL(encrypted()),
          this, SLOT(socketEncrypted()));

  /* ssl errors */
  QObject::connect(socket, SIGNAL(sslErrors(QList<QSslError>)),
          this, SLOT(SslErrors(QList<QSslError>)));

  /* ready read */
  QObject::connect(socket, SIGNAL(readyRead()),
          this, SLOT(SocketReadyRead()));
}

void ImapManager::DisconnectSlots()
{
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

  /* disconnected */
  QObject::disconnect(socket, SIGNAL(disconnected()),
          this, SLOT(socketDisconnected()));
}

void ImapManager::SocketConnected()
{
  qDebug() << "Connected to host";
}

void ImapManager::SocketDisconnected()
{
  qDebug() << "Disconnected from host";
}

void
  ImapManager::SocketStateChanged(
    QAbstractSocket::SocketState state)
{
  qDebug() << "Socket state has been changed:" << state;
}

void ImapManager::SocketEncrypted()
{
  if (socket) {
    QSslCipher cipher = socket->sessionCipher();
    QString cipherText
        = QString("%1, %2 (%3/%4)").arg(cipher.authenticationMethod())
        .arg(cipher.name()).arg(cipher.usedBits()).arg(cipher.supportedBits());

    qDebug() << "Using cipher:" << cipherText.toStdString().c_str();
  }
}

void ImapManager::SocketReadyRead()
{
  // parse response.
}

void ImapManager::SslErrors(const QList<QSslError> &errors)
{
  qDebug() << "SSL error: ";
  for(QList<QSslError>::const_iterator it = errors.cbegin();
      it != errors.cend();
      ++it) {
    qDebug() << "\t" << *it;
  }
  qDebug() << "";
}

}
