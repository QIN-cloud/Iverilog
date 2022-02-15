/*
<<<<<<< Updated upstream
 * Copyright (c) 1998-2021 Stephen Williams (steve@icarus.com)
=======
 * Copyright (c) 1998-2019 Stephen Williams (steve@icarus.com)
>>>>>>> Stashed changes
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

# include  "Module.h"
# include  "PGate.h"
# include  "PWire.h"
# include  <cassert>
<<<<<<< Updated upstream
=======
# include  "string.h"
>>>>>>> Stashed changes

using namespace std;

list<Module::named_expr_t> Module::user_defparms;

/* n is a permallocated string. */
Module::Module(LexicalScope*parent, perm_string n)
: PScopeExtra(n, parent)
{
      library_flag = false;
      is_cell = false;
      is_interface = false;
      program_block = false;
      uc_drive = UCD_NONE;
}

Module::~Module()
{
}

void Module::add_gate(PGate*gate)
{
      gates_.push_back(gate);
}

unsigned Module::port_count() const
{
      return ports.size();
}

/*
 * Return the array of PEIdent object that are at this port of the
 * module. If the port is internally unconnected, return an empty
 * array.
 */
const vector<PEIdent*>& Module::get_port(unsigned idx) const
{
      assert(idx < ports.size());
      static const vector<PEIdent*> zero;

      if (ports[idx])
	    return ports[idx]->expr;
      else
	    return zero;
}

unsigned Module::find_port(const char*name) const
{
      assert(name != 0);
      for (unsigned idx = 0 ;  idx < ports.size() ;  idx += 1) {
	    if (ports[idx] == 0) {
		    /* It is possible to have undeclared ports. These
		       are ports that are skipped in the declaration,
		       for example like so: module foo(x ,, y); The
		       port between x and y is unnamed and thus
		       inaccessible to binding by name. */
		  continue;
	    }
	    assert(ports[idx]);
	    if (ports[idx]->name == name)
		  return idx;
      }

      return ports.size();
}

perm_string Module::get_port_name(unsigned idx) const
{

      assert(idx < ports.size());
      if (ports[idx] == 0 || ports[idx]->name.str() == 0) {
              /* It is possible to have undeclared ports. These
                 are ports that are skipped in the declaration,
                 for example like so: module foo(x ,, y); The
                 port between x and y is unnamed and thus
                 inaccessible to binding by name. Port references
		 that aren't simple or escaped identifiers are
		 also inaccessible to binding by name. */
            return perm_string::literal("unnamed");
      }
      return ports[idx]->name;
}

<<<<<<< Updated upstream
PExpr* Module::get_port_default_value(unsigned idx) const
{
      assert(idx < ports.size());
      return ports[idx] ? ports[idx]->default_value : 0;
}

=======
PTask* Module::get_task(const string&name)
{
      const char* tmpname = name.c_str();
      perm_string tmp(tmpname);
	map<perm_string,PTask*>::const_iterator obj = tasks.find(tmp);
	if(obj==tasks.end())
		return 0;
	else
		return (*obj).second;
}

PTask* Module::get_task(perm_string name)
{
	map<perm_string,PTask*>::const_iterator obj = tasks.find(name);
	if(obj==tasks.end())
		return 0;
	else
		return (*obj).second;
}


PWire* Module::get_wire(perm_string name) const
{
      map<perm_string,PWire*>::const_iterator obj = wires.find(name);
      if (obj == wires.end())
	    return 0;
      else
	    return (*obj).second;
}

const map<perm_string,PWire*>& Module::get_wires() const
{
      return wires;
}

void Module::build_cfgs()
{
	cfg_ = mn_->build_cfgs();
}

set<string>& Module::get_mods()
{
	set<string>* tmp = new set<string>;
	set<string>::const_iterator pos;
	list<PGate*>::const_iterator ppos;
	set<string> tmp1;

	for(ppos = gates_.begin(); ppos != gates_.end(); ++ppos)
	{
		tmp1 = (*ppos)->get_modname();
		for(pos = tmp1.begin(); pos != tmp1.end(); ++pos)
			tmp->insert(*pos);
	}
	return *tmp;
}

