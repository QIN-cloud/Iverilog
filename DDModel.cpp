#include "DDModel.h"
#include <string>
#include "testpath.h"

using namespace std;

dd_node::~dd_node(void)
{
	
	
	
}

dd_calc_node * dd_var::find_node_from_TF(const char TF_values[DD_DEPTH])
//TF_values 的格式为 <01>([01]* [(case condotion)])*
{
	int i = 1;
	int idx;
	
//	char TF_01[DD_DEPTH];
	char TF_case_con[CON_LENGTH];
	char c[1];
	
	dd_calc_node * t;
	
	if (TF_values[0] == '0')
		t = this->var_head_ptr->false_ptr;
	else
		t = this->var_head_ptr->true_ptr;
	
	while(i < strlen(TF_values))
	{
		if(TF_values[i] == '(')
		{
			i++;
			strcpy(TF_case_con,"");
			while(TF_values[i] != ')')
			{
				c[0] = TF_values[i];
				c[1] = '\0';
				strcat(TF_case_con,c);
				i++;
			}
			idx = atoi(TF_case_con);
			t = t->dd_node_ptr->false_ptr;
			if(t->dd_node_ptr != NULL)
				while((t->dd_node_ptr->con_index != idx))
				{
					t = t->dd_node_ptr->false_ptr;
					if((t == NULL))
						return NULL;
				}
			if (t->dd_node_ptr->kind !=1)
				return NULL;
			t = t->dd_node_ptr->true_ptr;
			i++;
		}
		else
		{
			if(TF_values[i] == '0')
				t = t->dd_node_ptr->false_ptr;
			else
				t = t->dd_node_ptr->true_ptr;
			i++;
		}
	}
	return t;
	
}

void dd_var::delete_dd_node(dd_node * ddn)
{
	if (ddn->true_ptr->dd_node_ptr != NULL)
		delete_dd_node(ddn->true_ptr->dd_node_ptr);
	if (ddn->false_ptr->dd_node_ptr != NULL)
		delete_dd_node(ddn->false_ptr->dd_node_ptr);
	delete ddn->false_ptr;
	delete ddn->true_ptr;
	delete ddn;
}

dd_var::~dd_var(void)
{
	if (this->var_head_ptr == NULL ) return;
	this->delete_dd_node(this->var_head_ptr);
	this->var_head_ptr = NULL;
}

void dd_var::reset_all_node(dd_node * n)
{
	n->false_ptr->pass_through = false;
	n->true_ptr->pass_through = false;
	if (n->true_ptr->dd_node_ptr != NULL)
		reset_all_node(n->true_ptr->dd_node_ptr);
	if (n->false_ptr->dd_node_ptr != NULL)
		reset_all_node(n->false_ptr->dd_node_ptr);
}

bool dd_var::find_path(dd_node * c_node,char TF_values[DD_DEPTH],int final_con_idx)
{
	strcpy(TF_values,"1");
	if(find_path(this->var_head_ptr->true_ptr,TF_values,final_con_idx))
		return true;
	strcpy(TF_values,"0");
	if(find_path(this->var_head_ptr->false_ptr,TF_values,final_con_idx))
		return true;
	strcpy(TF_values,"");
	return false;
}

bool dd_var::find_path(dd_calc_node * c_node,char TF_values[DD_DEPTH],int final_con_idx)//,int level)
{
	if ((c_node->rvalue_index[c_node->ri_idx-1] == final_con_idx)&&(!c_node->pass_through))
	{
		c_node->pass_through = true;
		return true;
	}
	char c[1];
	if (c_node->dd_node_ptr != NULL)
	{
		c[0] = '1';
		c[1] = '\0';
		strcat(TF_values,c);
		if(find_path(c_node->dd_node_ptr->true_ptr,TF_values,final_con_idx))
			return true;
		else
			TF_values[strlen(TF_values)-1] = '\0';
	}
	if (c_node->dd_node_ptr != NULL)
	{
		c[0] = '0';
		c[1] = '\0';
		strcat(TF_values,c);
		if(find_path(c_node->dd_node_ptr->false_ptr,TF_values,final_con_idx))
			return true;
		else
		{
			TF_values[strlen(TF_values)-1] = '\0';
			return false;
		}
	}
	return false;
}


