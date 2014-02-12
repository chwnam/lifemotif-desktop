#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preferencewindow.h"

#include <QMessageBox>
#include <iostream>
#include <QVector>

#include "lifemotif_settings.h"
#include "lifemotif_config.h"
#include "lifemotif_path_helper.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitWrappers();
    LoadLocalStructure();
    UpdateCalendar();

    QString emailCacheDir = LifeMotifSettings::CacheDir();
    emailCache = boost::shared_ptr<EmailCache>(
          new EmailCache(emailCacheDir));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitWrappers()
{
  // wrapper initialization
  oauth2Wrapper = boost::shared_ptr<GoogleOauth2Wrapper>(
        new GoogleOauth2Wrapper(
        LIFEMOTIF_GOOGLE_OAUTH2_WRAPPER_MODULE,
        LIFEMOTIF_GOOGLE_OAUTH2_WRAPPER_CLASS));


  std::string tempStorageName;
  std::string scriptPath, storageName, emailAddress;
  int debugLevel;

  scriptPath = LifeMotifSettings::PythonScriptPath().toStdString();
  tempStorageName = LifeMotifSettings::StorageName().toStdString();

  storageName = LifeMotifPathHelper::Join(
                  2, scriptPath.c_str(), tempStorageName.c_str());

  emailAddress = LifeMotifSettings::EmailAddress().toStdString();

  debugLevel = LifeMotifSettings::DebugLevel();

  bp::object imapObject
      = oauth2Wrapper->ImapAuthenticate(storageName, emailAddress, debugLevel);

  imapWrapper = boost::shared_ptr<GoogleImapWrapper>(
        new GoogleImapWrapper(
        LIFEMOTIF_GOOGLE_IMAP_WRAPPER_MODULE,
        LIFEMOTIF_GOOGLE_IMAP_WRAPPER_CLASS,
        imapObject));
}

void MainWindow::LoadLocalStructure()
{
  std::string tmpls = LifeMotifSettings::LocalStructure().toStdString();
  std::string scriptPath = LifeMotifSettings::PythonScriptPath().toStdString();
  std::string lsPath;

  lsPath = LifeMotifPathHelper::Join(
        2, scriptPath.c_str(), tmpls.c_str());

  bp::object obj = LocalStructureWrapper(
                    LIFEMOTIF_LOCAL_STRUCTURE_WRAPPER_MODULE,
                    LIFEMOTIF_LOCAL_STRUCTURE_WRAPPER_CLASS).Load(
                      lsPath);

  LocalStructureExtract(obj, localStructure);
}

void MainWindow::UpdateCalendar()
{
  // sorted list of date
  QVector<QString> dateVector;
  dateVector.reserve(localStructure.size());
  for(LocalStructureType::iterator it = localStructure.begin();
      it != localStructure.end(); ++it) {
    dateVector.push_back(QString::fromStdString(it->first));
  }
  qSort(dateVector);

  if (dateVector.empty() == false) {

    QDate minDate, maxDate;

    minDate = QDate::fromString(*(dateVector.begin()), "yyyyMMdd");
    maxDate = QDate::fromString(*(dateVector.end()-1), "yyyyMMdd");

    ui->calendarWidget->setMinimumDate(minDate);
    ui->calendarWidget->setMaximumDate(maxDate);
    ui->calendarWidget->setEnabled(true);
  } else {
    ui->calendarWidget->setEnabled(false);
  }

  for(QVector<QString>::iterator it = dateVector.begin();
      it != dateVector.end(); ++it) {

    QDate d = QDate::fromString(*it, "yyyyMMdd");
    QTextCharFormat charFormat = ui->calendarWidget->dateTextFormat(d);

    charFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    charFormat.setFontWeight(QFont::Bold);

    ui->calendarWidget->setDateTextFormat(d, charFormat);
  }
}

void MainWindow::on_activateButton_clicked()
{
    QMessageBox::information(this, "Hello!", "Activated!", QMessageBox::Ok);
}

void MainWindow::on_authenticateButton_clicked()
{
    QMessageBox::information(this, "Authenticate", "Authentication", QMessageBox::Ok);
}

void MainWindow::on_actionOptions_triggered()
{
  // show preference modal dialog
  PreferenceWindow pref(this);

  // some tuning can be here ... center of the main window...
  int result = pref.exec();
  if (result == QDialog::Accepted) {
    std::cout << "User Accepted\n";
  } else if (result == QDialog::Rejected) {
    std::cout << "User canceled\n";
  }
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    DateType datestring = date.toString("yyyyMMdd").toStdString();
    const MessageGroup& group = localStructure[datestring];

    QListWidget& list = *ui->diaryList;
    list.clear();

    for(std::size_t i = 0; i < group.messageIds.size(); ++i) {
      list.addItem(QString::number(group.messageIds[i]));
    }
}

void MainWindow::on_diaryList_clicked(const QModelIndex &index)
{
  QCalendarWidget& cal = *ui->calendarWidget;
  QDate d = cal.selectedDate();
  DateType ds = d.toString("yyyyMMdd").toStdString();

  const MessageGroup& group = localStructure[ds];
  MsgIdType msgId = group.messageIds[index.row()];

  // query cache
  std::string rawMessage;
  if (emailCache->HasCache(msgId) == false) {
    std::cout << msgId << " not found from cache...";
    std::cout << " build new file\n";
    std::string label = LifeMotifSettings::Label().toStdString();
    rawMessage = imapWrapper->FetchMail(label, msgId);
    emailCache->SetCache(msgId, rawMessage);
  } else {
    std::cout << "cache hit: " << msgId << "\n";
    rawMessage = emailCache->GetCache(msgId);
  }

  // parse email message
  ParseMessage(rawMessage);

  // display message
  QString qrawMessage = QString::fromStdString(rawMessage);
  QPlainTextEdit& text = *ui->diaryText;
  text.clear();
  text.setPlainText(qrawMessage);
}

void MainWindow::on_actionBuild_Local_Structure_triggered()
{
  bp::object pythonStructureObject =
  imapWrapper->FetchThreadStructure(LifeMotifSettings::Label().toStdString());

  std::string tempPath;
  std::string scriptPath;
  std::string lsPath;

  scriptPath = LifeMotifSettings::PythonScriptPath().toStdString();
  tempPath = LifeMotifSettings::LocalStructure().toStdString();
  lsPath = LifeMotifPathHelper::Join(2, scriptPath.c_str(), tempPath.c_str());

  LocalStructureWrapper(
        LIFEMOTIF_LOCAL_STRUCTURE_WRAPPER_MODULE,
        LIFEMOTIF_LOCAL_STRUCTURE_WRAPPER_CLASS).Build(
        lsPath, pythonStructureObject);

  LocalStructureExtract(pythonStructureObject, localStructure);
  UpdateCalendar();
}

void MainWindow::ParseMessage(const std::string& rawMessage)
{

}
