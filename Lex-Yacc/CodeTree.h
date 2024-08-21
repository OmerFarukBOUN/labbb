//
// Created by omerfaruk on 16.06.2024.
//

#ifndef LABBB_CODETREE_H
#define LABBB_CODETREE_H

#define hash_prime 3591538823

#include <string>
#include <vector>
#include <iostream>
#include "cudd.h"
#include "Operations.h"
#include <variant>


enum Operator {
    SINCE,
    OR,
    AND,
    NOT,
    PRE,
    ONCE,
    HISTORICALLY,
    EXISTS,
    FORALL,
    IMPLIES,
    ATOM,
    ATOM_REF
};

size_t hash(std::initializer_list<int>);

class LTLNode {
public:
    int id;
    Operator op;
    LTLNode(Operator op): op(op) {}
    static void* operator new(size_t size);
    virtual std::variant<DdNode*, bool>  operate();
    virtual size_t hash();
    virtual bool operator==(const LTLNode& other);
};

class NOTNode : public LTLNode {
public:
    LTLNode* child;
    NOTNode(LTLNode* child): LTLNode(NOT), child(child) {}
    std::variant<DdNode*, bool> operate();
    size_t hash() {
        return ::hash( {op, child->id} );
    }
    bool operator==(const LTLNode& other) {
        if (op != other.op) return false;
        return child->id == ((NOTNode*)&other)->child->id;
    }
};

class ANDNode : public LTLNode {
public:
    LTLNode* left;
    LTLNode* right;
    ANDNode(LTLNode* left, LTLNode* right): LTLNode(AND), left(left), right(right) {}
    std::variant<DdNode*, bool> operate();
    size_t hash() {
        int small = std::min(left->id, right->id);
        int big = std::max(left->id, right->id);
        return ::hash( {op, small, big} );
    }
    bool operator==(const LTLNode& other) {
        if (op != other.op) return false;
        return (left->id == ((ANDNode*)&other)->left->id && right->id == ((ANDNode*)&other)->right->id) ||
               (left->id == ((ANDNode*)&other)->right->id && right->id == ((ANDNode*)&other)->left->id);
    }
};

class ORNode : public LTLNode {
public:
    LTLNode* left;
    LTLNode* right;
    ORNode(LTLNode* left, LTLNode* right): LTLNode(OR), left(left), right(right) {}
    std::variant<DdNode*, bool> operate();
    size_t hash() {
        int small = std::min(left->id, right->id);
        int big = std::max(left->id, right->id);
        return ::hash( {op, small, big} );
    }
    bool operator==(const LTLNode& other) {
        if (op != other.op) return false;
        return (left->id == ((ORNode*)&other)->left->id && right->id == ((ORNode*)&other)->right->id) ||
               (left->id == ((ORNode*)&other)->right->id && right->id == ((ORNode*)&other)->left->id);
    }
};

class SINCENode : public LTLNode {
public:
    LTLNode* left;
    LTLNode* right;
    time_id_s time;
    SINCENode(LTLNode* left, LTLNode* right, time_id_s time): LTLNode(SINCE), left(left), right(right), time(time) {}
    std::variant<DdNode*, bool> operate();
    size_t hash() {
        return ::hash( {op, time.from, time.to, left->id, right->id} );
    }
    bool operator==(const LTLNode& other) {
        if (op != other.op) return false;
        return left->id == ((SINCENode*)&other)->left->id && right->id == ((SINCENode*)&other)->right->id &&
               time.from == ((SINCENode*)&other)->time.from && time.to == ((SINCENode*)&other)->time.to;
    }
};

class PRENode : public LTLNode {
public:
    LTLNode* child;
    PRENode(LTLNode* child): LTLNode(PRE), child(child) {}
    std::variant<DdNode*, bool> operate();
    size_t hash() {
        return ::hash( {op, child->id} );
    }
    bool operator==(const LTLNode& other) {
        if (op != other.op) return false;
        return child->id == ((PRENode*)&other)->child->id;
    }
};

