#include "config.h"
#include "PDesign.h"
#include "Module.h"
#include "CfgNode.h"
#include "PExpr.h"
#include <iostream>
#include <string.h>
#include <iomanip>
#include <list>

using namespace std;

PDesign::PDesign()
{
	evaluate_out = ofstream("dump/evaluate.txt");
	control_out = ofstream("dump/control.txt");
}

PDesign:: ~PDesign()
{
	if(!mt_nodes.empty()){
		cout << "Release the module tree nodes..." << endl;
		for(map<perm_string, ModuleTreeNode*>::iterator pos = mt_nodes.begin(); pos != mt_nodes.end(); pos++){
			delete pos->second;
			pos->second = 0;
		}
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
}

map<perm_string, Module*> PDesign::get_modules()
{
	return modules_;
}

map<perm_string, PUdp*> PDesign::get_udps()
{
	return udps_;
}

map<perm_string, vector<string> >* PDesign::get_lines()
{
	return &(lines_);
}

void PDesign::dump(ostream&o)
{
	for(map<perm_string, Module*>::iterator pos = modules_.begin(); pos != modules_.end(); ++pos){
		pos->second->dump(o);
	}
}

void PDesign::initialize(const char* fsm_selects)
{
	build_module_tree(top_);
	top_->build_vartab(des);

	if(fsm_) fsm_var_parse(fsm_selects);

	cout << "Build paths and sort cfgs for select modules..." << endl;
	map<Module*, bool>::iterator md = select_mod.begin();
	for(md; md != select_mod.end(); md++){
		md->first->initialize(statement_, path_, branch_, combine_, false);
		md->first->get_lines(lines_);
	}
	
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
	char* v = strtok(strdup(fsm_selects), ",");
	while(v != NULL) {
		fsm_names[string(v)] = true;
		v = strtok(NULL, ",");
	}
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
		top_ = modules_[top_module];

	else{
		cerr << "The top module " << top_module << " can not be found in Verilog file." << endl;
		exit(1);
	}

	if(select_module){
		if(modules_.find(select_module) != modules_.end())
			select_mod[modules_[select_module]] = true;
		else{
			cerr << "The select module " << select_module << " can not be found in Verilog file." << endl;
			exit(1);
		}
	}

	else{
		map<perm_string, Module*>::iterator md = modules_.begin();
		for(md; md != modules_.end(); md++){
			if(md->second != top_)
				select_mod[md->second] = true;
		}
	}

	cout << "Initializing for modules before coverage analysis..." << endl;

	initialize(fsm_selects);

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
	cur_scope = nullptr;
	found_in_vcd = false;

	while( !enddef_found && (fscanf( vcd, "%s%n", keyword, &chars_read ) == 1) ) {
		
		assert( chars_read <= 256 );
		if( keyword[0] == '$' ) 
		{
			if( strncmp( "var", (keyword + 1), 3 ) == 0 ) 
			{
				if(!enter_scopes.empty()) vcd_parse_def_var( vcd );
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
				if(enter_scopes.size() == scope_count) enter_scopes.pop();
				VcdScope* scope = scope_layer.top();
				scope_layer.pop();
				scope->report_->initial();
				cur_scope = scope_layer.empty() ? nullptr : scope_layer.top();
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
		if(string(type).find("wire") != 0 && string(type).find("reg") != 0) return;
		if(select_mod.find(cur_scope->module_) != select_mod.end() || cur_scope->module_ == top_){
			VcdVar* vv = new VcdVar;
			vv->name = ref;
			vv->scope = cur_scope;
			vv->little_endia = msb >= lsb ? true : false;
			if(!vv->little_endia || lsb != 0 || lsb < 0 || msb < 0)
			{
				cerr << vv->name << "[" << msb << ":" << lsb << "]" << " format is unsupported." << endl;
				exit(1);
			}
			assert(cur_scope->module_->vartab_.find(vv->name) != cur_scope->module_->vartab_.end());
			string type = cur_scope->module_->vartab_[vv->name]->ptype;
			vv->type = type != "PINPUT" ? (type != "POUTPUT" ? (type != "PINOUT" ? 
							VcdVar::SIGNAL : VcdVar::INOUT) : VcdVar::OUTPUT) : VcdVar::INPUT;
			verinum vn(verinum::Vx, size);
			vv->cur_val = vn;
			vv->pre_val = vn;
			vv->sim_val = vn;
			vv->width = size;
			vv->lsb = lsb;
			vv->msb = msb;
			vv->symbol = string(id_code);
			if(cur_scope->vars_.find(string(id_code)) == cur_scope->vars_.end())
				cur_scope->vars_[string(id_code)] = vector<VcdVar*>();
			cur_scope->vars_[string(id_code)].push_back(vv);
			cur_scope->defines_[vv->name] = vv;
			symbol_vars[string(id_code)].push_back(make_pair(vv->name, cur_scope));
			if(fsm_names.find(vv->name) != fsm_names.end())
			{
				fsm_vars.insert(make_pair(cur_scope, vv));
				cur_scope->fsm_vars_.insert(vv);
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
			if(enter_scopes.empty()){
				if(module_name == top_->pscope_name())
					enter_scopes.push(mt_nodes[top_->pscope_name()]);
				else{
					cerr << "Top module in vcd file is " << module_name << ", but your input is " << top_->pscope_name() << endl;
					exit(1);
				}
			}
			else{
				ModuleTreeNode* last_ = enter_scopes.top();
				ModuleTreeNode* current_ = last_->next_[module_name];
				if(current_)
					enter_scopes.push(current_);
				else{
					cerr << "Problem of matching gate module in vcd_parse_def_scope." << endl;
					exit( 1 );
				}
			}

			Module* cur_module = enter_scopes.top()->get_module();
			VcdScope* new_scope = new VcdScope(cur_module, this);
			new_scope->report_ = new ScopeRep(toggle_, fsm_, statement_, path_, branch_, combine_, new_scope);
			new_scope->name_ = string(id);
			if(cur_scope) cur_scope->instans_.push_back(new_scope);
			cur_scope = new_scope;
			scope_layer.push(cur_scope);
			cur_module->vcd_scopes.push_back(new_scope);
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

	/*
	if(statement_ || path_ || branch_ || combine_){
		evaluate_out << left << setw(10) << "Time";
		evaluate_out << left << setw(40) << "File";
		evaluate_out << left << setw(15) << "Module";
		evaluate_out << left << setw(10) << "Line";
		evaluate_out << left << setw(25) << "Node";
		evaluate_out << left << setw(10) << "Jump Line";
		evaluate_out << "Cond Expr Value" << endl;
	}
	*/

	cur_sim_time = -1;

	while( !feof( vcd ) && (fscanf( vcd, "%s%n", token, &chars_read ) == 1) ){
		assert( chars_read <= 4100 );
		if( token[0] == '$' ){
			/* Currently ignore all simulation keywords */
		} 
		else if( (token[0] == 'b') || (token[0] == 'B') )
			vcd_parse_sim_vector( vcd, (token + 1) );

		else if( (token[0] == 'r') || (token[0] == 'B') )
			vcd_parse_sim_ignore( vcd );

		else if( token[0] == '#' ){
			update_vcd_vars();
			toggle_timestep();
			fsm_timestep();
			sim_timestep();
			cur_sim_time = atol( token + 1 );
			variety_symbols.clear();
		} 

		else if( (token[0] == '0') ||
			(token[0] == '1') ||
			(token[0] == 'x') ||
			(token[0] == 'X') ||
			(token[0] == 'z') ||
			(token[0] == 'Z') ) 
			set_symbol_char( token + 1, token[0] );

		else{
			cerr << "Badly placed token " << token << endl;
			exit( 1 );
		}
	}
}

void PDesign::vcd_parse_sim_ignore( FILE* vcd ) 
{
	char sym[256];    /* String value of signal symbol   */
	int  chars_read;  /* Number of characters scanned in */
	
	if( fscanf( vcd, "%s%n", sym, &chars_read ) != 1 ){
		cerr << "Bad file format!" << endl;
		exit( 1 );
	}
	assert( chars_read <= 256 );
}

void PDesign::vcd_parse_sim_vector( FILE* vcd, char* value )
{
	char sym[256];    /* String value of signal symbol   */
	int  chars_read;  /* Number of characters scanned in */

	if( fscanf( vcd, "%s%n", sym, &chars_read ) == 1 ) {
		assert( chars_read <= 256 );
		string symbol = string(sym);
		if(symbol_vars.find(symbol) != symbol_vars.end()){
			pair<string, VcdScope*> var = symbol_vars[symbol].front();
			VcdVar* vv = var.second->vars_[symbol].front();
			assert(vv);
			assert(strlen(value) <= vv->width);
			verinum var_value(uint64_t(0),vv->width);
			int index = 0;
			for(int idx = strlen(value)-1; idx >= 0; idx--){
				switch(value[idx]){
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
			if(var_value.get(0) == verinum::Vx || var_value[0] == verinum::Vz){
				for(unsigned i = 1; i < var_value.len(); i++)
					var_value.set(i, var_value[0]);

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
		VcdVar* vv = var.second->vars_[symbol].front();
		assert(vv);
		assert(1 <= vv->width);
		verinum var_value = vv->cur_val;
		switch(value){
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
	for(variety_symbol = variety_symbols.begin(); variety_symbol != variety_symbols.end(); variety_symbol++){
		vector<pair<string, VcdScope*> > vars = symbol_vars[variety_symbol->first];
		for(pair<string, VcdScope*> var_pos : vars){
			assert(var_pos.second->defines_.find(var_pos.first) != var_pos.second->defines_.end());
			VcdVar* var = var_pos.second->defines_[var_pos.first];
			var->pre_val = var->cur_val;
			var->cur_val = variety_symbol->second;
		}
	}
}

void PDesign::toggle_timestep()
{
	//Generate Toggle coverage by changed value informations.
	if(toggle_){
		map<VcdScope*, set<VcdVar*> > scopes;
		//Generate for all symbols changed last time.
		map<string, verinum>::iterator variety_symbol = variety_symbols.begin();
		for(; variety_symbol != variety_symbols.end(); variety_symbol++){
			vector<pair<string, VcdScope*> > var_locates = symbol_vars[variety_symbol->first];
			for(pair<string, VcdScope*> var_locate : var_locates){
				VcdVar* var = var_locate.second->defines_[var_locate.first];
				scopes[var_locate.second].insert(var);
			}
		}
		map<VcdScope*, set<VcdVar*> >::iterator pos = scopes.begin();
		for(; pos != scopes.end(); pos++){
			//if(select_mod.find(pos->first->module_) != select_mod.end())
			pos->first->report_->add_tgl_report(pos->second);
		}
	}

}

void PDesign::fsm_timestep()
{ 	
	//Generate FSM coverage by changed value informations.
	if(fsm_){
		//Search all changed symbols.
		for(pair<VcdScope*, VcdVar*> pos : fsm_vars){
			if(variety_symbols.find(pos.second->symbol) != variety_symbols.end())
				//if(select_mod.find(pos.first->module_) != select_mod.end())
				pos.first->report_->add_fsm_report(pos.second);
		}
	}
}

void PDesign::sim_timestep()
{
	//We need to replay process when we using -S, -P, -B or -C.
	if(statement_ || path_ || branch_ || combine_)
	{
		//Replay for selected modules.
		map<Module*, bool>::iterator md = select_mod.begin();
		for(; md != select_mod.end(); md++){
			//Replay for all instantiations of this module.
			for(VcdScope* scope : md->first->vcd_scopes){
				//Print control information.
				control_out << "Time#" << cur_sim_time << " " << scope->module_->pscope_name() << " " << scope->name_ << endl;
				control_out << "---------------------------------------------------------------------------------" << endl;
				//Initialize for assign statements.
				scope->initialize(variety_symbols, control_out, combine_);
				list<list<Cfg*> >::iterator pos = md->first->cfg_list.begin();
				set<string> defs;
				for(; pos != md->first->cfg_list.end(); pos++) {
					for(Cfg* cfg : (*pos)) {
						cfg_replay(cfg, scope, defs);
					}
					if(!defs.empty())
						scope->update(defs, control_out, combine_);
				}
				control_out << "---------------------------------------------------------------------------------" << endl << endl;
			}
		}
	}
}

void PDesign::cfg_replay(Cfg* cfg, VcdScope* scope, set<string>& defs)
{
	set<unsigned, less<unsigned> > path; /* Store the path of in replaying process. */
	unsigned blineno = 0;
	map<unsigned, vector<unsigned> > bvalues;
	int index = 1;                       /* The current node id in cfg replay. */

	//Start replay for selected cfg.
	while((index > 0) && (index <= int(cfg->root->count() - 2))){
		//Get current cfg node.
		Cfg_Node* node = (*(cfg->root))[index];
		//For the process event node, we need to judge whether this process is executing at this time.
		if(node->type == "ISCONTROL.EVENT"){
			if(!execute_process(node, scope, cfg))
				return;
			index++;
			path.insert(node->lineno);
		}
		//For a blocking or non-blocking assign node, record defs and jump to the next node. 
		else if(node->type.find("ISCONTROL") != 0){
			path.insert(node->lineno);
			index = node->dsuc[0]->index;
			for(string def : node->defs){
				if(variety_symbols.find(scope->defines_[def]->symbol) != variety_symbols.end())
					defs.insert(def);
			}
		}
		//For a control node, evaluate the condit expression and get the next node, then record.
		else{
			index = eval_cond_expr(node, scope, cfg, bvalues, blineno);
			path.insert(node->lineno);
		}
		//The value of cond expression is uncertained.
		if(index < 0)
			return;
	}
	//Print the control information.
	string s = cfg->sync ? "$Synchorous Process$ " : "$Combine Process ";
	control_out << s;
	control_out << "(Process" << cfg->id << ") ";
	for(set<unsigned>::iterator pos = path.begin(); pos != path.end(); pos++)
	{
		control_out << *pos << " -> ";
	}
	control_out << "end" << endl;
	//Generate coverage report.
	add_path(scope, cfg, path);
	if(branch_)
		add_branch(scope, bvalues);
}

bool PDesign::execute_process(Cfg_Node* node, VcdScope* vs, Cfg* cfg)
{
	bool res = false;
	//Synchorous process usually control by Clock port and Rst port.
	//Like always@(posedge clk or negedge rst)
	//Or always@(posedge clk)
	if(cfg->sync)
	{
		for(unsigned i = 0; i < node->expr.count(); i++)
		{
			PEEvent* event = dynamic_cast<PEEvent*>(node->expr[i]);
			perm_string name = dynamic_cast<PEIdent*>(event->expr())->path().front().name;
			PEEvent::edge_t tp= event->type();

			assert(vs->defines_.find(name.str()) != vs->defines_.end());
			VcdVar* var = vs->defines_[name.str()];

			//If the port is not changed, then we need to judge the next port.
			if(variety_symbols.find(var->symbol) == variety_symbols.end()) 
				continue;

			//Clock or Rst port should be one bit.
			assert(var->cur_val.get_nbits()==1);

			verinum::V pre_val = var->pre_val[0];
			verinum::V cur_val = var->cur_val[0];
			//Posedge Port
			if(tp == PEEvent::edge_t::POSEDGE)
				if(pre_val == verinum::V0 && cur_val == verinum::V1
				|| pre_val == verinum::Vx && cur_val == verinum::V1)
					res = true;
			//Negedge Port
			else if(tp == PEEvent::edge_t::NEGEDGE)
				if(pre_val == verinum::V1 && cur_val == verinum::V0
				|| pre_val == verinum::Vx && cur_val == verinum::V0)
					res = true;
			//Rarely used
			else if(tp == PEEvent::edge_t::EDGE)
				if(pre_val == verinum::V0 && cur_val == verinum::V1
				|| pre_val == verinum::Vx && cur_val == verinum::V1
				|| pre_val == verinum::V1 && cur_val == verinum::V0
				|| pre_val == verinum::Vx && cur_val == verinum::V0
				)
					res = true;
			//Others
			else
				res = true;
			if(res)
				break;
		}
	}
	//Always like always@(a or b) or always@(*)
	else
	{	
		//Get refs in cfg, if this process need to replay, one signal or port changed at least.
		for(string ref : cfg->refs)
		{
			if(vs->defines_.find(ref) != vs->defines_.end())
			{
				VcdVar* var = vs->defines_[ref];
				//Identify whether this variable has changed in the last time.
				if(variety_symbols.find(var->symbol) != variety_symbols.end()){
					res = true;
					break;
				}
			}
		}
	}
	return res;
}

void PDesign::add_path(VcdScope* scope, Cfg* cfg, set<unsigned>& path)
{
	if(!path.empty()){
		if(statement_) 
			scope->report_->add_line_report(cfg, path);
		if(path_) 
			scope->report_->add_path_report(cfg, path);
	}
}

void PDesign::add_branch(VcdScope* scope, map<unsigned, vector<unsigned> >& bvalues)
{
	map<unsigned, vector<unsigned> >::iterator pos = bvalues.begin();
	for(; pos != bvalues.end(); pos++){
		scope->report_->add_branch_report(pos->first, pos->second);
	}
}

int PDesign::eval_cond_expr(Cfg_Node* node, VcdScope* vs, Cfg* cfg, map<unsigned, vector<unsigned> >& bvalues, unsigned& blineno)
{	
	int index = -1; /* Next node position. */
	hname_t name(vs->module_->pscope_name()); 
	NetScope* scope = des->find_scope(name); 
	assert(scope);

	//Evaluate the conditional expression and collect the result of combine coverage.
	map<PExpr*, map<PExpr*, bool> > cvalues;
	verinum* vn = node->expr[0]->evaluate(des, scope, vs, combine_, cvalues);

	if(combine_)
		vs->report_->add_cond_report(cvalues);

	unsigned value;

	//If this control node type is not case, true jump the if-statement, false jump the else-statement.
	if(node->type.find("CASE") == string::npos){
		//The cond expression are mutiple bits like a, a | b, a & b, transfer bits to one bit.
		verinum::V v = vn->len() > 1 ? unary_or(*vn) : (*vn)[0];
		switch(v){
			case verinum::V1 : 
				index = node->dsuc[0]->index;
				break;
			case verinum::Vx :
			case verinum::Vz :
			case verinum::V0 : 
				index = node->dsuc[1]->index;
				break;
			default : 
				break;
		}
		value = v == verinum::V1 ? BRANCH_TRUE_VALUE : BRANCH_FALSE_VALUE;
	}
	//If the case node, compare the caseitems and jump the correct statement.
	else{
		for(unsigned idx = 0; idx < node->dsuc.count(); ++idx){
			if(index > 0) break;
			value = idx;
			set<PExpr*>::const_iterator pos = node->dsuc[idx]->caseitem.begin();
			for(; pos != node->dsuc[idx]->caseitem.end(); ++pos){
				verinum* ci = (*pos)->evaluate(des, scope, vs, combine_, cvalues);
				if(node->type == "ISCONTROL.CASE"){
					if(*vn == *ci){
						index = node->dsuc[idx]->index;
					}
				}
				else{
					if(case_x_or_z(*vn, *ci, node->type)){
						index = node->dsuc[idx]->index;
					}
				}
			}
			//Caseitem is default
			if(idx == node->dsuc.count()-1 && node->dsuc[idx]->caseitem.size() == 0){
				index = node->dsuc[idx]->index;
			}
		}
	}

	if(branch_){
		if(vs->module_->branchs_.find(node->lineno) != vs->module_->branchs_.end())
			blineno = node->lineno;
		bvalues[blineno].push_back(value);
	}

	/*Dump evaluate information.
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
	*/
	delete vn;
	return index;
}

void PDesign::report_coverage(ostream& o)
{
	VcdScope* scope = top_->vcd_scopes.front();
	scope->report(o);
}

