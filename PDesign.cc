#include "config.h"
#include "PDesign.h"
#include "Module.h"
#include "CfgNode.h"
#include "PExpr.h"
#include <iostream>
#include <string.h>
#include <iomanip>

using namespace std;

PDesign::PDesign()
{
	evaluate_out = ofstream("dump/evaluate.txt");
	control_out = ofstream("dump/control.txt");
}

PDesign:: ~PDesign()
{
	if(!mt_nodes.empty())
	{
		cout << "Release the module tree nodes..." << endl;
		for(map<perm_string, ModuleTreeNode*>::iterator pos = mt_nodes.begin(); pos != mt_nodes.end(); pos++)
		{
			delete pos->second;
			pos->second = 0;
		}
	}
	if(report)
	{
		cout << "Delete the report strctures..." << endl;
		delete report;
	}
	evaluate_out.close();
	control_out.close();
}

void PDesign::set_modules(map<perm_string, Module*> modules)
{
	modules_ = modules;
}

void PDesign::set_udps(map<perm_string, PUdp*>& udps)
{
	udps_ = udps;
}

void PDesign::set_design(Design* design)
{
	des = design;
	map<perm_string, Module*>::iterator md = modules_.begin();
	for(md; md != modules_.end(); md++)
	{
		md->second->set_design(design);
	}
}

map<perm_string, Module*> PDesign::get_modules()
{
	return modules_;
}

map<perm_string, PUdp*> PDesign::get_udps()
{
	return udps_;
}

void PDesign::dump(ostream&o)
{
	for(map<perm_string, Module*>::iterator pos = modules_.begin(); pos != modules_.end(); ++pos)
	{
		pos->second->dump(o);
	}
}

void PDesign::dump_module_tree(ostream& o)
{
	o << "----------------module tree information----------------" << endl;
	map<perm_string, ModuleTreeNode*>::iterator mt_node;
	for(mt_node = mt_nodes.begin(); mt_node != mt_nodes.end(); mt_node++)
	{
		o << "module " << mt_node->first <<endl;
		o << "{" << endl;
		map<perm_string, ModuleTreeNode*>::iterator module_gate;
		for(module_gate = mt_node->second->next_.begin(); module_gate != mt_node->second->next_.end(); module_gate++)
		{
			o << module_gate->second->get_module()->pscope_name() << " " << module_gate->first << endl;
		}
		o<< "}" << endl;
	}
}

void PDesign::dump_symbol_vars(ostream& o)
{
	o << "----------------symbol vars----------------" << endl;
	unordered_map<string, vector<pair<string, VcdScope*> > >::iterator symbol_var;
	for(symbol_var = symbol_vars.begin(); symbol_var != symbol_vars.end(); symbol_var++)
	{
		o << "symbol " << symbol_var->first <<endl;
		o << "{" << endl;
		vector<pair<string, VcdScope*> >::iterator var;
		for(var = symbol_var->second.begin(); var != symbol_var->second.end(); var++)
		{
			o << "module " << var->second->module_->pscope_name() << " " << "var " << var->first << endl;
		}
		o<< "}" << endl;
	}
}

void PDesign::dump_module_vars(ostream& o)
{
	map<perm_string, Module*>::iterator module_;
	for(module_ = modules_.begin(); module_ != modules_.end(); module_++)
	{
		o << "module " << module_->first << endl;
		o << "{" << endl;
		module_->second->dump_vars(o);
		o << "}" << endl;
		o << "endmodule" << endl;
	}
}

void PDesign::dump_everytime_vars(ostream& o)
{
	o << "Current time : " << cur_sim_time << endl;
	map<perm_string, Module*>::iterator module_;
	for(module_ = modules_.begin(); module_ != modules_.end(); module_++)
	{
		module_->second->dump_values(o);
	}
	 map<string, verinum>::iterator variety_symbol;
	 for(variety_symbol = variety_symbols.begin(); variety_symbol != variety_symbols.end(); variety_symbol++)
	 {
		o << "symbol : " << variety_symbol->first;
		variety_symbol->second.dump(o);
		o << endl;
	 }
	 o << endl;
}

