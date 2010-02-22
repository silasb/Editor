#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>

class QPaintEvent;
class QResizeEvent;
class QKeyEvent;
class QSize;
class QWidget;

class LineNumberArea;

class CodeEditor : public QPlainTextEdit
{
  Q_OBJECT

  public:
    CodeEditor(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void setHighlightCurrentLine(bool b);
    bool highlightCurrentLine();

    void setLineNumbersVisible(bool b);
    bool lineNumbersVisible();


  protected:
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);

  private slots:
    void slotUpdateExtraAreaWidth();
    void slotCursorPositionChanged();
    void updateLineNumberArea(const QRect &, int);

  private:
    QWidget *lineNumberArea;
    void updateCurrentLineHighlight();
    bool m_highlightCurrentLine;
    bool m_lineNumbersVisible;
    int  m_spaces;
};

class LineNumberArea : public QWidget
{
  public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
      codeEditor = editor;
    }
    QSize sizeHint() const {
      return QSize(codeEditor->lineNumberAreaWidth() + 1, 0);
    }

  protected:
    void paintEvent(QPaintEvent *event) {
      codeEditor->lineNumberAreaPaintEvent(event);
    }

  private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H
