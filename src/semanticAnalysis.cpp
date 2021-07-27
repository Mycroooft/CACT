#include "semanticAnalysis.h"
#include<string>
#include<vector>
#include<iostream>
//#include "symboltable.h"
//#include "CACTLexer.h"
#include "utils.h"
/*
extern VarSymbolTable *root_table;
extern VarSymbolTable *cur_table;
extern FuncSymbolTable * func_table;
extern FuncSymbolTable * cur_func_table;
extern int func_level;
extern int while_level;
*/
void SemanticAnalysis::enterCompUnit(CACTParser::CompUnitContext * ctx)
{
    func_table = new FuncSymbolTable();
    root_table = new VarSymbolTable(nullptr);
    cur_table = root_table;
    func_level = 0;
    while_level = 0;

    //load i/o function;
    FuncInfo* print_int = new FuncInfo("print_int", TYPE_VOID, 1, {TYPE_INT}, {"para_name"}, {0});
    FuncInfo* print_float = new FuncInfo("print_float", TYPE_VOID, 1, {TYPE_FLOAT}, {"para_name"}, {0});
    FuncInfo* print_double = new FuncInfo("print_double", TYPE_VOID, 1, {TYPE_DOUBLE}, {"para_name"}, {0});
    FuncInfo* print_bool = new FuncInfo("print_bool", TYPE_VOID, 1, {TYPE_BOOL}, {"para_name"}, {0});
    FuncInfo* get_int = new FuncInfo("get_int", TYPE_INT, 0, {TYPE_VOID}, {"para_name"}, {0});
    FuncInfo* get_float = new FuncInfo("get_float", TYPE_FLOAT, 0, {TYPE_VOID}, {"para_name"}, {0});
    FuncInfo* get_double = new FuncInfo("get_double", TYPE_DOUBLE, 0, {TYPE_VOID}, {"para_name"}, {0});

    func_table->add_func("print_int", *print_int);
    func_table->add_func("print_float", *print_float);
    func_table->add_func("print_double", *print_double);
    func_table->add_func("print_bool", *print_bool);
    func_table->add_func("get_int", *get_int);
    func_table->add_func("get_float", *get_float);
    func_table->add_func("get_double", *get_double);
}

void SemanticAnalysis::exitCompUnit(CACTParser::CompUnitContext* ctx)
{
    FuncInfo* func_main = func_table->lookup("main");
    if(func_main == NULL)
    {
         std::cout <<"Error: main function undefined"; 
         exit(-1);
    }
    if(func_main->ret_type != TYPE_INT)
    {
        std::cout <<"Error: unexpected return type of main";
        exit(-1);
    }
    if(func_main->num_param != 0)
    {
        std::cout <<"Error: unexpected parameter of main";
        exit(-1);
    }
}

//void SemanticAnalysis::enterDecl(CACTParser::DeclContext * ctx)
//{}
//void SemanticAnalysis::exitDecl(CACTParser::DeclContext * ctx)
//{}

int myStrToInt(std::string rt_type)
{
    int rt_type_int;

   	if (rt_type == "int")
    {
        rt_type_int = TYPE_INT;
    }
    else if (rt_type == "float")
    {
        rt_type_int = TYPE_FLOAT;
    }
    else if (rt_type == "double")
    {
    rt_type_int = TYPE_DOUBLE;
    }
    else if (rt_type == "bool")
    {
        rt_type_int = TYPE_BOOL;
    }
    else if (rt_type == "void")
    {
        rt_type_int = TYPE_VOID;
    }
    else
    {
    	rt_type_int = -1;
        std::cout<< rt_type << std::endl;
    }

    return rt_type_int;		
}

