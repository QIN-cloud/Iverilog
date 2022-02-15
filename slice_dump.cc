#include "config.h"
#include "HName.h"
#include "slice.h"
#include "Module.h"
#include "Statement.h"
#include "PGate.h"
#include "PExpr.h"
#include "PTask.h"
#include "PUdp.h"
#include "PWire.h"
#include "PEvent.h"
#include "named.h"
#include "PGenerate.h"

#include <string>
#include <list>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <typeinfo>

void slice_dump(std::ostream& o, Module* mod, slicer* slice)
{
	//first find main module in modules
	mod->dump_slice(o, slice);
}

void PGenerate::dump_slice(std::ostream&out, unsigned ind, slicer* s) const
{

	out << setw(ind) << "" << "generate(" << id_number << ")";

      PGenerate*parent = dynamic_cast<PGenerate*>(parent_scope());
      switch (scheme_type) {
	  case GS_NONE:
	    break;
	  case GS_LOOP:
	    out << " for ("
		<< loop_index
		<< "=" << *loop_init
		<< "; " << *loop_test
		<< "; " << loop_index
		<< "=" << *loop_step << ")";
	    break;
	  case GS_CONDIT:
	    out << " if (" << *loop_test << ")";
	    break;
	  case GS_ELSE:
	    out << " else !(" << *loop_test << ")";
	    break;
	  case GS_CASE:
	    out << " case (" << *loop_test << ")";
	    break;
	  case GS_CASE_ITEM:
            assert(parent);
	    if (loop_test)
		  out << " (" << *loop_test << ") == (" << *parent->loop_test << ")";
	    else
		  out << " default:";
	    break;
	  case GS_NBLOCK:
	    out << " begin";
      }

      if (scope_name)
	    out << " : " << scope_name;

      out << endl;
	typedef map<perm_string,param_expr_t*>::const_iterator parm_iter_t;
    typedef list<PGenerate::named_expr_t>::const_iterator parm_hiter_t;
    for(parm_iter_t cur = localparams.begin()
		; cur != localparams.end() ; cur ++) 
	{
		out << "    localparam ";
	    if (cur->second->data_type) {
		  cur->second->data_type->debug_dump(out);
		  out << " ";
	    }
		out << (*cur).first << " = ";
		if ((*cur).second->expr)
			out << *(*cur).second->expr << ";" << endl;
		else
			out << "/* ERROR */;" << endl;
	}
	for(parm_hiter_t hcur = defparms.begin()
		; hcur != defparms.end() ;  hcur ++) 
	{
		out << "    defparam " << (*hcur).first << " = ";
		if ((*hcur).second)
			out << *(*hcur).second << ";" << endl;
		else
			out << "/* ERROR */;" << endl;
	}
	for(map<perm_string, PEvent*>::const_iterator ecur = events.begin()
		; ecur != events.end() ;  ecur ++ ) 
	{
		PEvent*ev = (*ecur).second;
		out << "    event " << ev->name() << "; // "
			<< ev->get_fileline() << endl;
	}
	// Iterate through and display all the wires.
	for(map<perm_string, PWire*>::const_iterator wire = wires.begin()
		; wire != wires.end()
		; wire ++ ) 
	{
		if(s->vars.find(wire->first.str()) != s->vars.end())
			(*wire).second->dump(out);
	}
	// Iterate through and display all the gates
	for(list<PGate*>::const_iterator gate = gates.begin()
		; gate != gates.end()
		; gate ++ ) {
		if(s->slice.find((*gate)->get_lineno()) != s->slice.end())
			(*gate)->dump(out);
	}

	// Iterate through and display all the processes
	for(list<PProcess*>::const_iterator behav = behaviors.begin()
		; behav != behaviors.end()
		; behav ++ ) {//printf("22222222222\n");
		if(s->slice.find((*behav)->get_lineno()) != s->slice.end())	
		{//printf("111111111111\n");
			(*behav)->dump_slice(out, ind+2, s);
		}
	}
	typedef list<PGenerate*>::const_iterator genscheme_iter_t;
	for (genscheme_iter_t gene = generate_schemes.begin()
		; gene != generate_schemes.end() ; ++ gene ) {
		if(s->slice.find((*gene)->get_lineno()) != s->slice.end())
	    {
	    	(*gene)->dump_slice(out, ind+2, s);
	    }
	}

      if (scheme_type == GS_NBLOCK) {
	    out << setw(ind) << "" << "end endgenerate" << endl;
      } else {
	    out << setw(ind) << "" << "endgenerate" << endl;
      }
}

