#ifndef W_ASK_AI_H
#define W_ASK_AI_H

#include <QDialog>

namespace Ui
{
class W_Ask_AI;
}

class W_Ask_AI : public QDialog
{
  Q_OBJECT

public:
  explicit W_Ask_AI(const QString& ctx, const QString& prompt, QWidget* parent = nullptr);
  ~W_Ask_AI();

  QString contextual_data;
  QString user_prompt;

  void send_to_ai();

private:
  Ui::W_Ask_AI* ui;
};

#endif // W_ASK_AI_H
