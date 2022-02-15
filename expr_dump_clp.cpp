#include "config.h"

/*
* This file contains all the dump methods of the netlist classes.
*/
#include <typeinfo>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include "netlist.h"
#include "cfg.h"
#include "testpath.h"

/*
int power(int base, int mi)
{
	if(mi == 0)
		return 1;
	int res = base;
	for(unsigned idx = 1; idx < mi; ++idx)
		res = res * base;
	return res;
}
*/

/* Dump an expression that no one wrote a dump method for. */
clp_var NetExpr::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	o << lineno << "(?" << typeid(*this).name() << "?)";
	clp_var tmp;
	return tmp;
}

clp_var NetEBinary::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	clp_var left =	left_->dump_cond_clp(o, lineno, refs, used, expr, temp_idx);
	clp_var right = right_->dump_cond_clp(o, lineno, refs, used, expr, temp_idx);
	clp_var var;
	ostringstream tmp;

	switch (op_){
	case '+':
	case '-':
	case '*':
	case '/':
	case '%'://for all the arithmetic operations
		var.basename = left.basename + "_" + right.basename;
		tmp << temp_idx;
		temp_idx += 1;
		var.clpname = "T_" + left.clpname + "_" + right.clpname + "_" + tmp.str();
		var.index = -1;
		var.width = expr_width();
		var.msb = expr_width() - 1;
		var.lsb = 0;
		var.parentname = "";
		var.type = "NOINPUT";
		
		if(op_ == '%')
			expr << var.clpname << " #= " << left.clpname << " " << "rem" << " " << right.clpname << "," << endl;
		else
			expr << var.clpname << " #= " << left.clpname << " " << op_ << " " << right.clpname << "," << endl;
		used.insert(var);
		
		return var;
		
		break;
	case '^'://Bit-wise exclusive OR
		if((left.width == 1) && (right.width == 1))
		{
			//first new a 1-bit variable;
			var = left;
			var.type = "NOINPUT";
			tmp << temp_idx;
			temp_idx += 1;
			var.clpname = var.clpname + "_" + right.clpname + "_" + tmp.str();
			used.insert(var);
			expr << var.clpname << " #<=> " << left.clpname << " ## " << right.clpname << "," << endl;
			used.insert(var);
			return var;
		}
		else if((left.width > 1) || (right.width > 1))
		{
			//first new a 1-bit variable;
			string expr1, expr2, expr3;
			ostringstream os;
			var = left;
			var.type = "NOINPUT";
			tmp << temp_idx;
			temp_idx += 1;
			var.clpname = var.clpname + "_" + right.clpname + "_" + tmp.str();
			var.width = left.width > right.width ? left.width : right.width;
			var.lsb = 0;
			var.msb = var.width - 1;
			expr1 = left.clpname + " #<=> ";
			expr2 = right.clpname + " #<=> ";
			expr3 = var.clpname + " #<=> ";

			unsigned itmp1 = var.lsb > var.msb ? var.msb : var.lsb;
			unsigned itmp2 = var.lsb > var.msb ? var.lsb : var.msb;
			unsigned left_max = left.lsb > left.msb ? left.lsb : left.msb;
			unsigned right_max = right.lsb > right.msb ? right.lsb : right.msb;

			bool left_order = left.lsb > left.msb ? false : true;
			bool right_order = right.lsb > right.msb ? false : true;
			bool var_order = var.lsb > var.msb ? false : true;

			for(unsigned idx = itmp1; idx <= itmp2; ++idx)
			{
				//When the operands are of unequal bit length, 
				//the shorter operand is zero-filled in the most significant bit positions.
				clp_var tmp_left, tmp_right, tmp_var;
				char* left_str = new char[3];
				char* right_str = new char[3];
				char* var_str = new char[3];
				
				if(left_order)
					sprintf(left_str, "%d", idx);
				else
					sprintf(left_str, "%d", left_max - idx);

				if(right_order)
					sprintf(right_str, "%d", idx);
				else
					sprintf(right_str, "%d", right_max - idx);

				if(var_order)
					sprintf(var_str, "%d", idx);
				else
					sprintf(var_str, "%d", itmp2 - idx);

				tmp_var.basename = var.basename;
				tmp_var.clpname = var.clpname + "_" + var_str;
				tmp_var.index = var_order ? idx : itmp2 - idx;
				tmp_var.lsb = var_order ? idx : itmp2 - idx;;
				tmp_var.msb = var_order ? idx : itmp2 - idx;;
				tmp_var.width = 1;
				tmp_var.parentname = var.parentname;

				used.insert(tmp_var);
				
				int i;
				i = pow(2, idx);
				ostringstream os;


				os.flush();
				os << i;
				if(idx == itmp2)
					expr3 = expr3 + os.str() + " * " + tmp_var.clpname + ",";
				else
					expr3 = expr3 + os.str() + " * " + tmp_var.clpname + " + ";

				if(idx <= left_max)
				{
					tmp_left.basename = left.basename;
					tmp_left.clpname = left.clpname + "_" + left_str;
					tmp_left.index = left_order ? idx : left_max - idx;;
					tmp_left.lsb = left_order ? idx : left_max - idx;;
					tmp_left.msb = left_order ? idx : left_max - idx;;
					tmp_left.width = 1;
					tmp_left.parentname = left.parentname;

					if(idx == left_max)
						expr1 = expr1 + os.str() + " * " + tmp_left.clpname + ",";
					else
						expr1 = expr1 + os.str() + " * " + tmp_left.clpname + " + ";

					used.insert(tmp_left);
				}
				
				if(idx <= right_max)
				{
					tmp_right.basename = right.basename;
					tmp_right.clpname = right.clpname + "_" + right_str;
					tmp_right.index = right_order ? idx : right_max - idx;;
					tmp_right.lsb = right_order ? idx : right_max - idx;;
					tmp_right.msb = right_order ? idx : right_max - idx;;
					tmp_right.width = 1;
					tmp_right.parentname = right.parentname;

					if(idx == right_max)
						expr2 = expr2 + os.str() + " * " + tmp_right.clpname + ",";
					else
						expr2 = expr2 + os.str() + " * " + tmp_right.clpname + " + ";

					used.insert(tmp_right);
				}

				if(idx <= left_max)
				{
					if(idx <= right_max)
					{
						expr << tmp_var.clpname << " #<=> " << tmp_left.clpname << " ## " << tmp_right.clpname << "," << endl;
					}
					else
					{
						expr << tmp_var.clpname << " #<=> " << tmp_left.clpname << " ## 0" << "," << endl;
					}
				}
				else
				{
					if(idx <= right_max)
					{
						expr << tmp_var.clpname << " #<=> " << "0 ## " << tmp_right.clpname << "," << endl;
					}
				}
			}
			
			//dump expr for ~
			expr << expr1 << endl;
			expr << expr2 << endl;
			expr << expr3 << endl;
			
			used.insert(var);
			return var;
		}

		break;
	case '&'://Bit-wise AND
		if((left.width == 1) && (right.width == 1))
		{
			//first new a 1-bit variable;
			var = left;
			var.type = "NOINPUT";
			tmp << temp_idx;
			temp_idx += 1;
			var.clpname = var.clpname + "_" + right.clpname + "_" + tmp.str();
			used.insert(var);
			expr << var.clpname << " #<=> " << left.clpname << " #/\\ " << right.clpname << "," << endl;
			used.insert(var);
			return var;
		}
		else if((left.width > 1) || (right.width > 1))
		{
			//first new a 1-bit variable;
			string expr1, expr2, expr3;
			ostringstream os;
			var = left;
			var.type = "NOINPUT";
			tmp << temp_idx;
			temp_idx += 1;
			var.clpname = var.clpname + "_" + right.clpname + "_" + tmp.str();
			var.type = "NOINPUT";
			var.width = left.width > right.width ? left.width : right.width;
			var.lsb = 0;
			var.msb = var.width - 1;
			expr1 = left.clpname + " #<=> ";
			expr2 = right.clpname + " #<=> ";
			expr3 = var.clpname + " #<=> ";

			unsigned itmp1 = var.lsb > var.msb ? var.msb : var.lsb;
			unsigned itmp2 = var.lsb > var.msb ? var.lsb : var.msb;
			unsigned left_max = left.lsb > left.msb ? left.lsb : left.msb;
			unsigned right_max = right.lsb > right.msb ? right.lsb : right.msb;

			bool left_order = left.lsb > left.msb ? false : true;
			bool right_order = right.lsb > right.msb ? false : true;
			bool var_order = var.lsb > var.msb ? false : true;

			for(unsigned idx = itmp1; idx <= itmp2; ++idx)
			{
				//When the operands are of unequal bit length, 
				//the shorter operand is zero-filled in the most significant bit positions.
				clp_var tmp_left, tmp_right, tmp_var;
				char* left_str = new char[3];
				char* right_str = new char[3];
				char* var_str = new char[3];
				
				if(left_order)
					sprintf(left_str, "%d", idx);
				else
					sprintf(left_str, "%d", left_max - idx);

				if(right_order)
					sprintf(right_str, "%d", idx);
				else
					sprintf(right_str, "%d", right_max - idx);

				if(var_order)
					sprintf(var_str, "%d", idx);
				else
					sprintf(var_str, "%d", itmp2 - idx);

				tmp_var.basename = var.basename;
				tmp_var.clpname = var.clpname + "_" + var_str;
				tmp_var.type = "NOINPUT";
				tmp_var.index = var_order ? idx : itmp2 - idx;
				tmp_var.lsb = var_order ? idx : itmp2 - idx;;
				tmp_var.msb = var_order ? idx : itmp2 - idx;;
				tmp_var.width = 1;
				tmp_var.parentname = var.parentname;

				used.insert(tmp_var);
				
				int i;
				i = pow(2, idx);
				ostringstream os;


				os.flush();
				os << i;
				if(idx == itmp2)
					expr3 = expr3 + os.str() + " * " + tmp_var.clpname + ",";
				else
					expr3 = expr3 + os.str() + " * " + tmp_var.clpname + " + ";

				if(idx <= left_max)
				{
					tmp_left.basename = left.basename;
					tmp_left.clpname = left.clpname + "_" + left_str;
					tmp_left.index = left_order ? idx : left_max - idx;;
					tmp_left.lsb = left_order ? idx : left_max - idx;;
					tmp_left.msb = left_order ? idx : left_max - idx;;
					tmp_left.width = 1;
					tmp_left.parentname = left.parentname;

					if(idx == left_max)
						expr1 = expr1 + os.str() + " * " + tmp_left.clpname + ",";
					else
						expr1 = expr1 + os.str() + " * " + tmp_left.clpname + " + ";

					used.insert(tmp_left);
				}
				
				if(idx <= right_max)
				{
					tmp_right.basename = right.basename;
					tmp_right.clpname = right.clpname + "_" + right_str;
					tmp_right.index = right_order ? idx : right_max - idx;;
					tmp_right.lsb = right_order ? idx : right_max - idx;;
					tmp_right.msb = right_order ? idx : right_max - idx;;
					tmp_right.width = 1;
					tmp_right.parentname = right.parentname;

					if(idx == right_max)
						expr2 = expr2 + os.str() + " * " + tmp_right.clpname + ",";
					else
						expr2 = expr2 + os.str() + " * " + tmp_right.clpname + " + ";

					used.insert(tmp_right);
				}

				if(idx <= left_max)
				{
					if(idx <= right_max)
					{
						expr << tmp_var.clpname << " #<=> " << tmp_left.clpname << " #/\\ " << tmp_right.clpname << "," << endl;
					}
					else
					{
						expr << tmp_var.clpname << " #<=> " << tmp_left.clpname << " #/\\" << " 0" << "," << endl;
					}
				}
				else
				{
					if(idx <= right_max)
					{
						expr << tmp_var.clpname << " #<=> " << "0 #/\\ " << tmp_right.clpname << "," << endl;
					}
				}
			}
			
			//dump expr for ~
			expr << expr1 << endl;
			expr << expr2 << endl;
			expr << expr3 << endl;
			
			used.insert(var);
			return var;
		}

		break;
	case '|'://Bit-wise OR
		if((left.width == 1) && (right.width == 1))
		{
			//first new a 1-bit variable;
			var = left;
			var.type = "NOINPUT";
			tmp << temp_idx;
			temp_idx += 1;
			var.clpname = var.clpname + "_" + right.clpname + "_" + tmp.str();
			used.insert(var);
			expr << var.clpname << " #<=> " << left.clpname << " #\\" << "/ " << right.clpname << "," << endl;
			used.insert(var);
			return var;
		}
		else if((left.width > 1) || (right.width > 1))
		{
			//first new a 1-bit variable;
			string expr1, expr2, expr3;
			ostringstream os;
			var = left;
			var.type = "NOINPUT";
			tmp << temp_idx;
			temp_idx += 1;
			var.clpname = var.clpname + "_" + right.clpname + "_" + tmp.str();
			var.width = left.width > right.width ? left.width : right.width;
			var.lsb = 0;
			var.msb = var.width - 1;
			expr1 = left.clpname + " #<=> ";
			expr2 = right.clpname + " #<=> ";
			expr3 = var.clpname + " #<=> ";

			unsigned itmp1 = var.lsb > var.msb ? var.msb : var.lsb;
			unsigned itmp2 = var.lsb > var.msb ? var.lsb : var.msb;
			unsigned left_max = left.lsb > left.msb ? left.lsb : left.msb;
			unsigned right_max = right.lsb > right.msb ? right.lsb : right.msb;

			bool left_order = left.lsb > left.msb ? false : true;
			bool right_order = right.lsb > right.msb ? false : true;
			bool var_order = var.lsb > var.msb ? false : true;

			for(unsigned idx = itmp1; idx <= itmp2; ++idx)
			{
				//When the operands are of unequal bit length, 
				//the shorter operand is zero-filled in the most significant bit positions.
				clp_var tmp_left, tmp_right, tmp_var;
				char* left_str = new char[3];
				char* right_str = new char[3];
				char* var_str = new char[3];
				
				if(left_order)
					sprintf(left_str, "%d", idx);
				else
					sprintf(left_str, "%d", left_max - idx);

				if(right_order)
					sprintf(right_str, "%d", idx);
				else
					sprintf(right_str, "%d", right_max - idx);

				if(var_order)
					sprintf(var_str, "%d", idx);
				else
					sprintf(var_str, "%d", itmp2 - idx);

				tmp_var.basename = var.basename;
				tmp_var.clpname = var.clpname + "_" + var_str;
				tmp_var.index = var_order ? idx : itmp2 - idx;
				tmp_var.lsb = var_order ? idx : itmp2 - idx;;
				tmp_var.msb = var_order ? idx : itmp2 - idx;;
				tmp_var.width = 1;
				tmp_var.parentname = var.parentname;

				used.insert(tmp_var);
				
				int i;
				i = pow(2, idx);
				ostringstream os;


				os.flush();
				os << i;
				if(idx == itmp2)
					expr3 = expr3 + os.str() + " * " + tmp_var.clpname + ",";
				else
					expr3 = expr3 + os.str() + " * " + tmp_var.clpname + " + ";

				if(idx <= left_max)
				{
					tmp_left.basename = left.basename;
					tmp_left.clpname = left.clpname + "_" + left_str;
					tmp_left.index = left_order ? idx : left_max - idx;;
					tmp_left.lsb = left_order ? idx : left_max - idx;;
					tmp_left.msb = left_order ? idx : left_max - idx;;
					tmp_left.width = 1;
					tmp_left.parentname = left.parentname;

					if(idx == left_max)
						expr1 = expr1 + os.str() + " * " + tmp_left.clpname + ",";
					else
						expr1 = expr1 + os.str() + " * " + tmp_left.clpname + " + ";

					used.insert(tmp_left);
				}
				
				if(idx <= right_max)
				{
					tmp_right.basename = right.basename;
					tmp_right.clpname = right.clpname + "_" + right_str;
					tmp_right.index = right_order ? idx : right_max - idx;;
					tmp_right.lsb = right_order ? idx : right_max - idx;;
					tmp_right.msb = right_order ? idx : right_max - idx;;
					tmp_right.width = 1;
					tmp_right.parentname = right.parentname;

					if(idx == right_max)
						expr2 = expr2 + os.str() + " * " + tmp_right.clpname + ",";
					else
						expr2 = expr2 + os.str() + " * " + tmp_right.clpname + " + ";

					used.insert(tmp_right);
				}

				if(idx <= left_max)
				{
					if(idx <= right_max)
					{
						expr << tmp_var.clpname << " #<=> " << tmp_left.clpname << " #\\" << "/ " << tmp_right.clpname << "," << endl;
					}
					else
					{
						expr << tmp_var.clpname << " #<=> " << tmp_left.clpname << " #\\" << "/ 0" << "," << endl;
					}
				}
				else
				{
					if(idx <= right_max)
					{
						expr << tmp_var.clpname << " #<=> " << "0 #\\" << "/ " << tmp_right.clpname << "," << endl;
					}
				}
			}
			
			//dump expr for ~
			expr << expr1 << endl;
			expr << expr2 << endl;
			expr << expr3 << endl;
			
			used.insert(var);
			return var;
		}

		break;
	case '<'://Less than
		var.basename = left.basename + "_" + right.basename;
		var.type = "NOINPUT";
		tmp << temp_idx;
		temp_idx += 1;
		var.clpname = "T_" + left.clpname + "_" + right.clpname + "_" + tmp.str();
		var.index = -1;
		var.width = expr_width();
		var.msb = expr_width() - 1;
		var.lsb = 0;
		var.parentname = "";
		
		expr << var.clpname << " #<=> " << left.clpname << " #< " << right.clpname << "," << endl;
		used.insert(var);
		
		return var;
		
		break;
	case '>'://Greater than
		var.basename = left.basename + "_" + right.basename;
		var.type = "NOINPUT";
		tmp << temp_idx;
		temp_idx += 1;
		var.clpname = "T_" + left.clpname + "_" + right.clpname + "_" + tmp.str();
		var.index = -1;
		var.width = expr_width();
		var.msb = expr_width() - 1;
		var.lsb = 0;
		var.parentname = "";
		
		expr << var.clpname << " #<=> " << left.clpname << " #> " << right.clpname << "," << endl;
		used.insert(var);
		
		return var;
		
		break;
	case 'a'://Logical AND (&&)
		var.basename = left.basename + "_" + right.basename;
		var.type = "NOINPUT";
		tmp << temp_idx;
		temp_idx += 1;
		var.clpname = "T_" + left.clpname + "_" + right.clpname + "_" + tmp.str();
		var.index = -1;
		var.width = expr_width();
		var.msb = expr_width() - 1;
		var.lsb = 0;
		var.parentname = "";
		
		expr << var.clpname << " #<=> " << left.clpname << " #/\\ " << right.clpname << "," << endl;
		used.insert(var);
		
		return var;
		
		break;
	case 'A'://Bitwise NAND (~&)
		if((left.width == 1) && (right.width == 1))
		{
			//first new a 1-bit variable;
			var = left;
			var.type = "NOINPUT";
			tmp << temp_idx;
			temp_idx += 1;
			var.clpname = var.clpname + "_" + right.clpname + "_" + tmp.str();
			used.insert(var);
			expr << var.clpname << " #<=> " << left.clpname << " #\\" << "/\\ " << right.clpname << "," << endl;
			used.insert(var);
			return var;
		}
		else if((left.width > 1) || (right.width > 1))
		{
			//first new a 1-bit variable;
			string expr1, expr2, expr3;
			ostringstream os;
			var = left;
			var.type = "NOINPUT";
			tmp << temp_idx;
			temp_idx += 1;
			var.clpname = var.clpname + "_" + right.clpname + "_" + tmp.str();
			var.width = left.width > right.width ? left.width : right.width;
			var.lsb = 0;
			var.msb = var.width - 1;
			expr1 = left.clpname + " #<=> ";
			expr2 = right.clpname + " #<=> ";
			expr3 = var.clpname + " #<=> ";

			unsigned itmp1 = var.lsb > var.msb ? var.msb : var.lsb;
			unsigned itmp2 = var.lsb > var.msb ? var.lsb : var.msb;
			unsigned left_max = left.lsb > left.msb ? left.lsb : left.msb;
			unsigned right_max = right.lsb > right.msb ? right.lsb : right.msb;

			bool left_order = left.lsb > left.msb ? false : true;
			bool right_order = right.lsb > right.msb ? false : true;
			bool var_order = var.lsb > var.msb ? false : true;

			for(unsigned idx = itmp1; idx <= itmp2; ++idx)
			{
				//When the operands are of unequal bit length, 
				//the shorter operand is zero-filled in the most significant bit positions.
				clp_var tmp_left, tmp_right, tmp_var;
				char* left_str = new char[3];
				char* right_str = new char[3];
				char* var_str = new char[3];
				
				if(left_order)
					sprintf(left_str, "%d", idx);
				else
					sprintf(left_str, "%d", left_max - idx);

				if(right_order)
					sprintf(right_str, "%d", idx);
				else
					sprintf(right_str, "%d", right_max - idx);

				if(var_order)
					sprintf(var_str, "%d", idx);
				else
					sprintf(var_str, "%d", itmp2 - idx);

				tmp_var.basename = var.basename;
				tmp_var.clpname = var.clpname + "_" + var_str;
				tmp_var.index = var_order ? idx : itmp2 - idx;
				tmp_var.lsb = var_order ? idx : itmp2 - idx;;
				tmp_var.msb = var_order ? idx : itmp2 - idx;;
				tmp_var.width = 1;
				tmp_var.parentname = var.parentname;

				used.insert(tmp_var);
				
				int i;
				i = pow(2, idx);
				ostringstream os;


				os.flush();
				os << i;
				if(idx == itmp2)
					expr3 = expr3 + os.str() + " * " + tmp_var.clpname + ",";
				else
					expr3 = expr3 + os.str() + " * " + tmp_var.clpname + " + ";

				if(idx <= left_max)
				{
					tmp_left.basename = left.basename;
					tmp_left.clpname = left.clpname + "_" + left_str;
					tmp_left.index = left_order ? idx : left_max - idx;;
					tmp_left.lsb = left_order ? idx : left_max - idx;;
					tmp_left.msb = left_order ? idx : left_max - idx;;
					tmp_left.width = 1;
					tmp_left.parentname = left.parentname;

					if(idx == left_max)
						expr1 = expr1 + os.str() + " * " + tmp_left.clpname + ",";
					else
						expr1 = expr1 + os.str() + " * " + tmp_left.clpname + " + ";

					used.insert(tmp_left);
				}
				
				if(idx <= right_max)
				{
					tmp_right.basename = right.basename;
					tmp_right.clpname = right.clpname + "_" + right_str;
					tmp_right.index = right_order ? idx : right_max - idx;;
					tmp_right.lsb = right_order ? idx : right_max - idx;;
					tmp_right.msb = right_order ? idx : right_max - idx;;
					tmp_right.width = 1;
					tmp_right.parentname = right.parentname;

					if(idx == right_max)
						expr2 = expr2 + os.str() + " * " + tmp_right.clpname + ",";
					else
						expr2 = expr2 + os.str() + " * " + tmp_right.clpname + " + ";

					used.insert(tmp_right);
				}

				if(idx <= left_max)
				{
					if(idx <= right_max)
					{
						expr << tmp_var.clpname << " #<=> " << tmp_left.clpname << " #\\" << "/\\ " << tmp_right.clpname << "," << endl;
					}
					else
					{
						expr << tmp_var.clpname << " #<=> " << tmp_left.clpname << " #\\" << "/\\" << " 0" << "," << endl;
					}
				}
				else
				{
					if(idx <= right_max)
					{
						expr << tmp_var.clpname << " #<=> " << "0 #\\" << "/\\ " << tmp_right.clpname << "," << endl;
					}
				}
			}
			
			//dump expr for ~
			expr << expr1 << endl;
			expr << expr2 << endl;
			expr << expr3 << endl;
			
			used.insert(var);
			return var;
		}

		break;
	case 'E'://Case equality (===)
		cerr << "Unsupported operation for test generation : ===" << endl;
		exit(1);
		break;
	case 'e'://Logical equality (==)
		var.basename = left.basename + "_" + right.basename;
		var.type = "NOINPUT";
		tmp << temp_idx;
		temp_idx += 1;
		var.clpname = "T_" + left.clpname + "_" + right.clpname + "_" + tmp.str();
		var.index = -1;
		var.width = expr_width();
		var.msb = expr_width() - 1;
		var.lsb = 0;
		var.parentname = "";
		
		expr << var.clpname << " #<=> " << left.clpname << " #= " << right.clpname << "," << endl;
		used.insert(var);
		
		return var;
		
		break;
	case 'G'://Greater or equal
		var.basename = left.basename + "_" + right.basename;
		var.type = "NOINPUT";
		tmp << temp_idx;
		temp_idx += 1;
		var.clpname = "T_" + left.clpname + "_" + right.clpname + "_" + tmp.str();
		var.index = -1;
		var.width = expr_width();
		var.msb = expr_width() - 1;
		var.lsb = 0;
		var.parentname = "";
		
		expr << var.clpname << " #<=> " << left.clpname << " #>= " << right.clpname << "," << endl;
		used.insert(var);
		
		return var;
		
		break;
	case 'l'://Left shift (<<)
		var.basename = left.basename + "_" + right.basename;
		var.type = "NOINPUT";
		tmp << temp_idx;
		temp_idx += 1;
		var.clpname = "T_" + left.clpname + "_" + right.clpname + "_" + tmp.str();
		var.index = -1;
		var.width = expr_width();
		var.msb = expr_width() - 1;
		var.lsb = 0;
		var.parentname = "";
		
		expr << var.clpname << " #= " << left.clpname << "* (2 ** " << right.clpname << ")," << endl;
		used.insert(var);
		
		return var;
		
		break;
	case 'L'://Less or equal
		var.basename = left.basename + "_" + right.basename;
		var.type = "NOINPUT";
		tmp << temp_idx;
		temp_idx += 1;
		var.clpname = "T_" + left.clpname + "_" + right.clpname + "_" + tmp.str();
		var.index = -1;
		var.width = expr_width();
		var.msb = expr_width() - 1;
		var.lsb = 0;
		var.parentname = "";
		
		expr << var.clpname << " #<=> " << left.clpname << " #=< " << right.clpname << "," << endl;
		used.insert(var);
		
		return var;
		
		break;
	case 'n'://Logical inequality (!=) 
		var.basename = left.basename + "_" + right.basename;
		var.type = "NOINPUT";
		tmp << temp_idx;
		temp_idx += 1;
		var.clpname = "T_" + left.clpname + "_" + right.clpname + "_" + tmp.str();
		var.index = -1;
		var.width = expr_width();
		var.msb = expr_width() - 1;
		var.lsb = 0;
		var.parentname = "";
		
		expr << var.clpname << " #<=> " << left.clpname << " #\\" << "= " << right.clpname << "," << endl;
		used.insert(var);
		
		return var;
		
		break;
	case 'N'://Case inequality (!==)
		cerr << "Unsupported operation for test generation : !== " << endl;
		exit(1);
		break;
	case 'o'://Logical OR (||)
		var.basename = left.basename + "_" + right.basename;
		var.type = "NOINPUT";
		tmp << temp_idx;
		temp_idx += 1;
		var.clpname = "T_" + left.clpname + "_" + right.clpname + "_" + tmp.str();
		var.index = -1;
		var.width = expr_width();
		var.msb = expr_width() - 1;
		var.lsb = 0;
		var.parentname = "";
		
		expr << var.clpname << " #<=> " << left.clpname << " #\\" << "/ " << right.clpname << "," << endl;
		used.insert(var);
		
		return var;
		
		break;
	case 'O'://Bit-wise NOR (~|) 
		if((left.width == 1) && (right.width == 1))
		{
			//first new a 1-bit variable;
			var = left;
			var.type = "NOINPUT";
			tmp << temp_idx;
			temp_idx += 1;
			var.clpname = var.clpname + "_" + right.clpname + "_" + tmp.str();
			used.insert(var);
			expr << var.clpname << " #<=> " << left.clpname << " #\\" << "\\" << "/ " << right.clpname << "," << endl;
			used.insert(var);
			return var;
		}
		else if((left.width > 1) || (right.width > 1))
		{
			//first new a 1-bit variable;
			string expr1, expr2, expr3;
			ostringstream os;
			var = left;
			var.type = "NOINPUT";
			tmp << temp_idx;
			temp_idx += 1;
			var.clpname = var.clpname + "_" + right.clpname + "_" + tmp.str();
			var.width = left.width >= right.width ? left.width : right.width;
			var.lsb = 0;
			var.msb = var.width - 1;
			expr1 = left.clpname + " #<=> ";
			expr2 = right.clpname + " #<=> ";
			expr3 = var.clpname + " #<=> ";

			unsigned itmp1 = var.lsb > var.msb ? var.msb : var.lsb;
			unsigned itmp2 = var.lsb > var.msb ? var.lsb : var.msb;
			unsigned left_max = left.lsb > left.msb ? left.lsb : left.msb;
			unsigned right_max = right.lsb > right.msb ? right.lsb : right.msb;

			bool left_order = left.lsb > left.msb ? false : true;
			bool right_order = right.lsb > right.msb ? false : true;
			bool var_order = var.lsb > var.msb ? false : true;

			for(unsigned idx = itmp1; idx <= itmp2; ++idx)
			{
				//When the operands are of unequal bit length, 
				//the shorter operand is zero-filled in the most significant bit positions.
				clp_var tmp_left, tmp_right, tmp_var;
				char* left_str = new char[3];
				char* right_str = new char[3];
				char* var_str = new char[3];
				
				if(left_order)
					sprintf(left_str, "%d", idx);
				else
					sprintf(left_str, "%d", left_max - idx);

				if(right_order)
					sprintf(right_str, "%d", idx);
				else
					sprintf(right_str, "%d", right_max - idx);

				if(var_order)
					sprintf(var_str, "%d", idx);
				else
					sprintf(var_str, "%d", itmp2 - idx);

				tmp_var.basename = var.basename;
				tmp_var.clpname = var.clpname + "_" + var_str;
				tmp_var.index = var_order ? idx : itmp2 - idx;
				tmp_var.lsb = var_order ? idx : itmp2 - idx;;
				tmp_var.msb = var_order ? idx : itmp2 - idx;;
				tmp_var.width = 1;
				tmp_var.parentname = var.parentname;

				used.insert(tmp_var);
				
				int i;
				i = pow(2, idx);
				ostringstream os;


				os.flush();
				os << i;
				if(idx == itmp2)
					expr3 = expr3 + os.str() + " * " + tmp_var.clpname + ",";
				else
					expr3 = expr3 + os.str() + " * " + tmp_var.clpname + " + ";

				if(idx <= left_max)
				{
					tmp_left.basename = left.basename;
					tmp_left.clpname = left.clpname + "_" + left_str;
					tmp_left.index = left_order ? idx : left_max - idx;;
					tmp_left.lsb = left_order ? idx : left_max - idx;;
					tmp_left.msb = left_order ? idx : left_max - idx;;
					tmp_left.width = 1;
					tmp_left.parentname = left.parentname;

					if(idx == left_max)
						expr1 = expr1 + os.str() + " * " + tmp_left.clpname + ",";
					else
						expr1 = expr1 + os.str() + " * " + tmp_left.clpname + " + ";

					used.insert(tmp_left);
				}
				
				if(idx <= right_max)
				{
					tmp_right.basename = right.basename;
					tmp_right.clpname = right.clpname + "_" + right_str;
					tmp_right.index = right_order ? idx : right_max - idx;;
					tmp_right.lsb = right_order ? idx : right_max - idx;;
					tmp_right.msb = right_order ? idx : right_max - idx;;
					tmp_right.width = 1;
					tmp_right.parentname = right.parentname;

					if(idx == right_max)
						expr2 = expr2 + os.str() + " * " + tmp_right.clpname + ",";
					else
						expr2 = expr2 + os.str() + " * " + tmp_right.clpname + " + ";

					used.insert(tmp_right);
				}

				if(idx <= left_max)
				{
					if(idx <= right_max)
					{
						expr << tmp_var.clpname << " #<=> " << tmp_left.clpname << " #\\" << "\\" << "/ " << tmp_right.clpname << "," << endl;
					}
					else
					{
						expr << tmp_var.clpname << " #<=> " << tmp_left.clpname << " #\\" << "\\" << "/ 0" << "," << endl;
					}
				}
				else
				{
					if(idx <= right_max)
					{
						expr << tmp_var.clpname << " #<=> " << "0 #\\" << "\\" << "/ " << tmp_right.clpname << "," << endl;
					}
				}
			}
			
			//dump expr for ~
			expr << expr1 << endl;
			expr << expr2 << endl;
			expr << expr3 << endl;
			
			used.insert(var);
			return var;
		}

		break;
	case 'r'://Right shift (>>)
		var.basename = left.basename + "_" + right.basename;
		var.type = "NOINPUT";
		tmp << temp_idx;
		temp_idx += 1;
		var.clpname = "T_" + left.clpname + "_" + right.clpname + "_" + tmp.str();
		var.index = -1;
		var.width = expr_width();
		var.msb = expr_width() - 1;
		var.lsb = 0;
		var.parentname = "";
		
		expr << var.clpname << " #= " << left.clpname << "/ (2 ** " << right.clpname << ")," << endl;
		used.insert(var);
		
		return var;
		
		break;
	case 'X'://Bitwise exclusive NOR (~^)
		if((left.width == 1) && (right.width == 1))
		{
			//first new a 1-bit variable;
			var = left;
			var.type = "NOINPUT";
			tmp << temp_idx;
			temp_idx += 1;
			var.clpname = var.clpname + "_" + right.clpname + "_" + tmp.str();
			used.insert(var);
			expr << var.clpname << " #<=> #\\" << "(" << left.clpname << " ## " << right.clpname << ")," << endl;
			used.insert(var);
			return var;
		}
		else if((left.width > 1) || (right.width > 1))
		{
			//first new a 1-bit variable;
			string expr1, expr2, expr3;
			ostringstream os;
			var = left;
			var.type = "NOINPUT";
			tmp << temp_idx;
			temp_idx += 1;
			var.clpname = var.clpname + "_" + right.clpname + "_" + tmp.str();
			var.width = left.width >= right.width ? left.width : right.width;
			var.lsb = 0;
			var.msb = var.width - 1;
			expr1 = left.clpname + " #<=> ";
			expr2 = right.clpname + " #<=> ";
			expr3 = var.clpname + " #<=> ";

			unsigned itmp1 = var.lsb > var.msb ? var.msb : var.lsb;
			unsigned itmp2 = var.lsb > var.msb ? var.lsb : var.msb;
			unsigned left_max = left.lsb > left.msb ? left.lsb : left.msb;
			unsigned right_max = right.lsb > right.msb ? right.lsb : right.msb;

			bool left_order = left.lsb > left.msb ? false : true;
			bool right_order = right.lsb > right.msb ? false : true;
			bool var_order = var.lsb > var.msb ? false : true;

			for(unsigned idx = itmp1; idx <= itmp2; ++idx)
			{
				//When the operands are of unequal bit length, 
				//the shorter operand is zero-filled in the most significant bit positions.
				clp_var tmp_left, tmp_right, tmp_var;
				char* left_str = new char[3];
				char* right_str = new char[3];
				char* var_str = new char[3];
				
				if(left_order)
					sprintf(left_str, "%d", idx);
				else
					sprintf(left_str, "%d", left_max - idx);

				if(right_order)
					sprintf(right_str, "%d", idx);
				else
					sprintf(right_str, "%d", right_max - idx);

				if(var_order)
					sprintf(var_str, "%d", idx);
				else
					sprintf(var_str, "%d", itmp2 - idx);

				tmp_var.basename = var.basename;
				tmp_var.clpname = var.clpname + "_" + var_str;
				tmp_var.index = var_order ? idx : itmp2 - idx;
				tmp_var.lsb = var_order ? idx : itmp2 - idx;;
				tmp_var.msb = var_order ? idx : itmp2 - idx;;
				tmp_var.width = 1;
				tmp_var.parentname = var.parentname;

				used.insert(tmp_var);
				
				int i;
				i = pow(2, idx);
				ostringstream os;


				os.flush();
				os << i;
				if(idx == itmp2)
					expr3 = expr3 + os.str() + " * " + tmp_var.clpname + ",";
				else
					expr3 = expr3 + os.str() + " * " + tmp_var.clpname + " + ";

				if(idx <= left_max)
				{
					tmp_left.basename = left.basename;
					tmp_left.clpname = left.clpname + "_" + left_str;
					tmp_left.index = left_order ? idx : left_max - idx;;
					tmp_left.lsb = left_order ? idx : left_max - idx;;
					tmp_left.msb = left_order ? idx : left_max - idx;;
					tmp_left.width = 1;
					tmp_left.parentname = left.parentname;

					if(idx == left_max)
						expr1 = expr1 + os.str() + " * " + tmp_left.clpname + ",";
					else
						expr1 = expr1 + os.str() + " * " + tmp_left.clpname + " + ";

					used.insert(tmp_left);
				}
				
				if(idx <= right_max)
				{
					tmp_right.basename = right.basename;
					tmp_right.clpname = right.clpname + "_" + right_str;
					tmp_right.index = right_order ? idx : right_max - idx;;
					tmp_right.lsb = right_order ? idx : right_max - idx;;
					tmp_right.msb = right_order ? idx : right_max - idx;;
					tmp_right.width = 1;
					tmp_right.parentname = right.parentname;

					if(idx == right_max)
						expr2 = expr2 + os.str() + " * " + tmp_right.clpname + ",";
					else
						expr2 = expr2 + os.str() + " * " + tmp_right.clpname + " + ";

					used.insert(tmp_right);
				}

				if(idx <= left_max)
				{
					if(idx <= right_max)
					{
						expr << tmp_var.clpname << " #<=> #\\" << "(" << tmp_left.clpname << " ## " << tmp_right.clpname << ")," << endl;
					}
					else
					{
						expr << tmp_var.clpname << " #<=> #\\" << "(" << tmp_left.clpname << " ## 0)," << endl;
					}
				}
				else
				{
					if(idx <= right_max)
					{
						expr << tmp_var.clpname << " #<=> #\\" << "(0 ## " << tmp_right.clpname << ")," << endl;
					}
				}
			}
			
			//dump expr for ~
			expr << expr1 << endl;
			expr << expr2 << endl;
			expr << expr3 << endl;
			
			used.insert(var);
			return var;
		}

		break;
		break;
	}
}