//常数类型声明
void SemanticAnalysis::enterConstDecl(CACTParser::ConstDeclContext * ctx)
{}
void SemanticAnalysis::exitConstDecl(CACTParser::ConstDeclContext * ctx)
{
    std::string name;
    std::string type_string;
    std::string length_str;
    int array_length;
    int type;
    int cls;
    int is_array;
    int is_const;

    //std::cout << "const variable define: " << std::endl;
    for(const auto & const_def : ctx->constDef())
    {
        //std::cout << "\tname: " << const_def->Ident()->getText().c_str() \
        //        << " type: " << ctx->bType()->getText().c_str() << std::endl;
        //符号表信息
        if(const_def->Ident() == NULL)
            return;
        name = const_def->Ident()->getText().c_str();
        type_string = ctx->bType()->getText().c_str();
        type = myStrToInt(type_string);
        is_const = 1;
        //检查类型匹配
        if(const_def->IntConst())
        {//存在[IntConst]
            cls = CLS_CONST_ARRAY;
            is_array = 1;
            length_str = const_def -> /*arraySymbol()->*/IntConst()->getText();
            array_length = std::stoi(length_str, 0, getIntStringBase(length_str));
            if(type != myStrToInt(const_def->constInitVal()->type))
            {    //等号两端的类型匹配
                std::cout<<"Error: constdecl type error";
                exit(-1);
            }
            if(is_array != const_def->constInitVal()->is_array)
            {
                std::cout<<"Error: array_const = const";
                exit(-1);
            }
            if(is_array && const_def->constInitVal()->is_array && array_length != const_def->constInitVal()->length)
            {
                std::cout<<"Error: array length error";
                exit(-1);
            }            
        }
        else
        {//不存在[IntConst]
            cls = CLS_CONST;
            is_array = 0;
            array_length = 0;
            if(type_string != const_def->constInitVal()->type)
            {    //等号两端的类型匹配
                std::cout<<"Error: constdecl type error";
                exit(-1);
            }
            if(is_array != const_def->constInitVal()->is_array)
            {
                std::cout<<"Error: array_const = const";
                exit(-1);
            }
            if(is_array && const_def->constInitVal()->is_array && array_length != const_def->constInitVal()->length)//
            {
                std::cout<<"Error: array length error";
                exit(-1);
            }  
        }
    }

        //检查是否重定义
        VarInfo* const_symbol = new VarInfo(name, type, cls, is_const,is_array, array_length);
        if(cur_table->lookup_name(name) != NULL)
        {
            std::cout<<"Error: const declared twice";
            exit(-1);
        }
        else
        {
            cur_table->add_symbol(name, *const_symbol);
        }    

}

//void SemanticAnalysis::enterBType(CACTParser::BTypeContext * ctx)
//{}
//void SemanticAnalysis::exitBType(CACTParser::BTypeContext * ctx)
//{}
//void SemanticAnalysis::enterConstDef(CACTParser::ConstDefContext * ctx) {}
//void SemanticAnalysis::exitConstDef(CACTParser::ConstDefContext * ctx) {}

void SemanticAnalysis::enterNumconstInitVal(CACTParser::NumconstInitValContext * ctx) {}
void SemanticAnalysis::exitNumconstInitVal(CACTParser::NumconstInitValContext * ctx) 
{
    ctx->type = ctx->constExp()->type;
    ctx->is_array = 0;
    ctx->length = 0;
}

void SemanticAnalysis::enterArrayconstInitVal(CACTParser::ArrayconstInitValContext * ctx) {}
void SemanticAnalysis::exitArrayconstInitVal(CACTParser::ArrayconstInitValContext * ctx) 
{
    int sign = 0;
    int type;
    //以第一个值为基准，检查数组中是否每个值的类型均相同
    ctx->is_array = 1;
    if(ctx->constExp().empty())
    {
        type = TYPE_VOID;
        ctx->length=0;
    }
    for(const auto & const_exp : ctx->constExp())
    {
        if(!sign)
        {
            type = myStrToInt(const_exp->type); 
        }
        else
        { //数组中类型不一致
            if(type != myStrToInt(const_exp->type))
            {
                std::cout<<"Error: different type in one array";
                exit(-1);
            }
        }
        sign++;
    }
    ctx->type=type;
    ctx->length=ctx->constExp().size();    
}

