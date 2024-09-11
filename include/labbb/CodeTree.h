//
// Created by omerfaruk on 16.06.2024.
//

#ifndef LABBB_CODETREE_H
#define LABBB_CODETREE_H

#define hash_prime 3591538823

#include <string>
#include <vector>
#include <iostream>
#include <cudd.h>
#include "Operations.h"
#include <variant>
#include <cstddef>

using namespace std;


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
    virtual bool operator==(const LTLNode& other) {
        if (op != other.op) return false;
        switch (op)
        {
        case NOT:
            const NOTNode* otherNotNode = dynamic_cast<const NOTNode*>(&other);
            const NOTNode* thisNotNode = dynamic_cast<const NOTNode*>(this);
            return thisNotNode == otherNotNode;
        case AND:
            const ANDNode* otherAndNode = dynamic_cast<const ANDNode*>(&other);
            const ANDNode* thisAndNode = dynamic_cast<const ANDNode*>(this);
            return thisAndNode == otherAndNode;
        case OR:
            const ORNode* otherOrNode = dynamic_cast<const ORNode*>(&other);
            const ORNode* thisOrNode = dynamic_cast<const ORNode*>(this);
            return thisOrNode == otherOrNode;
        case SINCE:
            const SINCENode* otherSinceNode = dynamic_cast<const SINCENode*>(&other);
            const SINCENode* thisSinceNode = dynamic_cast<const SINCENode*>(this);
            return thisSinceNode == otherSinceNode;
        case PRE:
            const PRENode* otherPreNode = dynamic_cast<const PRENode*>(&other);
            const PRENode* thisPreNode = dynamic_cast<const PRENode*>(this);
            return thisPreNode == otherPreNode;
        case ONCE:
            const ONCENode* otherOnceNode = dynamic_cast<const ONCENode*>(&other);
            const ONCENode* thisOnceNode = dynamic_cast<const ONCENode*>(this);
            return thisOnceNode == otherOnceNode;
        case HISTORICALLY:
            const HISTORICALLYNode* otherHistoricallyNode = dynamic_cast<const HISTORICALLYNode*>(&other);
            const HISTORICALLYNode* thisHistoricallyNode = dynamic_cast<const HISTORICALLYNode*>(this);
            return thisHistoricallyNode == otherHistoricallyNode;
        case EXISTS:
            const EXISTSNode* otherExistsNode = dynamic_cast<const EXISTSNode*>(&other);
            const EXISTSNode* thisExistsNode = dynamic_cast<const EXISTSNode*>(this);
            return thisExistsNode == otherExistsNode;
        case FORALL:
            const FORALLNode* otherForallNode = dynamic_cast<const FORALLNode*>(&other);
            const FORALLNode* thisForallNode = dynamic_cast<const FORALLNode*>(this);
            return thisForallNode == otherForallNode;
        case IMPLIES:
            const IMPLIESNode* otherImpliesNode = dynamic_cast<const IMPLIESNode*>(&other);
            const IMPLIESNode* thisImpliesNode = dynamic_cast<const IMPLIESNode*>(this);
            return thisImpliesNode == otherImpliesNode;
        case ATOM:
            const ATOMNode* otherAtomNode = dynamic_cast<const ATOMNode*>(&other);
            const ATOMNode* thisAtomNode = dynamic_cast<const ATOMNode*>(this);
            return thisAtomNode == otherAtomNode;
        case ATOM_REF:
            const ATOM_REFNode* otherAtomRefNode = dynamic_cast<const ATOM_REFNode*>(&other);
            const ATOM_REFNode* thisAtomRefNode = dynamic_cast<const ATOM_REFNode*>(this);
            return thisAtomRefNode == otherAtomRefNode;
        default:
            return false;
        }
    }
};

class NOTNode : public LTLNode {
public:
    LTLNode* child;
    NOTNode(LTLNode* child): LTLNode(NOT), child(child) {}
    std::variant<DdNode*, bool> operate();
    size_t hash() {
        return ::hash( {op, child->id} );
    }
    bool operator==(const NOTNode& other) {
        return child->id == other.child->id;
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
    bool operator==(const ANDNode& other) {
        return (left->id == other.left->id && right->id == other.right->id) ||
               (left->id == other.right->id && right->id == other.left->id);
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
    bool operator==(const ORNode& other) {
        return (left->id == other.left->id && right->id == other.right->id) ||
               (left->id == other.right->id && right->id == other.left->id);
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
    bool operator==(const SINCENode& other) {
        return left->id == other.left->id && right->id == other.right->id &&
               time.from == other.time.from && time.to == other.time.to;
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
    bool operator==(const PRENode& other) {
        return child->id == other.child->id;
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
    bool operator==(const ONCENode& other) {
        return child->id == other.child->id && time.from == other.time.from && time.to == other.time.to;
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
    bool operator==(const HISTORICALLYNode& other) {
        return child->id == other.child->id && time.from == other.time.from && time.to == other.time.to;
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
    bool operator==(const EXISTSNode& other) {
        return child->id == other.child->id;
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
    bool operator==(const FORALLNode& other) {
        return child->id == other.child->id;
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
    bool operator==(const IMPLIESNode& other) {
        return left->id == other.left->id && right->id == other.right->id;
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
    bool operator==(const ATOMNode& other) {
        return var == other.value;
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
    bool operator==(const ATOM_REFNode& other) {
        return var == other.reference;
    }
};

#endif //LABBB_CODETREE_H