clp_var NetEConcat::dump_cond_clp(ofstream& o, unsigned lineno, std::set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	/*
	if (repeat_calculated_) {
		if (repeat_value_ != 1)
			o << repeat_value_;
	} else if (repeat_) {
		o << "<" << *repeat_ << ">";
	}
	
	if (parms_[0])
		o << "{" << *parms_[0];
	else
		o << "{";
	
	for (unsigned idx = 1 ;  idx < parms_.count() ;  idx += 1) {
		if (parms_[idx])
			o << ", " << *parms_[idx];
		else
			o << ", ";
	}
	o << "}";
	*/
	cerr << "Concatation operator is unsupported now!" << endl;
	exit(1);
	clp_var tmp;
	return tmp;
}

clp_var NetEConst::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	if (value_.is_string())
	{
		cerr << "String in expression is unpported for test generation!" << endl;
		exit(1);
	}
	else
	{
		clp_var tmp;
		ostringstream os;
		os << lineno << "_" << temp_idx;
		temp_idx = temp_idx + 1;
		tmp.basename = "";
		tmp.clpname = "C_" + os.str();
		tmp.width = expr_width();
		tmp.index = -1;
		tmp.lsb = 0;
		tmp.msb = tmp.width - 1;
		tmp.parentname = "";
		tmp.type = "NOINPUT";
		expr << tmp.clpname << " #= " << value_.as_ulong() << "," << endl;
		used.insert(tmp);
		return tmp;
	}
}

