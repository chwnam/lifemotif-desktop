#ifndef BASE_IMAP_H
#define BASE_IMAP_H

#include <QObject>
#include <QSslSocket>
#include <QString>

#include "imap_tag.h"

class LifeMotifImap : public QObject
{
  Q_OBJECT

public:
  explicit LifeMotifImap(QObject *parent = 0);

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

#endif // LIFEMOTIF_IMAP_H
