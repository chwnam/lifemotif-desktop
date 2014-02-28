#include "main_window.h"
#include "ui_main_window.h"
#include "preference_window.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QScopedPointer>
#include <QVector>
#include <QThread>
#include <QThreadPool>
#include <QWebSettings>

#include <QTextStream>

#include "lifemotif_config.h"
#include "localstructure_extract.h"
#include "web_browser_dialog.h"
#include "lifemotif_google_oauth2.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    imap(0),
    _consoleDialog(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    oauth2 = new LifeMotifGoogleOauth2(this); // QObject

    LoadLocalStructure();
    UpdateCalendar();
    UpdateMenu();
}

MainWindow::~MainWindow()
{
    delete ui;

    // removes memory leak of QWebkit...
    QWebSettings::clearMemoryCaches();
}

void MainWindow::AuthenticateOnConsoleByPython()
{
  QString secretPath = LifeMotifSettings::SecretPath(true);
  QString storageName = LifeMotifSettings::StorageName(true);

  qDebug() << "Authentication by console (Python mode)."
    << "Client secret path:" << secretPath;

  const std::string
    url = oauth2Wrapper()->GetAuthorizationURL(secretPath.toStdString());

  const std::string
    code = oauth2Wrapper()->GrantUserPermission(url);

  bp::object
    credentials = oauth2Wrapper()->MakeCredentials(code);

  // keep this credentials
  oauth2Wrapper()->SetCredentials(storageName.toStdString(), credentials);

  qDebug() << "Successfully authorized.";

  UpdateMenu();
}

void MainWindow::AuthenticateUsingWebBrowser()
{
  WebBrowserDialog wbDlg(this);

  wbDlg.SetAuthorizationUrl(
        oauth2->GetAuthorizationUrl(
          LifeMotifSettings::SecretPath(true)));
  wbDlg.setWindowModality(Qt::ApplicationModal);

  if (wbDlg.exec() != QDialog::Accepted) {
    QMessageBox::warning(
          this, "Failure", "Authentication failed.");
    return;
  }

  oauth2->MakeCredentials(
        LifeMotifSettings::SecretPath(),
        wbDlg.AuthorizationCode());

  oauth2->SetCredentials(
    LifeMotifSettings::StorageName());

  UpdateMenu();
}

void MainWindow::BuildLocalStructre()
{
  QString label = LifeMotifSettings::Label();
  bp::object structureObject =
      imapWrapper()->FetchThreadStructure(label.toStdString());

  // extract object
  LocalStructureExtract(structureObject, localStructure);

  // save new object
  QScopedPointer<LocalStructureWrapper>
      lsWrapper(LifeMotifUtils::CreateLocalStructureWrapper());

  const QString lsPath
      = LifeMotifSettings::LocalStructure(true);

  lsWrapper->Build(lsPath.toStdString(), structureObject);
}

void MainWindow::LoadLocalStructure()
{
  const QString lsPath = LifeMotifSettings::LocalStructure();

  if (LifeMotifUtils::IsFileReadable(lsPath)) {
    bp::object structurePythonObject;
    QScopedPointer<LocalStructureWrapper>
        lsWrapper(LifeMotifUtils::CreateLocalStructureWrapper());

    structurePythonObject = lsWrapper->Load(lsPath.toStdString());
    LocalStructureExtract(structurePythonObject, localStructure);
  }
}

void MainWindow::ParseMessage(const std::string& rawMessage)
{
  diary = LifeMotifDiaryPtr(new LifeMotifDiary(rawMessage));

  qDebug() << diary->NumberOfAttachments() << "attachment(s)";
  if (diary->NumberOfAttachments() > 0) {
      for(int i = 0; i < diary->NumberOfAttachments(); ++i) {
          qDebug() << "  " << diary->GetAttachment(i).name;
      }
  }
}

void MainWindow::RevokeAuthentication()
{
  // revoke authentication and remove file
  oauth2->Revoke(LifeMotifSettings::StorageName(true));
}

