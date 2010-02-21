#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QCloseEvent>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setupEditor();

  connect(ui->textEdit->document(), SIGNAL(contentsChanged()),
      this, SLOT(documentWasModified()));

  setCurrentFile("");
  setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::setupEditor()
{
  QSettings settings(QSettings::IniFormat, QSettings::UserScope, "silasbaronda", "Editor");

  QFont font;
  if(settings.contains("font/family"))
    font.setFamily(settings.value("font/family").toString());
  else
    font.setFamily("DroidSansMono");

  if(settings.contains("font/fixed"))
    font.setFixedPitch(settings.value("font/fixed").toBool());
  else
    font.setFixedPitch(false);

  if(settings.contains("font/size"))
    font.setPointSize(settings.value("font/size").toInt());
  else
    font.setPointSize(9);

  ui->textEdit->setFont(font);

  // Eliminate cursor width
  // should always be 2
  if(settings.contains("general/cursor_width"))
    ui->textEdit->setCursorWidth(settings.value("general/cursor_width").toInt());
  else
    ui->textEdit->setCursorWidth(2);

  if(settings.contains("general/tabstop"))
    ui->textEdit->setTabStopWidth(settings.value("general/tabstop").toInt());
  else
    ui->textEdit->setTabStopWidth(80);

  if(settings.contains("general/indention_width"))
    ui->textEdit->document()->setIndentWidth(settings.value("general/indention_width").toInt());
  else
    ui->textEdit->document()->setIndentWidth(80);

  ui->textEdit->setWordWrapMode(QTextOption::NoWrap);

  highlighter = new Highlighter(ui->textEdit->document());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if(maybeSave()) {
    writeSettings();
    event->accept();
  } else
    event->ignore();
}

void MainWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    default:
      break;
  }
}

/*
 * SLOTS
 */
void MainWindow::quit()
{
  if(maybeSave()) {
    writeSettings();

    QCoreApplication::exit(0);
  }
}

void MainWindow::open()
{
  if(maybeSave()) {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"),
        "",
        tr("Source Files (*.cpp *.c *.h *.py);;All Files(*)"));
    if(!fileName.isEmpty())
      loadFile(fileName);
  }
}
bool MainWindow::save()
{
  if (curFile.isEmpty())
    return saveAs();

  return saveFile(curFile);
}

void MainWindow::documentWasModified()
{
  setWindowModified(ui->textEdit->document()->isModified());
}

void MainWindow::about()
{
  QMessageBox::about(this, tr("About Editor"),
      tr("<b>Editor</b> brought to you by Silas Baronda"));
}
/*
 * END SLOTS
 */

bool MainWindow::saveAs()
{
  QString fileName = QFileDialog::getSaveFileName(this);
  if(fileName.isEmpty())
    return false;

  return saveFile(fileName);
}

bool MainWindow::maybeSave()
{
  if(ui->textEdit->document()->isModified()) {
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Editor"),
        tr("The document has been modified.\n" "Do you want to save your changes?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
      return save();
    else if (ret == QMessageBox::Cancel)
      return false;
  }
  return true;
}

void MainWindow::loadFile(const QString &fileName)
{
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("Editor"),
        tr("Cannot read file %1:\n%2.")
        .arg(fileName)
        .arg(file.errorString()));
    return;
  }

  QTextStream in(&file);
  // do threading
  QApplication::setOverrideCursor(Qt::WaitCursor);
  ui->textEdit->setPlainText(in.readAll());
  QApplication::restoreOverrideCursor();

  setCurrentFile(fileName);
  ui->statusBar->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
  QFile file(fileName);
  if(!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("Editor"),
        tr("Cannot write file %1:\n%2.")
        .arg(fileName)
        .arg(file.errorString()));
    return false;
  }

  QTextStream out(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  out << ui->textEdit->toPlainText();
  QApplication::restoreOverrideCursor();

  setCurrentFile(fileName);
  ui->statusBar->showMessage(tr("File saved"), 2000);
  return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
  curFile = fileName;
  ui->textEdit->document()->setModified(false);
  setWindowModified(false);

  QString shownName;
  if (curFile.isEmpty())
    shownName = "untitled";
  else
    shownName = strippedName(curFile);

  setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Editor")));
}

/* Utility methods */
QString MainWindow::strippedName(const QString &fullFileName)
{
  return QFileInfo(fullFileName).fileName();
}

void MainWindow::writeSettings()
{
  QSettings settings(QSettings::IniFormat, QSettings::UserScope, "silasbaronda", "Editor");

  settings.beginGroup("general");
  settings.setValue("tabstop", ui->textEdit->tabStopWidth());
  settings.setValue("cursor_width", ui->textEdit->cursorWidth());
  settings.setValue("indention_width", ui->textEdit->document()->indentWidth());
  settings.endGroup();

  settings.beginGroup("font");
  settings.setValue("family", ui->textEdit->font().family());
  settings.setValue("size", ui->textEdit->font().pointSize());
  settings.setValue("fixed", ui->textEdit->font().fixedPitch());
  settings.endGroup();

  settings.setValue("highlight_line", true);
  settings.setValue("syntax", true);
}
