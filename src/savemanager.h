#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QTimer>

enum class ESaveMode : uint8_t {
  NONE,
  new_save,
  override,
  no_save,
};

class Database;

class SaveManager
{
public:
  static void engage_autosave();
  static bool save_current(ESaveMode mode = ESaveMode::NONE, const QString& method = {});
  static bool export_save(const QString& src, const QString& dest);
  static bool delete_save(const QString& path);
  static bool load_save(const QString& path);

private:
  static QTimer autosave_timer;
};


#endif // SAVEMANAGER_H