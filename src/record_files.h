#ifndef RECORD_FILES_H
#define RECORD_FILES_H

#include "record_file.h"

#include <QString>

enum class ETable;

class Record_Files
{
public:
  Record_Files()
  {
  }
  Record_Files(ETable _table, int _record_id);


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
  [[nodiscard]] const std::vector<Record_File>& get_files()
  {
    return files;
  }

  [[nodiscard]] bool is_loaded()
  {
    return loaded;
  }

  // database operations
  bool delete_all_files();
  void refresh_data();

private:
  bool loaded = false;

  ETable                   table;
  int                      record_id = -1;
  std::vector<Record_File> files;
};

#endif // RECORD_FILES_H
