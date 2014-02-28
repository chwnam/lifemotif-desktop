#ifndef PREFERENCE_WINDOW_H
#define PREFERENCE_WINDOW_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class PreferenceWindow;
}

class PreferenceWindow : public QDialog
{
  Q_OBJECT

public:
  explicit PreferenceWindow(QWidget *parent = 0);

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
  QStringList          mailboxList;
};

#endif // PREFERENCEWINDOW_H