clp_var NetEConstParam::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	clp_var tmp = NetEConst::dump_cond_clp(o, lineno, refs, used, expr, temp_idx);
	return tmp;
}

clp_var NetECReal::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	cerr << lineno << " : Real number is unsupported now!" << endl;
	exit(1);
	clp_var tmp;
	return tmp;
}

clp_var NetEScope::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	cerr << lineno << " : Scope name unspported!" << endl;
	exit(1);
}

clp_var NetESFunc::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	cerr << lineno << " : System function call unspported!" << endl;
	exit(1);
}

clp_var NetESignal::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	for(set<RefVar*>::const_iterator rvpos = refs.begin(); rvpos != refs.end(); ++rvpos)
	{
		if(name() == (*rvpos)->name) //find the variables
		{
			if((*rvpos)->width == expr_width()) //use the whole signal in expression
			{
				clp_var cv;
				ostringstream name;
				unsigned time = (*rvpos)->time;
				//if((*rvpos)->ptype == "NOINPUT")
				//	time = time - 1;
				name << "S_" << (*rvpos)->name << "_" << time << "_" << (*rvpos)->space;
				cv.basename = (*rvpos)->name;
				cv.clpname = name.str();
				cv.index = -1;
				cv.parentname = "";
				cv.width = (*rvpos)->width;
				cv.lsb = (*rvpos)->lsb;
				cv.msb = (*rvpos)->msb;
				cv.type = (*rvpos)->ptype;
				used.insert(cv);
				return cv;
			}
			else //use only selected bit(s) in expression
			{
				//generate a variable for the whole variable
				ostringstream name, name1, name2, name3;
				clp_var var;
				unsigned time = (*rvpos)->time;
				//if((*rvpos)->ptype == "NOINPUT")
				//	time = time -1;
				name << "S_" << (*rvpos)->name << "_" << time << "_" << (*rvpos)->space;
				var.basename = (*rvpos)->name;
				var.clpname = name.str();
				var.index = -1;
				var.parentname = "";
				var.width = (*rvpos)->width;
				var.lsb = (*rvpos)->lsb;
				var.msb = (*rvpos)->msb;
				var.type = (*rvpos)->ptype;
				used.insert(var);

				expr << var.clpname << " #= ";

				//first new a temp variable for the bit(s) selection
				clp_var p3;
				name1 << "S_" << (*rvpos)->name << "_" << (*rvpos)->time << "_" << (*rvpos)->space << "_" << msi_;
				p3.basename = (*rvpos)->name;
				p3.clpname = name1.str();
				p3.index = msi_;
				p3.parentname = (*rvpos)->name;
				p3.width = expr_width();
				p3.lsb = lsi_ > msi_ ? expr_width() - 1 : 0;
				p3.msb = lsi_ > msi_ ? 0 : expr_width() - 1;
				p3.type = "NOINPUT";
				used.insert(p3);

				int i = lsi_ > msi_ ? pow(2, (var.width - 1 - msi_)) : pow(2, msi_);

				expr << i << " * " << p3.clpname;

				//the selection divide the variable_time_space into 2 or 3 parts
				//genearte temp variables for the other two parts
				if(msi_ != (*rvpos)->msb) //3 parts, and p1 is the first part, from msb to msi_ - 1;
				{
					clp_var p1;
					name2 << "S_" << (*rvpos)->name << "_" << (*rvpos)->time << "_" << (*rvpos)->space << "_" << (*rvpos)->msb;
					p1.basename = (*rvpos)->name;
					p1.clpname = name2.str();
					p1.index = (*rvpos)->msb;
					p1.parentname = (*rvpos)->name;
					p1.width = (*rvpos)->lsb > (*rvpos)->msb ? msi_ - (*rvpos)->msb : (*rvpos)->msb - msi_;
					p1.lsb = (*rvpos)->lsb > (*rvpos)->msb ? msi_ - 1: msi_ + 1;
					p1.msb = (*rvpos)->msb;
					p1.type = "NOINPUT";
					used.insert(p1);

					i = p1.lsb > p1.msb ? pow(2, ((*rvpos)->width - 1)) : pow(2, p1.msb);
					expr << " + " << i << " * " << p1.clpname;
				}
				//generate the least part, p2 is the part from lsi_+1 to lsb
				if(lsi_ != (*rvpos)->lsb)
				{
					clp_var p2;
					name3 << "S_" << (*rvpos)->name << "_" << (*rvpos)->time << "_" << (*rvpos)->space << "_" << msi_ - expr_width();
					p2.basename = (*rvpos)->name;
					p2.clpname = name3.str();
					p2.index = msi_ - expr_width();
					p2.parentname = (*rvpos)->name;
					p2.width = (*rvpos)->lsb > (*rvpos)->msb ? (*rvpos)->lsb - lsi_ : lsi_ - (*rvpos)->lsb;
					p2.lsb = (*rvpos)->lsb;
					p2.msb = (*rvpos)->lsb > (*rvpos)->msb ? lsi_ + 1: lsi_ - 1;
					p2.type = "NOINPUT";
					used.insert(p2);

					i = pow(2, (p2.width - 1));
					expr << " + " << i << " * " << p2.clpname;
				}
				expr << "," << endl;
				return p3;
			}
		}
	}
}

