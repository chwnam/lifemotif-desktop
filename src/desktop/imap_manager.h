#ifndef IMAP_MANAGER_H
#define IMAP_MANAGER_H

#include <QObject>
#include <QSslSocket>
#include <QString>
#include <QByteArray>

#include "imap_tag.h"


namespace LifeMotif {

class ImapManager : public QObject
{
  Q_OBJECT

public:
  explicit ImapManager(QObject *parent = 0);
  ~ImapManager();

  bool Connect(const QString& host, const quint16 port, bool useSsl = true);
  void Disconnect();
  void SendCommand(const QByteArray& data);

  //bool AuthenticateXOAuth2(
  //    const QString& emailAddress,
  //    const QByteArray& accessToken);

signals:
  void dataReceived(const QByteArray&);
  void clientRequest(const QByteArray&);

private slots:
  void SocketConnected();
  void SocketDisconnected();
  void SocketStateChanged(QAbstractSocket::SocketState state);
  void SocketEncrypted();
  void SocketReadyRead();
  void SslErrors(const QList<QSslError> &errors);

private:
  void ConnectSlots();
  void DisconnectSlots(); 

  //QByteArray XOAuth2String(
  //  const QString&    emailAddress,
  //  const QByteArray& accessToken);

  /* response functions */
  //void ResponseReady(const QByteArray& response);
  //void ResponseCapability(const QByteArray& response);
  //void ResponseXOAuth2(const QByteArray& response);

private:
  QSslSocket *socket;
  ImapTag     tag;
  

  //typedef void (ImapManager::*ResponseFunction) (const QByteArray& data);
  //ResponseFunction responseFunction;
};

}

#endif // IMAP_MANAGER_H
