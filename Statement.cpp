#include "config.h"

#include "Statement.h"
#include "PExpr.h"

svector<unsigned>& Statement::get_linenos()
{
	svector<unsigned>*tmp = new svector<unsigned>();
	tmp = new svector<unsigned>(*tmp, get_lineno());
	return *tmp;
}

Statement::~Statement()
{
}

set<string>& Statement::get_funcs()
{
	set<string>* tmp = new set<string>;
	return *tmp;
}

PAssign_::PAssign_(PExpr*lval, PExpr*ex)
: event_(0), lval_(lval), rval_(ex)
{
      delay_ = 0;
}

PAssign_::PAssign_(PExpr*lval, PExpr*de, PExpr*ex)
: event_(0), lval_(lval), rval_(ex)
{
      delay_ = de;
}

PAssign_::PAssign_(PExpr*lval, PEventStatement*ev, PExpr*ex)
: event_(ev), lval_(lval), rval_(ex)
{
      delay_ = 0;
}

PAssign_::~PAssign_()
{
      delete lval_;
      delete rval_;
}

PAssign::PAssign(PExpr*lval, PExpr*ex)
: PAssign_(lval, ex)
{
}

PAssign::PAssign(PExpr*lval, PExpr*d, PExpr*ex)
: PAssign_(lval, d, ex)
{
}

PAssign::PAssign(PExpr*lval, PEventStatement*d, PExpr*ex)
: PAssign_(lval, d, ex)
{
}

