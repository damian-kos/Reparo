#include "alias.h"

Alias::Alias() { }

Alias::Alias(int id, std::string name, int link_id) : id(id), name(name), link_id(link_id) {}

const std::string Alias::ToString() const {
    return std::to_string(id) + " | " + name + " | " + std::to_string(link_id);
}
