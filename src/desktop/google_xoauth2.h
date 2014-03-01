#ifndef LIFEMOTIF_GOOGLE_XOAUTH2_H
#define LIFEMOTIF_GOOGLE_XOAUTH2_H

#include <QObject>

#include "imap_manager.h"


namespace LifeMotif {

class GoogleXOAuth2 : public QObject
{
  Q_OBJECT

public:
  explicit GoogleXOAuth2(
    ImapManager* _imapManager,
    QObject* parent = 0);
  ~GoogleXOAuth2();

signals:
  void authorized();

private slots:
  void Request(const QByteArray& request);
  void Response(const QByteArray& response);

public:
  ImapManager* Imap();

  bool
  Authorize(const QString& emailAddress, const QByteArray& accessToken);

private:
  QByteArray
  XOAuth2String(const QString& emailAddress, const QByteArray& accessToken);

  void CheckCapability(const QByteArray& capability);
  void CheckAuthResponse(const QByteArray& authResponse);

  void ConnectImapSignals();
  void DisconnectImapSignals();

private:
  ImapManager* imapManager;
  bool         ok;

  typedef void (GoogleXOAuth2::*ResponseFunction) (const QByteArray&);
  ResponseFunction responseFunction;
};

}

#endif //LIFEMOTIF_GOOGLE_XOAUTH2_H