void SemanticAnalysis::enterVarDecl(CACTParser::VarDeclContext * ctx)
{}
void SemanticAnalysis::exitVarDecl(CACTParser::VarDeclContext * ctx)
{
    std::string name;
    std::string type_string;
    std::string length_str;
    int array_length;
    int type;
    int cls;
    int is_array;
    int is_const;
    //std::cout << "variable define: " << std::endl;
    for(const auto & var_def : ctx->varDef())
    {
    //    std::cout << "\tname: " << var_def->Ident()->getText().c_str() \
    //            << " type: " << ctx->bType()->getText().c_str() << std::endl;
    //符号表项信息
        if(var_def->Ident() == NULL)
            return;
        name = var_def->Ident()->getText().c_str();
        type_string = ctx->bType()->getText().c_str();
        type = myStrToInt(type_string);
        is_const = 0;
        
        //检查类型匹配
        if(var_def->constInitVal())
        {//存在显示初始化
            if(var_def->IntConst())
            {//存在[IntConst]
                cls = CLS_ARRAY;
                is_array = 1;
                length_str = var_def -> /*arraySymbol()->*/IntConst()->getText();
                array_length = std::stoi(length_str, 0, getIntStringBase(length_str));
            }
            else
            {
                cls = CLS_VAR;
                is_array = 0;
                array_length = 0;
            }
            if(type != myStrToInt(var_def->constInitVal()->type))
            {    //等号两端的类型匹配
                std::cout<<"Error: vardecl type error";
                exit(-1);
            }
            if(is_array != var_def->constInitVal()->is_array)
            {
                std::cout<<"Error: array_var = var";
                exit(-1);
            }
            if(is_array && var_def->constInitVal()->is_array && array_length != var_def->constInitVal()->length) //array_length => length
            {
                std::cout<<"Error: var array length error";
                exit(-1);
            }            
        }
        else
        {//不存在显示初始化
            if(var_def->IntConst())
            {//存在[IntConst]
                cls = CLS_ARRAY;
                is_array = 1;
                length_str = var_def -> /*arraySymbol()->*/IntConst()->getText();
                array_length = std::stoi(length_str, 0, getIntStringBase(length_str));
            }
            else
            {
                cls = CLS_VAR;
                is_array = 0;
                array_length = 0;
            }
        }
        //检查是否重定义
        VarInfo* var_symbol = new VarInfo(name, type, cls,is_const, is_array, array_length);
        if(cur_table->lookup_name(name) != NULL)
        {
            std::cout<<"Error: var declared twice";
            exit(-1);
        }
        else
        {
            cur_table->add_symbol(name, *var_symbol);
        }    
    }
}

void SemanticAnalysis::enterFuncDef(CACTParser::FuncDefContext * ctx)
{
    func_level++;
    std::string func_name = ctx->Ident()->getText().c_str();
    std::string ret_type_string = ctx->funcType()->getText();
    int func_ret_type = func_table->StringToInt(ret_type_string);
    int func_num_param = 0;

    if(func_table->lookup(func_name) != NULL)
    {
        std::cout <<"Error: there exists more than one functions with same name";
        exit(-1);
    }

    std::vector<std::string> func_param_type_string;
    std::vector<int> func_param_type;
    std::vector<std::string> func_param_name;
    std::vector<int> func_param_is_array;

    if(ctx->funcFParams())
    {
        func_num_param = ctx->funcFParams()->funcFParam().size();
        auto func_param = ctx->funcFParams()->funcFParam();
        int i = 0;
        for(auto item : func_param)
        {
            auto fp_type_string = item->bType()->getText();//.ctr;
            auto fp_name = item->Ident()->getText();
            func_param_type_string.push_back(fp_type_string);
            func_param_type[i] = myStrToInt(func_param_type_string[i]);
            func_param_name.push_back(fp_name);
            i++;
            if(item->arrayParam()==NULL)
                func_param_is_array.push_back(0);
            else 
                func_param_is_array.push_back(1);
        }
    }

    if(func_name=="main")
    {
        if(func_param_type.size() != 0)
        {
            std::cout<<"Error: main function should have no parameters";
            exit(-1);
        }
        if(func_ret_type!=TYPE_INT)
        {
            std::cout<<"Error:main function should return `int` type";
            exit(-1);
        }
    }

    FuncInfo *func = new FuncInfo(func_name, func_ret_type, func_num_param, func_param_type, func_param_name, func_param_is_array);
    func_table->add_func(func_name, *func);
    cur_func_table = func_table;
}
void SemanticAnalysis::exitFuncDef(CACTParser::FuncDefContext * ctx){}

