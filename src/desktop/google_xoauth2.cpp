#include "google_xoauth2.h"

#include <QDebug>
#include <QJsonDocument>

namespace LifeMotif {

GoogleXOAuth2::GoogleXOAuth2(
  ImapManager* _imapManager,
  QObject* parent /* = 0 */)
  : imapManager(_imapManager), QObject(parent)
{
  ConnectImapSignals();
}

GoogleXOAuth2::~GoogleXOAuth2()
{
  DisconnectImapSignals();
}

ImapManager* GoogleXOAuth2::Imap() 
{
  return imapManager;
}

bool GoogleXOAuth2::Authorize(
  const QString& emailAddress, const QByteArray& accessToken) 
{
  // check server capability
  // assure that the server supports XOAUTH2
  responseFunction = &GoogleXOAuth2::CheckCapability;
  imapManager->SendCommand("CAPABILITY");

  // server does not support XOAUTH2.
  if (ok == false) { return false; }

  // proceed authorization
  responseFunction = &GoogleXOAuth2::CheckAuthResponse;
  const QByteArray authCmd
    = QByteArray("AUTHENTICATE XOAUTH2 ") +
      XOAuth2String(emailAddress, accessToken);
  imapManager->SendCommand(authCmd);

  return ok;
}

QByteArray GoogleXOAuth2::XOAuth2String(
  const QString& emailAddress, const QByteArray& accessToken)
{
  QByteArray authString;
  const char a = 0x1;

  authString += "user=" + emailAddress.toUtf8() + a;
  authString += "auth=Bearer ";
  authString += accessToken + a + a;

  return authString.toBase64();
}

void GoogleXOAuth2::CheckCapability(const QByteArray& capability)
{
  qDebug() << "S:" << capability;
  
  /* response example:

  S: * CAPABILITY IMAP4rev1 UNSELECT IDLE NAMESPACE QUOTA ID XLIST CHILDREN
     X-GM-EXT-1 XYZZY SASL-IR AUTH=XOAUTH AUTH=XOAUTH2 AUTH=PLAIN
     AUTH=PLAIN-CLIENTTOKEN
     A0 OK Thats all she wrote! cv10mb19402931pac
  */

  ok = false;

  QStringList lines = QString(capability).split("\r\n");
  if (lines.count() < 2) return;

  QStringList rsplit = lines[1].split(QChar(' '));
  if (rsplit[1] != QString("OK")) ok = false;
  
  QStringList csplit = lines[0].split(QChar(' ')); 
  for(QStringList::const_iterator
      it = csplit.cbegin(); it != csplit.cend(); ++it) {

    if (it->startsWith(QString("AUTH="))) {
      QStringList kv = it->split(QChar('='));
      if (kv.size() == 2 && kv[1] == QString("XOAUTH2")) {
        ok = true;
        return;
      }
    }
  }  
}

void GoogleXOAuth2::CheckAuthResponse(const QByteArray& authResponse)
{
  qDebug() << "S:" << authResponse;

  QByteArray::const_iterator it = authResponse.cbegin();
  while(*it == QChar('+') || *it == QChar(' ')) ++it;

  const QJsonDocument json
    = QJsonDocument::fromJson(
        QByteArray::fromBase64(
          authResponse.mid(it - authResponse.cbegin())));

  // check the response is 200
  /* sample :
  {"status":"400","schemes":"Bearer","scope":"https://mail.google.com/"}
  */

  ok = false;
  if (json.isObject()) {
    const QVariantMap& response = json.toVariant().toMap();

    qDebug() << response;
    if (response[QString("status")].toInt() == 200) {
      ok = true;
    }
  }   
}

void GoogleXOAuth2::ConnectImapSignals()
{
  if (imapManager) {
    QObject::connect(
      imapManager, SIGNAL(dataReceived(const QByteArray&)),
      this, SLOT(Response(const QByteArray&)));
    QObject::connect(
      imapManager, SIGNAL(clientRequest(const QByteArray&)),
      this, SLOT(Request(const QByteArray&)));
  }
}

void GoogleXOAuth2::DisconnectImapSignals()
{
  if (imapManager) {
    QObject::disconnect(
      imapManager, SIGNAL(dataReceived(const QByteArray&)),
      this, SLOT(Response(const QByteArray&)));
    QObject::disconnect(
      imapManager, SIGNAL(clientRequest(const QByteArray&)),
      this, SLOT(Request(const QByteArray&)));
  }
}

void GoogleXOAuth2::Request(const QByteArray& request)
{
  qDebug() << "C:" << request;
}

void GoogleXOAuth2::Response(const QByteArray& response)
{
  if (responseFunction) {
    (this->*responseFunction)(response);
  }
}


}