void Module::dump_slice(std::ostream&out, slicer* s) const
{
	out << "module " << mod_name();
    if(ports.size() < 1)
		out<< ";" << endl;
    else
	{
		out<<"(";
		svector<string>* stmp = new svector<string>;
		for(unsigned i = 0; i < ports.size(); i += 1)
		{
			port_t*c = ports[i];
			if((s->vars.find(c->name.str())) != (s->vars.end()))
			{
				string s(c->name);
				stmp = new svector<string>(*stmp, s);
			}
		}
		for(unsigned idx = 0; idx < stmp->count(); ++idx)
			if(idx != stmp->count()-1)
				out<<(*stmp)[idx]<<",";
			else
				out<<(*stmp)[idx];
		out<<");"<<endl;
	}
	typedef map<perm_string,param_expr_t*>::const_iterator parm_iter_t;
    typedef list<named_expr_t>::const_iterator parm_hiter_t;
    for(parm_iter_t cur = parameters.begin()
		; cur != parameters.end() ; cur ++) 
	{
		out << "    parameter ";
		/*if (cur->second->signed_flag)
			out << "signed ";
		*/
		if (cur->second->data_type)
			cur->second->data_type->debug_dump(out);
		else
			out << "(nil type)";
		out << (*cur).first << " = ";
		if ((*cur).second->expr)
			out << *(*cur).second->expr << ";" << endl;
		else
			out << "/* ERROR */;" << endl;
	}
	for(parm_iter_t cur = localparams.begin()
		; cur != localparams.end() ; cur ++) 
	{
		out << "    localparam ";
	    if (cur->second->data_type) {
		  cur->second->data_type->debug_dump(out);
		  out << " ";
	    }
		out << (*cur).first << " = ";
		if ((*cur).second->expr)
			out << *(*cur).second->expr << ";" << endl;
		else
			out << "/* ERROR */;" << endl;
	}
	for(parm_hiter_t hcur = defparms.begin()
		; hcur != defparms.end() ;  hcur ++) 
	{
		out << "    defparam " << (*hcur).first << " = ";
		if ((*hcur).second)
			out << *(*hcur).second << ";" << endl;
		else
			out << "/* ERROR */;" << endl;
	}
	for(map<perm_string, PEvent*>::const_iterator ecur = events.begin()
		; ecur != events.end() ;  ecur ++ ) 
	{
		PEvent*ev = (*ecur).second;
		out << "    event " << ev->name() << "; // "
			<< ev->get_fileline() << endl;
	}
	// Iterate through and display all the wires.
	for(map<perm_string, PWire*>::const_iterator wire = wires.begin()
		; wire != wires.end()
		; wire ++ ) 
	{
		if(s->vars.find(wire->first.str()) != s->vars.end())
			(*wire).second->dump(out);
	}

	// Dump the task definitions.
	typedef map<perm_string,PTask*>::const_iterator task_iter_t;
	set<string>::const_iterator pos;
	set<string> tmp1, tmp;
	tmp = s->funcs;
	for(task_iter_t tcur = tasks.begin()
		; tcur != tasks.end() ; tcur ++) 
	{
		if(s->funcs.find((*tcur).first.str()) != s->funcs.end())
		{
			tmp1 = (*tcur).second->get_funcs();
			for(pos = tmp1.begin();
			    pos != tmp1.end();
				++pos)
					tmp.insert(*pos);
		}
			out << "    task " << (*tcur).first << ";" << endl;
			(*tcur).second->dump(out, 6);
			out << "    endtask;" << endl;
	}

	for(task_iter_t tcur = tasks.begin()
		; tcur != tasks.end() ; tcur ++) 
	{
		if(tmp.find((*tcur).first.str()) != tmp.end())
		{
			out << "    task " << (*tcur).first << ";" << endl;
			(*tcur).second->dump(out, 6);
			out << "    endtask;" << endl;
		}
	}
	
	// Dump the function definitions.
	typedef map<perm_string,PFunction*>::const_iterator func_iter_t;
	tmp = s->funcs;
	for(func_iter_t fcur = funcs.begin()
		; fcur != funcs.end() ; fcur ++) 
	{
		if(s->funcs.find((*fcur).first.str()) != s->funcs.end())
		{
			tmp1 = (*fcur).second->get_funcs();
			for(pos = tmp1.begin();
			    pos != tmp1.end();
				++pos)
					tmp.insert(*pos);
		}
	}

	for(func_iter_t fcur = funcs.begin()
		; fcur != funcs.end() ; fcur ++) 
	{
		if(tmp.find((*fcur).first.str()) != tmp.end())
		{
			out << "    function " << (*fcur).first << ";" << endl;
			(*fcur).second->dump(out, 6);
			out << "    endfunction;" << endl;
		}
	}

	// Iterate through and display all the gates
	for(list<PGate*>::const_iterator gate = gates_.begin()
		; gate != gates_.end()
		; gate ++ ) {
		if(s->slice.find((*gate)->get_lineno()) != s->slice.end())
			(*gate)->dump(out);
	}

	// Iterate through and display all the processes
	for(list<PProcess*>::const_iterator behav = behaviors.begin()
		; behav != behaviors.end()
		; behav ++ ) {//printf("22222222222\n");
		if(s->slice.find((*behav)->get_lineno()) != s->slice.end())	
		{//printf("111111111111\n");
			(*behav)->dump_slice(out, 4, s);
		}
	}
	for(set<int, less<int> >::iterator it = s->slice.begin(); it != s->slice.end(); ++it)
		cout<<"s->slice:"<<*it<<endl;
	typedef list<PGenerate*>::const_iterator genscheme_iter_t;
	cout << "module " << mod_name()<<endl;
	for (genscheme_iter_t gene = generate_schemes.begin()
		; gene != generate_schemes.end() ; ++ gene ) {
		cout<<"slice find :"<<(*gene)->get_lineno()<<endl;
		if(s->slice.find((*gene)->get_lineno()) != s->slice.end())
	    {
	    	cout<<"find success"<<endl;
	    	(*gene)->dump_slice(out, 4, s);
	    }
	}
	
	out << "endmodule" << endl;
}