void SemanticAnalysis::enterFuncType(CACTParser::FuncTypeContext * ctx){}
void SemanticAnalysis::exitFuncType(CACTParser::FuncTypeContext * ctx){}

void SemanticAnalysis::enterFuncFParams(CACTParser::FuncFParamsContext * ctx){}
void SemanticAnalysis::exitFuncFParams(CACTParser::FuncFParamsContext * ctx){}

void SemanticAnalysis::enterFuncFParam(CACTParser::FuncFParamContext * ctx){}
void SemanticAnalysis::exitFuncFParam(CACTParser::FuncFParamContext * ctx){}

void SemanticAnalysis::enterBlock(CACTParser::BlockContext * ctx)
{
    //创建一个符号表
    VarSymbolTable *pointer;
    if(pointer==NULL)
    {
        pointer=new VarSymbolTable(cur_table);
        cur_table=pointer;
    }
    //不需要创建符号表了，用上一个就可以
    else
    {
        cur_table=pointer;
    }
}

void SemanticAnalysis::exitBlock(CACTParser::BlockContext * ctx)
{
    cur_table = cur_table->prev;
}

void SemanticAnalysis::enterBlockItem(CACTParser::BlockItemContext * ctx){}
void SemanticAnalysis::exitBlockItem(CACTParser::BlockItemContext * ctx){}

void SemanticAnalysis::enterAssignStmt(CACTParser::AssignStmtContext * ctx) {}
void SemanticAnalysis::exitAssignStmt(CACTParser::AssignStmtContext * ctx) 
{
    if(ctx->lVal()->type != ctx->exp()->type|| ctx->lVal()->is_array != ctx->exp()->is_array || (ctx->lVal()->is_array && ctx->exp()->is_array && ctx->lVal()->length != ctx->exp()->length))
    {
        std::cout<<"Error: assignstmt = type error";
        exit(-1);
    }
    if(ctx->lVal()->is_const==1)
    {
        std::cout<<"Error: assign to const";
        exit(-1);
    }
}

void SemanticAnalysis::enterExprStmt(CACTParser::ExprStmtContext * ctx) {}
void SemanticAnalysis::exitExprStmt(CACTParser::ExprStmtContext * ctx) {}

void SemanticAnalysis::enterBlockStmt(CACTParser::BlockStmtContext * ctx) {}
void SemanticAnalysis::exitBlockStmt(CACTParser::BlockStmtContext * ctx) {}

void SemanticAnalysis::enterIfStmt(CACTParser::IfStmtContext * ctx) {}
void SemanticAnalysis::exitIfStmt(CACTParser::IfStmtContext * ctx) {}

void SemanticAnalysis::enterWhileStmt(CACTParser::WhileStmtContext * ctx) 
{
    while_level++;
}
void SemanticAnalysis::exitWhileStmt(CACTParser::WhileStmtContext * ctx) 
{
    while_level--;
}
// TODO: many while and break
void SemanticAnalysis::enterBreakStmt(CACTParser::BreakStmtContext * ctx)
{
    if(while_level == 0)
    {
        std::cout<<"Error: break should be in while block";
        exit(-1);
    }
}
void SemanticAnalysis::exitBreakStmt(CACTParser::BreakStmtContext * ctx) {}

