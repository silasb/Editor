#include <QtGui>
#include "codeeditor.h"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
  gutterArea = new GutterArea(this);

  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(slotUpdateGutterAreaWidth()));
  connect(this, SIGNAL(updateRequest(const QRect &, int)), this, SLOT(slotUpdateRequest(QRect, int)));
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(slotCursorPositionChanged()));

  slotUpdateGutterAreaWidth();
  updateCurrentLineHighlight();
  m_spaces = 4;
}

// SLOTS
void CodeEditor::slotCursorPositionChanged()
{
  updateCurrentLineHighlight();
}

void CodeEditor::slotUpdateGutterAreaWidth()
{
  setViewportMargins(gutterAreaWidth(), 0, 0, 0);
}

void CodeEditor::slotUpdateRequest(const QRect &rect, int dy)
{
  if (dy)
    gutterArea->scroll(0, dy);
  else if(rect.width() > 4) {
    gutterArea->update(0, rect.y(), gutterArea->width(), rect.height());
  }

  if (rect.contains(viewport()->rect()))
    slotUpdateGutterAreaWidth();
}
// SLOTS

// EVENTS
void CodeEditor::resizeEvent(QResizeEvent *e)
{
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  gutterArea->setGeometry(QRect(cr.left(), cr.top(), 
        gutterAreaWidth(), cr.height()));
}

void CodeEditor::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Tab) {
    QTextCursor cursor;
    cursor = textCursor();
    cursor.clearSelection();
    for(int i = 0; i < m_spaces; i++)
      cursor.insertText(" ");
    setTextCursor(cursor);
  } else if(event->key() == Qt::Key_Backtab) {
    QTextCursor cursor;
    cursor = textCursor();
    cursor.clearSelection();
    for(int i = 0; i < m_spaces; i++)
      cursor.deletePreviousChar();
    setTextCursor(cursor);
  } else {
    QPlainTextEdit::keyPressEvent(event);
  }
}

void CodeEditor::gutterAreaPaintEvent(QPaintEvent *event)
{
  QPainter painter(gutterArea);
  painter.fillRect(event->rect(), Qt::lightGray);

  QTextBlock block = firstVisibleBlock();

  int blockNumber = block.blockNumber();
  int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int) blockBoundingRect(block).height();

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(0, top, gutterArea->width() - 2,
          fontMetrics().height(), Qt::AlignRight, number);
    }

    block = block.next();
    top = bottom;
    bottom = top + (int) blockBoundingRect(block).height();
    ++blockNumber;
  }
}

// end EVENTS

// SETTERS GETTERS
void CodeEditor::setHighlightCurrentLine(bool b)
{
  m_highlightCurrentLine = b;
  updateCurrentLineHighlight();
}

bool CodeEditor::highlightCurrentLine()
{
  return m_highlightCurrentLine;
}

void CodeEditor::setLineNumbersVisible(bool b)
{
  m_highlightCurrentLine = b;
  slotUpdateGutterAreaWidth();
}

bool CodeEditor::lineNumbersVisible()
{
  return m_lineNumbersVisible;
}
// end SETTERS GETTERS

void CodeEditor::updateCurrentLineHighlight()
{
  QList<QTextEdit::ExtraSelection> extraSelections;
  if (m_highlightCurrentLine) {
    QTextEdit::ExtraSelection selection;
    QColor lineColor = QColor(Qt::yellow).lighter(160);
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
  }
  setExtraSelections(extraSelections);
}

int CodeEditor::gutterAreaWidth()
{
  int digits = 3;

  int max = qMax(1, blockCount());
  if(max >= 1000 && max < 10000)
    digits = 4;
  else if(max >= 10000)
    digits = 5;

  int space = 5 + fontMetrics().width(QLatin1Char('9')) * digits;

  return space;
}