void PDesign::dump_def_information()
{
	cout << "Dump the module trees..." << endl;

	const char* mt_path = "dump/module_tree.txt";
	ofstream mt_out(mt_path);
	dump_module_tree(mt_out);
	
	cout << "Dump the map of relationship between symbol and variables..." << endl;

	const char* symbol_vars_path = "dump/symbol_vars.txt";
	ofstream symbol_vars_out(symbol_vars_path);
	dump_symbol_vars(symbol_vars_out);

	cout << "Dump the variables of modules..." << endl;

	const char* module_vars_path = "dump/module_vars.txt";
	ofstream module_vars_out(module_vars_path);
	dump_module_vars(module_vars_out);

	cout << "Dump the cfgs..." << endl;

	const char* module_cfgs_path = "dump/module_cfgs.txt";
	ofstream module_cfgs_out(module_cfgs_path);
	for(map<perm_string, Module*>::iterator module = modules_.begin(); module != modules_.end(); module++)
	{
		module->second->dump_cfg(module_cfgs_out);
	}

}

void PDesign::build_before_cover(const char* fsm_selects)
{

	cout << "Build module instantiated tree." << endl;

	build_module_tree(top_);

	if(fsm_)
	{
		fsm_var_parse(fsm_selects);
	}

	cout << "Build paths and sort cfgs for select modules." << endl;

	map<Module*, bool>::iterator md = select_mod.begin();
	for(md; md != select_mod.end(); md++)
	{
		md->first->initialize();
		if(path_)
			md->first->build_paths();
	}

	report = new FinalRep(toggle_, fsm_, statement_, path_, branch_, combine_);
	
}

ModuleTreeNode* PDesign::build_module_tree(Module* module_)
{
	ModuleTreeNode * top = new ModuleTreeNode(module_);
	mt_nodes[module_->pscope_name()] = top;

	/*Use DFS to search and build a module tree.*/
	if(top)
	{
		/*Search the instantiations in this module.*/
		list<PGate*> inst_modules = module_->get_gates();

		for(PGate* inst_module : inst_modules)
		{
			if(inst_module->ty != PGate::MODULE)
			{
				continue;
			}

			perm_string module_name = inst_module->get_name();
			perm_string module_type = inst_module->get_type();

			assert(module_name);
			assert(module_type);

			Module* son_module = modules_[module_type];
			
			if(son_module)
			{
				ModuleTreeNode* son = mt_nodes[module_type];

				if(!son)
				{
					son = build_module_tree(son_module);
				}

				top->next_[module_name] = son;
			}

			else
			{
				cerr << "Question in module tree building." << endl;
				exit(1);
			}
		}
	}
	return top;
}

void PDesign::fsm_var_parse(const char* fsm_selects)
{
	cout << "Select fsm variables : " ;
	unsigned i = 0;
	string tmp;
	while(i < strlen(fsm_selects))
	{
		if(fsm_selects[i] == ',')
		{
			if(!tmp.empty())
			{
				fsm_names[tmp] = true;
				tmp.clear();
			}
		}
		else
		{
			tmp = tmp + fsm_selects[i];
		}
		i++;
	}
	if(!tmp.empty())
	{
		fsm_names[tmp] = true;
	}
	map<string, bool>::iterator fsm_name;
	for(fsm_name = fsm_names.begin(); fsm_name != fsm_names.end(); fsm_name++)
	{
		cout << fsm_name->first << " ";
	}
	cout << endl;
}

void PDesign::function_cover(perm_string top_module, perm_string select_module, const char* fsm_selects, const char* vcd_file, ostream& target_file, 
bool toggle, bool fsm, bool statement, bool path, bool branch, bool combine)
{
	toggle_ = toggle;
	fsm_ = fsm;
	statement_ = statement;
	path_ = path;
	branch_ = branch;
	combine_ = combine;

	if(modules_.find(top_module) != modules_.end())
	{
		top_ = modules_[top_module];
	}

	else
	{
		cerr << "The top module " << top_module << " can not be found in Verilog file." << endl;
		exit(1);
	}

	if(select_module)
	{
		if(modules_.find(select_module) != modules_.end())
		{
			select_mod[modules_[select_module]] = true;
		}
		else
		{
			cerr << "The select module " << select_module << " can not be found in Verilog file." << endl;
			exit(1);
		}
	}

	else
	{
		map<perm_string, Module*>::iterator md = modules_.begin();
		for(md; md != modules_.end(); md++)
		{
			if(md->second != top_)
				select_mod[md->second] = true;
		}
	}

	cout << "Initializing for modules before coverage analysis." << endl;

	build_before_cover(fsm_selects);

	FILE* vcd_handle;        /* Pointer to opened VCD file */

	if( (vcd_handle = fopen( vcd_file, "r" )) != NULL ) 
	{
		cout << "Parsing the definition lines in vcd file..." << endl;

		vcd_parse_def(vcd_handle);

		cout << "Parsing the variables part in vcd file..." << endl;
		
		vcd_parse_sim(vcd_handle);

		cout << "Generate the coverage report..." << endl;
		
		report_coverage(target_file);
	} 

	else 
	{
		cerr << "Unable to open specified VCD file" << endl;
		exit( 1 );
	}
}

