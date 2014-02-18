#include "main_window.h"
#include "ui_main_window.h"
#include "preference_window.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QUrl>
#include <QVector>

#include "lifemotif_config.h"
#include "localstructure_extract.h"
#include "web_browser_dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
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
}

void MainWindow::BuildLocalStructre()
{
  QString label = LifeMotifSettings::Label();
  bp::object structureObject =
      imapWrapper()->FetchThreadStructure(label.toStdString());

  // extract object
  LocalStructureExtract(structureObject, localStructure);

  // save new object
  LocalStructureWrapperPtr lsWrapper
      = LifeMotifUtils::CreateLocalStructureWrapper();

  const QString lsPath
      = LifeMotifSettings::LocalStructure(true);

  lsWrapper->Build(lsPath.toStdString(), structureObject);
}

void MainWindow::LoadLocalStructure()
{
  const QString lsPath = LifeMotifSettings::LocalStructure();

  if (LifeMotifUtils::IsFileReadable(lsPath)) {
    LocalStructureWrapperPtr lsWrapper
        = LifeMotifUtils::CreateLocalStructureWrapper();

    bp::object object = lsWrapper->Load(lsPath.toStdString());
    LocalStructureExtract(object, localStructure);
  }
}

void MainWindow::UpdateCalendar()
{
  ui->calendarWidget->SetDate(localStructure);
  ui->calendarWidget->UpdateUI();
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

void MainWindow::on_diaryList_clicked(const QModelIndex &index)
{
  // get message id
  DateType ds = GetDateFromCalendar();

  if (localStructure.find(ds) != localStructure.end()) {
    MsgIdType msgId = localStructure[ds].messageIds[index.row()];
    // fetch message by message id, and parse email message
    ParseMessage(FetchMessage(msgId).toStdString());
    // update ui
    UpdateDiaryInformationUI();
  }
}

void MainWindow::UpdateDiaryInformationUI(void)
{
    // from, to, subject
    ui->fromEdit->setText(diary->From());
    ui->toEdit->setText(diary->To());
    ui->subjectEdit->setText(diary->Subject());

    // diary text
    ui->diaryPlainText->clear();
    ui->dirayHtmlText->clear();
    if (diary->TextPlainContent().isEmpty() == false) {
      ui->diaryPlainText->setPlainText(diary->TextPlainContent());
    }
    if (diary->TextHtmlContent().isEmpty() == false){
      ui->dirayHtmlText->setHtml(diary->TextHtmlContent());
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

void MainWindow::on_actionBuild_Local_Structure_triggered()
{
  BuildLocalStructre();
  UpdateCalendar();
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

void MainWindow::on_actionBrowserAuthentication_triggered()
{
  if (HasCredentials() &&
      QMessageBox::No == QMessageBox::question(
          this,
          QString("Double Authentication"),
          QString("인증은 이미 받았어요! 또 인증 절차를 진행할까요?"))) {
      return;
  }

  AuthenticateUsingWebBrowser();
}

void MainWindow::on_actionConsoleAuthentication_triggered()
{
  // TODO: if the program already has been authenticated?
  AuthenticateOnConsole();
}

void MainWindow::AuthenticateOnConsole()
{
  QString secretPath = LifeMotifSettings::SecretPath(true);
  QString storageName = LifeMotifSettings::StorageName(true);

  qDebug() << "Authentication by console."
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
  WebBrowserDialog wbDlg(this, oauth2Wrapper());

  wbDlg.setWindowModality(Qt::ApplicationModal);
  int result = wbDlg.exec();
  if (result == QDialog::Accepted) {
    QMessageBox::information(this, "Authentication Success", "인증에 성공했어요.");
  } else {
    QMessageBox::warning(this, "Authentication failed", "인증에 실패했어요.");
  }

  UpdateMenu();
}

bool MainWindow::HasCredentials() const
{
  return LifeMotifUtils::IsFile(LifeMotifSettings::StorageName(true));
}

DateType MainWindow::GetDateFromCalendar() const
{
  return ui->calendarWidget->selectedDate().toString("yyyyMMdd").toStdString();
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

void MainWindow::UpdateMenu()
{
  // Cannot do those actions if not authenticated.
  //  - build local structre

  if (HasCredentials()) {
    ui->actionBuild_Local_Structure->setEnabled(true);
  } else {
    ui->actionBuild_Local_Structure->setEnabled(false);
  }
}

void MainWindow::on_clearTextButton_clicked()
{
  // from, to, subject
  ui->fromEdit->clear();
  ui->toEdit->clear();
  ui->subjectEdit->clear();

  // diary text
  ui->diaryPlainText->clear();
  ui->dirayHtmlText->clear();

  // attachment
  ui->attatchmentComboBox->clear();
}
