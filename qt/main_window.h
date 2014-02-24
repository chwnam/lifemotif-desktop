#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QSharedPointer>

#include "./python_wrapper/python_wrapper.h"

#include "email_cache.h"
#include "lifemotif_diary.h"
#include "lifemotif_settings.h"
#include "lifemotif_utils.h"
#include "loading_dialog.h"
#include "message_types.h"
#include "mime_raw_message_dialog.h"
#include "mime_structure_dialog.h"

namespace Ui {
class MainWindow;
}

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

     // authentication by console (CLI)
     void on_actionConsoleAuthentication_triggered();

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

private:
    void AuthenticateOnConsoleByPython();
    void AuthenticateOnConsole();
    void AuthenticateUsingWebBrowser();
    void BuildLocalStructre();
    void ClearDiaryInformationUI();
    void LoadLocalStructure();
    void ParseMessage(const std::string& rawMessage);
    void RevokeAuthentication();
    void UpdateCalendar();
    void UpdateDiaryInformationUI();
    void UpdateMenu();
    void ShowDiary(const int entry);

    QString  FetchMessage(const MsgIdType& id);
    DateType GetDateFromCalendar() const;

private:
    Ui::MainWindow *ui;

    // wrappers
    typedef QSharedPointer<GoogleOauth2Wrapper> GoogleOauth2WrapperPtr;
    typedef QSharedPointer<GoogleImapWrapper>   GoogleImapWrapperPtr;

    GoogleOauth2WrapperPtr _oauth2Wrapper;
    GoogleImapWrapperPtr   _imapWrapper;

    // email cache
    typedef QSharedPointer<EmailCache> EmailCachePtr;
    EmailCachePtr _emailCache;

    // MIME raw message dialog
    typedef QSharedPointer<MimeRawMessageDialog> MimeRawMessageDialogPtr;
    MimeRawMessageDialogPtr _mimeRawMessageDialog;

    // MIME structure dialog
    typedef QSharedPointer<MimeStructureDialog> MimeStructureDialogPtr;
    MimeStructureDialogPtr _mimeStructureDialog;

    // late type binding.
    // GoogleOauth2Wrapper /////////////////////////////////////////////////
    inline GoogleOauth2WrapperPtr& oauth2Wrapper() {
      if (_oauth2Wrapper == NULL) {
        _oauth2Wrapper
            = GoogleOauth2WrapperPtr(LifeMotifUtils::CreateOauth2Wrapper());
      }
      return _oauth2Wrapper;
    }
    inline const GoogleOauth2WrapperPtr& oauth2Wrapper() const {
      return oauth2Wrapper();
    }

    // GoogleImapWrapper ///////////////////////////////////////////////////
    inline GoogleImapWrapperPtr& imapWrapper() {
      if (_imapWrapper == NULL) {
        _imapWrapper
            = GoogleImapWrapperPtr(
              LifeMotifUtils::CreateImapWrapper(oauth2Wrapper().data()));
      }
      return _imapWrapper;
    }
    inline const GoogleImapWrapperPtr& imapWrapper() const {
      return imapWrapper();
    }

    // EmailChache ////////////////////////////////////////////////////////
    inline EmailCachePtr& emailCache() {
      if (_emailCache == NULL) {
        _emailCache
            = EmailCachePtr(new EmailCache(LifeMotifSettings::CacheDir()));
      }
      return _emailCache;
    }
    inline const EmailCachePtr& emailCache() const {
      return emailCache();
    }

    // MimeRawMessageDialog ///////////////////////////////////////////////
    inline MimeRawMessageDialogPtr& mimeRawMessageDialog() {
      if (_mimeRawMessageDialog == NULL) {
        _mimeRawMessageDialog
            = MimeRawMessageDialogPtr(new MimeRawMessageDialog());
        _mimeRawMessageDialog->setWindowModality(Qt::NonModal);
      }
      return _mimeRawMessageDialog;
    }
    inline const MimeRawMessageDialogPtr& mimeRawMessageDialog() const {
      return mimeRawMessageDialog();
    }

    // MimeStructureDialog /////////////////////////////////////////////////////
    inline MimeStructureDialogPtr& mimeStructureDialog() {
      if (_mimeStructureDialog == NULL) {
        _mimeStructureDialog
            = MimeStructureDialogPtr(new MimeStructureDialog());
        _mimeStructureDialog->setWindowModality(Qt::NonModal);
      }
      return _mimeStructureDialog;
    }
    inline const MimeStructureDialogPtr& mimeStructureDialog() const {
      return mimeStructureDialog();
    }

    // local structure
    LocalStructureType localStructure;

    // parsed email becomes diary
    typedef QSharedPointer<LifeMotifDiary> LifeMotifDiaryPtr;
    LifeMotifDiaryPtr diary;
};

#endif // MAIN_WINDOW_H
