#include "config.h"

#include "PTask.h"
#include "Statement.h"

PTask::PTask()
: ports_(0), statement_(0)
{
}

PTask::~PTask()
{
}

void PTask::set_ports(svector<PWire*>*p)
{
      assert(ports_ == 0);
      ports_ = p;
}

void PTask::set_statement(Statement*s)
{
      assert(statement_ == 0);
      statement_ = s;
}

void PTask::add_decl(PWire* decl)
{
	/*
	for(unsigned idx = 0; idx < decls_.count(); ++idx)
	{
		if(decls_[idx]->path() == decl->path()) goto l0;
	}
	decls_ = svector<PWire*>(decls_, decl);
l0:;
*/
	PWire*&ep = decls_[decl->path()];
        if (ep == 0){
            assert(ep == 0);
            ep = decl;
	}
}

unsigned PTask::port_count()
{
	if(ports_)
		return ports_->count();
}

PWire* PTask::get_port(unsigned idx)
{
	assert(idx < ports_->count());
	return (*ports_)[idx];
}

set<string>& PTask::get_funcs()
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