void PDesign::vcd_parse_def( FILE* vcd ) 
{
	bool enddef_found = false;  /* If set to true, definition section is finished */
	char keyword[256];          /* Holds keyword value                            */
	int  chars_read;            /* Number of characters scanned in                */
	
	scope_count = 0;
	found_in_vcd = false;

	while( !enddef_found && (fscanf( vcd, "%s%n", keyword, &chars_read ) == 1) ) {
		
		assert( chars_read <= 256 );
		
		if( keyword[0] == '$' ) 
		{
			if( strncmp( "var", (keyword + 1), 3 ) == 0 ) 
			{
				if(!enter_scopes.empty())
					vcd_parse_def_var( vcd );
				else
				{
					cerr << "Problem of parsing ""var"" with scope handling of keyword " << keyword << endl;
					exit( 1 );
				}
			} 
			else if( strncmp( "scope", (keyword + 1), 5 ) == 0 ) 
			{
				vcd_parse_def_scope( vcd );
				scope_count += 1;
			} 
			else if( strncmp( "upscope", (keyword + 1), 7 ) == 0 ) 
			{
				if(enter_scopes.size() == scope_count)
				{
					enter_scopes.pop();
				}
				current_scope = 0;

				scope_count -=1;
				if(scope_count < 0)
				{
					cerr << "Problem in upscope that scope_count < 0" << endl;
					exit( 1 );
				}
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

void PDesign::vcd_parse_def_ignore( FILE* vcd ) 
{
	bool end_seen = false;  /* If set to true, $end keyword was seen */
	char token[256];        /* String value of current token         */
	int  tokval;            /* Set to number of tokens found         */
	int  chars_read;        /* Number of characters scanned in       */
	
	while( !end_seen && ((tokval = fscanf( vcd, "%s%n", token, &chars_read )) == 1) ) 
	{
		assert( chars_read <= 256 );
		if( strncmp( "$end", token, 4 ) == 0 ) 
		{
			end_seen = true;
		}
	}
	assert( end_seen );
}

void PDesign::vcd_parse_def_var( FILE* vcd ) 
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
				else
				{
					msb = lsb;
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

		if(select_mod.find(current_scope->module_) != select_mod.end())
		{
			//Initial varialble
			VcdVar* vv = new VcdVar;
			vv->name = ref;
			vv->scope = current_scope;
			vv->little_endia = msb >= lsb ? true : false;
			assert(vv->little_endia);
			verinum vn(verinum::Vx, size);
			vv->cur_val = vn;
			vv->pre_val = vn;
			vv->width = size;
			vv->lsb = lsb;
			vv->msb = msb;
			vv->symbol = string(id_code);
			current_scope->vars_[string(id_code)] = vv;
			symbol_vars[string(id_code)].push_back(make_pair(vv->name, current_scope));
			if(fsm_names.find(vv->name) != fsm_names.end())
			{
				fsm_vars[vv] = true;
			}
		}
	} 
	else 
	{
		cout << "Unrecognized $var format" << endl;
		exit( 1 );
    }
}

void PDesign:: vcd_parse_def_scope( FILE* vcd) 
{
	char type[256];     /* Scope type                 */
	char id[256];       /* Name of scope to change to */

	if( fscanf( vcd, "%s %s $end", type, id ) == 2 ) 
	{
		/* Make sure that we have not exceeded any array boundaries */
		assert( strlen( type ) <= 256 );
		assert( strlen( id )   <= 256 );

		/* Format of $scope module module_name ...... */
		if(strcmp(type, "module") == 0)    
		{
			found_in_vcd = true;
			perm_string module_name = perm_string(id);
			if(enter_scopes.empty())
			{
				if(module_name == top_->pscope_name())
				{
					enter_scopes.push(mt_nodes[top_->pscope_name()]);
				}
				else
				{
					cerr << "Top module in vcd file is " << module_name << ", but your input is " << top_->pscope_name() << endl;
					exit(1);
				}
			}
			else
			{
				ModuleTreeNode* last_ = enter_scopes.top();
				ModuleTreeNode* current_ = last_->next_[module_name];
				if(current_)
				{
					enter_scopes.push(current_);
				}
				else
				{
					cerr << "Problem of matching gate module in vcd_parse_def_scope." << endl;
					exit( 1 );
				}
			}

			Module* current_module = enter_scopes.top()->get_module();
			VcdScope* new_scope = new VcdScope(current_module);
			new_scope->name_ = string(id);
			current_scope = new_scope;
			current_module->vcd_scopes.push_back(new_scope);

		}
		/*Format of $scope (begin, task , fork , others...))*/
		else
		{
			//pass
		}

	}
	else 
	{
		cerr << "Unrecognized $scope format" << endl;
		exit( 1 );
	}
}

void PDesign::vcd_parse_sim(FILE* vcd)
{
	char token[4100];         /* Current token from VCD file       */
	int  chars_read;          /* Number of characters scanned in   */

	
	if(statement_ || path_ || branch_ || combine_)
	{
		evaluate_out << left << setw(10) << "Time";
		evaluate_out << left << setw(40) << "File";
		evaluate_out << left << setw(15) << "Module";
		evaluate_out << left << setw(10) << "Line";
		evaluate_out << left << setw(25) << "Node";
		evaluate_out << left << setw(10) << "Jump Line";
		evaluate_out << "Cond Expr Value" << endl;
	}

	cur_sim_time = -1;

	while( !feof( vcd ) && (fscanf( vcd, "%s%n", token, &chars_read ) == 1) ) 
	{
		assert( chars_read <= 4100 );
		if(cur_sim_time >= 100000) break;
		if( token[0] == '$' ) 
		{
			/* Currently ignore all simulation keywords */
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
			if( cur_sim_time >= 0 ) 
			{
				update_vcd_vars();
				toggle_timestep();
				fsm_timestep();
				sim_timestep();
			}
			cur_sim_time = atol( token + 1 );
			variety_symbols.clear();
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

void PDesign::vcd_parse_sim_ignore( FILE* vcd ) 
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

void PDesign::vcd_parse_sim_vector( FILE* vcd, char* value )
{
	char sym[256];    /* String value of signal symbol   */
	int  chars_read;  /* Number of characters scanned in */
	
	if( fscanf( vcd, "%s%n", sym, &chars_read ) == 1 ) 
	{
		assert( chars_read <= 256 );
		string symbol = string(sym);

		if(symbol_vars.find(symbol) != symbol_vars.end())
		{
			pair<string, VcdScope*> var = symbol_vars[symbol].front();
			VcdVar* vv = var.second->vars_[symbol];

			assert(vv);
			assert(strlen(value) <= vv->width);

			verinum var_value(uint64_t(0),vv->width);

			int index = 0;
			for(int idx = strlen(value)-1; idx >= 0; idx--)
			{
				switch(value[idx])
				{
				case '0':
					var_value.set(index, verinum::V0);
					break;
				case '1':
					var_value.set(index, verinum::V1);
					break;
				case 'x':
				case 'X':
					var_value.set(index, verinum::Vx);
					break;
				case 'z':
				case 'Z':
					var_value.set(index, verinum::Vz);
					break;
				default :
					cerr << "Question in vcd vector parse." << endl;
					exit(1);
					break;
				}
				index = index + 1;
			}

			if(var_value.get(0) == verinum::Vx || var_value[0] == verinum::Vz)
			{
				for(unsigned i = 1; i < var_value.len(); i++)
				{
					var_value.set(i, var_value[0]);
				}
			}
			variety_symbols[symbol] = (var_value);
		}
	} 

	else 
	{
		cerr << "Bad file format!" << endl;
		exit( 1 );
	}
}

void PDesign::set_symbol_char( char* sym, char value ) 
{
	string symbol = string(sym);
	if(symbol_vars.find(symbol) != symbol_vars.end())
	{
		pair<string, VcdScope*> var = symbol_vars[symbol].front();
		VcdVar* vv = var.second->vars_[symbol];

		assert(vv);
		assert(1 <= vv->width);

		verinum var_value = vv->cur_val;

		switch(value)
		{
		case '0' :
			var_value.set(0, verinum::V0);
			break;
		case '1':
			var_value.set(0, verinum::V1);
			break;
		case 'x':
		case 'X':
			var_value.set(0, verinum::Vx);
			break;
		case 'z':
		case 'Z':
			var_value.set(0, verinum::Vz);
			break;
		}

		variety_symbols[symbol] = (var_value);
	}
}

void PDesign::update_vcd_vars()
{
	map<string, verinum>::iterator variety_symbol;
	for(variety_symbol = variety_symbols.begin(); variety_symbol != variety_symbols.end(); variety_symbol++)
	{
		vector<pair<string, VcdScope*> > vars = symbol_vars[variety_symbol->first];
		for(pair<string, VcdScope*> var_pos : vars)
		{
			VcdVar* var = 0;
			for(map<string, VcdVar*>::iterator pos = var_pos.second->vars_.begin(); pos != var_pos.second->vars_.end(); pos++)
			{
				if(pos->second->name == var_pos.first)
				{
					var = pos->second;
					break;
				}
			}
			assert(var);
			var->pre_val = var->cur_val;
			var->cur_val = variety_symbol->second;
		}
	}
}

void PDesign::toggle_timestep()
{
	if(toggle_)
	{
		map<string, verinum>::iterator variety_symbol;
		for(variety_symbol = variety_symbols.begin() ; variety_symbol != variety_symbols.end(); variety_symbol++)
		{
			vector<pair<string, VcdScope*> > var_locates = symbol_vars[variety_symbol->first];
			for(pair<string, VcdScope*> var_locate : var_locates)
			{
				VcdVar* var= var_locate.second->vars_[variety_symbol->first];
				report->add_toggle(var_locate.second->module_, var);
			}
		}
	}
}

void PDesign::fsm_timestep()
{ 	
	if(fsm_)
	{
		map<string, verinum>::iterator variety_symbol;
		for(variety_symbol = variety_symbols.begin() ; variety_symbol != variety_symbols.end(); variety_symbol++)
		{
			vector<pair<string, VcdScope*> > var_locates = symbol_vars[variety_symbol->first];
			for(pair<string, VcdScope*> var_locate : var_locates)
			{
				VcdVar* var= var_locate.second->vars_[variety_symbol->first];
				if(fsm_vars.find(var) != fsm_vars.end())
				{
					report->add_fsm(var_locate.second->module_, var);
				}
			}
		}
	}
}

void PDesign::sim_timestep()
{
	if(statement_ || path_ || branch_ || combine_)
	{
		map<Module*, bool>::iterator md = select_mod.begin();
		for(md; md != select_mod.end(); md++)
		{
			for(VcdScope* scope : md->first->vcd_scopes)
			{
				control_out << "Time#" << cur_sim_time << " " << scope->module_->pscope_name() << " " << scope->name_ << endl;
				control_out << "---------------------------------------------------------------------------------" << endl;
				scope->initialize(variety_symbols, control_out);
				list<Cfg*>::iterator pos1 = md->first->sync_cfgs_.begin();
				set<string> defs;
				for(pos1; pos1 != md->first->sync_cfgs_.end(); pos1++)
				{
					cfg_replay(*pos1, scope, defs);
				}
				if(scope->toggle_)
				{
					list<Cfg*>::iterator pos2 = md->first->combine_cfgs_.begin();
					for(pos2; pos2 != md->first->combine_cfgs_.end(); pos2++)
					{
						scope->update(defs, control_out);
						cfg_replay(*pos2, scope, defs);
					}
				}
				if(!defs.empty())
					scope->update(defs, control_out);
				control_out << "---------------------------------------------------------------------------------" << endl << endl;
				defs.clear();
			}
		}
	}
}

void PDesign::cfg_replay(Cfg* cfg, VcdScope* scope, set<string>& defs)
{
	set<unsigned, less<unsigned> > path;
	int index = 1;

	//Start replay for selected cfg
	while((index > 0) && (index <= int(cfg->root->count() - 2)))
	{
		Cfg_Node* node = (*(cfg->root))[index];

		//For the process event node, we need to judge whether this process is executing at this time.
		if(node->type == "ISCONTROL.EVENT")
		{
			if(!execute_process(node, scope, cfg))
				return;
			index = index + 1;
			path.insert(node->lineno);
		}

		//For a not-control node, record defs and jump to the next node. 
		else if(node->type.find("ISCONTROL") != 0)
		{
			path.insert(node->lineno);
			index = node->dsuc[0]->index;
			for(string def : node->defs)
			{
				defs.insert(def);
			}
		}

		//For a control node, evaluate the condit expression and get the next node, then record.
		else
		{
			index = eval_cond_expr(node, scope, cfg, combine_);
			path.insert(node->lineno);
		}
		
		//The value of cond expression is uncertained.
		if(index < 0)
		{
			return;
		}
	}
	string s = cfg->sync ? "$Synchorous Process$ " : "$Combine Process ";
	control_out << s;
	control_out << "(Process" << cfg->id << ") ";
	for(set<unsigned>::iterator pos = path.begin(); pos != path.end(); pos++)
	{
		control_out << *pos << " -> ";
	}
	control_out << "end" << endl;
	add_path(scope->module_, cfg, path);
}

bool PDesign::execute_process(Cfg_Node* node, VcdScope* vs, Cfg* cfg)
{
	bool res = false;
	//Always like always@(posedge clk or negedge rst).
	if(cfg->sync)
	{
		for(unsigned i = 0; i < node->expr.count(); i++)
		{
			PEEvent* event = dynamic_cast<PEEvent*>(node->expr[i]);
			perm_string name = dynamic_cast<PEIdent*>(event->expr())->path().front().name;
			PEEvent::edge_t tp= event->type();

			//Identify whether this variable has changed in the last time.
			map<string, VcdVar*>::iterator var;
			for(var = vs->vars_.begin(); var != vs->vars_.end(); var++)
			{
				if(var->second->name == string(name.str())) break;
			}

			if(var == vs->vars_.end()) continue;

			if(variety_symbols.find(var->second->symbol) == variety_symbols.end()) continue;

			assert(var->second->cur_val.get_nbits()==1);
			verinum::V pre_val = var->second->pre_val[0];
			verinum::V cur_val = var->second->cur_val[0];

			if(tp == PEEvent::edge_t::POSEDGE)
			{
				if(pre_val == verinum::V0 && cur_val == verinum::V1
				|| pre_val == verinum::Vx && cur_val == verinum::V1)
				{
					res = true;
					break;
				}
			}
			else if(tp == PEEvent::edge_t::NEGEDGE)
			{
				if(pre_val == verinum::V1 && cur_val == verinum::V0
				|| pre_val == verinum::Vx && cur_val == verinum::V0)
				{
					res = true;
					break;
				}
			}
			else if(tp == PEEvent::edge_t::EDGE)
			{
				if(pre_val == verinum::V0 && cur_val == verinum::V1
				|| pre_val == verinum::Vx && cur_val == verinum::V1
				|| pre_val == verinum::V1 && cur_val == verinum::V0
				|| pre_val == verinum::Vx && cur_val == verinum::V0
				)
				{
					res = true;
					break;
				}
			}
			else
			{
				res = true;
				break;
			}
		}
		vs->toggle_ = res;
	}
	//Always like always@(a or b) or always@(*)
	else
	{
		//The type is always@(*), get refs from the Cfg.
		if(node->expr.count() == 0)
		{
			for(string ref : cfg->refs)
			{
				//Identify whether this variable has changed in the last time.
				map<string, VcdVar*>::iterator var;
				for(var = vs->vars_.begin(); var != vs->vars_.end(); var++)
				{
					if(var->second->name == ref) break;
				}

				if(var == vs->vars_.end()) continue;

				if(variety_symbols.find(var->second->symbol) == variety_symbols.end()) continue;

				//If a variable, compare the last value and current value.
				long pre_val = var->second->pre_val.as_long();
				long cur_val = var->second->cur_val.as_long();
				if(pre_val != cur_val)
				{
					res = true;
					break;
				}
				continue;
			}
		}
		else
		{
			//The event of process is a combination of variables.
			for(unsigned i = 0; i < node->expr.count(); i++)
			{
				PEEvent* event = dynamic_cast<PEEvent*>(node->expr[i]);
				perm_string name = dynamic_cast<PEIdent*>(event->expr())->path().front().name;
				PEEvent::edge_t tp= event->type();

				//Identify whether this variable has changed in the last time.
				map<string, VcdVar*>::iterator var;
				for(var = vs->vars_.begin(); var != vs->vars_.end(); var++)
				{
					if(var->second->name == string(name.str())) break;
				}

				if(var == vs->vars_.end()) continue;

				if(variety_symbols.find(var->second->symbol) == variety_symbols.end()) continue;

				//If a variable, compare the last value and current value.
				if(tp == PEEvent::edge_t::ANYEDGE)
				{
					long pre_val = var->second->pre_val.as_long();
					long cur_val = var->second->cur_val.as_long();
					if(pre_val != cur_val)
					{
						res = true;
						break;
					}
					continue;
				}
				else
				{
					cerr << "Format error in event " << node->lineno << endl;
					exit(1);
				}
			}
		}
	}
	return res;
}

void PDesign::add_path(Module* var_module, Cfg* cfg, const set<unsigned>& path)
{
	if(cur_sim_time > 0 && path.size())
	{
		if(statement_)
		{
			report->add_stat(var_module, path);
		}
		if(path_)
		{
			report->add_path(var_module, path, cur_sim_time, cfg);
		}
		if(branch_)
		{
			report->add_branch(var_module, path);
		}
	}
}

int PDesign::eval_cond_expr(Cfg_Node* node, VcdScope* vs, Cfg* cfg, bool combine)
{	
	int index = -1;
	hname_t name(vs->module_->pscope_name());
	NetScope* scope = des->find_scope(name); 
	assert(scope);

	//Evaluate the conditional expression and collect the result of combine coverage.
	map<PExpr*, verinum> items;
	verinum* vn = node->expr[0]->evaluate(des, scope, vs, combine, items);

	if(combine)
	{
		report->add_combine(vs->module_, node, *vn, items);
	}
	items.clear();

	//If this control node type is not case, true jump the if-statement, false jump the else-statement.
	if(node->type.find("CASE") == string::npos)
	{
		//The value after evaluating the condition expression is mutiple bits.
		if(vn->len() > 1)
		{
			verinum::V v = unary_or(*vn);
			switch(v)
			{
				case verinum::V1 : 
					index = node->dsuc[0]->index;
					break;
				case verinum::V0 : 
					index = node->dsuc[1]->index;
					break;
				default : break;
			}
		}
		//The value is a bool expression.
		else
		{
			switch((*vn)[0])
			{
				case verinum::V1 : 
					index = node->dsuc[0]->index;
					break;
				case verinum::V0 : 
					index = node->dsuc[1]->index;
					break;
				default : break;
			}
		}
	}
	//If the case node, compare the caseitems and jump the correct statement.
	else
	{
		for(unsigned idx = 0; idx < node->dsuc.count(); ++idx)
		{
			if(index > 0)
				break;
			for(set<PExpr*>::const_iterator pos = node->dsuc[idx]->caseitem.begin(); pos != node->dsuc[idx]->caseitem.end(); ++pos)
			{
				verinum* ci = (*pos)->evaluate(des, scope, vs, combine, items);
				if(node->type == "ISCONTROL.CASE")
				{
					if(*vn == *ci)
						index = node->dsuc[idx]->index;
				}
				if((node->type == "ISCONTROL.CASEX") || (node->type == "ISCONTROL.CASEZ"))
				{
					if(ceq(*vn, *ci))
						index = node->dsuc[idx]->index;
				}
			}
			//Caseitem is default
			if(idx == node->dsuc.count()-1 && node->dsuc[idx]->caseitem.size() == 0)
			{
				index = node->dsuc[idx]->index;
			}
		}
	}

	
	int lineno = index >= 0 && index < cfg->root->count() ? (*(cfg->root))[index]->lineno : -1;
	evaluate_out << left << setw(10);
	evaluate_out << "#" + to_string(cur_sim_time);
	evaluate_out << left << setw(40);
	evaluate_out << vs->module_->get_file();
	evaluate_out << left << setw(15);
	evaluate_out << vs->module_->pscope_name();
	evaluate_out << left << setw(10);
	evaluate_out << node->lineno;
	evaluate_out << left << setw(25);
	evaluate_out << node->type;
	evaluate_out << left << setw(10);
	evaluate_out << lineno;
	node->expr[0]->dump(evaluate_out);
	evaluate_out << " = ";
	vn->dump(evaluate_out);
	evaluate_out << endl;
	
	delete vn;
	return index;
}

void PDesign::report_coverage(ostream& o)
{
	report->dump(o);
}