void SemanticAnalysis::enterContinueStmt(CACTParser::ContinueStmtContext * ctx) 
{
    if(while_level == 0)
    {
        std::cout<<"Error: continue should be in while block";
        exit(-1);
    }
}
void SemanticAnalysis::exitContinueStmt(CACTParser::ContinueStmtContext * ctx) {}

void SemanticAnalysis::enterReturnStmt(CACTParser::ReturnStmtContext * ctx) {}
void SemanticAnalysis::exitReturnStmt(CACTParser::ReturnStmtContext * ctx) 
{
    if(ctx->exp()){
        if(ctx->exp()->is_array==1){
            std::cout<<"Error: func return error";
            exit(-1);
        }
        if(ctx->exp()->type != cur_func_table->ret_type){
            std::cout<< cur_func_table->ret_type << std::endl;
            std::cout<<"Error: func return error";
            exit(-1);
        }
    }else{
        if(cur_func_table->ret_type != "void"){
            std::cout<<"Error: func return should be no void";
            exit(-1);
        }
    }
}

void SemanticAnalysis::enterExp(CACTParser::ExpContext * ctx) {}
void SemanticAnalysis::exitExp(CACTParser::ExpContext * ctx) 
{
    if(ctx->BoolConst()){
        ctx->type = TYPE_BOOL;
        ctx->is_array = 0;
        ctx->length = 0;
    }else{
        ctx->type = ctx->addExp()->type;
        ctx->is_array = ctx->addExp()->is_array;
        ctx->length = ctx->addExp()->length;
    }
}

void SemanticAnalysis::enterCond(CACTParser::CondContext * ctx) {}
void SemanticAnalysis::exitCond(CACTParser::CondContext * ctx) 
{
    ctx->type = ctx->lOrExp()->type;
    ctx->is_array = ctx->lOrExp()->is_array;
    ctx->length = ctx->lOrExp()->length;
}

void SemanticAnalysis::enterLVal(CACTParser::LValContext * ctx) {}
void SemanticAnalysis::exitLVal(CACTParser::LValContext * ctx) 
{
    //从符号表中读取信息 如果不存在则ERROR
    std::string name = ctx->Ident()->getText();
    VarInfo *s = cur_table->lookup_name(name);
    if(s==NULL){
        std::cout<< name <<std::endl;
        std::cout<<"Error: not exist this lval";
        exit(-1);
    }
    ctx->type = s->type;
    ctx->is_const = s->is_const;
    ctx->is_array = s->is_array;
    ctx->length = s->array_length;
    //判断
    if(ctx->exp()){
        if(!ctx->is_array){
            std::cout<<"Error: Cannot index a non-array type";
            exit(-1);
        }
        //exp的类型得是int
        if(myStrToInt(ctx->exp()->type) != TYPE_INT){
            std::cout<<"Error: The subscript of the array must be an integer";
            exit(-1);
        }
        //exp的值大于length则越界，不知要不要管。

        ctx->is_array = 0;//表示对数组内的元素赋值，则lval是数组中元素
        ctx->length = 0;
    }
}

void SemanticAnalysis::enterPrimaryExp(CACTParser::PrimaryExpContext * ctx) {}
void SemanticAnalysis::exitPrimaryExp(CACTParser::PrimaryExpContext * ctx) 
{
    if(ctx->exp()){
        ctx->type = ctx->exp()->type;
        ctx->is_array = ctx->exp()->is_array;
        ctx->length = ctx->exp()->length;
    }else if(ctx->lVal()){
        ctx->type = ctx->lVal()->type;
        ctx->is_array = ctx->lVal()->is_array;
        ctx->length = ctx->lVal()->length;
    }else if(ctx->number()){
        ctx->type = ctx->number()->type;
        ctx->is_array = 0;
        ctx->length = 0;
    }
}