ostream& Module::dump_cfg(ostream& o)
{
	for(unsigned y = 0; y < cfg_->cfgs->count(); ++y)
	{
		o << "Process ID: " << (*(cfg_->cfgs))[y]->id << endl;
		for(unsigned z = 0; z < (*(cfg_->cfgs))[y]->root->count(); ++z)
		{
			o << "Index = " << z <<", ";
			o << *(*((*(cfg_->cfgs))[y]->root))[z];
			o << endl;
		}
	}

	return o;
}

void Module::build_cetable()
{
	for(unsigned idx = 0; idx < cfg_->cfgs->count(); ++idx)
	{
		unsigned curprocid = (*(cfg_->cfgs))[idx]->id;
		unsigned count = 0;
		for(unsigned z = 0; z < (*(cfg_->cfgs))[idx]->root->count(); ++z)
		{
			if((*((*(cfg_->cfgs))[idx]->root))[z]->type.find("ISCONTROL") == 0)
			{
				cond_expr* ce = new cond_expr;
				ce->procid = curprocid;
				ce->condid = count;
				ce->lineno = (*((*(cfg_->cfgs))[idx]->root))[z]->lineno;
				(*((*(cfg_->cfgs))[idx]->root))[z]->condid = count;
				svector<string> sset;
				for(unsigned x = 0;	x < (*((*(cfg_->cfgs))[idx]->root))[z]->expr.count(); ++x)
				{
					sset = svector<string>(sset, (*((*(cfg_->cfgs))[idx]->root))[z]->expr[x]->get_vars());
				}
				for(unsigned y = 0; y < sset.count(); ++y)
				{
					RefVar* rv = new RefVar;
					rv->name = sset[y];
					rv->time = 0;
					rv->space = 0;
					ce->refs.insert(rv);
				}
				ce->expr = (*((*(cfg_->cfgs))[idx]->root))[z]->expr;
				cetab_.push_back(ce);
				count = count + 1;
			}
		}
	}
}

void Module::coverage(Design* des, const char* vcd_file)
{
	FILE* vcd_handle;        /* Pointer to opened VCD file */
	
	if( (vcd_handle = fopen( vcd_file, "r" )) != NULL ) 
	{
		enter_scope = false;
		scope_count = 0;
		found_in_vcd = false;
		vcd_parse_def( vcd_handle );
		//dump_vcd_vars();
		cur_sim_time = 0;
		//vcd_parse_sim( des, vcd_handle );
		//sim_timestep(des);
	} else 
	{
		cout << "Unable to open specified VCD file" << endl;
		exit( 1 );
	}
}

void Module::vcd_parse_def_ignore( FILE* vcd ) 
{
	bool end_seen = false;  /* If set to true, $end keyword was seen */
	char token[256];        /* String value of current token         */
	int  tokval;            /* Set to number of tokens found         */
	int  chars_read;        /* Number of characters scanned in       */
	
	while( !end_seen && ((tokval = fscanf( vcd, "%s%n", token, &chars_read )) == 1) ) 
	{
		assert( chars_read <= 256 );
		if( strncmp( "$end", token, 4 ) == 0 ) {
			end_seen = true;
		}
	}
	assert( end_seen );
}

