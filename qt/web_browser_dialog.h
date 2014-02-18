#ifndef WEB_BROWSER_DIALOG_H
#define WEB_BROWSER_DIALOG_H

#include <QDialog>

#include "lifemotif_settings.h"
#include "lifemotif_utils.h"

namespace Ui {
class WebBrowserDialog;
}

class WebBrowserDialog : public QDialog
{
  Q_OBJECT

public:
  explicit WebBrowserDialog(
      GoogleOauth2WrapperPtr& _oauth2Wrapper,
      QWidget *parent = 0);
      ;
  ~WebBrowserDialog();

private slots:
  void on_webView_loadFinished(bool arg1);

private:
  void Init();

private:
  Ui::WebBrowserDialog *ui;
  GoogleOauth2WrapperPtr& oauth2Wrapper;
};

#endif // WEB_BROWSER_DIALOG_H
