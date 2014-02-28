#ifndef WEB_BROWSER_DIALOG_H
#define WEB_BROWSER_DIALOG_H

#include <QDialog>
#include <QUrl>
#include <QByteArray>

namespace Ui {
class WebBrowserDialog;
}

class WebBrowserDialog : public QDialog
{
  Q_OBJECT

public:
  explicit WebBrowserDialog(QWidget *parent = 0);
  ~WebBrowserDialog();

  const QByteArray& AuthorizationStatus() const { return authorizationStatus; }
  const QByteArray& AuthorizationCode() const   { return authorizationCode; }
  void SetAuthorizationUrl(const QUrl& url)     { authorizationUrl = url; }
  int  exec();

private slots:
  void on_webView_loadFinished(bool arg1);

private:
  void ExtractAuthorizationCodeFromTitleTag();

private:
  Ui::WebBrowserDialog *ui;  
  QByteArray            authorizationStatus;
  QByteArray            authorizationCode;
  QUrl                  authorizationUrl;
};

#endif // WEB_BROWSER_DIALOG_H