void MainWindow::UpdateCalendar()
{
  ui->calendarWidget->SetDate(localStructure);
  ui->calendarWidget->UpdateUI();
}

void MainWindow::UpdateDiaryInformationUI(void)
{
    // from, to, subject
    ui->fromEdit->setText(diary->From());
    ui->toEdit->setText(diary->To());
    ui->subjectEdit->setText(diary->Subject());

    // diary text
    ui->diaryPlainText->clear();
    ui->diaryHtmlText->clear();
    if (diary->TextPlainContent().isEmpty() == false) {
      ui->diaryPlainText->setPlainText(diary->TextPlainContent());
    }
    if (diary->TextHtmlContent().isEmpty() == false){
      if (ui->showHtmlCodeCheckBox->checkState() == Qt::Checked) {
        // show html code
        ui->diaryHtmlText->setPlainText(diary->TextHtmlContent());
      } else if (ui->showHtmlCodeCheckBox->checkState() == Qt::Unchecked) {
        // render as html code
        ui->diaryHtmlText->setHtml(diary->TextHtmlContent());
      }
    }

    // attachment
    const int nAttachments = diary->NumberOfAttachments();
    ui->attatchmentComboBox->clear();
    if (nAttachments > 0) {
      for(int i = 0; i < nAttachments; ++i) {
        const LifeMotifAttachment& att = diary->GetAttachment(i);
        ui->attatchmentComboBox->insertItem(i, att.name);
      }
    }
}

QString MainWindow::FetchMessage(const MsgIdType& id)
{
  QString rawMessage;

  if (emailCache()->HasCache(id)) {
    qDebug() << id << "is cached. Load from local disk.";
    rawMessage = emailCache()->GetCache(id);
  } else {
    qDebug() << id << "is uncached. Fetch from the server.";
    std::string label = LifeMotifSettings::Label().toStdString();
    rawMessage = QString::fromStdString(imapWrapper()->FetchMail(label, id));
    emailCache()->SetCache(id, rawMessage);
  }

  return rawMessage;
}

DateType MainWindow::GetDateFromCalendar() const
{
  return
    ui->calendarWidget->selectedDate().toString("yyyyMMdd").toStdString();
}

void MainWindow::UpdateMenu()
{
  const bool enable = LifeMotifUtils::HasCredentials(true);

  // Cannot do those actions if authenticated:
  //  - authentication 
  ui->actionBrowserAuthentication->setEnabled(!enable);

  // Cannot do those actions if not authenticated.
  //  - build local structre
  //  - revoke
  ui->actionBuildLocalStructure->setEnabled(enable);
  ui->revokeAuthentication->setEnabled(enable);
}

void MainWindow::ClearDiaryInformationUI()
{
  // from, to, subject
  ui->fromEdit->clear();
  ui->toEdit->clear();
  ui->subjectEdit->clear();

  // diary text
  ui->diaryPlainText->clear();
  ui->diaryHtmlText->clear();

  // attachment
  ui->attatchmentComboBox->clear();
}

void MainWindow::OpenImapConsole()
{
  ConsoleDialog()->show();
  ConsoleDialog()->raise();
  ConsoleDialog()->activateWindow();
}

void MainWindow::on_mimeRawMessageButton_clicked()
{
  const QListWidgetItem* item = ui->diaryList->currentItem();

  if (item) {
    mimeRawMessageDialog()->SetEditText(
          FetchMessage(item->text().toULongLong()));
    mimeRawMessageDialog()->setFocus();
    mimeRawMessageDialog()->show();
    mimeRawMessageDialog()->raise();
    mimeRawMessageDialog()->activateWindow();
  }
}

