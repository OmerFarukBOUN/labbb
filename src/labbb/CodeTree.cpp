#include <string>
#include <vector>
#include <iostream>
#include "cudd.h"
#include "include/labbb/Operations.h"
#include <variant>
#include "CodeTree.h"


int id_counter = 0;

int new_id() {
    return id_counter++;
}

size_t hash(std::initializer_list<int> list) {
    size_t hash = 0;
    for (auto i : list) {
        hash = hash * hash_prime + i;
    }
    return hash;
}

void* LTLNode::operator new(size_t size) {
    return malloc(size);
}
