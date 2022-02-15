#include "config.h"

#include "PTask.h"
#include "Statement.h"

PFunction::PFunction()
: out_(0), ports_(0), statement_(0)
{
}

PFunction::~PFunction()
{
}

void PFunction::set_ports(svector<PWire *>*p)
{
      assert(ports_ == 0);
      ports_ = p;
}

void PFunction::set_statement(Statement*s)
{
      assert(s != 0);
      assert(statement_ == 0);
      statement_ = s;
}

void PFunction::set_output(PWire*o)
{
      assert(out_ == 0);
      out_ = o;
}

void PFunction::add_decl(PWire* decl)
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

set<string>& PFunction::get_funcs()
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