set<string>& PAssign::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	if(lval_)
	{
		tmp1 = lval_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(rval_)
	{
		tmp1 = rval_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(event_)
	{
		tmp1 = event_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(delay_)
	{
		tmp1 = delay_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

PAssign::~PAssign()
{
}

PAssignNB::PAssignNB(PExpr*lval, PExpr*ex)
: PAssign_(lval, ex)
{
}

PAssignNB::PAssignNB(PExpr*lval, PExpr*d, PExpr*ex)
: PAssign_(lval, d, ex)
{
}

set<string>& PAssignNB::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	if(lval_)
	{
		tmp1 = lval_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(rval_)
	{
		tmp1 = rval_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(event_)
	{
		tmp1 = event_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(delay_)
	{
		tmp1 = delay_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

PAssignNB::~PAssignNB()
{
}

PBlock::PBlock(const string&n, BL_TYPE t, const svector<Statement*>&st)
: name_(n), bl_type_(t), list_(st)
{
}

PBlock::PBlock(BL_TYPE t, const svector<Statement*>&st)
: bl_type_(t), list_(st)
{
}

PBlock::PBlock(BL_TYPE t)
: bl_type_(t)
{
}

svector<unsigned>& PBlock::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>();
	for(unsigned idx = 0; idx < list_.count(); ++idx)
	{
		svector<unsigned>*tmps = new svector<unsigned>(*tmp, list_[idx]->get_linenos());
		delete tmp;
		tmp = tmps;
	}
	return *tmp;
}

set<string>& PBlock::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	for(unsigned idx = 0; idx < list_.count(); ++idx)
	{
		tmp1 = list_[idx]->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

PBlock::~PBlock()
{
      for (unsigned idx = 0 ;  idx < list_.count() ;  idx += 1)
	    delete list_[idx];
}

PCallTask::PCallTask(const hname_t&n, const svector<PExpr*>&p)
: path_(n), parms_(p)
{
}

PCallTask::~PCallTask()
{
}

const hname_t& PCallTask::path() const
{
      return path_;
}

set<string>& PCallTask::get_funcs()
{
	set<string>* tmp = new set<string>;
	tmp->insert(path_.peek_name(path_.component_count()-1));
	return *tmp;
}

PCase::PCase(NetCase::TYPE t, PExpr*ex, svector<PCase::Item*>*l)
: type_(t), expr_(ex), items_(l)
{
}

set<string>& PCase::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = expr_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	for(unsigned i=0; i < items_->count(); ++i)
	{
		tmp1 = (*items_)[i]->stat->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
		for(unsigned j = 0; j < (*items_)[i]->expr.count(); ++j)
		{
			tmp1 = (*items_)[i]->expr[j]->get_funcname();
			for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
				tmp->insert(*pos);
		}
	}
	return *tmp;
}

svector<unsigned>& PCase::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>();
	tmp = new svector<unsigned>(*tmp, get_lineno());
	for(unsigned idx = 0; idx < items_->count(); ++idx)
	{
	    tmp = new svector<unsigned>(*tmp, (*items_)[idx]->stat->get_linenos());
	}
	return *tmp;
}

PCase::~PCase()
{
      delete expr_;
      for (unsigned idx = 0 ;  idx < items_->count() ;  idx += 1)
	    if ((*items_)[idx]->stat) delete (*items_)[idx]->stat;

      delete[]items_;
}

PCAssign::PCAssign(PExpr*l, PExpr*r)
: lval_(l), expr_(r)
{
}

set<string>& PCAssign::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = lval_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	tmp1 = expr_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	return *tmp;
}

PCAssign::~PCAssign()
{
      delete lval_;
      delete expr_;
}

PCondit::PCondit(PExpr*ex, Statement*i, Statement*e)
: expr_(ex), if_(i), else_(e)
{
}

set<string>& PCondit::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = expr_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	if(if_)
	{
		tmp1 = if_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(else_)
	{
		tmp1 = else_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

PCondit::~PCondit()
{
      delete expr_;
      delete if_;
      delete else_;
}

svector<unsigned>& PCondit::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>();
        tmp = new svector<unsigned>(*tmp, get_lineno());
	if(if_)
		tmp = new svector<unsigned>(*tmp, if_->get_linenos());
	if(else_)
		tmp = new svector<unsigned>(*tmp, else_->get_linenos());
	return *tmp;
}

PDeassign::PDeassign(PExpr*l)
: lval_(l)
{
}

set<string>& PDeassign::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = lval_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	return *tmp;
}

PDeassign::~PDeassign()
{
      delete lval_;
}


PDelayStatement::PDelayStatement(PExpr*d, Statement*st)
: delay_(d), statement_(st)
{
}

set<string>& PDelayStatement::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = delay_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	if(statement_)
	{
		tmp1 = statement_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

PDelayStatement::~PDelayStatement()
{
}

svector<unsigned>& PDelayStatement::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>(statement_->get_linenos());
	return *tmp;
}

PDisable::PDisable(const hname_t&sc)
: scope_(sc)
{
}

PDisable::~PDisable()
{
}

PEventStatement::PEventStatement(const svector<PEEvent*>&ee)
: expr_(ee), statement_(0)
{
      assert(expr_.count() > 0);
}

set<string>& PEventStatement::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	for(unsigned idx = 0; idx < expr_.count(); ++idx)
	{
		tmp1 = expr_[idx]->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(statement_)
	{
		tmp1 = statement_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

PEventStatement::PEventStatement(PEEvent*ee)
: expr_(1), statement_(0)
{
      expr_[0] = ee;
}

PEventStatement::PEventStatement(void)
: statement_(0)
{
}

PEventStatement::~PEventStatement()
{
	// delete the events and the statement?
}

void PEventStatement::set_statement(Statement*st)
{
      statement_ = st;
}

PForce::PForce(PExpr*l, PExpr*r)
: lval_(l), expr_(r)
{
}

set<string>& PForce::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = lval_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	tmp1 = expr_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	return *tmp;
}

PForce::~PForce()
{
      delete lval_;
      delete expr_;
}

PForever::PForever(Statement*s)
: statement_(s)
{
}

PForever::~PForever()
{
      delete statement_;
}

set<string>& PForever::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	if(statement_)
	{
		tmp1 = statement_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

svector<unsigned>& PForever::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>(statement_->get_linenos());
	return *tmp;
}

PForStatement::PForStatement(PExpr*n1, PExpr*e1, PExpr*cond,
			     PExpr*n2, PExpr*e2, Statement*st)
: name1_(n1), expr1_(e1), cond_(cond), name2_(n2), expr2_(e2),
  statement_(st)
{
}

PForStatement::~PForStatement()
{
}

set<string>& PForStatement::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	if(name1_)
	{
		tmp1 = name1_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(name2_)
	{
		tmp1 = name2_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(expr1_)
	{
		tmp1 = expr1_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(expr2_)
	{
		tmp1 = expr2_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(cond_)
	{
		tmp1 = cond_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	if(statement_)
	{
		tmp1 = statement_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

svector<unsigned>& PForStatement::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>();
	tmp = new svector<unsigned>(*tmp, get_lineno());
	tmp = new svector<unsigned>(*tmp, statement_->get_linenos());
	return *tmp;
}

PProcess::~PProcess()
{
      delete statement_;
}

set<string>& PProcess::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	if(statement_)
	{
		tmp1 = statement_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

PRelease::PRelease(PExpr*l)
: lval_(l)
{
}

set<string>& PRelease::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = lval_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	return *tmp;
}

PRelease::~PRelease()
{
      delete lval_;
}

PRepeat::PRepeat(PExpr*e, Statement*s)
: expr_(e), statement_(s)
{
}

set<string>& PRepeat::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = expr_->get_funcname();
	for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
		tmp->insert(*pos);
	if(statement_)
	{
		tmp1 = statement_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

PRepeat::~PRepeat()
{
      delete expr_;
      delete statement_;
}

svector<unsigned>& PRepeat::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>();
	tmp = new svector<unsigned>(*tmp, get_lineno());
	tmp = new svector<unsigned>(*tmp, statement_->get_linenos());
	return *tmp;
}

PTrigger::PTrigger(const hname_t&e)
: event_(e)
{
	string tmps(e.peek_name(e.component_count()-1));
}

PTrigger::~PTrigger()
{
}

PWhile::PWhile(PExpr*e1, Statement*st)
: cond_(e1), statement_(st)
{
}

set<string>& PWhile::get_funcs()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	set<string> tmp1;
	tmp1 = cond_->get_funcname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	if(statement_)
	{
		tmp1 = statement_->get_funcs();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

PWhile::~PWhile()
{
      delete cond_;
      delete statement_;
}

svector<unsigned>& PWhile::get_linenos()
{
	svector<unsigned>* tmp = new svector<unsigned>();
	tmp = new svector<unsigned>(*tmp, get_lineno());
	tmp = new svector<unsigned>(*tmp, statement_->get_linenos());
	return *tmp;
}
