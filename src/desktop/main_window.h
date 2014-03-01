#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QSharedPointer>

#include "diary.h"
#include "email_cache.h"
#include "google_oauth2.h"
#include "imap_console_dialog.h"
#include "imap_manager.h"
#include "message_types.h"
#include "mime_raw_message_dialog.h"
#include "mime_structure_dialog.h"
#include "settings.h"
#include "utils.h"


namespace Ui {
class MainWindow;
}

using namespace LifeMotif;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
     // when the preference menu is clicked
     void on_actionPreference_triggered();

     // when the calendar widget is clicked
     void on_calendarWidget_clicked(const QDate &date);

     // when the diary list widget is clicked
     void on_diaryList_clicked(const QModelIndex &index);

     // build local structure menu
     void on_actionBuildLocalStructure_triggered();

     // authentication by webbrowser (GUI)
     void on_actionBrowserAuthentication_triggered();

     // show MIME raw message
     void on_mimeRawMessageButton_clicked();

     void on_attatchmentSaveAsButton_clicked();

     void on_mimeStructureButton_clicked();

     void on_actionExit_triggered();

     void on_clearTextButton_clicked();

     void on_showHtmlCodeCheckBox_clicked();

     void on_revokeAuthentication_triggered();

     void on_actionAbout_triggered();

     void on_actionQt_triggered();

     void on_OpenImapConsole_clicked();

     void on_ImapAuthenticateButton_clicked();

private:
    void Authorize();
    void BuildLocalStructre();
    void ClearDiaryInformationUI();
    void LoadLocalStructure();
    void ParseMessage(const std::string& rawMessage);
    void RevokeAuthentication();
    void UpdateCalendar();
    void UpdateDiaryInformationUI();
    void UpdateMenu();
    void ShowDiary(const int entry);
    void OpenImapConsole();
    void ImapAuthenticate();

    QString  FetchMessage(const MsgIdType& id);
    DateType GetDateFromCalendar() const;

private:
    Ui::MainWindow *ui;

    // Application settings
    Settings* _settings;
    inline Settings* settings() {
      if (_settings == NULL) _settings = new Settings(this);
      return _settings; }

    // LifeMotif Google OAuth2
    GoogleOAuth2* _oauth2;
    inline GoogleOAuth2* oauth2() {
      if (_oauth2 == NULL) _oauth2 = new GoogleOAuth2(this);
      return _oauth2; }

    ImapManager *_imapManager;
    inline ImapManager* imapManager() {
      if (_imapManager == NULL) _imapManager = new ImapManager(this);
      return _imapManager; }

    ImapConsoleDialog* _imapconsoleDialog;
    inline ImapConsoleDialog* imapConsoleDialog() {
      if (_imapconsoleDialog == NULL) _imapconsoleDialog = new ImapConsoleDialog(this);
      return _imapconsoleDialog; }

    // email cache
    typedef QSharedPointer<EmailCache> EmailCachePtr;
    EmailCachePtr _emailCache;
    inline EmailCachePtr& emailCache() {
      if (_emailCache == NULL) {
        _emailCache
          = EmailCachePtr(new EmailCache(settings()->CacheDir())); }
      return _emailCache; }

    // MIME raw message dialog
    typedef QSharedPointer<MimeRawMessageDialog> MimeRawMessageDialogPtr;
    MimeRawMessageDialogPtr _mimeRawMessageDialog;
    inline MimeRawMessageDialogPtr& mimeRawMessageDialog() {
      if (_mimeRawMessageDialog == NULL) {
        _mimeRawMessageDialog
          = MimeRawMessageDialogPtr(new MimeRawMessageDialog());
        _mimeRawMessageDialog->setWindowModality(Qt::NonModal); }
      return _mimeRawMessageDialog; }

    // MIME structure dialog
    typedef QSharedPointer<MimeStructureDialog> MimeStructureDialogPtr;
    MimeStructureDialogPtr _mimeStructureDialog;
    inline MimeStructureDialogPtr& mimeStructureDialog() {
      if (_mimeStructureDialog == NULL) {
        _mimeStructureDialog
          = MimeStructureDialogPtr(new MimeStructureDialog());
        _mimeStructureDialog->setWindowModality(Qt::NonModal); }
      return _mimeStructureDialog; }

    // local structure
    GoogleLocalStructureType localStructure;

    // parsed email becomes diary
    typedef QSharedPointer<Diary> DiaryPtr;
    DiaryPtr diary;
};

#endif // MAIN_WINDOW_H
