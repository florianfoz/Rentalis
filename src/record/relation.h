#ifndef RELATION_H
#define RELATION_H

#include "record/record.h"


template <class DERIVED>
class Relation : public Record<DERIVED>
{
  [[nodiscard]] bool is_record() override
  {
    return false;
  }
};

#endif // RELATION_H