void SemanticAnalysis::enterNumber(CACTParser::NumberContext * ctx) {}
void SemanticAnalysis::exitNumber(CACTParser::NumberContext * ctx) 
{
    if(ctx->IntConst()){
        ctx->type = TYPE_INT;
    }else if(ctx->DoubleConst()){
        ctx->type = TYPE_DOUBLE;
    }else if(ctx->FloatConst()){
        ctx->type = TYPE_FLOAT;
    }
}

void SemanticAnalysis::enterPrimaryunaryExp(CACTParser::PrimaryunaryExpContext * ctx) {}
void SemanticAnalysis::exitPrimaryunaryExp(CACTParser::PrimaryunaryExpContext * ctx) 
{
    ctx->type = ctx->primaryExp()->type;
    ctx->is_array = ctx->primaryExp()->is_array;
    ctx->length = ctx->primaryExp()->length;
}

void SemanticAnalysis::enterFuncCallunaryExp(CACTParser::FuncCallunaryExpContext * ctx) {}
void SemanticAnalysis::exitFuncCallunaryExp(CACTParser::FuncCallunaryExpContext * ctx) 
{
    //查找函数的符号表，如果不存在则error
    std::string name = ctx->Ident()->getText();
    FuncInfo *f = func_table->lookup(name);
    if(f==NULL){
        std::cout<<"Error: no such function";
        exit(-1);
    }
    //参数的个数是否匹配，参数的类型是否匹配
    if(f->num_param!=ctx->funcRParams()->exp().size()){
        std::cout<<"Error: the num of parameter of function is wrong";
        exit(-1);
    }
    int index=0;
    for(auto&f_para:ctx->funcRParams()->exp()){
        if(myStrToInt(f_para->type)!=f->param_type[index]||f_para->is_array!=f->param_is_array[index]){
            std::cout<<"the "<<index<<" parameter is not matched";
            exit(-1);
        }
        index++;
    }
    //类型为函数返回值类型
    ctx->type=f->ret_type;
    ctx->is_array=0;
    ctx->length=0;


}

void SemanticAnalysis::enterUnaryOpunaryExp(CACTParser::UnaryOpunaryExpContext * ctx) {}
void SemanticAnalysis::exitUnaryOpunaryExp(CACTParser::UnaryOpunaryExpContext * ctx) 
{
    if(ctx->op->getType() == CACTLexer::NOT && myStrToInt(ctx->unaryExp()->type) != TYPE_BOOL){
        std::cout<<"Error: unaryopunaryexp NOT not bool";
        exit(-1); 
    }
    if(ctx->op->getType() == CACTLexer::ADD && myStrToInt(ctx->unaryExp()->type) == TYPE_BOOL){
        std::cout<<"Error: unaryop ADD bool";
        exit(-1);         
    }
    if(ctx->op->getType() == CACTLexer::SUB && myStrToInt(ctx->unaryExp()->type) == TYPE_BOOL){
        std::cout<<"Error: unaryop SUB bool";
        exit(-1);         
    }
    if(ctx->unaryExp()->is_array==1){
        std::cout<<"Error: opunary used to array";
        exit(-1);         
    }
    ctx->type = ctx->unaryExp()->type;
    ctx->is_array = ctx->unaryExp()->is_array;
    ctx->length = ctx->unaryExp()->length;
}

//void SemanticAnalysis::enterUnaryOp(CACTParser::UnaryOpContext * ctx) {}
//void SemanticAnalysis::exitUnaryOp(CACTParser::UnaryOpContext * ctx) {}

void SemanticAnalysis::enterFuncRParams(CACTParser::FuncRParamsContext * ctx) {}
void SemanticAnalysis::exitFuncRParams(CACTParser::FuncRParamsContext * ctx) {}

