#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "highlighter.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QString curFile;

    Highlighter *highlighter;
    void setupEditor();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    bool saveAs();
    void setCurrentFile(const QString &fileName);
    bool maybeSave();
    QString strippedName(const QString &fullFileName);

private slots:
    void quit();
    void open();
    bool save();
    void documentWasModified();
};

#endif // MAINWINDOW_H
