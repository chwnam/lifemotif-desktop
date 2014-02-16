#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "./python_wrapper/python_wrapper.h"

#include "email_cache.h"
#include "lifemotif_diary.h"
#include "lifemotif_settings.h"
#include "lifemotif_utils.h"
#include "message_types.h"


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
    // when the option menu is clicked
     void on_actionOptions_triggered();

     // when the calendar widget is clicked
     void OnCalendarWidgetClicked(const QDate &date);

     // when the diary list widget is clicked
     void on_diaryList_clicked(const QModelIndex &index);

     // build local structure menu
     void on_actionBuild_Local_Structure_triggered();

     // authentication by webbrowser (GUI)
     void on_actionBrowserAuthentication_triggered();

     // authentication by console (CLI)
     void on_actionConsoleAuthentication_triggered();

private:
    void AuthenticateOnConsole();
    void AuthenticateUsingWebBrowser();
    void BuildLocalStructre();
    void LoadLocalStructure();
    void ParseMessage(const std::string& rawMessage);
    void UpdateCalendar();
    void UpdateDiaryInformationUI();

    QString FetchMessage(const MsgIdType& id);
    bool HasCredentials();

private:
    Ui::MainWindow *ui;

    // wrappers
    GoogleOauth2WrapperPtr _oauth2Wrapper;
    GoogleImapWrapperPtr _imapWrapper;

    // email cache
    typedef boost::shared_ptr<EmailCache> EmailCachePtr;
    EmailCachePtr _emailCache;

    // late type binding.
    inline GoogleOauth2WrapperPtr& oauth2Wrapper() {
      if (_oauth2Wrapper == NULL) {
        _oauth2Wrapper = LifeMotifUtils::CreateOauth2Wrapper();
      }
      return _oauth2Wrapper;
    }
    inline const GoogleOauth2WrapperPtr& oauth2Wrapper() const {
      return oauth2Wrapper();
    }

    inline GoogleImapWrapperPtr& imapWrapper() {
      if (_imapWrapper == NULL) {
        _imapWrapper = LifeMotifUtils::CreateImapWrapper(oauth2Wrapper());
      }
      return _imapWrapper;
    }
    inline const GoogleImapWrapperPtr& imapWrapper() const {
      return imapWrapper();
    }

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

    // local structure
    LocalStructureType localStructure;

    // parsed email becomes diary
    typedef boost::shared_ptr<LifeMotifDiary> LifeMotifDiaryPtr;
    LifeMotifDiaryPtr diary;
};

#endif // MAINWINDOW_H
