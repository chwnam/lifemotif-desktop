#include "imap_manager.h"

#include <QDebug>
#include <QByteArray>
#include <QSslCipher>

#include "utils.h"

namespace LifeMotif {

ImapManager::ImapManager(QObject *parent) :
  QObject(parent),
  socket(0)
{
}

ImapManager::~ImapManager()
{
  if (socket) {
    DisconnectSlots();
  }
}

bool ImapManager::Connect(
  const QString& host,
  const quint16  port,
  const bool     useSsl)
{
  if (socket == NULL) {
    socket = new QSslSocket(this);
    ConnectSlots();
  }

  if (useSsl) {
    socket->connectToHostEncrypted(host, port);
    if (socket->waitForConnected() == false) return false;
    if (socket->waitForEncrypted() == false) return false;
  } else {
    socket->connectToHost(host, port);
    if (socket->waitForConnected() == false) return false;
  }

  return true;
}

void ImapManager::Disconnect()
{
  if (socket && socket->state() == QAbstractSocket::ConnectedState) {
    socket->disconnectFromHost();
  }
}

void ImapManager::SendCommand(const QByteArray& data)
{
  if (socket) {
    // append tag
    const QByteArray request = tag.Issue() + ' ' + data;

    emit clientRequest(request);
    socket->write(request + "\r\n");
    tag.Advance();
    socket->waitForReadyRead();
  }
}

//void ImapManager::ResponseReady(const QByteArray& response)
//{
//  qDebug() << __FUNCTION__ << ">>" << response;
//}
//
//void ImapManager::ResponseCapability(const QByteArray& response)
//{
//  qDebug() << __FUNCTION__ << ">>" << response;
//}
//
//void ImapManager::ResponseXOAuth2(const QByteArray& response)
//{
//  qDebug() << __FUNCTION__ << ">>" << response;
//}


/* * * * * * * * * * * * * * * * * * Slots * * * * * * * * * * * * * * * * * */
void ImapManager::ConnectSlots()
{
  /* connected */
  QObject::connect(socket, SIGNAL(connected()),
          this, SLOT(SocketConnected()));

  /* disconnected */
  QObject::connect(socket, SIGNAL(disconnected()),
          this, SLOT(SocketDisconnected()));

  /* state changed */
  QObject::connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
          this, SLOT(SocketStateChanged(QAbstractSocket::SocketState)));

  /* encrypted */
  QObject::connect(socket, SIGNAL(encrypted()),
          this, SLOT(SocketEncrypted()));

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
          this, SLOT(SocketConnected()));

  /* state changed */
  QObject::disconnect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
          this, SLOT(SocketStateChanged(QAbstractSocket::SocketState)));

  /* encrypted */
  QObject::disconnect(socket, SIGNAL(encrypted()),
          this, SLOT(SocketEncrypted()));

  /* ssl errors */
  QObject::disconnect(socket, SIGNAL(sslErrors(QList<QSslError>)),
          this, SLOT(SslErrors(QList<QSslError>)));

  /* ready read */
  QObject::disconnect(socket, SIGNAL(readyRead()),
          this, SLOT(SocketReadyRead()));

  /* disconnected */
  QObject::disconnect(socket, SIGNAL(disconnected()),
          this, SLOT(SocketDisconnected()));
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
  if (socket) {
    emit dataReceived(socket->readAll());
  }
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
