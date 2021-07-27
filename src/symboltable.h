#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <vector>

enum
{
	TYPE_INT,
	TYPE_DOUBLE,
	TYPE_FLOAT,
	TYPE_BOOL,
	TYPE_VOID
};
enum
{
	CLS_CONST,
	CLS_VAR,
	CLS_ARRAY,
	CLS_FUNC,
	CLS_PARAM,
	CLS_CONST_ARRAY
};

//Var Information
class VarInfo
{
public:
    int type;	   		// class: int double float bool void
	int cls;	   		// cls: const var array param
	int is_array;		// if cls==CLS_ARRAY || cls==CLS_ARRAY, is_array = 1
	int array_length; 		// 仅类型为array时有效
	int is_const;
	std::string name;	//变量名

public:
	VarInfo(std::string var_name, int var_type, int var_cls,int var_is_const, int var_is_array, int var_array_length)
	{
		name = var_name;
		type = var_type;
		cls = var_cls;
		is_const = var_is_const;
		is_array = var_is_array;
		if(cls == CLS_ARRAY||cls == CLS_CONST_ARRAY)
		{
			array_length = var_array_length;
		}
		else
		{
			array_length = 0;
		}
			
	}
};

//Func Information
class FuncInfo
{
public:
	int ret_type;					// class of return value
	int num_param;					// number of paras
	std::vector<int> param_type;	//函数参数类型
	std::vector<std::string> param_name; //函数参数名字
	std::vector<int> param_is_array;
	std::string name;				//函数名

public:
	FuncInfo(std::string func_name, int func_ret_type, int func_num_param, std::vector<int> func_param_type, std::vector<std::string> func_param_name, std::vector<int> func_param_is_array)
	{
		name = func_name;
		num_param = func_num_param;
		ret_type = func_ret_type;
		param_type = func_param_type;
		param_name = func_param_name;
		param_is_array = func_param_is_array;
	}
};

class VarSymbolTable 
{
public:
    std::map<std::string, VarInfo> global_table;
	VarSymbolTable* prev;
	VarSymbolTable(VarSymbolTable* p)	//初始化
	{
		prev = p;
	}

	void add_symbol(std::string name, VarInfo symbol)
	{
		global_table.emplace(name, symbol);
	} 
	//全局查找符号，用于查找函数中所需的某一变量
	VarInfo* lookup_symbol(std::string name)
	{
		for(VarSymbolTable* q = this; q != NULL; q = q->prev)
		{
			auto sign = q->global_table.find(name);
			if(sign != q->global_table.end())
				return &(sign->second);
		}
		return NULL;
	}
	//当前作用域查找符号，用于检查符号命名是否重复
	VarInfo* lookup_name(std::string name)
	{
		VarSymbolTable* q= this;
		auto sign = q->global_table.find(name);
			if(sign != q->global_table.end())
				return &(sign->second);
		return NULL;
	}

	int StringToInt(std::string rt_type)
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

};

class FuncSymbolTable
{
public:
	std::string ret_type;
	std::map<std::string, FuncInfo> func_table;
	FuncSymbolTable() {} //初始化

	void add_func(std::string name, FuncInfo symbol)
	{
		func_table.emplace(name, symbol);
	}

	FuncInfo* lookup(std::string name)
	{
		FuncSymbolTable* q = this;
		auto sign = q->func_table.find(name);
			if(sign != q->func_table.end())
				return &(sign->second);
		return NULL;
	}

	int StringToInt(std::string rt_type)
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
};

#endif