void PProcess::dump_slice(std::ostream&out, unsigned ind, slicer* s) const
{
	switch (type_) {
	case IVL_PR_INITIAL:
		out << setw(ind) << "" << "initial";
		break;
	case IVL_PR_ALWAYS:
		out << setw(ind) << "" << "always";
		break;
	}
	
	out << " /* " << get_fileline() << " */" << endl;

	for (map<perm_string,PExpr*>::const_iterator idx = attributes.begin()
		; idx != attributes.end() ; idx++ ) {
		
		out << setw(ind+2) << "" << "(* " << (*idx).first;
		if ((*idx).second) {
			out << " = " << *(*idx).second;
		}
		out << " *)" << endl;
	}

	statement_->dump_slice(out, ind+2, s);
}

void Statement::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	/* I give up. I don't know what type this statement is,
	   so just print the C++ typeid and let the user figure
	   it out. */
      out << setw(ind) << "";
      out << "/* " << get_fileline() << ": " << typeid(*this).name()
	  << " */ ;" << endl;
}

void PAssign::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	if(s->slice.find(get_lineno()) != s->slice.end())
	{
		out << setw(ind) << "";
		if(delay_!=0)
			out << *lval() << " = " << delay_ << " " << *rval() << ";";
		else
			out << *lval() << " = " << " " << *rval() << ";";
		out << "  /* " << get_fileline() << " */" << endl;
	}
}

