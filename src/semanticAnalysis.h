#pragma

#include "../grammar/CACTBaseListener.h"
#include "symboltable.h"
#include "CACTLexer.h"

class SemanticAnalysis : public CACTBaseListener
{
public:
    /* data */
public:
    VarSymbolTable *root_table;
    VarSymbolTable *cur_table;
    FuncSymbolTable * func_table;
    FuncSymbolTable * cur_func_table;
    int func_level;
    int while_level;

    SemanticAnalysis(/* args */){}
    ~SemanticAnalysis(){}


    void enterCompUnit(CACTParser::CompUnitContext * /*ctx*/) override;// { }
    void exitCompUnit(CACTParser::CompUnitContext * /*ctx*/) override;// { }

    void enterDecl(CACTParser::DeclContext * /*ctx*/) override { }
    void exitDecl(CACTParser::DeclContext * /*ctx*/) override { }

    void enterConstDecl(CACTParser::ConstDeclContext * /*ctx*/) override;
    void exitConstDecl(CACTParser::ConstDeclContext * /*ctx*/) override;

    void enterBType(CACTParser::BTypeContext * /*ctx*/) override { }
    void exitBType(CACTParser::BTypeContext * /*ctx*/) override { }

    void enterConstDef(CACTParser::ConstDefContext * /*ctx*/) override { }
    void exitConstDef(CACTParser::ConstDefContext * /*ctx*/) override { }

    void enterNumconstInitVal(CACTParser::NumconstInitValContext * /*ctx*/) override;
    void exitNumconstInitVal(CACTParser::NumconstInitValContext * /*ctx*/) override;

    void enterArrayconstInitVal(CACTParser::ArrayconstInitValContext * /*ctx*/) override;
    void exitArrayconstInitVal(CACTParser::ArrayconstInitValContext * /*ctx*/) override;

    void enterVarDecl(CACTParser::VarDeclContext * /*ctx*/) override;
    void exitVarDecl(CACTParser::VarDeclContext * /*ctx*/) override;

    void enterVarDef(CACTParser::VarDefContext * /*ctx*/) override { }
    void exitVarDef(CACTParser::VarDefContext * /*ctx*/) override { }

    void enterFuncDef(CACTParser::FuncDefContext * ctx) override;
    void exitFuncDef(CACTParser::FuncDefContext * /*ctx*/) override;

    void enterFuncType(CACTParser::FuncTypeContext * /*ctx*/) override;
    void exitFuncType(CACTParser::FuncTypeContext * /*ctx*/) override;

    void enterFuncFParams(CACTParser::FuncFParamsContext * /*ctx*/) override;
    void exitFuncFParams(CACTParser::FuncFParamsContext * /*ctx*/) override;

    void enterFuncFParam(CACTParser::FuncFParamContext * /*ctx*/) override;
    void exitFuncFParam(CACTParser::FuncFParamContext * /*ctx*/) override;

    void enterBlock(CACTParser::BlockContext * ctx) override;
    void exitBlock(CACTParser::BlockContext * ctx) override;

    void enterBlockItem(CACTParser::BlockItemContext * /*ctx*/) override;
    void exitBlockItem(CACTParser::BlockItemContext * /*ctx*/) override;

    void enterAssignStmt(CACTParser::AssignStmtContext * /*ctx*/) override;
    void exitAssignStmt(CACTParser::AssignStmtContext * /*ctx*/) override;

    void enterExprStmt(CACTParser::ExprStmtContext * /*ctx*/) override;
    void exitExprStmt(CACTParser::ExprStmtContext * /*ctx*/) override;

    void enterBlockStmt(CACTParser::BlockStmtContext * /*ctx*/) override;
    void exitBlockStmt(CACTParser::BlockStmtContext * /*ctx*/) override;

    void enterIfStmt(CACTParser::IfStmtContext * /*ctx*/) override;
    void exitIfStmt(CACTParser::IfStmtContext * /*ctx*/) override;

