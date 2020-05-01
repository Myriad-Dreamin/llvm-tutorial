//
// Created by kamiyoru on 2020/5/1.
//

#ifndef PASCAL_S_LLVM_CODEGEN_H
#define PASCAL_S_LLVM_CODEGEN_H

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include "ast.h"


// 用于生成目标中间代码LLVM-IR
struct LLVMBuilder {
    using LLVMContext = llvm::LLVMContext;
    using Module = llvm::Module;
    using Value = llvm::Value;

    LLVMContext ctx;
    llvm::IRBuilder<> ir_builder;
    Module modules;
    std::map<std::string, Value *> ident_mapping;

    LLVMBuilder() :ir_builder(ctx), modules("kale-llvm", ctx) {}

    Value *code_gen(ASTNode *node) {
        switch (node->type) {
            case NodeType::ExpNumber:
                return llvm::ConstantFP::get(ctx, llvm::APFloat(
                        //todo: int
                        double(reinterpret_cast<ExpNumber*>(node)->num->value)));
            case NodeType::ExpIdent:
                return ident_mapping.at(reinterpret_cast<ExpIdent*>(node)->ident->content);
            case NodeType::BiExp:
                return code_gen_binary_exp(reinterpret_cast<BiExp*>(node));
            case NodeType::UnExp:
                return code_gen_unary_exp(reinterpret_cast<UnExp*>(node));
            case NodeType::ExpFuncCall:
                break;
            default:
                assert(false);
                return nullptr;
        }
        assert(false);
        return nullptr;
    }

    Value *code_gen_unary_exp(UnExp *pExp) {
        auto lhs = code_gen(pExp->lhs);
        assert(lhs != nullptr);
        switch (pExp->marker->marker_type) {
            case MarkerType::Add:
                return lhs;
            case MarkerType::Sub:
                return ir_builder.CreateFNeg(lhs, "negative_tmp");
            default :
                assert(false);
                return nullptr;
        }
    }

    Value *code_gen_binary_exp(BiExp *pExp) {
        auto lhs = code_gen(pExp->lhs), rhs = code_gen(pExp->rhs);
        assert(lhs != nullptr);
        assert(rhs != nullptr);
        switch (pExp->marker->marker_type) {
            case MarkerType::Add:
                return ir_builder.CreateFAdd(lhs, rhs, "add_tmp");
            case MarkerType::Sub:
                return ir_builder.CreateFSub(lhs, rhs, "sub_tmp");
            case MarkerType::Mul:
                return ir_builder.CreateFMul(lhs, rhs, "mul_tmp");
            case MarkerType::Quo:
                return ir_builder.CreateFDiv(lhs, rhs, "quo_tmp");
            case MarkerType::Mod:
                throw std::runtime_error("fload mod is not allowed");
            default :
                assert(false);
                return nullptr;
        }
    }

};

#endif //PASCAL_S_LLVM_CODEGEN_H