void Module::vcd_parse_def_var( FILE* vcd ) 
{
	char type[256];        /* Variable type                   */
	int  size;             /* Bit width of specified variable */
	char id_code[256];     /* Unique variable identifier_code */
	char ref[256];         /* Name of variable in design      */
	char tmp[15];          /* Temporary string holder         */
	int  msb = -1;         /* Most significant bit            */
	int  lsb = -1;         /* Least significant bit           */
	//char t1, t2;
	
	if( fscanf( vcd, "%s %d %s %s %s", type, &size, id_code, ref, tmp ) == 5 )
	{
		/* Make sure that we have not exceeded array boundaries */
		assert( strlen( type )    <= 256 );
		assert( strlen( ref )     <= 256 );
		assert( strlen( tmp )     <= 15  );
		assert( strlen( id_code ) <= 256 );
		
		if( strncmp( "$end", tmp, 4 ) != 0 ) 
		{
			/* A bit select was specified for this signal, get the size */
			if( sscanf( tmp, "\[%d:%d]", &msb, &lsb ) != 2 ) 
			{        
				if( sscanf( tmp, "\[%d]", &lsb ) != 1 ) 
				{
					cerr << "Unrecognized $var format" << endl;
					exit( 1 );
				}
			}
			if( (fscanf( vcd, "%s", tmp ) != 1) || (strncmp( "$end", tmp, 4 ) != 0) ) 
			{
				cerr << "Unrecognized $var format" << endl;
				exit( 1 );
			}
		} 
		else 
		{
			msb = size - 1;
			lsb = 0;
		}
		
		//add vcd var define to module vcd_var;
		vcd_var vv;
		vv.name_ = ref;
		vv.little_endia = msb >= lsb ? true : false;
		verinum vn(verinum::Vx, msb - lsb + 1);
		vv.pre_val_ = vn;
		vv.cur_val_ = vn;
		vcd_vars_[id_code] = vv;
		
	} 
	else 
	{
		cout << "Unrecognized $var format" << endl;
		exit( 1 );
    }
}

void Module::vcd_parse_def_scope( FILE* vcd) 
{
	char type[256];     /* Scope type                 */
	char id[256];       /* Name of scope to change to */
	
	if( fscanf( vcd, "%s %s $end", type, id ) == 2 ) 
	{
		/* Make sure that we have not exceeded any array boundaries */
		assert( strlen( type ) <= 256 );
		assert( strlen( id )   <= 256 );
		if((strcmp(type, "module") == 0) && (strcmp(pscope_name(), id) == 0))    
		{
			enter_scope = true;
			found_in_vcd = true;
		}
	} 
	else 
	{
		cerr << "Unrecognized $scope format" << endl;
		exit( 1 );
	}
}

void Module::vcd_parse_def( FILE* vcd ) 
{
	bool enddef_found = false;  /* If set to true, definition section is finished */
	char keyword[256];          /* Holds keyword value                            */
	int  chars_read;            /* Number of characters scanned in                */
	
	while( !enddef_found && (fscanf( vcd, "%s%n", keyword, &chars_read ) == 1) ) {
		
		assert( chars_read <= 256 );
		
		if( keyword[0] == '$' ) 
		{
			
			if( strncmp( "var", (keyword + 1), 3 ) == 0 ) 
			{
				if(enter_scope)
					vcd_parse_def_var( vcd );
			} 
			else if( strncmp( "scope", (keyword + 1), 5 ) == 0 ) 
			{
				vcd_parse_def_scope( vcd );
				if(enter_scope)
					scope_count = scope_count + 1;
			} 
			else if( strncmp( "upscope", (keyword + 1), 7 ) == 0 ) 
			{
				if(enter_scope)
					scope_count = scope_count - 1;
				if(scope_count < 0)
					enter_scope = false;
				vcd_parse_def_ignore( vcd );
			} 
			else if( strncmp( "enddefinitions", (keyword + 1), 14 ) == 0 )
			{
				enddef_found = true;
				vcd_parse_def_ignore( vcd );
			} 
			else 
			{
				vcd_parse_def_ignore( vcd );
			}
		} 
		else 
		{
			cerr << "Non-keyword located where one should have been " << keyword << endl;
			exit( 1 );
		}  
	}
	
	assert( enddef_found );
	
	/* Check to see that at least one instance was found */
	if( !found_in_vcd) 
	{
		cerr << "No module were found in specified VCD file that matched design!" << endl;
		exit( 1 );
	}
}
/*
void Module::vcd_parse_sim( Design* des, FILE* vcd )
{
	char token[4100];         // Current token from VCD file    
	int  chars_read;          // Number of characters scanned in   
	
	//cur_sim_time = -1;
	while( !feof( vcd ) && (fscanf( vcd, "%s%n", token, &chars_read ) == 1) ) 
	{
		assert( chars_read <= 4100 );
		if( token[0] == '$' ) 
		{
			// Currently ignore all simulation keywords 
		} 
		else if( (token[0] == 'b') || (token[0] == 'B') ) 
		{
			vcd_parse_sim_vector( vcd, (token + 1) );
		} 
		else if( (token[0] == 'r') || (token[0] == 'B') ) 
		{
			vcd_parse_sim_ignore( vcd );
		} 
		else if( token[0] == '#' ) 
		{
			if( cur_sim_time >= 0 ) {
				sim_timestep(des);
			}
			cur_sim_time = atol( token + 1 );
		} 
		else if( (token[0] == '0') ||
			(token[0] == '1') ||
			(token[0] == 'x') ||
			(token[0] == 'X') ||
			(token[0] == 'z') ||
			(token[0] == 'Z') ) 
		{
			set_symbol_char( token + 1, token[0] );
		} 
		else 
		{
			cerr << "Badly placed token " << token << endl;
			exit( 1 );
		}
	}
}
*/

