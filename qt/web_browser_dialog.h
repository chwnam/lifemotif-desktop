#ifndef WEB_BROWSER_DIALOG_H
#define WEB_BROWSER_DIALOG_H

#include <QDialog>

#include "lifemotif_settings.h"
#include "python_wrapper/python_wrapper.h"

namespace Ui {
class WebBrowserDialog;
}

class WebBrowserDialog : public QDialog
{
  Q_OBJECT

public:
  explicit WebBrowserDialog(
      GoogleOauth2Wrapper* _oauth2Wrapper,
      QWidget *parent = 0);
  ~WebBrowserDialog();

private slots:
  void on_webView_loadFinished(bool arg1);

private:
  void Init();

private:
  Ui::WebBrowserDialog *ui;
  GoogleOauth2Wrapper* oauth2Wrapper;
};

#endif // WEB_BROWSER_DIALOG_H
