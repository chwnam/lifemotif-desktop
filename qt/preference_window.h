#ifndef PREFERENCE_WINDOW_H
#define PREFERENCE_WINDOW_H

#include <QDialog>
#include <QStringList>
#include <mailbox_list_extract.h>

#include "python_wrapper/googleimapwrapper.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class PreferenceWindow;
}

class PreferenceWindow : public QDialog
{
  Q_OBJECT

public:
  explicit PreferenceWindow(
      GoogleImapWrapper *_imap,
      QWidget *parent = 0);

  ~PreferenceWindow();

  void UpdateUI();
  void ReadFromSettings();

private slots:
  void on_refreshSettingButton_clicked();

  void on_cleanDiskCacheButton_clicked();

private:
  void WipeCacheEntries();
  void GetMailboxList();
  void UpdateMailboxComboBox();

private:
  Ui::PreferenceWindow *ui;
  GoogleImapWrapper    *imap;
  QStringList          mailboxList;
};

QT_END_NAMESPACE

#endif // PREFERENCEWINDOW_H