bool dd_var::backward_dd_model_first(char TF_values[DD_DEPTH],int final_con_idx)
{
	this->reset_all_node(this->var_head_ptr);
	char TF[DD_DEPTH];
	strcpy(TF,"");
	if(find_path(this->var_head_ptr,TF,final_con_idx))
	{
//		this->TF_to_TF_case(TF,TF_values);
		strcpy(TF_values,TF);
		return true;
	}
	else
		return false;
}

bool dd_var::backward_dd_model_next(char TF_values[DD_DEPTH],int final_con_idx)
{
	char TF[DD_DEPTH];
	strcpy(TF,"");
	if(find_path(this->var_head_ptr,TF,final_con_idx))
	{
//		this->TF_to_TF_case(TF,TF_values);
		strcpy(TF_values,TF);
		return true;
	}
	else
		return false;
}

bool dd_var::add_dd_case_head_node(const char TF_values[DD_DEPTH],int cons,int pro_idx,int r_n,int lineno)
{
	dd_calc_node * t = this->find_node_from_TF(TF_values);
	
	if(t == NULL)
		return false;

	dd_node * temp;
	dd_calc_node * temp1;
	
	temp = new dd_node;
	temp->set_node_con(cons);
	temp->recycle_num = r_n;
	temp->kind = 2;
	temp->lineno = lineno;
	temp->process_index = pro_idx;
			
	temp1 = new dd_calc_node;
	temp->true_ptr = temp1;					//制作true链
					
	temp1 = new dd_calc_node;
	temp->false_ptr = temp1;					//制作false链

	t->dd_node_ptr = temp;
	t = temp1;
			
	return true;
}

bool dd_var::add_dd_case_node(const char TF_values[DD_DEPTH],int cons,int pro_idx,int r_n,int lineno)
{
	dd_calc_node * t = this->find_node_from_TF(TF_values);
	
	if(t == NULL)
		return false;

	dd_node * temp;
	dd_calc_node * temp1;
	
	while(t->dd_node_ptr != NULL)	
	{
		t = t->dd_node_ptr->false_ptr;
	}

	temp = new dd_node;
	temp->set_node_con(cons);
	temp->recycle_num = r_n;
	temp->kind = 1;
	temp->lineno = lineno;
	temp->process_index = pro_idx;
			
	temp1 = new dd_calc_node;
	temp->true_ptr = temp1;					//制作true链
					
	temp1 = new dd_calc_node;
	temp->false_ptr = temp1;					//制作false链

	t->dd_node_ptr = temp;
	t = temp1;
			
	return true;
}

bool dd_var::add_dd_case_nodes(char TF_values[DD_DEPTH],char cons[CASE_DEPTH],int r_n)
{
	dd_calc_node * t = this->find_node_from_TF(TF_values);
	
	if(t == NULL)
		return false;
	
	char var[CON_LENGTH];
	int i = 0;
	char c[1];
	
	strcpy(var,"");
	
	dd_node * temp;
	dd_calc_node * temp1;
	
	for(; i < strlen(cons) ; i++ )
	{
		c[0] = cons[i];
		c[1] = '\0';
		if (c[0] == ',')
		{
			temp = new dd_node;
			temp->set_node_con(atoi(var));
			temp->recycle_num = r_n;
			temp->kind = 1;
			
			temp1 = new dd_calc_node;
			temp->true_ptr = temp1;					//制作true链
					
			temp1 = new dd_calc_node;
			temp->false_ptr = temp1;					//制作false链

			t->dd_node_ptr = temp;
			t = temp1;
			
			strcpy(var,"");
		}
		else
			strcat(var,c);
	}
	return true;
}