clp_var NetEBitSel::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	if (! dynamic_cast<NetEConst*>(idx_)) {
		cerr << lineno << " : Biot selection with non-const index unspported!" << endl;
		exit(1);
	}
	else
	{
		NetEConst* cur = dynamic_cast<NetEConst*>(idx_);
		clp_var tmp = sig_->dump_cond_clp(o, lineno, refs, used, expr, temp_idx);

		clp_var result;
		ostringstream s;
		unsigned itmp1 = tmp.lsb > tmp.msb ? tmp.msb : tmp.lsb;
		unsigned itmp2 = tmp.lsb > tmp.msb ? tmp.lsb : tmp.msb;
		bool order = tmp.lsb > tmp.msb ? false : true;
		expr << tmp.clpname << " #= ";
		for(unsigned idx = itmp1; idx < itmp2; ++idx)
		{
			clp_var var = tmp;
			ostringstream os;
			os.flush();
			os << idx;
			var.clpname = "S_" + var.clpname + "_" + os.str();
			var.index = idx;
			var.lsb = idx;
			var.msb = idx;
			var.parentname = tmp.basename;
			var.width = 1;
			var.type = "NOINPUT";

			int i ;
			if(order)
				i = pow(2, idx);
			else i = pow(2, (itmp2 - idx));

			if(idx == itmp2)
				expr << i << " * " << var.clpname << "," << endl;
			else
				expr << i << " * " << var.clpname << " + ";

			if(idx == cur->value().as_ulong())
			{
				result = var;
				s << result.clpname << " #= " << var.clpname << "," << endl;
			}
			used.insert(var);
		}
		expr << s.str() << endl;
		used.insert(tmp);
		used.insert(result);
		return result;
	}
}

