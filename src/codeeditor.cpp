#include <QtGui>
#include "codeeditor.h"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
  lineNumberArea = new LineNumberArea(this);

  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(slotUpdateExtraAreaWidth()));
  connect(this, SIGNAL(updateRequest(const QRect &, int)), this, SLOT(updateLineNumberArea(const QRect &, int)));
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(slotCursorPositionChanged()));

  slotUpdateExtraAreaWidth();
  updateCurrentLineHighlight();
}

void CodeEditor::slotCursorPositionChanged()
{
  updateCurrentLineHighlight();
}

void CodeEditor::slotUpdateExtraAreaWidth()
{
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

int CodeEditor::lineNumberAreaWidth()
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

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
  if (dy)
    lineNumberArea->scroll(0, dy);
  else if(rect.width() > 4) {
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
  }

  if (rect.contains(viewport()->rect()))
    slotUpdateExtraAreaWidth();
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), 
        lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Escape) {
    ;
  } else {
    QPlainTextEdit::keyPressEvent(event);
  }
}

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
  slotUpdateExtraAreaWidth();
}

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

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
  QPainter painter(lineNumberArea);
  painter.fillRect(event->rect(), Qt::lightGray);

  QTextBlock block = firstVisibleBlock();

  int blockNumber = block.blockNumber();
  int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int) blockBoundingRect(block).height();

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(0, top, lineNumberArea->width() - 2,
          fontMetrics().height(), Qt::AlignRight, number);
    }

    block = block.next();
    top = bottom;
    bottom = top + (int) blockBoundingRect(block).height();
    ++blockNumber;
  }
}