bool dd_var::add_dd_node(const char TF_values[DD_DEPTH],int con_idx,int pro_idx,int r_n,bool con_b,int lineno)
// 0  false 1- true
{
	if (this->var_head_ptr == NULL)
	{
		dd_node * temp;
		dd_calc_node * temp1;
		
		temp = new dd_node;
		temp->set_node_con(con_idx);
		temp->recycle_num = r_n;
		temp->kind = 3;		//头节点
		temp->lineno = lineno;
		temp->process_index = pro_idx;
		this->var_head_ptr = temp;

		temp1 = new dd_calc_node;
		this->var_head_ptr->true_ptr = temp1;//制作true链
				
		temp1 = new dd_calc_node;
		this->var_head_ptr->false_ptr = temp1;//制作false链
		
		return true;				
	}
	
	dd_calc_node * t;
	dd_node * temp;
	dd_calc_node * temp1;

	t = this->find_node_from_TF(TF_values);
	if(t == NULL)
		return false;
	
	if(con_b)
	{
		temp = new dd_node;
		temp->set_node_con(con_idx);
		temp->kind = 0;
		temp->recycle_num =r_n;
		temp->lineno = lineno;
		temp->process_index = pro_idx;
			
		temp1 = new dd_calc_node;
		temp->true_ptr = temp1;					//制作true链
		
		temp1 = new dd_calc_node;
		temp->false_ptr = temp1;					//制作false链

		t->dd_node_ptr = temp;
	}
	else
	{
		t->rvalue_index[t->ri_idx] = con_idx;
		t->lineno[t->ri_idx] = lineno;
		t->process_index = pro_idx;
		t->ri_idx++;

	}
	return true;
}

bool dd_var::get_final_value_from_cons(char TF_values[DD_DEPTH],int & final_value_idx)
{
	dd_calc_node * t = this->find_node_from_TF(TF_values);
	if(t == NULL)
		return false;
	else
	{
		final_value_idx = t->rvalue_index[t->ri_idx];
		return true;
	}
}

bool dd_var::combine_2_link(dd_node * ddn,const char TF[DD_DEPTH])
{
	dd_calc_node * t ;
	t = this->find_node_from_TF(TF);
	if(t == NULL)
		return false;
	t->dd_node_ptr = ddn;
	return true;
}

dd_node * dd_var::find_dd_node_sub(dd_node * ddn,int idx)
{
	dd_node * tmp;
	if((ddn->con_index == idx)&&(ddn->kind != 1))
		return ddn;
	if(ddn->false_ptr->dd_node_ptr != NULL)
	{
		tmp = find_dd_node_sub(ddn->false_ptr->dd_node_ptr,idx);
		if(tmp != NULL)
			return tmp;
		else
			if(ddn->true_ptr->dd_node_ptr != NULL)
				return find_dd_node_sub(ddn->true_ptr->dd_node_ptr,idx);
	}
	return NULL;
}

dd_node * dd_var::find_dd_node_from_cons(int idx)
{
	if(var_head_ptr->true_ptr->dd_node_ptr != NULL)
		return find_dd_node_sub(var_head_ptr->true_ptr->dd_node_ptr,idx);
	else
		return NULL;
}

void dd_var::dump_ddnode(dd_node * ddn,ostream &o)
{
	o << "( control->: " << ddn->lineno << endl << "<t>";
	for(int i = 0 ;i < ddn->true_ptr->ri_idx; i++)
		o << ddn->true_ptr->lineno[i];
	if (ddn->true_ptr->dd_node_ptr != NULL)
		dump_ddnode(ddn->true_ptr->dd_node_ptr,o);
	o << "<t> "<< endl <<"<f>";
	
	for(i = 0 ;i < ddn->false_ptr->ri_idx; i++)
		o << ddn->false_ptr->lineno[i];
	if (ddn->false_ptr->dd_node_ptr != NULL)
		dump_ddnode(ddn->false_ptr->dd_node_ptr,o);
	o << "<f>  )" << endl;
}

ostream& dd_var::dump(ostream& o)
{
	dump_ddnode(var_head_ptr,o);
	return o;
}

