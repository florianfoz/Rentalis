#include "widget/w_ask_ai.h"

#include "base.h"
#include "preferences.h"
#include "ui_w_ask_ai.h"

#include <QDesktopServices>

W_Ask_AI::W_Ask_AI(const QString& ctx, const QString& prompt, QWidget* parent)
  : QDialog(parent)
  , contextual_data(ctx)
  , user_prompt(prompt)
  , ui(new Ui::W_Ask_AI)
{
  ui->setupUi(this);
}

W_Ask_AI::~W_Ask_AI()
{
  delete ui;
}

void W_Ask_AI::send_to_ai()
{
  QString url_base = Preferences::ask_ai_url;
  QString prompt   = QUrl::toPercentEncoding("speak-in: " + Preferences::locale().toString(QLocale::ISO639)
                                             + "; context-data: " + ui->te_data->document()->toMarkdown()
                                             + "; user-question: " + ui->te_user->document()->toMarkdown());
  url_base         = url_base.arg(prompt);

  QDesktopServices::openUrl(QUrl(url_base));
}