void SemanticAnalysis::enterMulExp(CACTParser::MulExpContext * ctx) {}
void SemanticAnalysis::exitMulExp(CACTParser::MulExpContext * ctx) 
{
    if(ctx->mulExp()){
        if(ctx->mulExp()->type != ctx->unaryExp()->type|| ctx->mulExp()->is_array != ctx->unaryExp()->is_array || (ctx->mulExp()->is_array && ctx->unaryExp()->is_array && ctx->mulExp()->length != ctx->unaryExp()->length)){ 
            std::cout<<"Error: mulexp type error";
            exit(-1);              
        }
        if(myStrToInt(ctx->mulExp()->type) == TYPE_BOOL){
            std::cout<<"Error: mulexp bool type";
            exit(-1);  
        }
        if(ctx->op->getType() == CACTLexer::MOD && myStrToInt(ctx->mulExp()->type)!=TYPE_INT){
            std::cout<<"Error: mulexp mod not int";
            exit(-1);  
        }
        ctx->type = ctx->unaryExp()->type;
        ctx->is_array = ctx->unaryExp()->is_array;
        ctx->length = ctx->unaryExp()->length;
    }else{
        ctx->type = ctx->unaryExp()->type;
        ctx->is_array = ctx->unaryExp()->is_array;
        ctx->length = ctx->unaryExp()->length;
    }
}

void SemanticAnalysis::enterAddExp(CACTParser::AddExpContext * ctx) {}
void SemanticAnalysis::exitAddExp(CACTParser::AddExpContext * ctx) 
{
    if(ctx->addExp()){
        if(myStrToInt(ctx->mulExp()->type)!=TYPE_INT&&
        myStrToInt(ctx->mulExp()->type)!=TYPE_FLOAT&&
        myStrToInt(ctx->mulExp()->type)!=TYPE_DOUBLE){
            std::cout<<"Error: This type does not support addition and subtraction";
            exit(-1);
        }
        else if(ctx->addExp()->type!=ctx->mulExp()->type||
          ctx->addExp()->is_array!=ctx->mulExp()->is_array
          ||ctx->addExp()->is_array==ctx->mulExp()->is_array&&
          ctx->addExp()->length!=ctx->mulExp()->length){
              std::cout<<"Error: The types of the two operands of addition and subtraction do not match";
              exit(-1);
          }
    }
    ctx->type=ctx->mulExp()->type;
    ctx->is_array=ctx->mulExp()->is_array;
    ctx->length=ctx->mulExp()->length;
}

void SemanticAnalysis::enterRelExp(CACTParser::RelExpContext * ctx) {}
void SemanticAnalysis::exitRelExp(CACTParser::RelExpContext * ctx) 
{
    //关系运算符用来比较整型和浮点型表达式，结果是bool型
    if(ctx->BoolConst()){
        ctx->type = TYPE_BOOL;
        ctx->is_array = 0;
        ctx->length = 0;
    }else if(ctx->relExp()){
        if(ctx->relExp()->is_array==1 || ctx->addExp()->is_array==1){
            std::cout<<"Error: The array cannot be compared";
            exit(-1);
        }
        if(ctx->relExp()->type != ctx->addExp()->type){
            std::cout<<"> < >= <= type error\n";
        }
        if(ctx->relExp()->type=="bool" || myStrToInt(ctx->addExp()->type)==TYPE_BOOL){
            std::cout<<"Error: The bool cannot be compared";
            exit(-1);
        }
        ctx->type = TYPE_BOOL;
        ctx->is_array = 0;
        ctx->length = 0;
    }else{
        ctx->type = ctx->addExp()->type;
        ctx->is_array = ctx->addExp()->is_array;
        ctx->length = ctx->addExp()->length;
        if(ctx->is_array == 1){
            std::cout<<"Error: array will be used";
            exit(-1);
        }
    }
}