    void enterWhileStmt(CACTParser::WhileStmtContext * /*ctx*/) override;
    void exitWhileStmt(CACTParser::WhileStmtContext * /*ctx*/) override;

    void enterBreakStmt(CACTParser::BreakStmtContext * /*ctx*/) override;
    void exitBreakStmt(CACTParser::BreakStmtContext * /*ctx*/) override;

    void enterContinueStmt(CACTParser::ContinueStmtContext * /*ctx*/) override;
    void exitContinueStmt(CACTParser::ContinueStmtContext * /*ctx*/) override;

    void enterReturnStmt(CACTParser::ReturnStmtContext * /*ctx*/) override;
    void exitReturnStmt(CACTParser::ReturnStmtContext * /*ctx*/) override;

    void enterExp(CACTParser::ExpContext * /*ctx*/) override;
    void exitExp(CACTParser::ExpContext * /*ctx*/) override;

    void enterCond(CACTParser::CondContext * /*ctx*/) override;
    void exitCond(CACTParser::CondContext * /*ctx*/) override;

    void enterLVal(CACTParser::LValContext * /*ctx*/) override;
    void exitLVal(CACTParser::LValContext * /*ctx*/) override;

    void enterPrimaryExp(CACTParser::PrimaryExpContext * /*ctx*/) override;
    void exitPrimaryExp(CACTParser::PrimaryExpContext * /*ctx*/) override;

    void enterNumber(CACTParser::NumberContext * /*ctx*/) override;
    void exitNumber(CACTParser::NumberContext * /*ctx*/) override;

    void enterPrimaryunaryExp(CACTParser::PrimaryunaryExpContext * /*ctx*/) override;
    void exitPrimaryunaryExp(CACTParser::PrimaryunaryExpContext * /*ctx*/) override;

    void enterFuncCallunaryExp(CACTParser::FuncCallunaryExpContext * /*ctx*/) override;
    void exitFuncCallunaryExp(CACTParser::FuncCallunaryExpContext * /*ctx*/) override;

    void enterUnaryOpunaryExp(CACTParser::UnaryOpunaryExpContext * /*ctx*/) override;
    void exitUnaryOpunaryExp(CACTParser::UnaryOpunaryExpContext * /*ctx*/) override;

    void enterFuncRParams(CACTParser::FuncRParamsContext * /*ctx*/) override;
    void exitFuncRParams(CACTParser::FuncRParamsContext * /*ctx*/) override;

    void enterMulExp(CACTParser::MulExpContext * /*ctx*/) override;
    void exitMulExp(CACTParser::MulExpContext * /*ctx*/) override;

    void enterAddExp(CACTParser::AddExpContext * /*ctx*/) override;
    void exitAddExp(CACTParser::AddExpContext * /*ctx*/) override;

    void enterRelExp(CACTParser::RelExpContext * /*ctx*/) override;
    void exitRelExp(CACTParser::RelExpContext * /*ctx*/) override;

    void enterEqExp(CACTParser::EqExpContext * /*ctx*/) override;
    void exitEqExp(CACTParser::EqExpContext * /*ctx*/) override;

    void enterLAndExp(CACTParser::LAndExpContext * /*ctx*/) override;
    void exitLAndExp(CACTParser::LAndExpContext * /*ctx*/) override;

    void enterLOrExp(CACTParser::LOrExpContext * /*ctx*/) override;
    void exitLOrExp(CACTParser::LOrExpContext * /*ctx*/) override;

    void enterConstExpNumber(CACTParser::ConstExpNumberContext * /*ctx*/) override; //{ }
    void exitConstExpNumber(CACTParser::ConstExpNumberContext * /*ctx*/) override;

    void enterConstExpBoolConst(CACTParser::ConstExpBoolConstContext * /*ctx*/) override;// { }
    void exitConstExpBoolConst(CACTParser::ConstExpBoolConstContext * /*ctx*/) override;// { }

    void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;// { }
    void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;// { }
    void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override;// { }
    void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override;// { }
};
