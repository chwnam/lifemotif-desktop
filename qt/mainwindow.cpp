#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preferencewindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QVector>

#include "email_parser.h"
#include "lifemotif_config.h"
#include "localstructure_extract.h"

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
  const Qstring lsPath = LifeMotifSettings::LocalStructure();

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

    // fetch message by message id
    QString rawMessage = FetchMessage(msgId);
    // parse email message
    ParseMessage(rawMessage.toStdString());

    // display message
    QPlainTextEdit& text = *ui->diaryText;
    text.clear();
    text.setPlainText(rawMessage);

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
  EmailParser parser;
  parser.FeedParser(rawMessage);
}

void MainWindow::on_actionBrowserAuthentication_triggered()
{

}

void MainWindow::on_actionConsoleAuthentication_triggered()
{
  // TODO: if the program already has been authenticated?
  AuthenticateOnConsole();
}

void MainWindow::AuthenticateOnConsole()
{
  if (oauth2Wrapper()) {

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
}
