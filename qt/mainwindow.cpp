#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preferencewindow.h"

#include <QDebug>
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

  LocalStructureExtract(structureObject, localStructure);
}

void MainWindow::LoadLocalStructure()
{
  const QString lsPath = LifeMotifSettings::LocalStructure();

  if (LifeMotifUtils::IsFileReadable(lsPath)) {
    LocalStructureWrapper lsWrapper(
          LIFEMOTIF_LOCAL_STRUCTURE_WRAPPER_MODULE,
          LIFEMOTIF_LOCAL_STRUCTURE_WRAPPER_CLASS);

    bp::object object = lsWrapper.Load(lsPath.toStdString());
    LocalStructureExtract(object, localStructure);
  }
}

void MainWindow::UpdateCalendar()
{
  if (localStructure.empty()) {
    ui->calendarWidget->setEnabled(false);
    return;
  }

  // sorted list of date
  QVector<QString> dateVector;
  dateVector.reserve(localStructure.size());
  for(LocalStructureType::iterator it = localStructure.begin();
      it != localStructure.end(); ++it) {
    dateVector.push_back(QString::fromStdString(it->first));
  }
  qSort(dateVector);

  // calendar min/max date
  QDate minDate, maxDate;

  minDate = QDate::fromString(*(dateVector.begin()), "yyyyMMdd");
  maxDate = QDate::fromString(*(dateVector.end()-1), "yyyyMMdd");

  ui->calendarWidget->setMinimumDate(minDate);
  ui->calendarWidget->setMaximumDate(maxDate);
  ui->calendarWidget->setEnabled(true);

  // calendar formatting.
  for(QVector<QString>::iterator it = dateVector.begin(); it != dateVector.end(); ++it) {
    QDate d = QDate::fromString(*it, "yyyyMMdd");
    QTextCharFormat charFormat = ui->calendarWidget->dateTextFormat(d);

    // currently underline, bold
    charFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    charFormat.setFontWeight(QFont::Bold);

    ui->calendarWidget->setDateTextFormat(d, charFormat);
  }
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
  QCalendarWidget& cal = *ui->calendarWidget;
  DateType ds = cal.selectedDate().toString("yyyyMMdd").toStdString();

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
    ui->diaryText->clear();
    if (diary->TextHtmlContent().isEmpty()) {
      ui->diaryText->setPlainText(diary->TextPlainContent());
    } else {
      ui->diaryText->setPlainText(diary->TextHtmlContent());
    }

    // attachment
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

  qDebug() << "From:" << diary->From();
  qDebug() << "To:" << diary->To();
  qDebug() << "Subject:" << diary->Subject();
  qDebug() << "Plain text:" << diary->TextPlainContent().mid(0, 20) << "...";
  qDebug() << "HTML text:" << diary->TextHtmlContent().mid(0, 20) << "...";

  qDebug() << diary->NumberOfAttach() << "attachment(s)";
  if (diary->NumberOfAttach() > 0) {
      for(int i = 0; i < diary->NumberOfAttach(); ++i) {
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
}

bool MainWindow::HasCredentials()
{
  return LifeMotifUtils::IsFile(LifeMotifSettings::StorageName(true));
}