void Module::vcd_parse_sim_ignore( FILE* vcd ) 
{
	char sym[256];    /* String value of signal symbol   */
	int  chars_read;  /* Number of characters scanned in */
	
	if( fscanf( vcd, "%s%n", sym, &chars_read ) != 1 ) 
	{
		cerr << "Bad file format!" << endl;
		exit( 1 );
	}
	assert( chars_read <= 256 );
}

void Module::vcd_parse_sim_vector( FILE* vcd, char* value )
{
	char sym[256];    /* String value of signal symbol   */
	int  chars_read;  /* Number of characters scanned in */
	
	if( fscanf( vcd, "%s%n", sym, &chars_read ) == 1 ) {
		assert( chars_read <= 256 );
		//find the symbol in vcd_vars_ table and set the value;
		map<string, vcd_var>::iterator pos = vcd_vars_.find(sym);
		if(pos != vcd_vars_.end())
		{
			assert(strlen(value) <= pos->second.cur_val_.len());
			pos->second.pre_val_ = pos->second.cur_val_;
			verinum vn(verinum::Vx, strlen(value));
			unsigned index;
			index = strlen(value) - 1;
			for(unsigned idx = 0; idx < strlen(value); ++idx)
			{
				switch(value[idx])
				{
				case '0' :
					vn.set(index, verinum::V0);
					break;
				case '1':
					vn.set(index, verinum::V1);
					break;
				case 'x':
				case 'X':
					vn.set(index, verinum::Vx);
					break;
				case 'z':
				case 'Z':
					vn.set(index, verinum::Vz);
					break;
				}
				index = index - 1;
			}
			pos->second.cur_val_ = vn;
		}
	} 
	else 
	{
		cerr << "Bad file format!" << endl;
		exit( 1 );
	}
}