void SemanticAnalysis::enterEqExp(CACTParser::EqExpContext * ctx) {}
void SemanticAnalysis::exitEqExp(CACTParser::EqExpContext * ctx) 
{
    //用于整型，浮点型，布尔型，结果是bool
    if(ctx->eqExp()){
        if(ctx->eqExp()->is_array==1 || ctx->relExp()->is_array==1){
            std::cout<<"Error: The array cannot be compared";
            exit(-1);
        }
        if(ctx->eqExp()->type != ctx->relExp()->type){
            std::cout<<"Error: different types cannot be compared";
            exit(-1);
        }
        ctx->type = TYPE_BOOL;
        ctx->is_array = 0;
        ctx->length = 0;
    }else{
        ctx->type = ctx->relExp()->type;
        ctx->is_array = ctx->relExp()->is_array;
        ctx->length = ctx->relExp()->length;
        if(ctx->is_array == 1){
            std::cout<<"Error: array will be used";
            exit(-1);
        }
    }
}

void SemanticAnalysis::enterLAndExp(CACTParser::LAndExpContext * ctx) {}
void SemanticAnalysis::exitLAndExp(CACTParser::LAndExpContext * ctx) 
{
    if(ctx->lAndExp()){
        if(ctx->lAndExp()->is_array==1 || ctx->eqExp()->is_array==1){
            std::cout<<"Error: The array cannot be and";
            exit(-1);
        }
        if(myStrToInt(ctx->lAndExp()->type) != TYPE_BOOL || myStrToInt(ctx->eqExp()->type) != TYPE_BOOL){
            std::cout<<"Error: not bool type in and";
            exit(-1);
        }
        ctx->type = TYPE_BOOL;
        ctx->is_array = 0;
        ctx->length = 0;
    }else{
        ctx->type = ctx->eqExp()->type;
        ctx->is_array = ctx->eqExp()->is_array;
        ctx->length = ctx->eqExp()->length;
        if(ctx->is_array == 1){
            std::cout<<"Error: array will be used";
            exit(-1);
        }
    }
}

void SemanticAnalysis::enterLOrExp(CACTParser::LOrExpContext * ctx) {}
void SemanticAnalysis::exitLOrExp(CACTParser::LOrExpContext * ctx) 
{
    if(ctx->lOrExp()){
        if(ctx->lOrExp()->is_array==1 || ctx->lAndExp()->is_array==1){
            std::cout<<"Error: The array cannot be or";
            exit(-1);
        }
        if(myStrToInt(ctx->lOrExp()->type) != TYPE_BOOL || myStrToInt(ctx->lAndExp()->type) != TYPE_BOOL){
            std::cout<<"Error: not bool type in or";
            exit(-1);
        }
        ctx->type = TYPE_BOOL;
        ctx->is_array = 0;
        ctx->length = 0;
    }else{
        ctx->type = ctx->lAndExp()->type;
        ctx->is_array = ctx->lAndExp()->is_array;
        ctx->length = ctx->lAndExp()->length;
        if(ctx->is_array == 1){
            std::cout<<"Error: array will be used";
            exit(-1);
        }
    }
}


void SemanticAnalysis::enterConstExpNumber(CACTParser::ConstExpNumberContext * ctx) {}
void SemanticAnalysis::exitConstExpNumber(CACTParser::ConstExpNumberContext * ctx) 
{
    ctx->type = ctx->number()->type;
}

void SemanticAnalysis::enterConstExpBoolConst(CACTParser::ConstExpBoolConstContext * ctx) {}
void SemanticAnalysis::exitConstExpBoolConst(CACTParser::ConstExpBoolConstContext * ctx) 
{
    ctx->type = TYPE_BOOL;
}


void SemanticAnalysis::enterEveryRule(antlr4::ParserRuleContext * ctx) {}
void SemanticAnalysis::exitEveryRule(antlr4::ParserRuleContext * ctx) {}
void SemanticAnalysis::visitTerminal(antlr4::tree::TerminalNode * node) {}

void SemanticAnalysis::visitErrorNode(antlr4::tree::ErrorNode * node)
{
    std::cout << "" << std::endl;
    std::cout << "Compile failed" << std::endl;
    exit(-1);
}



