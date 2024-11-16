#include "alias.h"

Alias::Alias() { }

Alias::Alias(int id, std::string name, int link_id) : id(id), name(name), link_id(link_id) {}

Alias& Alias::GetAlias() {
  return *this;
}
