#ifndef RECORD_FILE_H
#define RECORD_FILE_H

#include <QString>

enum class ETable;

class Record_File
{
public:
  Record_File()
  {
  }
  Record_File(ETable _table, const QString& _uuid)
    : table(_table)
    , uuid(_uuid)
  {
    ensure_loaded();
  }

private:
  void ensure_loaded();

public:
  // getters
  [[nodiscard]] ETable get_table()
  {
    return table;
  }
  [[nodiscard]] int get_record_id()
  {
    return record_id;
  }
  [[nodiscard]] QString get_uuid()
  {
    ensure_loaded();
    return uuid;
  }
  [[nodiscard]] QString get_info_name()
  {
    ensure_loaded();
    return info_name;
  }
  [[nodiscard]] QString get_extension()
  {
    ensure_loaded();
    return extension;
  }
  [[nodiscard]] QString get_file_path() const;

  [[nodiscard]] bool is_loaded()
  {
    return loaded;
  }
  [[nodiscard]] bool is_dirty()
  {
    return dirty;
  }

  // setters
  void set_info_name(const QString& _info_name)
  {
    dirty     = true;
    info_name = _info_name;
  }
  void set_file(const QString& _new_file_path, const QString& _extension)
  {
    dirty     = true;
    new_file  = _new_file_path;
    extension = _extension;
  }

  // database operations
  bool    delete_record(bool w_msg = true);
  QString insert_record();
  bool    update_record();

private:
  bool loaded = false;
  bool dirty  = false;

  ETable  table;
  int     record_id = -1;
  QString uuid;
  QString info_name;
  QString extension;
  QString new_file;
};

#endif // RECORD_FILE_H
