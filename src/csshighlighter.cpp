#include "csshighlighter.h"

CssHighlighter::CssHighlighter(QTextDocument* parent)
  : QSyntaxHighlighter(parent)
{
  Rule rule;

  // --- Default text (Qt Creator dark theme)
  defaultFormat.setForeground(QColor("#ffffff"));

  // --- Selectors (QWidget, QPushButton, .class, #id)
  selectorFormat.setForeground(QColor("#0091ff"));
  rule.pattern = QRegularExpression(R"(^\s*[\w\.\#-]+(?=\s*\{))");
  rule.format  = selectorFormat;
  rules.append(rule);

  // --- Properties (color, margin...)
  propertyFormat.setForeground(QColor("#da70d6"));
  rule.pattern = QRegularExpression(R"(\b[a-zA-Z-]+(?=\s*:))");
  rule.format  = propertyFormat;
  rules.append(rule);

  // --- Values (solid, red, #fff)
  valueFormat.setForeground(QColor("#22aa22"));
  rule.pattern = QRegularExpression(R"((?<=:)[^;]+(?=;))");
  rule.format  = valueFormat;
  rules.append(rule);

  // --- Numbers (12, 10px etc.)
  numberFormat.setForeground(QColor("#3cb371"));
  rule.pattern = QRegularExpression(R"(\b\d+(\.\d+)?(px|em|%|pt)?)");
  rule.format  = numberFormat;
  rules.append(rule);

  // --- Strings ("text")
  stringFormat.setForeground(QColor("#ce9178"));
  rule.pattern = QRegularExpression(R"(".*?")");
  rule.format  = stringFormat;
  rules.append(rule);

  // --- Comments /* ... */
  commentFormat.setForeground(QColor("#6a9955"));
  rule.pattern = QRegularExpression(R"(\/\*[\s\S]*?\*\/)");
  rule.format  = commentFormat;
  rules.append(rule);

  // --- Symbols { } : ;
  symbolFormat.setForeground(QColor("#d4d4d4"));
  rule.pattern = QRegularExpression(R"([\{\}\:\;])");
  rule.format  = symbolFormat;
  rules.append(rule);
}

void CssHighlighter::highlightBlock(const QString& text)
{
  // Default color
  setFormat(0, text.length(), defaultFormat);

  for (const Rule& r : rules) {
    auto it = r.pattern.globalMatch(text);
    while (it.hasNext()) {
      auto m = it.next();
      setFormat(m.capturedStart(), m.capturedLength(), r.format);
    }
  }
}
