//
// Created by kamiyoru on 2020/5/1.
//

#ifndef PASCAL_S_AST_H
#define PASCAL_S_AST_H

struct ASTNode {

};

struct Exp : public ASTNode {

};

struct ExpNumber : public Exp {
    const Number *num;

    explicit ExpNumber(Number *num) : num(num) {}
};

struct ExpIdent : public Exp {
    const Identifier *ident;

    explicit ExpIdent(Identifier *ident) : ident(ident) {}
};

using ParamList = std::vector<Exp*>;

struct ExpFuncCall: public Exp {
    const Identifier *fn;
    const ParamList *params;

    explicit ExpFuncCall(Identifier *fn, const ParamList *params) : fn(fn), params(params) {}
};

struct BiExp : public Exp {
    Exp *lhs, *rhs;
    const Marker *marker;

    explicit BiExp(Exp *lhs, Marker *marker, Exp *rhs) : lhs(lhs), rhs(rhs), marker(marker) {}
};

struct UnExp : public Exp {
    Exp *lhs;
    const Marker *marker;

    explicit UnExp(Marker *marker, Exp *lhs) : lhs(lhs), marker(marker) {}
};

void delete_ast(ASTNode *node) {

}

#endif //PASCAL_S_AST_H
