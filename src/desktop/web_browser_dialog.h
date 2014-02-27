#ifndef WEB_BROWSER_DIALOG_H
#define WEB_BROWSER_DIALOG_H

#include <QDialog>
#include <QUrl>

namespace Ui {
class WebBrowserDialog;
}

class WebBrowserDialog : public QDialog
{
  Q_OBJECT

public:
  explicit WebBrowserDialog(QWidget *parent = 0);
  ~WebBrowserDialog();

  const QString& AuthorizationStatus() const { return authorizationStatus; }
  const QString& AuthorizationCode() const   { return authorizationCode; }
  void SetAuthorizationUrl(const QUrl& url)  { authorizationUrl = url; }
  int  exec();

private slots:
  void on_webView_loadFinished(bool arg1);

private:
  void ExtractAuthorizationCodeFromTitleTag();

private:
  Ui::WebBrowserDialog *ui;  
  QString               authorizationStatus;
  QString               authorizationCode;
  QUrl                  authorizationUrl;
};

#endif // WEB_BROWSER_DIALOG_H
