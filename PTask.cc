/*
 * Copyright (c) 1999-2019 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

# include "config.h"
# include  "PTask.h"
# include  <cassert>

svector<unsigned>& PTask::get_linenos()
{
	svector<unsigned>*tmp = new svector<unsigned>();
	svector<unsigned> tmp1;

	//tmp = new svector<unsigned>(*tmp, get_lineno());
	if(statement_)
	{
		tmp1 = statement_->get_linenos();
		for(unsigned i = 0; i < tmp1.count(); ++i)
			tmp = new svector<unsigned>(*tmp, tmp1[i]);
			//tmp->insert(tmp1[i]);
	}
	return *tmp;
}

svector<unsigned>& PFunction::get_linenos()
{
	svector<unsigned>*tmp = new svector<unsigned>();
	svector<unsigned> tmp1;

	//tmp = new svector<unsigned>(*tmp, get_lineno());
	if(statement_)
	{
		tmp1 = statement_->get_linenos();
		for(unsigned i = 0; i < tmp1.count(); ++i)
			tmp = new svector<unsigned>(*tmp, tmp1[i]);
			//tmp->insert(tmp1[i]);
	}
	return *tmp;
}

PTaskFunc::PTaskFunc(perm_string n, LexicalScope*p)
: PScope(n,p), this_type_(0), ports_(0)
{
}

PTaskFunc::~PTaskFunc()
{
}

bool PTaskFunc::var_init_needs_explicit_lifetime() const
{
      return default_lifetime == STATIC;
}

void PTaskFunc::set_ports(vector<pform_tf_port_t>*p)
{
      assert(ports_ == 0);
      ports_ = p;
}

unsigned PTaskFunc::port_count() //2021.2.4
{
      if(ports_)
            return ports_->size();
}
PWire* PTaskFunc::get_port(unsigned idx)
{
	assert(idx < ports_->size());
	return (*ports_)[idx].port;
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

void PTaskFunc::set_this(class_type_t*type, PWire*this_wire)
{
      assert(this_type_ == 0);
      this_type_ = type;

	// Push a synthesis argument that is the "this" value.
      if (ports_==0)
	    ports_ = new vector<pform_tf_port_t>;

      size_t use_size = ports_->size();
      ports_->resize(use_size + 1);
      for (size_t idx = use_size ; idx > 0 ; idx -= 1)
	    ports_->at(idx) = ports_->at(idx-1);

      ports_->at(0) = pform_tf_port_t(this_wire);
}

PTask::PTask(perm_string name, LexicalScope*parent, bool is_auto__)
: PTaskFunc(name, parent), statement_(0)
{
      is_auto_ = is_auto__;
}

PTask::~PTask()
{
}

void PTask::set_statement(Statement*s)
{
      assert(statement_ == 0);
      statement_ = s;
}

PNamedItem::SymbolType PTask::symbol_type() const
{
      return TASK;
}
