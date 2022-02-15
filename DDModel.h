#ifndef __DDModel_H
#define __DDModel_H

#include <stdlib.h>
#include <string>
#include <iostream>
#include <set>
#include "testpath.h"

using namespace std;

#define CON_LENGTH 30
#define DD_DEPTH 50
#define CASE_DEPTH 15
#define MAX_CON_NUM 200
#define CALC_BB 20

//struct cond_time_expr;

class dd_calc_node;
//struct ct_result;

//class TestGen;

class dd_node
{
	public:
		//char condition[CON_LENGTH];
		int con_index;
		int process_index;
		int kind;	
		//0 - 普通的中间dd_node节点，1 - case 节点，其condition为它所表示的表达式
		//2 - case 的头节点 3 - dd_var 的头节点
		dd_calc_node * true_ptr;
		dd_calc_node * false_ptr;
		//	dd_node * father_ptr;
		int recycle_num;
		int lineno;
		dd_node()
		{
			kind =0;
			//strcpy(this->condition,"");
			con_index = -1;
			true_ptr = NULL;
			false_ptr = NULL;
			//		father_ptr = NULL;
			recycle_num = 0;
			lineno = -1;
		}
		void set_node_con(int i)//char c[CON_LENGTH])
		{
			//strcpy(this->condition,c);
			con_index = i;
		}
		~dd_node(void);
};

class dd_calc_node
{
public:
	dd_node * dd_node_ptr;
	int rvalue_index[CALC_BB];
	int ri_idx;
	int process_index;
	bool pass_through;
	int lineno[CALC_BB];

	dd_calc_node(void)
	{
		for (int i = 0;i < CALC_BB; i++)
		{
			rvalue_index[i] = -1;
			lineno[i] = -1;
		}
		ri_idx = 0;
		dd_node_ptr = NULL; 
	}
};

class dd_var
{
	public:
		dd_node * var_head_ptr;
		TestGen * _tg;
		string _s,var_name;
		cond_time_expr ce;
		int cycle;
		int _count;
		string _varname;
		int _space;
		
		bool find_path(dd_calc_node * c_node,char TF_values[DD_DEPTH],int final_con_idx);
		bool find_path(dd_node * c_node,char TF_values[DD_DEPTH],int final_con_idx);

		void reset_all_node(dd_node * n);
		
		//bool TF_to_TF_case(char TF[DD_DEPTH],char TF_case[DD_DEPTH]); 
		
		dd_calc_node * find_node_from_TF(const char TF_values[DD_DEPTH]);
		//TF_values 的格式为 <01>([01]* [(case condotion)])*
		
		bool add_dd_node(const char TF_values[DD_DEPTH],int con_idx,int pro_idx,int r_n,bool con_b,int lineno);
	
		//TF_values 此节点后的TF值 r_n 循环次数
		bool add_dd_case_nodes(char TF_values[DD_DEPTH],char cons[CASE_DEPTH],int r_n);
		//TF_values 此节点后的TF值 r_n 循环次数, case 中的各个条件之间用逗号分开，存储在cons中
		bool add_dd_case_node(const char TF_values[DD_DEPTH],int cons,int pro_idx,int r_n,int lineno);
		bool add_dd_case_head_node(const char TF_values[DD_DEPTH],int cons,int pro_idx,int r_n,int lineno);
		
		bool backward_dd_model_first(char TF_values[DD_DEPTH],int final_calc_idx);
		bool backward_dd_model_next(char TF_values[DD_DEPTH],int final_calc_idx);

		dd_calc_node * find_first(string, string, int, TestGen*, int,int);
		dd_calc_node * find_next();
		dd_calc_node * find_def_line_no(dd_node *);

		dd_node * find_dd_node_from_cons(int idx);

		dd_node * find_dd_node_sub(dd_node * ddn,int idx);
		
		bool get_final_value_from_cons(char TF_values[DD_DEPTH],int & final_value_idx);
		bool combine_2_link(dd_node * ddn,const char TF[DD_DEPTH]);

		void dump_ddnode(dd_node* ddn,ostream &o);

		ostream& dump(ostream& o);
		
		dd_var(void)
		{
			this->var_head_ptr = NULL;
		}
		
		~dd_var(void);
		
		void delete_dd_node(dd_node * ddn);	
		void set_name(string);

	private:
		//ct_result get_path_from_var(string, TestGen*);
};

#endif