clp_var NetEMemory::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	cerr << lineno << " : Memory unsupported!" << endl;
	exit(1);
}

clp_var NetEParam::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	cerr << lineno << " : Parameter unsupported!" << endl;
	exit(1);
}

clp_var NetETernary::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	clp_var cond, tru_, fals_;
	ostringstream tidx;
	cond = cond_->dump_cond_clp(o, lineno, refs, used, expr, temp_idx);
	tru_ = true_val_->dump_cond_clp(o,lineno,refs,used,expr, temp_idx);
	fals_ = false_val_->dump_cond_clp(o,lineno,refs,used,expr, temp_idx);
	clp_var tmp;
	tmp.basename = "";
	tidx << temp_idx;
	temp_idx += 1;
	tmp.clpname = "T_" + cond.clpname + "_" + tru_.clpname + "_" + fals_.clpname + "_" + tidx.str();
	tmp.index = -1;
	tmp.width = expr_width();
	tmp.lsb = 0;
	tmp.msb = expr_width() - 1;
	tmp.parentname = "";
	tmp.type = "NOINPIT";

	expr << "(" << cond.clpname << " #= 1) #==> (" << tmp.clpname << " #= " << tru_.clpname << "), ";
	expr << "(" << cond.clpname << " #= 0) #==> (" << tmp.clpname << " #= " << fals_.clpname << ")" << endl;

	used.insert(cond);
	used.insert(tru_);
	used.insert(fals_);
	used.insert(tmp);
	return tmp;
}