void MainWindow::on_attatchmentSaveAsButton_clicked()
{
  const int index = ui->attatchmentComboBox->currentIndex();

  if (index > -1) {
    const LifeMotifAttachment& attachment = diary->GetAttachment(index);
    QString fileName
        = QFileDialog::getSaveFileName(
            this,
            QString("Save attchment as..."),
            attachment.name);

    QFile file;
    file.setFileName(fileName);
    if (file.open(QIODevice::WriteOnly)) {
      qint64 writtenByte = file.write(attachment.data);
      qDebug() << fileName << "saved:" << writtenByte << "bytes";
    }
  }
}

void MainWindow::on_mimeStructureButton_clicked()
{
  const QListWidgetItem* item = ui->diaryList->currentItem();

  if (item) {
    QString rawMessage = FetchMessage(item->text().toULongLong());
    mimeStructureDialog()->Parse(rawMessage.toStdString());
    mimeStructureDialog()->show();
    mimeStructureDialog()->raise();
    mimeStructureDialog()->activateWindow();
  }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_clearTextButton_clicked()
{
  ClearDiaryInformationUI();
}

void MainWindow::on_actionBrowserAuthentication_triggered()
{
  if (LifeMotifUtils::HasCredentials() &&
      QMessageBox::No == QMessageBox::question(
          this,
          QString("Double Authentication"),
          QString("Already authenticated! Proceed again?"))) {
      return;
  }

  AuthenticateUsingWebBrowser();
}

void MainWindow::on_actionConsoleAuthentication_triggered()
{
  // TODO: if the program already has been authenticated?
  AuthenticateOnConsoleByPython();
}

void MainWindow::on_showHtmlCodeCheckBox_clicked()
{
  UpdateDiaryInformationUI();
}

void MainWindow::on_actionBuildLocalStructure_triggered()
{
  BuildLocalStructre();
  UpdateCalendar();
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
  DateType datestring = date.toString("yyyyMMdd").toStdString();

  // yyyyMMdd date string is the key.
  // find the email thread of the day and display in the list widget.
  if (localStructure.find(datestring) != localStructure.end()) {
    const MessageGroup& group = localStructure[datestring];
    QListWidget& list = *ui->diaryList;

    list.clear();
    for(std::size_t i = 0; i < group.messageIds.size(); ++i) {
      list.addItem(QString::number(group.messageIds[i]));
    }
  }
}

void MainWindow::ShowDiary(const int entry)
{
  // get message id
  DateType ds = GetDateFromCalendar();

  if (localStructure.find(ds) != localStructure.end()) {
    MsgIdType msgId = localStructure[ds].messageIds[entry];
    // fetch message by message id, and parse email message
    ParseMessage(FetchMessage(msgId).toStdString());
    // update ui
    UpdateDiaryInformationUI();
  }
}

void MainWindow::on_diaryList_clicked(const QModelIndex &index)
{
  ShowDiary(index.row());
}

void MainWindow::on_revokeAuthentication_triggered()
{
  if (LifeMotifUtils::HasCredentials() &&
      QMessageBox::No == QMessageBox::question(
          this,
          QString("Revoke Authentication"),
          QString("Revoke authentication from Google. Are you sure?"))) {
      return;
  }
  RevokeAuthentication();
  UpdateMenu();
}

void MainWindow::on_actionPreference_triggered()
{
  // show preference modal dialog
  PreferenceWindow pref(imapWrapper().data(), this);

  // some tuning can be here ... center of the main window...
  int result = pref.exec();
  if (result == QDialog::Accepted) {
    std::cout << "User Accepted\n";
  } else if (result == QDialog::Rejected) {
    std::cout << "User canceled\n";
  }
}

void MainWindow::on_actionAbout_triggered()
{
  QMessageBox::about(this, "About LifeMotif-Desktop", "LifeMotif-Desktop");
}

void MainWindow::on_actionQt_triggered()
{
  QMessageBox::aboutQt(this, "About LifeMotif-Desktop");
}

void MainWindow::on_OpenImapConsole_clicked()
{
  OpenImapConsole();
}