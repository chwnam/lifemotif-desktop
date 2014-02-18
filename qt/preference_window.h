#ifndef PREFERENCE_WINDOW_H
#define PREFERENCE_WINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE

namespace Ui {
class PreferenceWindow;
}

class PreferenceWindow : public QDialog
{
  Q_OBJECT

public:
  explicit PreferenceWindow(
      QWidget *parent = 0);

  ~PreferenceWindow();

  void UpdateUI();
  void ReadFromSettings();

private slots:
  void on_refreshSettingButton_clicked();

private:
  Ui::PreferenceWindow *ui;
};

QT_END_NAMESPACE

#endif // PREFERENCEWINDOW_H
