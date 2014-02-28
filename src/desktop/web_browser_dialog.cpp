#include <QDebug>

#include "web_browser_dialog.h"
#include "ui_web_browser_dialog.h"

WebBrowserDialog::WebBrowserDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::WebBrowserDialog)
{
  ui->setupUi(this);
}

WebBrowserDialog::~WebBrowserDialog()
{
  delete ui;
}

int WebBrowserDialog::exec()
{
  ui->webView->load(authorizationUrl);
  return QDialog::exec();
}

void WebBrowserDialog::ExtractAuthorizationCodeFromTitleTag()
{
  const QStringList titleSplitted = ui->webView->title().split(QChar(' '));

  if (titleSplitted.size() == 2) {

    const QString status = titleSplitted[0];
    const int     pos    = titleSplitted[1].indexOf(QChar('=')) + 1;
    const QString code   = titleSplitted[1].mid(pos);

    authorizationStatus = status.toUtf8();
    authorizationCode   = code.toUtf8();
  }
}

void WebBrowserDialog::on_webView_loadFinished(bool /*arg1*/)
{
  // when successful,
  //  - path: /o/oauth2/approval
  //  - title: Success code=<code>

  const QString  successPath = QString("/o/oauth2/approval");

  if (ui->webView->url().path() == successPath) {
    ExtractAuthorizationCodeFromTitleTag();
    done(QDialog::Accepted);
  }
}