void PAssignNB::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	if(s->slice.find(get_lineno()) != s->slice.end())
	{
		out << setw(ind) << "";
		if(delay_ != 0)
			out << *lval() << " <= " << delay_ << " " << *rval() << ";";
		else
			out << *lval() << " <= " << " " << *rval() << ";";
		out << "  /* " << get_fileline() << " */" << endl;
	}
}

void PBlock::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
      out << setw(ind) << "" << "begin";
      if (!pscope_name().nil())
	    out << " : " << pscope_name();
      out << endl;

      for (unsigned idx = 0 ;  idx < list_.size() ;  idx += 1) 
	  {
	    if (list_[idx])
		{
			//if(s->slice.find(list_[idx]->get_lineno()) != s->slice.end())
			//{
				list_[idx]->dump_slice(out, ind+2, s);
			//}
		}
	    else
		  out << setw(ind+2) << "" << "/* NOOP */ ;" << endl;
      }

      out << setw(ind) << "" << "end" << endl;
}

void PCallTask::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	if(s->slice.find(get_lineno()) != s->slice.end())
	{
		out << setw(ind) << "" << path_;
		
		if (parms_.size() > 0) {
			out << "(";
			if (parms_[0])
				out << *parms_[0];
			
			for (unsigned idx = 1 ;  idx < parms_.size() ;  idx += 1) {
				out << ", ";
				if (parms_[idx])
					out << *parms_[idx];
			}
			out << ")";
		}
		
		out << "; /* " << get_fileline() << " */" << endl;
	}
}

void PCase::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	if(s->slice.find(get_lineno()) != s->slice.end())
	{
		out << setw(ind) << "";
		switch (type_) {
		case NetCase::EQ:
			out << "case";
			break;
		case NetCase::EQX:
			out << "casex";
			break;
		case NetCase::EQZ:
			out << "casez";
			break;
		}
		out << " (" << *expr_ << ") /* " << get_fileline() << " */" << endl;
		
		for (unsigned idx = 0 ;  idx < items_->count() ;  idx += 1) {
			PCase::Item*cur = (*items_)[idx];
			
			if (cur->expr.size() == 0) {
				out << setw(ind+2) << "" << "default:";
				
			} else {
				out << setw(ind+2) << "" << *cur->expr.front();
				
				for(list<PExpr*>::iterator e = cur->expr.begin();
				 e != cur->expr.end() ; ++e)
					out << ", " << *e;
				
				out << ":";
			}
			
			if (cur->stat) {
				//if(s->slice.find(cur->stat->get_lineno()) != s->slice.end())
				//{
					out << endl;
					cur->stat->dump_slice(out, ind+6, s);
				//}
			} else {
				out << " ;" << endl;
			}
		}
		
		out << setw(ind) << "" << "endcase" << endl;
	}
}

void PCondit::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
    if(s->slice.find(get_lineno()) != s->slice.end())
	{  
		out << setw(ind) << "" << "if (" << *expr_ << ")" << endl;
		if (if_)
				if_->dump_slice(out, ind+3, s);
		else
			out << setw(ind) << ";" << endl;
		if (else_) {
			svector<unsigned> tmp = svector<unsigned>(else_->get_linenos());
			for(unsigned idx = 0; idx < tmp.count(); ++idx)
			{
				if(s->slice.find(tmp[idx]) != s->slice.end())
				{
					out << setw(ind) << "" << "else" << endl;
					else_->dump_slice(out, ind+3, s);
					break;
				}
			}
		}
	}
}

