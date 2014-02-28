#ifndef IMAP_MANAGER_H
#define IMAP_MANAGER_H

#include <QObject>
#include <QSslSocket>
#include <QString>

#include "imap_tag.h"

namespace LifeMotif {

class ImapManager : public QObject
{
  Q_OBJECT

public:
  explicit ImapManager(QObject *parent = 0);

  void Connect(const QString& host, const quint16 port, const bool useSsl);
  void Disconnect();

  bool
    AuthenticateXAuth2(
      const QString& emailAddress,
      const QString& accessToken);

signals:

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
  void Send(const QByteArray& data);

private:
  QSslSocket *socket;
  ImapTag     tag;
};

}

#endif // IMAP_MANAGER_H