clp_var NetEUFunc::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	/*
	o << name() << "(";
	if (parms_.count() > 0) {
		parms_[0]->dump_clp(o, lineno);
		for (unsigned idx = 1 ;  idx < parms_.count() ;  idx += 1) {
			o << ", ";
			parms_[idx]->dump_clp(o, lineno);
		}
	}
	o << ")";
	*/
	clp_var tmp;
	return tmp;
}

clp_var NetEUnary::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	
	clp_var tmp = expr_->dump_cond_clp(o, lineno, refs, used, expr, temp_idx);
	clp_var var;
	char ctmp[4];

	switch (op_) {
	case 'N': //Reduction NOR (~|)
		//o << "~|";
		//dump the expr for tmp;
		if(tmp.width == 1)
		{
			var = tmp;
			var.type = "NOINPUT";
			sprintf(ctmp, "%d", temp_idx);
			temp_idx = temp_idx + 1;
			var.clpname = var.clpname + "ro_" + ctmp;
			used.insert(var);
			expr << var.clpname << " #<=> " << "#\\(" << tmp.clpname << ")," << endl;
			used.insert(var);
			return var;
		}
		else if (tmp.width > 1)
		{
			//first new a 1-bit variable;
			var.basename = "";
			sprintf(ctmp, "%d", temp_idx);
			temp_idx = temp_idx + 1;
			var.clpname = tmp.clpname + "_RN_" + ctmp;
			var.index = -1;
			var.lsb = 0;
			var.msb = 0;
			var.width = 1;
			var.parentname = "";
			var.type = "NOINPUT";

			expr << tmp.clpname << " #<=> ";
			unsigned itmp1 = tmp.lsb > tmp.msb ? tmp.msb : tmp.lsb;
			unsigned itmp2 = tmp.lsb > tmp.msb ? tmp.lsb : tmp.msb;
			bool order = tmp.lsb > tmp.msb ? false : true;
			for(unsigned idx = itmp1; idx <= itmp2; ++idx)
			{
				clp_var tmp_var;
				char* str = new char[3];
				if(order)
					sprintf(str, "%d", idx);
				else
					sprintf(str, "%d", itmp2-idx);
				tmp_var.basename = tmp.basename;
				sprintf(ctmp, "%d", temp_idx);
			    temp_idx = temp_idx + 1;
				tmp_var.clpname = tmp.clpname + "_" + ctmp + "_" + str;
				tmp_var.index = order ? idx : itmp2 - idx;
				tmp_var.lsb = order ? idx : itmp2 - idx;
				tmp_var.msb = order ? idx : itmp2 - idx;
				tmp_var.width = 1;
				tmp_var.parentname = tmp.parentname;
				tmp_var.type = "NOINPUT";
				used.insert(tmp_var);
				
				int i;
				i = pow(2, idx);

				if(idx == itmp2)
					expr << i << " * " << tmp_var.clpname << "," << endl;
				else
					expr << i << " * " << tmp_var.clpname << " + ";
			}
			
			//dump expr for ~|
			expr << var.clpname << " #<=> ";
			for(idx = itmp1; idx <= itmp2; ++idx)
			{
				char* str = new char[3];
				if(order)
					sprintf(str, "%d", idx);
				else
					sprintf(str, "%d", itmp2-idx);
				if(idx == itmp2)
					expr << tmp.clpname + "_" + str << "," << endl;
				else
					expr << tmp.clpname + "_" + str << " #\\" << "\\/ ";
			}
			used.insert(var);
			return var;		
		}

		break;
	case '~': //Bit-wise negation
		if(tmp.width == 1)
		{
			//first new a 1-bit variable;
			var = tmp;
			var.type = "NOINPUT";
			sprintf(ctmp, "%d", temp_idx);
			temp_idx = temp_idx + 1;
			var.clpname = var.clpname + "r_" + ctmp;
			used.insert(var);
			expr << var.clpname << " #<=> " << "#\\(" << tmp.clpname << ")," << endl;
			used.insert(var);
			return var;
		}
		else if(tmp.width > 1)
		{
			//first new a 1-bit variable;
			string expr1, expr2;
			ostringstream os;
			var = tmp;
			var.type = "NOINPUT";
			sprintf(ctmp, "%d", temp_idx);
			temp_idx = temp_idx + 1;
			var.clpname = var.clpname + "_BE_" + ctmp;
			expr1 = tmp.clpname + " #<=> ";
			expr2 = var.clpname + " #<=> ";
			unsigned itmp1 = tmp.lsb > tmp.msb ? tmp.msb : tmp.lsb;
			unsigned itmp2 = tmp.lsb > tmp.msb ? tmp.lsb : tmp.msb;
			bool order = tmp.lsb > tmp.msb ? false : true;
			for(unsigned idx = itmp1; idx <= itmp2; ++idx)
			{
				clp_var tmp_var1, tmp_var2;
				char* str = new char[3];
				if(order)
					sprintf(str, "%d", idx);
				else
					sprintf(str, "%d", itmp2-idx);
				sprintf(ctmp, "%d", temp_idx);
			    temp_idx = temp_idx + 1;
				tmp_var1.basename = tmp.basename;
				tmp_var1.clpname = tmp.clpname + "_" + ctmp + "_" + str;
				tmp_var1.index = order ? idx : itmp2 - idx;
				tmp_var1.lsb = order ? idx : itmp2 - idx;
				tmp_var1.msb = order ? idx : itmp2 - idx;
				tmp_var1.width = 1;
				tmp_var1.parentname = tmp.parentname;
				tmp_var1.type = "NOINPUT";
				
				tmp_var2 = tmp_var1;
				tmp_var2.clpname = tmp_var2.clpname + "_r";
				
				expr << tmp_var2.clpname << " #<=> #\\(" << tmp_var1.clpname << ")," << endl; 

				int i ;
				i = pow(2, idx);

				if(idx == itmp2)
				{
					expr1 = expr1 + os.str() + " * " + tmp_var1.clpname + ",";
					expr2 = expr2 + os.str() + " * " + tmp_var2.clpname + ",";
				}
				else
				{
					expr1 = expr1 + os.str() + " * " + tmp_var1.clpname + " + ";
					expr2 = expr2 + os.str() + " * " + tmp_var2.clpname + " + ";
				}

				used.insert(tmp_var1);
				used.insert(tmp_var2);
			}
			
			//dump expr for ~
			expr << expr1 << endl;
			expr << expr2 << endl;
			
			used.insert(var);
			return var;
		}

		break;
	case '!':
		var = tmp;
		var.type = "NOINPUT";
		sprintf(ctmp, "%d", temp_idx);
		temp_idx = temp_idx + 1;
		var.clpname = var.clpname + "_n_" + ctmp;
		expr << var.clpname << " #<=> #\\(" << tmp.clpname << ")," << endl;
		used.insert(var);
		return var;
		break;
	case '&':
		//dump the expr for tmp;
		if(tmp.width == 1)
		{
			tmp.type = "NOINPUT";
			return tmp;
		}
		else if (tmp.width > 1)
		{
			//first new a 1-bit variable;
			var.basename = "";
			sprintf(ctmp, "%d", temp_idx);
			temp_idx = temp_idx + 1;
			var.clpname = tmp.clpname + "_n_" + ctmp;
			var.index = -1;
			var.lsb = 0;
			var.msb = 0;
			var.width = 1;
			var.parentname = "";
			var.type = "NOINPUT";

			expr << tmp.clpname << " #<=> ";
			unsigned itmp1 = tmp.lsb > tmp.msb ? tmp.msb : tmp.lsb;
			unsigned itmp2 = tmp.lsb > tmp.msb ? tmp.lsb : tmp.msb;
			bool order = tmp.lsb > tmp.msb ? false : true;
			for(unsigned idx = itmp1; idx <= itmp2; ++idx)
			{
				clp_var tmp_var;
				char* str = new char[3];
				if(order)
					sprintf(str, "%d", idx);
				else
					sprintf(str, "%d", itmp2-idx);
				sprintf(ctmp, "%d", temp_idx);
			    temp_idx = temp_idx + 1;
				tmp_var.basename = tmp.basename;
				tmp_var.clpname = tmp.clpname + "_" + ctmp + "_" + str;
				tmp_var.index = order ? idx : itmp2 - idx;
				tmp_var.lsb = order ? idx : itmp2 - idx;
				tmp_var.msb = order ? idx : itmp2 - idx;
				tmp_var.width = 1;
				tmp_var.parentname = tmp.parentname;
				tmp_var.type = "NOINPUT";
				used.insert(tmp_var);
				
				int i;
				i = pow(2, idx);

				if(idx == itmp2)
					expr << i << " * " << tmp_var.clpname << "," << endl;
				else
					expr << i << " * " << tmp_var.clpname << " + ";
			}
			
			//dump expr for ~|
			expr << var.clpname << " #<=> ";
			for(idx = itmp1; idx <= itmp2; ++idx)
			{
				char* str = new char[3];
				if(order)
					sprintf(str, "%d", idx);
				else
					sprintf(str, "%d", itmp2-idx);
				if(idx == itmp2)
					expr << tmp.clpname + "_" + str << "," << endl;
				else
					expr << tmp.clpname + "_" + str << " #/\\ ";
			}
			used.insert(var);
			return var;		
		}

		break;
	case '|':
		//dump the expr for tmp;
		if(tmp.width == 1)
		{
			tmp.type = "NOINPUT";
			return tmp;
		}
		else if (tmp.width > 1)
		{
			//first new a 1-bit variable;
			var.basename = "";
			sprintf(ctmp, "%d", temp_idx);
			temp_idx = temp_idx + 1;
			var.clpname = tmp.clpname + "_r_" + ctmp;
			var.index = -1;
			var.lsb = 0;
			var.msb = 0;
			var.width = 1;
			var.parentname = "";
			var.type = "NOINPUT";

			expr << tmp.clpname << " #<=> ";
			unsigned itmp1 = tmp.lsb > tmp.msb ? tmp.msb : tmp.lsb;
			unsigned itmp2 = tmp.lsb > tmp.msb ? tmp.lsb : tmp.msb;
			bool order = tmp.lsb > tmp.msb ? false : true;
			for(unsigned idx = itmp1; idx <= itmp2; ++idx)
			{
				clp_var tmp_var;
				char* str = new char[3];
				if(order)
					sprintf(str, "%d", idx);
				else
					sprintf(str, "%d", itmp2-idx);
				sprintf(ctmp, "%d", temp_idx);
			    temp_idx = temp_idx + 1;
				tmp_var.basename = tmp.basename;
				tmp_var.clpname = tmp.clpname + "_" + ctmp + "_" + str;
				tmp_var.index = order ? idx : itmp2 - idx;;
				tmp_var.lsb = order ? idx : itmp2 - idx;;
				tmp_var.msb = order ? idx : itmp2 - idx;;
				tmp_var.width = 1;
				tmp_var.parentname = tmp.parentname;
				tmp_var.type = "NOINPUT";
				used.insert(tmp_var);
				
				int i;
				i = pow(2, idx);

				if(idx == itmp2)
					expr << i << " * " << tmp_var.clpname << "," << endl;
				else
					expr << i << " * " << tmp_var.clpname << " + ";
			}
			
			//dump expr for ~|
			expr << var.clpname << " #<=> ";
			for(idx = itmp1; idx <= itmp2; ++idx)
			{
				char* str = new char[3];
				if(order)
					sprintf(str, "%d", idx);
				else
					sprintf(str, "%d", itmp2-idx);
				if(idx == itmp2)
					expr << tmp.clpname + "_" + str << "," << endl;
				else
					expr << tmp.clpname + "_" + str << " #\\/ ";
			}
			used.insert(var);
			return var;		
		}
		break;
	case '^':
		//dump the expr for tmp;
		if(tmp.width == 1)
		{
			return tmp;
		}
		else if (tmp.width > 1)
		{
			//first new a 1-bit variable;
			var.basename = "";
			sprintf(ctmp, "%d", temp_idx);
			temp_idx = temp_idx + 1;
			var.clpname = tmp.clpname + "_x_" + ctmp;
			var.index = -1;
			var.lsb = 0;
			var.msb = 0;
			var.width = 1;
			var.parentname = "";
			var.type = "NOINPUT";

			expr << tmp.clpname << " #<=> ";
			unsigned itmp1 = tmp.lsb > tmp.msb ? tmp.msb : tmp.lsb;
			unsigned itmp2 = tmp.lsb > tmp.msb ? tmp.lsb : tmp.msb;
			bool order = tmp.lsb > tmp.msb ? false : true;
			for(unsigned idx = itmp1; idx <= itmp2; ++idx)
			{
				clp_var tmp_var;
				char* str = new char[3];
				if(order)
					sprintf(str, "%d", idx);
				else
					sprintf(str, "%d", itmp2-idx);
				sprintf(ctmp, "%d", temp_idx);
			    temp_idx = temp_idx + 1;
				tmp_var.basename = tmp.basename;
				tmp_var.clpname = tmp.clpname + "_" + ctmp + "_" + str;
				tmp_var.index = order ? idx : itmp2 - idx;;
				tmp_var.lsb = order ? idx : itmp2 - idx;;
				tmp_var.msb = order ? idx : itmp2 - idx;;
				tmp_var.width = 1;
				tmp_var.parentname = tmp.parentname;
				tmp_var.type = "NOINPUT";
				used.insert(tmp_var);
				
				int i;
				i = pow(2, idx);

				if(idx == itmp2)
					expr << i << " * " << tmp_var.clpname << "," << endl;
				else
					expr << i << " * " << tmp_var.clpname << " + ";
			}
			
			//dump expr for ~|
			expr << var.clpname << " #<=> ";
			for(idx = itmp1; idx <= itmp2; ++idx)
			{
				char* str = new char[3];
				if(order)
					sprintf(str, "%d", idx);
				else
					sprintf(str, "%d", itmp2-idx);
				if(idx == itmp2)
					expr << tmp.clpname + "_" + str << "," << endl;
				else
					expr << tmp.clpname + "_" + str << " ## ";
			}
			used.insert(var);
			return var;		
		}
		break;
	case '+':
		tmp.type = "NOINPUT";
		return tmp;
		break;
	case '-':
		var = tmp;
		var.type = "NOINPUT";
		sprintf(ctmp, "%d", temp_idx);
		temp_idx = temp_idx + 1;
		var.clpname = var.clpname + "_M_" + ctmp;
		expr << var.clpname << " #= -(" << tmp.clpname << ")," << endl;
		used.insert(var);
		return var;
		break;
	case 'A':
		//dump the expr for tmp;
		if(tmp.width == 1)
		{
			tmp.type = "NOINPUT";
			return tmp;
		}
		else if (tmp.width > 1)
		{
			//first new a 1-bit variable;
			var.basename = "";
			sprintf(ctmp, "%d", temp_idx);
			temp_idx = temp_idx + 1;
			var.clpname = tmp.clpname + "_A_" + ctmp;
			var.index = -1;
			var.lsb = 0;
			var.msb = 0;
			var.width = 1;
			var.parentname = "";

			expr << tmp.clpname << " #<=> ";
			unsigned itmp1 = tmp.lsb > tmp.msb ? tmp.msb : tmp.lsb;
			unsigned itmp2 = tmp.lsb > tmp.msb ? tmp.lsb : tmp.msb;
			bool order = tmp.lsb > tmp.msb ? false : true;
			for(unsigned idx = itmp1; idx <= itmp2; ++idx)
			{
				clp_var tmp_var;
				char* str = new char[3];
				if(order)
					sprintf(str, "%d", idx);
				else
					sprintf(str, "%d", itmp2-idx);
				sprintf(ctmp, "%d", temp_idx);
			    temp_idx = temp_idx + 1;
				tmp_var.basename = tmp.basename;
				tmp_var.clpname = tmp.clpname + "_" + ctmp + "_" + str;
				tmp_var.index = order ? idx : itmp2 - idx;;
				tmp_var.lsb = order ? idx : itmp2 - idx;;
				tmp_var.msb = order ? idx : itmp2 - idx;;
				tmp_var.width = 1;
				tmp_var.parentname = tmp.parentname;
				used.insert(tmp_var);
				
				int i;
				i = pow(2, idx);

				if(idx == itmp2)
					expr << i << " * " << tmp_var.clpname << "," << endl;
				else
					expr << i << " * " << tmp_var.clpname << " + ";
			}
			
			//dump expr for ~|
			expr << var.clpname << " #<=> ";
			for(idx = itmp1; idx <= itmp2; ++idx)
			{
				char* str = new char[3];
				if(order)
					sprintf(str, "%d", idx);
				else
					sprintf(str, "%d", itmp2-idx);
				if(idx == itmp2)
					expr << tmp.clpname + "_" + str << "," << endl;
				else
					expr << tmp.clpname + "_" + str << " #\\" << "/\\ ";
			}
			used.insert(var);
			return var;		
		}

		break;
	case 'X':
				//dump the expr for tmp;
		if(tmp.width == 1)
		{
			tmp.type = "NOINPUT";
			return tmp;
		}
		else if (tmp.width > 1)
		{
			//first new a 1-bit variable;
			var.basename = "";
			sprintf(ctmp, "%d", temp_idx);
			temp_idx = temp_idx + 1;
			var.clpname = tmp.clpname + "_X_" + ctmp;
			var.index = -1;
			var.lsb = 0;
			var.msb = 0;
			var.width = 1;
			var.parentname = "";

			expr << tmp.clpname << " #<=> ";
			unsigned itmp1 = tmp.lsb > tmp.msb ? tmp.msb : tmp.lsb;
			unsigned itmp2 = tmp.lsb > tmp.msb ? tmp.lsb : tmp.msb;
			bool order = tmp.lsb > tmp.msb ? false : true;
			for(unsigned idx = itmp1; idx <= itmp2; ++idx)
			{
				clp_var tmp_var;
				char* str = new char[3];
				if(order)
					sprintf(str, "%d", idx);
				else
					sprintf(str, "%d", itmp2-idx);
				sprintf(ctmp, "%d", temp_idx);
			    temp_idx = temp_idx + 1;
				tmp_var.basename = tmp.basename;
				tmp_var.clpname = tmp.clpname + "_" + ctmp + "_" +str;
				tmp_var.index = order ? idx : itmp2 - idx;;
				tmp_var.lsb = order ? idx : itmp2 - idx;;
				tmp_var.msb = order ? idx : itmp2 - idx;;
				tmp_var.width = 1;
				tmp_var.parentname = tmp.parentname;
				used.insert(tmp_var);
				
				int i;
				i = pow(2, idx);

				if(idx == itmp2)
					expr << i << " * " << tmp_var.clpname << "," << endl;
				else
					expr << i << " * " << tmp_var.clpname << " + ";
			}
			
			//dump expr for ~|
			expr << var.clpname << " #<=> " << "#\\" << "(";
			for(idx = itmp1; idx <= itmp2; ++idx)
			{
				char* str = new char[3];
				if(order)
					sprintf(str, "%d", idx);
				else
					sprintf(str, "%d", itmp2-idx);
				if(idx == itmp2)
					expr << tmp.clpname + "_" + str << ")," << endl;
				else
					expr << tmp.clpname + "_" + str << " ## ";
			}
			used.insert(var);
			return var;		
		}
		break;
	}

	return tmp;
}

clp_var NetEVariable::dump_cond_clp(ofstream& o, unsigned lineno, set<RefVar*>& refs, set<clp_var>& used, ostringstream& expr, unsigned temp_idx) const
{
	clp_var cv;
	for(set<RefVar*>::const_iterator pos = refs.begin(); pos != refs.end(); ++pos)
	{
		if((*pos)->name == var_->basename())
		{
			ostringstream name;
			name << "V_" << (*pos)->name << "_" << (*pos)->time << "_" + (*pos)->space;
			cv.basename = (*pos)->name;
			cv.clpname = name.str();
			cv.index = -1;
			cv.parentname = "";
			cv.width = -1;
			cv.type = (*pos)->ptype;
			break;
		}
	}
	return cv;
}
