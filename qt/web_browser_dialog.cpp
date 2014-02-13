#include <QDebug>
#include <QUrl>

#include "lifemotif_config.h"
#include "web_browser_dialog.h"
#include "ui_web_browser_dialog.h"

WebBrowserDialog::WebBrowserDialog(
    QWidget *parent,
    GoogleOauth2WrapperPtr& _oauth2Wrapper) :
  QDialog(parent),
  oauth2Wrapper(_oauth2Wrapper),
  ui(new Ui::WebBrowserDialog)
{
  ui->setupUi(this);
  Init();
}

WebBrowserDialog::~WebBrowserDialog()
{
  delete ui;
}

void WebBrowserDialog::Init()
{
  QString secretPath = LifeMotifSettings::SecretPath(true);
  QString url
      = QString::fromStdString(
        oauth2Wrapper->GetAuthorizationURL(secretPath.toStdString()));

  ui->webView->load(QUrl(url));
}

void WebBrowserDialog::on_webView_loadFinished(bool /*arg1*/)
{
  // when successful,
  //  - path: /o/oauth2/approval
  //  - title: Success code=<code>

  QStringList splitted = ui->webView->title().split(QChar(' '));
  int result = QDialog::Rejected;

  if (ui->webView->url().path() == QString(LIFEMOTIF_GOOGLE_OAUTH_SUCCESS_PATH) &&
      splitted.size() == 2) {

      QString status = splitted[0];
      int     pos    = splitted[1].indexOf(QChar('=')) + 1;
      QString code   = splitted[1].mid(pos);

      qDebug() << "Status:" << status << "\tCode:" << code;

      bp::object
          credentials = oauth2Wrapper->MakeCredentials(code.toStdString());

      // keep this credentials
      QString storageName = LifeMotifSettings::StorageName(true);
      oauth2Wrapper->SetCredentials(storageName.toStdString(), credentials);

      qDebug() << "Successfully authorized.";

      result = QDialog::Accepted;

      // set returnCode and close the window
      done(result);
  }
}
