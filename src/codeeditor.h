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

    void gutterAreaPaintEvent(QPaintEvent *event);
    int gutterAreaWidth();

    // SETTERS GETTERS
    void setHighlightCurrentLine(bool b);
    bool highlightCurrentLine();

    void setLineNumbersVisible(bool b);
    bool lineNumbersVisible();
    // end

  protected:
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);

  private slots:
    void slotUpdateGutterAreaWidth();
    void slotCursorPositionChanged();
    void slotUpdateRequest(const QRect &, int);

  private:
    QWidget *gutterArea;
    bool m_highlightCurrentLine;
    bool m_lineNumbersVisible;
    int  m_spaces;

    void updateCurrentLineHighlight();
};

class GutterArea : public QWidget
{
  public:
    GutterArea(CodeEditor *editor) : QWidget(editor) {
      codeEditor = editor;
    }
    QSize sizeHint() const {
      return QSize(codeEditor->gutterAreaWidth() + 1, 0);
    }

  protected:
    void paintEvent(QPaintEvent *event) {
      codeEditor->gutterAreaPaintEvent(event);
    }

  private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H