void PCAssign::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
    if(s->slice.find(get_lineno()) != s->slice.end())
	{
		out << setw(ind) << "" << "assign " << *lval_ << " = " << *expr_
			<< "; /* " << get_fileline() << " */" << endl;
	}
}

void PDeassign::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
    if(s->slice.find(get_lineno()) != s->slice.end())
	{  
		out << setw(ind) << "" << "deassign " << *lval_ << "; /* "
			<< get_fileline() << " */" << endl;
	}
}

void PDelayStatement::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	if(s->slice.find(get_lineno()) != s->slice.end())
	{
		out << setw(ind) << "" << "#" << *delay_ << " /* " <<
			get_fileline() << " */";
		if (statement_) {
			out << endl;
			statement_->dump_slice(out, ind+2, s);  //modify!!
		} else {
			out << " /* noop */;" << endl;
		}
	}
}

void PDisable::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	if(s->slice.find(get_lineno()) != s->slice.end())
	{
		out << setw(ind) << "" << "disable " << scope_ << "; /* "
			<< get_fileline() << " */" << endl;
	}
}

void PEventStatement::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	if(s->slice.find(get_lineno()) != s->slice.end())
	{
		if (expr_.count() == 0) {
			out << setw(ind) << "" << "@* ";
			
		} else {
			if(s->vars.find(expr_[0]->expr()->get_var(0)) != s->vars.end())
			{
				out << setw(ind) << "" << "@(" << *(expr_[0]);
			}
			if (expr_.count() > 1)
				for (unsigned idx = 1 ;  idx < expr_.count() ;  idx += 1)
					if(s->vars.find(expr_[idx]->expr()->get_var(0)) != s->vars.end())
					{
						out << " or " << *(expr_[idx]);
					}
				
				out << ")";
		}
		
		if (statement_) {
			out << endl;
			statement_->dump_slice(out, ind+2, s);
		} else {
			out << " ;" << endl;
		}
	}
}

void PForce::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	if(s->slice.find(get_lineno()) != s->slice.end())
	{
		out << setw(ind) << "" << "force " << *lval_ << " = " << *expr_
			<< "; /* " << get_fileline() << " */" << endl;
	}
}

void PForever::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	if(s->slice.find(get_lineno()) != s->slice.end())
	{
		out << setw(ind) << "" << "forever /* " << get_fileline() << " */" << endl;
		statement_->dump_slice(out, ind+3, s);
	}
}

void PForStatement::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	if(s->slice.find(get_lineno()) != s->slice.end())
	{
		out << setw(ind) << "" << "for (" << *name1_ << " = " << *expr1_
			<< "; " << *cond_ << "; <for_step>)" << endl;
		step_->dump(out, ind+6);
		if(statement_)
			statement_->dump_slice(out, ind+3, s);
		else
			out << setw(ind+3) << "" << "/* NOOP */" << endl;
	}
}

void PRelease::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	if(s->slice.find(get_lineno()) != s->slice.end())
	{
		out << setw(ind) << "" << "release " << *lval_ << "; /* "
			<< get_fileline() << " */" << endl;
	}
}

void PRepeat::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	if(s->slice.find(get_lineno()) != s->slice.end())
	{
		out << setw(ind) << "" << "repeat (" << *expr_ << ")" << endl;
		statement_->dump_slice(out, ind+3, s);
	}
}

void PTrigger::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	if(s->slice.find(get_lineno()) != s->slice.end())
		out << setw(ind) << "" << "-> " << event_ << ";" << endl;
}

void PWhile::dump_slice(ostream&out, unsigned ind, slicer* s) const
{
	if(s->slice.find(get_lineno()) != s->slice.end())
	{
		out << setw(ind) << "" << "while (" << *cond_ << ")" << endl;
		statement_->dump_slice(out, ind+3, s);
	}
}