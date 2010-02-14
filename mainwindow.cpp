#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QCloseEvent>

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
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    ui->textEdit->setFont(font);

    highlighter = new Highlighter(ui->textEdit->document());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave())
        event->accept();
    else
        event->ignore();

}

void MainWindow::documentWasModified()
{
    setWindowModified(ui->textEdit->document()->isModified());
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

void MainWindow::quit()
{
    if(maybeSave()) {
        // write settings
        QCoreApplication::exit(0);
    }
}

void MainWindow::open()
{
    if(maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open File"),
                                                        "",
                                                        tr("Source Files (*.cpp *.c *.py);;All Files(*)"));
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

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