class ONCENode : public LTLNode {
public:
    LTLNode* child;
    time_id_s time;
    ONCENode(LTLNode* child, time_id_s time): LTLNode(ONCE), child(child), time(time) {}
    std::variant<DdNode*, bool> operate();
    size_t hash() {
        return ::hash( {op, time.from, time.to, child->id} );
    }
    bool operator==(const LTLNode& other) {
        if (op != other.op) return false;
        return child->id == ((ONCENode*)&other)->child->id && time.from == ((ONCENode*)&other)->time.from && time.to == ((ONCENode*)&other)->time.to;
    }
};

class HISTORICALLYNode : public LTLNode {
public:
    LTLNode* child;
    time_id_s time;
    HISTORICALLYNode(LTLNode* child, time_id_s time): LTLNode(HISTORICALLY), child(child), time(time) {}
    std::variant<DdNode*, bool> operate();
    size_t hash() {
        return ::hash( {op, time.from, time.to, child->id} );
    }
    bool operator==(const LTLNode& other) {
        if (op != other.op) return false;
        return child->id == ((HISTORICALLYNode*)&other)->child->id && time.from == ((HISTORICALLYNode*)&other)->time.from && time.to == ((HISTORICALLYNode*)&other)->time.to;
    }
};

class EXISTSNode : public LTLNode {
public:
    LTLNode* child;
    std::vector<std::string> vars;
    EXISTSNode(LTLNode* child, std::vector<std::string> vars): LTLNode(EXISTS), child(child), vars(vars) {}
    std::variant<DdNode*, bool> operate();
    size_t hash() {
        return ::hash( {op, child->id} );
    }
    bool operator==(const LTLNode& other) {
        if (op != other.op) return false;
        return child->id == ((EXISTSNode*)&other)->child->id;
    }
};

class FORALLNode : public LTLNode {
public:
    LTLNode* child;
    std::vector<std::string> vars;
    FORALLNode(LTLNode* child, std::vector<std::string> vars): LTLNode(FORALL), child(child), vars(vars) {}
    std::variant<DdNode*, bool> operate();
    size_t hash() {
        return ::hash( {op, child->id} );
    }
    bool operator==(const LTLNode& other) {
        if (op != other.op) return false;
        return child->id == ((FORALLNode*)&other)->child->id;
    }
};

class IMPLIESNode : public LTLNode {
public:
    LTLNode* left;
    LTLNode* right;
    IMPLIESNode(LTLNode* left, LTLNode* right): LTLNode(IMPLIES), left(left), right(right) {}
    std::variant<DdNode*, bool> operate();
    size_t hash() {
        return ::hash( {op, left->id, right->id} );
    }
    bool operator==(const LTLNode& other) {
        if (op != other.op) return false;
        return left->id == ((IMPLIESNode*)&other)->left->id && right->id == ((IMPLIESNode*)&other)->right->id;
    }
};

class ATOMNode : public LTLNode {
public:
    std::string var;
    std::string value;
    ATOMNode(std::string var, std::string value): LTLNode(ATOM), var(var), value(value) {}
    std::variant<DdNode*, bool> operate();
    size_t hash() {
        std::hash<std::string> hash_fn;
        int hash_var = hash_fn(var);
        int hash_value = hash_fn(value);
        return ::hash( {op, hash_var, hash_value} );
    }
    bool operator==(const LTLNode& other) {
        if (op != other.op) return false;
        return var == ((ATOMNode*)&other)->var && value == ((ATOMNode*)&other)->value;
    }
};

class ATOM_REFNode : public LTLNode {
public:
    std::string var;
    std::string reference;
    ATOM_REFNode(std::string var, std::string reference): LTLNode(ATOM_REF), var(var), reference(reference) {}
    std::variant<DdNode*, bool> operate();
    size_t hash() {
        std::hash<std::string> hash_fn;
        int hash_var = hash_fn(var);
        int hash_reference = hash_fn(reference);
        return ::hash( {op, hash_var, hash_reference} );
    }
    bool operator==(const LTLNode& other) {
        if (op != other.op) return false;
        return var == ((ATOM_REFNode*)&other)->var && reference == ((ATOM_REFNode*)&other)->reference;
    }
};

#endif //LABBB_CODETREE_H