dd_calc_node * dd_var::find_def_line_no(dd_node * ddn)
{
	ct_result rv;
	int i = 0;
	dd_node * t_ddn;
	dd_calc_node * t_ddcn;
	t_ddn = ddn;
	if((ddn->kind == 3)&&( _count < 2))//	是头节点
	{
		if(t_ddn->true_ptr->ri_idx > 0)
		{
			if(_count == 0)
				return NULL;
			if(t_ddn->true_ptr->pass_through)
				if(t_ddn->true_ptr->dd_node_ptr != NULL)
					return(this->find_def_line_no(t_ddn->true_ptr->dd_node_ptr));
				else
					return NULL;
			t_ddn->true_ptr->pass_through = true;
			_count--;
			return t_ddn->true_ptr;
		}
		else
			if(ddn->true_ptr->dd_node_ptr != NULL)
				return(find_def_line_no(ddn->true_ptr->dd_node_ptr));
			else
				return NULL;
	}
	else
		if((_count > 1)&&(ddn->kind == 3))
		{
			//if(t_ddn->true_ptr->pass_through)
				if(t_ddn->true_ptr->dd_node_ptr != NULL)
					return(this->find_def_line_no(t_ddn->true_ptr->dd_node_ptr));
		}
	//ce = _tg->generate_ce(_s, ddn->con_index, ddn->process_index);
	//if(_count == 3 || _count == 2)
	//	rv = _tg->compare_cetimetable(_s, ddn->con_index, ddn->process_index, ce, cycle);
	//else
	//rv = _tg->compare_cetimetable(_s, _varname, _space, ddn->con_index, ddn->process_index, ce, cycle);
 	if(ddn->kind == 2)	//case 头节点
	{
		if(rv.itemindex != -1)
		{
			for(i = 0; i <= rv.itemindex ; i++)
				t_ddn = t_ddn->false_ptr->dd_node_ptr;
			if(t_ddn->true_ptr->ri_idx > 0)
			{
				//if(_count == 0)
				//	return NULL;
				if(t_ddn->true_ptr->pass_through)
					if(t_ddn->true_ptr->dd_node_ptr != NULL)
						return(this->find_def_line_no(t_ddn->true_ptr->dd_node_ptr));
					else
						return NULL;
				t_ddn->true_ptr->pass_through = true;
			//	_count--;
				return t_ddn->true_ptr;
			}
			else
				if(t_ddn->true_ptr->dd_node_ptr != NULL)
					return (this->find_def_line_no(t_ddn->true_ptr->dd_node_ptr));
				else 
					return NULL;
		}
	}
	else
	{
		//rv = _tg->compare_cetimetable(_s, ddn->con_index, ddn->process_index, ce, cycle);
		if(rv.itemindex == -1)
		{
			if(rv.result)
			{
				t_ddcn = t_ddn->true_ptr;
				t_ddn = t_ddn->true_ptr->dd_node_ptr;
			}
			else
			{
				t_ddcn = t_ddn->false_ptr;
				t_ddn = t_ddn->false_ptr->dd_node_ptr;
			}
			if(t_ddcn->ri_idx > 0)
			{
				//if(_count == 0)
				//	return NULL;
				if(t_ddcn->pass_through)
					if(t_ddn != NULL)
						return(this->find_def_line_no(t_ddn));
					else
						return NULL;
				t_ddcn->pass_through = true;
			//	_count--;
				return t_ddcn;
			}
			else
				if(t_ddn != NULL)
					return (this->find_def_line_no(t_ddn));
				else
					return NULL;
		}
	}
}

dd_calc_node * dd_var::find_first(string modname, string varname, int space, TestGen* tg, int cycle,int count)
{
	this->reset_all_node(this->var_head_ptr);
	this->_s = modname;
	this->_tg = tg;
	//this->ce = ce;
	this->cycle = cycle;
	this->_count = count;
	this->_varname = varname;
	this->_space = space;

	return(this->find_def_line_no(this->var_head_ptr));
}

dd_calc_node * dd_var::find_next()
{
	return(this->find_def_line_no(this->var_head_ptr));
}

void dd_var::set_name(string name)
{
	this->var_name = name;
}

/*
ct_result dd_var::get_path_from_var(string modname, TestGen* tg)
{
	ct_result temp;
	temp.itemindex = -1;
	temp.result = true;

	return temp;
}

void main(void)
{
	dd_var a;

	char tf[DD_DEPTH],fv[CON_LENGTH];
	int i;
	a.add_dd_node("",111,0,true);
	
	a.add_dd_case_nodes("1","0,1,2,",0);
	a.add_dd_node("1(0)",222,0,true);
	a.add_dd_node("1(0)",220,0,false);
	a.add_dd_node("1(2)",333,0,true);
	a.add_dd_node("1(2)",330,0,false);
	a.add_dd_case_nodes("1(1)","0,1,",0);
	a.backward_dd_model_first(tf,220);
	a.backward_dd_model_first(tf,330);
	//a.backward_dd_model_first(tf,"1 b cb 1");
	a.backward_dd_model_next(tf,"false1 true2");

	a.~dd_var();
	
	
};
*/
