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

#include "config.h"
#include "web_browser_dialog.h"
#include "google_oauth2.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  _oauth2(0),
  _imapManager(0),
  _consoleDialog(0),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
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

void MainWindow::Authorize()
{
  WebBrowserDialog wbDlg(this);

  wbDlg.SetAuthorizationUrl(OAuth2()->GetAuthorizationUrl());
  wbDlg.setWindowModality(Qt::ApplicationModal);

  if (wbDlg.exec() != QDialog::Accepted) {
    QMessageBox::warning(this, "Failure", "Authentication failed.");
    return;
  }

  OAuth2()->Authorize(wbDlg.AuthorizationCode());
  UpdateMenu();
}

void MainWindow::BuildLocalStructre()
{
//  QString label = Settings::Label();
//  bp::object structureObject =
//      imapWrapper()->FetchThreadStructure(label.toStdString());

//  // extract object
//  LocalStructureExtract(structureObject, localStructure);

//  // save new object
//  QScopedPointer<LocalStructureWrapper>
//      lsWrapper(LifeMotifUtils::CreateLocalStructureWrapper());

//  const QString lsPath
//      = Settings::LocalStructure(true);

//  lsWrapper->Build(lsPath.toStdString(), structureObject);
}

void MainWindow::LoadLocalStructure()
{
//  const QString lsPath = Settings::LocalStructure();

//  if (LifeMotifUtils::IsFileReadable(lsPath)) {
//    bp::object structurePythonObject;
//    QScopedPointer<LocalStructureWrapper>
//        lsWrapper(LifeMotifUtils::CreateLocalStructureWrapper());

//    structurePythonObject = lsWrapper->Load(lsPath.toStdString());
//    LocalStructureExtract(structurePythonObject, localStructure);
//  }
}

void MainWindow::ParseMessage(const std::string& rawMessage)
{
  diary = DiaryPtr(new Diary(rawMessage));

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
  OAuth2()->Revoke();
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
      const Attachment& att = diary->GetAttachment(i);
      ui->attatchmentComboBox->insertItem(i, att.name);
    }
  }
}

QString MainWindow::FetchMessage(const MsgIdType& id)
{
//  QString rawMessage;

//  if (emailCache()->HasCache(id)) {
//    qDebug() << id << "is cached. Load from local disk.";
//    rawMessage = emailCache()->GetCache(id);
//  } else {
//    qDebug() << id << "is uncached. Fetch from the server.";
//    std::string label = Settings::Label().toStdString();
//    rawMessage = QString::fromStdString(imapWrapper()->FetchMail(label, id));
//    emailCache()->SetCache(id, rawMessage);
//  }

//  return rawMessage;
}

DateType MainWindow::GetDateFromCalendar() const
{
  return ui->calendarWidget->selectedDate().toString("yyyyMMdd");
}

void MainWindow::UpdateMenu()
{
  const bool enable = Utils::IsTokenAvailable();

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

void MainWindow::ImapAuthenticate()
{
  OAuth2()->ImapAuthenticate();
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
    const Attachment& attachment = diary->GetAttachment(index);
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
  if (Utils::IsTokenAvailable() &&
      QMessageBox::No == QMessageBox::question(
        this,
        QString("Double Authentication"),
        QString("Already authenticated! Proceed again?"))) {
    return;
  }

  Authorize();
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
  DateType datestring = date.toString("yyyyMMdd");

  // yyyyMMdd date string is the key.
  // find the email thread of the day and display in the list widget.
  if (localStructure.find(datestring) != localStructure.end()) {
    const GoogleMessageGroup& group = localStructure[datestring];
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
  if (Utils::IsTokenAvailable() &&
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
//  // show preference modal dialog
//  PreferenceWindow pref(imapWrapper().data(), this);

//  // some tuning can be here ... center of the main window...
//  int result = pref.exec();
//  if (result == QDialog::Accepted) {
//    std::cout << "User Accepted\n";
//  } else if (result == QDialog::Rejected) {
//    std::cout << "User canceled\n";
//  }
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

void MainWindow::on_ImapAuthenticateButton_clicked()
{
  ImapAuthenticate();
}
