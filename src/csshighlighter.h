#ifndef CSSHIGHLIGHTER_H
#define CSSHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class CssHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT
public:
  explicit CssHighlighter(QTextDocument* parent = nullptr);

protected:
  void highlightBlock(const QString& text) override;

private:
  struct Rule {
    QRegularExpression pattern;
    QTextCharFormat    format;
  };

  QVector<Rule> rules;

  QTextCharFormat selectorFormat;
  QTextCharFormat propertyFormat;
  QTextCharFormat valueFormat;
  QTextCharFormat numberFormat;
  QTextCharFormat stringFormat;
  QTextCharFormat commentFormat;
  QTextCharFormat symbolFormat;
  QTextCharFormat defaultFormat;
};

#endif // CSSHIGHLIGHTER_H
