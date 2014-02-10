#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preferencewindow.h"
#include <QMessageBox>
#include <iostream>
#include <QVector>
#include "settingscontrol.h"
#include "pythonscriptsettings.h"
#include "lifemotif_config.h"
#include "./python_wrapper/localstructurewrapper.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    localStructure = NULL;

    imapWrapper = NULL;
    oauth2Wrapper = NULL;

    // wrapper initialization
    oauth2Wrapper = new GoogleOauth2Wrapper(
          LIFEMOTIF_GOOGLE_OAUTH2_WRAPPER_MODULE,
          LIFEMOTIF_GOOGLE_OAUTH2_WRAPPER_CLASS);


    std::string path;
    std::string storageName;
    std::string emailAddress;

    path = SettingsControl::GetSettings().value("python_script_path").toString().toStdString();
    storageName = PythonScriptSettings::GetSettings()["storage_name"].toString().toStdString();
    storageName = path + storageName;
    emailAddress = PythonScriptSettings::GetSettings()["email_address"].toString().toStdString();

    bp::object imapObject = oauth2Wrapper->ImapAuthenticate(storageName, emailAddress, 4);

    imapWrapper = new GoogleImapWrapper(
          LIFEMOTIF_GOOGLE_IMAP_WRAPPER_MODULE,
          LIFEMOTIF_GOOGLE_IMAP_WRAPPER_CLASS,
          imapObject);
}

MainWindow::~MainWindow()
{
    delete ui;

    if (localStructure) { delete localStructure; localStructure = NULL; }
    if (imapWrapper) { delete imapWrapper; imapWrapper = NULL; }
    if (oauth2Wrapper) { delete oauth2Wrapper; oauth2Wrapper = NULL; }
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

void MainWindow::on_listStructureButton_clicked()
{
  if (localStructure) {
    return;
  }

  QSettings& pref = SettingsControl::GetSettings();
  QVariantMap map = PythonScriptSettings::GetSettings();

  QString pythonScriptRoot = pref.value("python_script_path").toString();
  QString localStructureFilename = map["local_database"].toString();

  // change to more robust one...
  QString locstructPath = pythonScriptRoot + localStructureFilename;

  LocalStructureWrapper
      wrapper(LIFEMOTIF_LOCAL_STRUCTURE_WRAPPER_MODULE,
              LIFEMOTIF_LOCAL_STRUCTURE_WRAPPER_CLASS);

  bp::object obj = wrapper.Load(locstructPath.toStdString());

  localStructure = new LocalStructure(obj);

  // sorted list of date
  QVector<QString> dateVector;
  for(LocalStructure::StructureType::const_iterator cit = (*localStructure)().begin();
      cit != (*localStructure)().end(); ++cit) {
    dateVector.push_back(QString::fromStdString(cit->first));
  }
  qSort(dateVector);

//  for(QVector<QString>::iterator it = dateVector.begin();
//      it != dateVector.end(); ++it) {
//    std::cout << it->toStdString() << "\n";
//  }

  QDate minDate, maxDate;
  minDate = QDate::fromString(*(dateVector.begin()), "yyyyMMdd");
  maxDate = QDate::fromString(*(dateVector.end()-1), "yyyyMMdd");

  ui->calendarWidget->setMinimumDate(minDate);
  ui->calendarWidget->setMaximumDate(maxDate);

  for(QVector<QString>::iterator it = dateVector.begin();
      it != dateVector.end(); ++it) {

    QDate d = QDate::fromString(*it, "yyyyMMdd");
    QTextCharFormat charFormat = ui->calendarWidget->dateTextFormat(d);

    charFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    charFormat.setFontWeight(QFont::Bold);

    ui->calendarWidget->setDateTextFormat(d, charFormat);
  }
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    LocalStructure::DateType
        datestring = date.toString("yyyyMMdd").toStdString();

    const LocalStructure::StructureType& struc = (*localStructure)();

    const LocalStructure::DailyContainer& cont = struc.at(datestring);

    std::cout << datestring << '\t';
    std::cout << cont.threadId << std::endl;

    QListWidget& list = *ui->diaryList;
    list.clear();

    for(std::size_t i = 0; i < cont.messageIds.size(); ++i) {
      list.addItem(QString::number(cont.messageIds[i]));
      std::cout << cont.messageIds[i] << ' ';
    }
    std::cout << "\n\n";
}

void MainWindow::on_diaryList_clicked(const QModelIndex &index)
{
  QCalendarWidget& cal = *ui->calendarWidget;

  QDate d = cal.selectedDate();
  LocalStructure::DateType ds = d.toString("yyyyMMdd").toStdString();

  const LocalStructure::StructureType& struc = (*localStructure)();
  const LocalStructure::DailyContainer& cont = struc.at(ds);

  LocalStructure::MsgIdType msgId = cont.messageIds[index.row()];
  std::string label = PythonScriptSettings::GetSettings()["label"].toString().toStdString();
  std::string rawMessage = imapWrapper->FetchMail(label, msgId);

  QPlainTextEdit& edit = *ui->plainTextEdit;
  edit.clear();
  edit.setPlainText(QString::fromStdString(rawMessage));
}