void Module::set_symbol_char( char* sym, char value ) 
{
	map<string, vcd_var>::iterator pos = vcd_vars_.find(sym);
	if(pos != vcd_vars_.end())
	{
		assert(1 <= pos->second.cur_val_.len());
		pos->second.pre_val_ = pos->second.cur_val_;
		verinum vn(verinum::Vx, 1);
		switch(value)
		{
		case '0' :
			vn.set(0, verinum::V0);
			break;
		case '1':
			vn.set(0, verinum::V1);
			break;
		case 'x':
		case 'X':
			vn.set(0, verinum::Vx);
			break;
		case 'z':
		case 'Z':
			vn.set(0, verinum::Vz);
			break;
		}
		pos->second.cur_val_ = vn;
	}
}
/*
void Module::sim_timestep(Design* des)
{
	bool found = false;
	set<int, less<int> > path;
	cout << "At time step : " << cur_sim_time << endl;
	//for each process, iterator over the vcd_vars_ table find the value change
	//signals, if find one, simulate the process and report result
	for(unsigned idx = 0; idx < cfg_->cfgs->count(); ++idx)
	{
		path.clear();
		for(set<string>::const_iterator spos = (*(cfg_->cfgs))[idx]->refs.begin(); spos != (*(cfg_->cfgs))[idx]->refs.end(); ++spos)
		{
			for(map<string, vcd_var>::const_iterator vpos = vcd_vars_.begin(); vpos != vcd_vars_.end(); ++vpos)
			{
				if((*spos == vpos->second.name_) && (vpos->second.cur_val_ != vpos->second.pre_val_))
				{
					//do simulation
					unsigned index = 1;
					while((index > 0) && (index <= (*(cfg_->cfgs))[idx]->root->count() - 2))
					{
						//jump over start @ statement
						if((*(*(cfg_->cfgs))[idx]->root)[index]->type == "ISCONTROL.EVENT")
						{
							index = index + 1;
							continue;
						}
						//jump over 
						if((*(*(cfg_->cfgs))[idx]->root)[index]->type.find("ISCONTROL") != 0)
						{
							path.insert(index);
							index = index + 1;
							continue;
						}
						//for condition node
						path.insert(index);
						//evaluate the condition expression and get the direction of the condition evaluate
						index = eval_cond_expr(des, (*(*(cfg_->cfgs))[idx]->root)[index]);
					}
					found = true;
					break;
				}
			}
			if(found) break;
		}
		cout << "Process : " << (*(cfg_->cfgs))[idx]->id << ", coverage path is:" << endl;
		for(set<int, less<int> >::const_iterator ipos = path.begin(); ipos != path.end(); ++ipos)
		{
			cout << *ipos << ",";
		}
		cout << endl;
	}
}

void Module::dump_vcd_vars()
{
	for(map<string, vcd_var>::const_iterator pos = vcd_vars_.begin(); pos != vcd_vars_.end(); ++pos)
	{
		cout << "Symbol in VCD file is : " << pos->first << ", Variable name is : " << pos->second.name_;
		cout << ", Prevoius value is : " << pos->second.pre_val_ << ", Current value is : " << pos->second.cur_val_  << endl;
	}
}

int Module::eval_cond_expr(Design* des, Cfg_Node* node)
{
	int index;
	hname_t name(pscope_name());
	NetScope*scope = des->find_scope(name); 
	verinum* vn = node->expr[0]->evaluate(des, scope, vcd_vars_);
	if(node->type.find("CASE") == string::npos) //not case statement
	{
		if(vn->len() > 1)
		{
			verinum::V v = unary_or(*vn);
			if(v == verinum::V0)
				return node->dsuc[1]->index;
			if(v == verinum::V1)
				return node->dsuc[0]->index;
		}
		else
		{
			if(vn[0] == verinum::V0)
				return node->dsuc[1]->index;
			if(vn[0] == verinum::V1)
				return node->dsuc[0]->index;
		}
	}
	else //case statement
	{
		for(unsigned idx = 0; idx < node->dsuc.count(); ++idx)
		{
			for(set<PExpr*>::const_iterator pos = node->dsuc[idx]->caseitem.begin(); pos != node->dsuc[idx]->caseitem.end(); ++pos)
			{
				verinum* ci = (*pos)->evaluate(des, scope, vcd_vars_);
				if(node->type == "ISCONTROL.CASE")
				{
					if(*vn == *ci)
						return node->dsuc[idx]->index;
				}
				if((node->type == "ISCONTROL.CASEX") || (node->type == "ISCONTROL.CASEZ"))
				{
					if(ceq(*vn, *ci))
						return node->dsuc[idx]->index;
				}
			}
		}
	}
	return index;
}
*/
>>>>>>> Stashed changes

PGate* Module::get_gate(perm_string name)
{
      for (list<PGate*>::iterator cur = gates_.begin()
		 ; cur != gates_.end() ; ++ cur ) {

	    if ((*cur)->get_name() == name)
		  return *cur;
      }

      return 0;
}

const list<PGate*>& Module::get_gates() const
{
      return gates_;
}

PNamedItem::SymbolType Module::symbol_type() const
{
      if (program_block)
            return PROGRAM;
      if (is_interface)
            return INTERFACE;

      return MODULE;
}
<<<<<<< Updated upstream

bool Module::can_be_toplevel() const
{
      // Don't choose library modules.
      if (library_flag)
	    return false;

      // Don't choose modules with parameters without default value
      for (std::map<perm_string,param_expr_t*>::const_iterator cur =
	    parameters.begin(); cur != parameters.end(); cur++) {
	    if (cur->second->expr == 0)
		  return false;
      }

      return true;
}
=======
>>>>>>> Stashed changes
