#include "vcdvar.h"
#include <iomanip>

CoverNum::~CoverNum()
{

}

void CoverNum::add(const CoverNum* num)
{
	total_ += num->total_;
	active_ += num->active_;
	hit_ += num->hit_;
	miss_ += num->miss_;
}

void CoverNum::dump(ostream& o, string s){
	o << left << setw(10);
	o << left << setw(20) << s;
	o << left << setw(10) << active_;
	o << left << setw(10) << total_;
	o << left << setw(10) << miss_;
	o << left << setw(10) << hit_;
	o << left << setw(10) << (double(hit_*100)/double(total_));
	o << endl;
}

void VcdVar::dump(ostream& o)
{
	o << name << "[" << msb << ":" << lsb <<"]" << " cur_var = ";
	cur_val.dump(o);
	o << " pre_val = ";
	pre_val.dump(o);
	o << endl; 
}

VarToggle::VarToggle(const VcdVar* var)
{
	lsb_ = var->lsb;
	msb_ = var->msb;
	little_endia_ = var->little_endia;
	width_ = var->width;
	name_ = var->name;
	num_ = new CoverNum;
	num_->total_ = width_ * 2;
	num_->miss_ = width_ * 2;
	for(unsigned idx = 0; idx < width_; ++idx){
		toggle01_[idx] = 0;
		toggle10_[idx] = 0;
	}
}

VarToggle::~VarToggle()
{
	delete num_;
}

void VarToggle::add(const VcdVar* var)
{
	//Toggle bit change from 0 to 1
	for(unsigned idx = 0; idx < width_; ++idx){
		//Toggle bit change from 0 to 1
		if((var->pre_val[idx] == verinum::V0) && (var->cur_val[idx] == verinum::V1)){
			if(toggle01_[idx] == 0){
				num_->hit_ += 1;
				num_->miss_ -= 1;
			}
			toggle01_[idx] += 1;
		}
		//Toggle bit change from 1 to 0
		if((var->pre_val[idx] == verinum::V1) && (var->cur_val[idx] == verinum::V0)){
			if(toggle10_[idx] == 0){
				num_->hit_ += 1;
				num_->miss_ -= 1;
			}
			toggle10_[idx] += 1;
		}

	}
	num_->active_ += 2;
}

void VarToggle::dump(ostream& o)
{
	for(unsigned idx = 0; idx < width_; idx++){
		long locate;
		if(little_endia_)
			locate = idx + lsb_;
		else
			locate = idx + msb_;
		o << left << setw(25) << name_ + "[" + to_string(locate) + "]";
		o << right << setw(10) << toggle01_[idx];
		o << right << setw(10) << toggle10_[idx];
		o << endl;
	}
}

ToggleRep::ToggleRep()
{
	num_ = new CoverNum;
}

ToggleRep::~ToggleRep()
{
	delete num_;
	for(map<string, VarToggle*>::iterator pos = vars_.begin(); pos != vars_.end(); pos++){
		delete pos->second;
	}
}

void ToggleRep::add(const VcdVar* var)
{
	if(vars_.find(var->name) == vars_.end()){
		vars_[var->name] = new VarToggle(var);
	}
	vars_[var->name]->add(var);
}

void ToggleRep::dump(ostream& o)
{
	for(map<string, VarToggle*>::iterator pos = vars_.begin(); pos != vars_.end(); pos++)
	{
		pos->second->dump(o);
		num_->add(pos->second->get_num());
	}
}

FsmRep::FsmRep()
{

}

FsmRep::~FsmRep()
{
	for(map<string, CoverState*>::iterator pos = var_states_.begin(); pos != var_states_.end(); pos++){
		delete pos->second;
	}
	for(map<string, CoverTran*>::iterator pos = var_trans_.begin(); pos != var_trans_.end(); pos++){
		delete pos->second;
	}
}

void FsmRep::add(const VcdVar* var)
{
	//add the state
	if(var_states_.find(var->name) == var_states_.end()){
		var_states_[var->name] = new CoverState();
	}
	CoverState* cs = var_states_[var->name];
	vector<pair<unsigned, verinum> >::iterator spos;
	for(spos = cs->states_.begin(); spos != cs->states_.end(); spos++){
		if((*spos).second == var->cur_val){
			(*spos).first += 1;
			break;
		}
	}
	if(spos == cs->states_.end()){
		cs->states_.push_back(make_pair(1, var->cur_val));
	}
	cs->num_ += 1;

	//add the tran
	if(var_trans_.find(var->name) == var_trans_.end()){
		var_trans_[var->name] = new CoverTran();
	}
	CoverTran* ct = var_trans_[var->name];
	vector<pair<unsigned, pair<verinum, verinum> > >::iterator tpos;
	for(tpos = ct->trans_.begin(); tpos != ct->trans_.end(); tpos++){
		if((*tpos).second.first == var->pre_val
		&& (*tpos).second.second == var->cur_val){
			(*tpos).first += 1;
			break;
		}
	}
	if(tpos == ct->trans_.end()){
		ct->trans_.push_back(make_pair(1, make_pair(var->pre_val, var->cur_val)));
	}
	ct->num_ += 1;
}

void FsmRep::dump(ostream& o)
{
	for(map<string, CoverState*>::iterator pos = var_states_.begin(); pos != var_states_.end(); pos++){
		o << "Variable : " << pos->first<< " States : " << pos->second->num_ << endl;
		o << "------------------------------------------------------------------------------" << endl;
		o << left << setw(10) << "Cover%" << left << setw(10) << "Times" << "State" << endl;
		o << left << setw(10) << "------" << left << setw(10) << "-----" << "----------------" << endl;
		for(pair<unsigned, verinum> state : pos->second->states_){
			o << left << setw(10) << double(state.first*100)/double(pos->second->num_);
			o << left << setw(10) << state.first;
			state.second.dump(o);
			o << endl;
		}
		o << "------------------------------------------------------------------------------" << endl;
		o << left << setw(10) << "Cover%" << left << setw(10) << "Times" << "State Tran" << endl;
		o << left << setw(10) << "------" << left << setw(10) << "-----" << "----------------" << endl;
		for(pair<unsigned, pair<verinum, verinum> > tran : var_trans_[pos->first]->trans_){
			o << left << setw(10) << double(tran.first*100)/double(pos->second->num_);
			o << left << setw(10) << tran.first;
			tran.second.first.dump(o);
			o << " -> ";
			tran.second.second.dump(o);
			o << endl;
		}
		o << "------------------------------------------------------------------------------" << endl;
	}
}

StatRep::StatRep(Module_Cfgs* cfgs)
{
	for(unsigned i = 0; i < cfgs->cfgs->count(); i++){
		Cfg* cfg = (*(cfgs->cfgs))[i];
		if((*(cfg->root))[1]->type == "ISCONTROL.EVENT"){
			for(unsigned j = 0; j < cfg->root->count(); j++){
				Cfg_Node* node = (*(cfg->root))[j];
				if(node->lineno > 0){
					stmts_[node->lineno] = 0;
					nodes_[node->lineno] = node;
				}
			}
		}
	}
	num_ = new CoverNum;
	num_->total_ = stmts_.size();
	num_->miss_ = stmts_.size();
}

StatRep::~StatRep()
{
	delete num_;
}	

void StatRep::add(const set<unsigned>& stmt)
{
	for(set<unsigned, less<unsigned> >::iterator spos = stmt.begin(); spos != stmt.end(); ++spos){
		if(stmts_.find(*spos) != stmts_.end()){
			if(stmts_[*spos] == 0){
				num_->hit_ += 1;
				num_->miss_ -= 1;
			}
			stmts_[*spos] += 1;
			num_->active_ += 1;
		}
	}
}

void StatRep::dump(ostream& o)
{
    map<unsigned, unsigned>::iterator stmt;
	for(stmt = stmts_.begin(); stmt != stmts_.end(); stmt++){
		o << left << setw(10) << stmt->first;
		o << left << setw(25) << nodes_[stmt->first]->type;
		o << left << setw(10) << stmt->second;
		o << endl;
	}
}

ProcessRep::ProcessRep(Module* mod, Cfg* cfg)
{
	module_ = mod;
	cfg_ = cfg;
	num_ = new CoverNum;
	num_->total_ = module_->routes_[cfg_].size();
	num_->miss_ = module_->routes_[cfg_].size();
	for(unsigned i = 0; i < num_->total_; i++){
		paths_[i] = 0;
	}
}

ProcessRep::~ProcessRep()
{
	delete num_;
}

bool ProcessRep::equal(const set<unsigned>& p1, const set<unsigned>& p2)
{
	set<unsigned, less<unsigned> >::iterator ppos1 = p1.begin();
	set<unsigned, less<unsigned> >::iterator ppos2 = p2.begin();
	while((ppos1 != p1.end()) && (ppos2 != p2.end())){
		if((*ppos1) == (*ppos2)){
			++ppos1;
			++ppos2;
		}
		else{
			return false;
		}
	}
	return true;
}


void ProcessRep::add(unsigned time, const set<unsigned>& path)
{
	vector<set<unsigned> >& routes = module_->routes_[cfg_];
	for(unsigned i = 0; i < routes.size(); i++){
		set<unsigned>& route = routes[i];
		if(equal(route, path)){
			time_paths_[time].push_back(i);
			if(!paths_[i]){
				num_->hit_ += 1;
				num_->miss_ -= 1;
			}
			paths_[i] += 1;
			break;
		}
	}
	num_->active_ += 1;
}

void ProcessRep::dump_time_paths(ostream& o)
{
	o << "------------------------------------------------------------------------------" << endl;
	o << "Process id : " << cfg_->id << endl;
	o << "------------------------------------------------------------------------------" << endl;
	map<unsigned, vector<unsigned> >::iterator pos;
	for(pos = time_paths_.begin(); pos != time_paths_.end(); pos++){
		o << "Time : " << pos->first << " " << "Path : ";
		for(unsigned idx : pos->second){
			set<unsigned> route = module_->routes_[cfg_][idx];
			o << "{ ";
			for(unsigned lineno : route){
				o << lineno << " ";
			}
			o << "}" << endl;
		}
	}
	o << "------------------------------------------------------------------------------" << endl;
}

void ProcessRep::dump_cover_paths(ostream& o)
{
	o << "------------------------------------------------------------------------------" << endl;
	o << "Process id : " << cfg_->id << endl;
	o << "------------------------------------------------------------------------------" << endl;
	o << left << setw(10) << "Hit" << left << setw(10) << "Covered%" << "Path" << endl;
	o << left << setw(10) << "---" << left << setw(10) << "--------" << "----" << endl;
	map<unsigned, unsigned>::iterator pos;
	for(pos = paths_.begin(); pos != paths_.end(); pos++){
		o << left << setw(10) << pos->second;
		o << left << setw(10) << double(pos->second*100)/double(num_->active_);
		o << "{ ";
		for(unsigned lineno : module_->routes_[cfg_][pos->first]){
			o << lineno << " ";
		}
		o << "}" << endl;
	}
}

PathRep::PathRep(Module* mod)
{
	num_ = new CoverNum;
	module_ = mod;
}

PathRep::~PathRep()
{
	delete num_;
	for(map<Cfg*, ProcessRep*>::iterator pos = procs_.begin(); pos != procs_.end(); pos++){
		delete pos->second;
	}
}

void PathRep::add(unsigned time, Cfg* cfg, const set<unsigned>& path)
{
	if(procs_.find(cfg) == procs_.end()){
		procs_[cfg] = new ProcessRep(module_, cfg);
	}
	procs_[cfg]->add(time, path);
}

void PathRep::dump(ostream& o)
{
	for(map<Cfg*, ProcessRep*>::iterator pos = procs_.begin(); pos != procs_.end(); pos++){
		pos->second->dump_cover_paths(o);
		//pos->second->dump_time_paths(o);
		num_->add(pos->second->get_num());
	}
}

ExprBranch::ExprBranch(Cfg*  cfg, Cfg_Node* node, map<unsigned, ExprBranch*>& branchs)
{
	node_ = node;
	for(unsigned idx = 0; idx < node->dsuc.count(); idx++){
		if((*(cfg->root))[node->dsuc[idx]->index]->lineno > 0){
			unsigned lineno = (*(cfg->root))[node->dsuc[idx]->index]->lineno;
			branch_[lineno] = 0;
			branchs[lineno] = this;
		}
	}
	num_ = new CoverNum;
	num_->total_ = branch_.size();
	num_->miss_ = branch_.size();
}

ExprBranch::~ExprBranch()
{
	delete num_;
}

void ExprBranch::add(unsigned lineno)
{
	if(branch_[lineno] == 0){
		num_->hit_ += 1;
		num_->miss_ -=1;
	}
	branch_[lineno] += 1;
	num_->active_ += 1;
}

void ExprBranch::dump(ostream& o)
{	
	o << "------------------------------------Branch------------------------------------" << endl;
	o << "Count coming in to " << node_->type << " in line " << node_->lineno << endl;
	o << left << setw(10) << "Line" << left << setw(10) << "Hit Times" << endl;
	o << left << setw(10) << "----" << left << setw(10) << "---------" << endl;
	map<unsigned, unsigned>::iterator pos;
	for(pos = branch_.begin(); pos != branch_.end(); pos++){
		o << left << setw(10) << pos->first << left << setw(10) << pos->second << endl;
	}
	o << "Branch Coverage = " << double(num_->hit_*100)/double(num_->total_);
	o << "%(" << num_->hit_ << " hit of " << num_->total_ << " branches" << ")";
	o << endl;
}

BranchRep::BranchRep(Module_Cfgs* cfgs)
{
	num_ = new CoverNum;
	//Search every cfg
	for(unsigned i = 0; i < cfgs->cfgs->count(); i++){
		Cfg* cfg = (*(cfgs->cfgs))[i];{
			//Search every cfg node
			if((*(cfg->root))[1]->type == "ISCONTROL.EVENT"){
				for(unsigned j = 0; j < cfg->root->count(); j++){
					Cfg_Node* node = (*(cfg->root))[j];
					//Build branch report for control node
					if(node->type.find("ISCONTROL") == 0 && node->type != "ISCONTROL.EVENT"){
						ExprBranch* branch_ = new ExprBranch(cfg, node, locations_);
						branchs_.push_back(branch_);
					}
				}
			}
		}
	}
}

BranchRep::~BranchRep()
{
	vector<ExprBranch*>::iterator pos;
	for(pos = branchs_.begin(); pos != branchs_.end(); pos++){
		delete *pos;
	}
	delete num_;
}

void BranchRep::add(const set<unsigned>& linenos)
{
	for(unsigned lineno : linenos){
		if(locations_.find(lineno) != locations_.end()){
			locations_[lineno]->add(lineno);
		}
	}
}

void BranchRep::dump(ostream& o)
{
	vector<ExprBranch*>::iterator pos;
	for(pos = branchs_.begin(); pos != branchs_.end(); pos++){
		(*pos)->dump(o);
		num_->add((*pos)->get_num());
	}
}


CondValue::CondValue(const vector<pair<PExpr*, verinum> >& items)
{
	copy(items.begin(), items.end(), inserter(items_, items_.begin()));
	num_ = 1;
}

CondValue::~CondValue()
{

}

void CondValue::dump(ostream& o)
{
	vector<pair<PExpr*, verinum> >::iterator pos;
	o << "{ ";
	for(pos = items_.begin(); pos != items_.end();pos++){
		pos->first->dump(o);
		o << "{";
		pos->second.dump(o);
		o << "} ";
	}
	o << "} ";
}

ExprCombine::ExprCombine(Cfg_Node* node)
{
	node_ = node;
	num_ = 0;
}

ExprCombine::~ExprCombine()
{
	
}

bool operator == (const vector<pair<PExpr*, verinum> >& v1, const vector<pair<PExpr*, verinum> >& v2)
{
	if(v1.size() != v2.size()) return false;
	for(unsigned i = 0; i < v1.size(); i++){
		if(v1[i].second != v2[i].second){
			return false;
		}
	}
	return true;
}

void ExprCombine::add(verinum value, const map<PExpr*, verinum>& items)
{
	vector<pair<PExpr*, verinum> > temp_items;
	map<PExpr*, verinum>::const_iterator pos;
	for(pos = items.begin(); pos != items.end(); pos++){
		temp_items.push_back(make_pair(pos->first, pos->second));
	}

	bool find = false;
	if(cond_items_.find(value) != cond_items_.end()){

		if(!cond_items_[value].second.empty()){

			for(CondValue* cv : cond_items_[value].second){

				if(cv->items_ == temp_items){
					find = true;
					cv->num_ += 1;
					break;
				}
			}
		}
	}

	if(!find){
		CondValue* cond_value = new CondValue(temp_items);
		cond_items_[value].second.push_back(cond_value);
	}

	cond_items_[value].first += 1;
	num_ += 1;
}

void ExprCombine::dump(ostream& o)
{
	o << "------------------------------------Combine------------------------------------" << endl;
	o << "Count coming in to " << node_->type << " in line " << node_->lineno << " ";
	node_->expr[0]->dump(o);
	o << endl;
	map<verinum, pair<unsigned, vector<CondValue*> > >::iterator pos;
	for(pos = cond_items_.begin(); pos != cond_items_.end(); pos++){
		o << "-------------------------------------------------------------------------------" << endl;
		o << "Value : ";
		pos->first.dump(o);
		o << " Hit Times : " << pos->second.first << " Covered : " << double(pos->second.first*100)/double(num_) << endl;
		o << "-------------------------------------------------------------------------------" << endl;
		o << left << setw(10) << "Hit Times" << left << setw(10) << "Covered%" << "Items" << endl;
		o << left << setw(10) << "---------" << left << setw(10) << "--------" << "-----" << endl;
		for(CondValue* cv : pos->second.second){
			o << left << setw(10) << cv->num_ << left << setw(10) << double(cv->num_*100)/double(pos->second.first);
			cv->dump(o);
			o << endl;
		}
	}
}

CombineRep::CombineRep(Module_Cfgs* cfgs)
{
	//build combine report for every cfg
	for(unsigned i = 0; i < cfgs->cfgs->count(); i++){
		Cfg* cfg = (*(cfgs->cfgs))[i];

		//select the always block
		if((*(cfg->root))[1]->type == "ISCONTROL.EVENT"){
			for(unsigned j = 0; j < cfg->root->count(); j++){
				Cfg_Node* node = (*(cfg->root))[j];

				//build combine report for a condit expression
				if(node->type.find("ISCONTROL") == 0 && node->type != "ISCONTROL.EVENT"){
					exprs_[node] = new ExprCombine(node);
				}
			}
		}
	}
}

CombineRep::~CombineRep(){
	if(!exprs_.empty()){
		map<Cfg_Node* , ExprCombine*>::iterator pos;
		for(pos = exprs_.begin(); pos != exprs_.end(); pos++){
			delete pos->second;
		}
	}
}

void CombineRep::add(Cfg_Node* node, verinum value, const map<PExpr*, verinum>& items)
{
	if(exprs_.find(node) != exprs_.end()){
		exprs_[node]->add(value, items);
	}
}

void CombineRep::dump(ostream& o)
{
	map<Cfg_Node* , ExprCombine*>::iterator pos;
	for(pos = exprs_.begin(); pos != exprs_.end(); pos++){
		pos->second->dump(o);
	}
}

BasicRep::BasicRep()
{
	toggle_num_ = new CoverNum;
	stat_num_ = new CoverNum;
	path_num_ = new CoverNum;
	branch_num_ = new CoverNum;
}

BasicRep::~BasicRep(){
	delete toggle_num_;
	delete stat_num_;
	delete path_num_;
	delete branch_num_;
}

ModuleRep::ModuleRep(Module* mod)
{
	module_ = mod;
	toggle_ = new ToggleRep;
	fsm_ = new FsmRep;
	statement_ = new StatRep(module_->get_cfg());
	path_ = new PathRep(module_);
	branch_ = new BranchRep(module_->get_cfg());
	combine_ = new CombineRep(module_->get_cfg());
}

ModuleRep::~ModuleRep()
{
	delete toggle_;
	delete fsm_;
	delete statement_;
	delete path_;
	delete branch_;
	delete combine_;
}

void ModuleRep::add_toggle(const VcdVar* var)
{
	toggle_->add(var);
}

void ModuleRep::add_fsm(const VcdVar* var)
{
	fsm_->add(var);
}

void ModuleRep::add_stat(const set<unsigned, less<unsigned> >& paths)
{
	statement_->add(paths);
}

void ModuleRep::add_path(const set<unsigned, less<unsigned> >& paths, unsigned time, Cfg* cfg)
{
	path_->add(time, cfg, paths);
}

void ModuleRep::add_branch(const set<unsigned, less<unsigned> >& paths)
{
	branch_->add(paths);
}

void ModuleRep::add_combine(Cfg_Node* node, verinum value, const map<PExpr*, verinum>& items)
{
	combine_->add(node, value, items);
}

void ModuleRep::dump_toggle(ostream& o)
{
	o << "Toggle Coverage for " << module_->get_file() << " " << module_->pscope_name() << endl;
	o << "------------------------------------------------------------------------------" << endl;
	o << left << setw(25) << "Node Type" << right << setw(10) << "0H->1H" << right << setw(10) << "1H->0H" << endl;
	o << left << setw(25) << "---------" << right << setw(10) << "------" << right << setw(10) << "------" << endl;
	toggle_->dump(o);
	o << "------------------------------------------------------------------------------" << endl;
	toggle_num_->add(toggle_->get_num());
	o << left << setw(10) << module_->pscope_name();
	o << left << setw(10) << "Toggle Coverage = " << double(toggle_num_->hit_*100)/double(toggle_num_->total_);
	o << "%(" << toggle_num_->hit_ << " of " << toggle_num_->total_ << " bits" << ")";
	o << endl;
	o << "------------------------------------------------------------------------------" << endl;
}

void ModuleRep::dump_fsm(ostream& o)
{
	o << "Fsm Coverage for " << module_->get_file() << " " << module_->pscope_name() << endl;
	o << "------------------------------------------------------------------------------" << endl;
	fsm_->dump(o);
	o << "------------------------------------------------------------------------------" << endl;
}

void ModuleRep::dump_stat(ostream& o)
{
	o << "Statement Coverage for " << module_->get_file() << " " << module_->pscope_name() << endl;
	o << "------------------------------------------------------------------------------" << endl;
	o << left << setw(10) << "Line" << left << setw(25) << "Node Type" << left << setw(10) << "Hit Time" << endl;
	o << left << setw(10) << "----" << left << setw(25) << "---------" << left << setw(10) << "--------" << endl;
	statement_->dump(o);
	o << "------------------------------------------------------------------------------" << endl;
	stat_num_->add(statement_->get_num());
	o << left << setw(10) << module_->pscope_name();
	o << left << setw(10) << "Statement Coverage = " << double(stat_num_->hit_*100)/double(stat_num_->total_);
	o << "%(" << stat_num_->hit_ << " of " << stat_num_->total_ << " lines" << ")";
	o << endl;
	o << "------------------------------------------------------------------------------" << endl;
}

void ModuleRep::dump_path(ostream& o)
{
	o << "Path Coverage for " << module_->get_file() << " " << module_->pscope_name() << endl;
	path_->dump(o);
	o << "------------------------------------------------------------------------------" << endl;
	path_num_->add(path_->get_num());
	o << left << setw(10) << module_->pscope_name();
	o << left << setw(10) << "Path Coverage = " << double(path_num_->hit_*100)/double(path_num_->total_);
	o << "%(" << path_num_->hit_ << " of " << path_num_->total_ << " paths" << ")";
	o << endl;
	o << "------------------------------------------------------------------------------" << endl;
}

void ModuleRep::dump_branch(ostream& o)
{
	o << "Branch Coverage for " << module_->get_file() << " " << module_->pscope_name() << endl;
	branch_->dump(o);
	o << "------------------------------------------------------------------------------" << endl;
	branch_num_->add(branch_->get_num());
	o << left << setw(10) << module_->pscope_name();
	o << left << setw(10) << "Branch Coverage = " << double(branch_num_->hit_*100)/double(branch_num_->total_);
	o << "%(" << branch_num_->hit_ << " hit of " << branch_num_->total_ << " branches" << ")";
	o << endl;
	o << "------------------------------------------------------------------------------" << endl;
}

void ModuleRep::dump_combine(ostream& o)
{
	o << "Combine Coverage for " << module_->get_file() << " " << module_->pscope_name() << endl;
	combine_->dump(o);
	o << "-------------------------------------------------------------------------------" << endl;
}

//Select the cover fuctions.
FinalRep::FinalRep(bool t, bool f, bool s, bool p, bool b, bool c)
{
	toggle_flag_ = t;
	fsm_flag_ = f;
	stat_flag_ = s;
	path_flag_ = p;
	branch_flag_ = b;
	combine_flag_ = c;
}

FinalRep::~FinalRep()
{
	map<Module*, ModuleRep*>::iterator pos;
	for(pos = reports_.begin(); pos != reports_.end(); pos++){
		delete pos->second;
	}
}

void FinalRep::check(Module* mod)
{
	if(reports_.find(mod) == reports_.end()){
		reports_[mod] = new ModuleRep(mod);
	}
}

void FinalRep::add_toggle(Module* mod, const VcdVar* var)
{
	check(mod);
	reports_[mod]->add_toggle(var);
}

void FinalRep::add_fsm(Module* mod, const VcdVar* var)
{
	check(mod);
	reports_[mod]->add_fsm(var);
}

void FinalRep::add_stat(Module* mod, const set<unsigned, less<unsigned> >& path_)
{
	check(mod);
	reports_[mod]->add_stat(path_);
}

void FinalRep::add_path(Module* mod, const set<unsigned, less<unsigned> >& path_, unsigned time, Cfg* cfg)
{
	check(mod);
	reports_[mod]->add_path(path_, time, cfg);
}

void FinalRep::add_branch(Module* mod, const set<unsigned, less<unsigned> >& path_)
{
	check(mod);
	reports_[mod]->add_branch(path_);
}

void FinalRep::add_combine(Module* mod, Cfg_Node* node, verinum value, const map<PExpr*, verinum>& items)
{
	check(mod);
	reports_[mod]->add_combine(node, value, items);
}

void FinalRep::dump(ostream& o)
{
	if(toggle_flag_){
		cout << "Generate the toggle coverage report..." << endl;
		dump_toggle(o);
	}

	if(fsm_flag_){
		cout << "Generate the fsm coverage report..." << endl;
		dump_fsm(o);
	}

	if(stat_flag_){
		cout << "Generate the statement coverage report..." << endl;
		dump_stat(o);
	}

	if(path_flag_){
		cout << "Generate the path coverage report..." << endl;
		dump_path(o);
	}

	if(branch_flag_){
		cout << "Generate the branch coverage report..." << endl;
		dump_branch(o);
	}

	if(combine_flag_){
		cout << "Generate the combine coverage report..." << endl;
		dump_combine(o);
	}

	cout << "Genetate the summary Date by files..." << endl;
	dump_summary(cout);
	dump_summary(o);
}

void FinalRep::dump_summary(ostream& o)
{
	o << "Coverage BasicRep Summary Data by files" << endl;
	o << left << setw(10);
	o << left << setw(20) << "Enabled Coverage";
	o << left << setw(10) << "Active";
	o << left << setw(10) << "Total";
	o << left << setw(10) << "Misses";
	o << left << setw(10) << "Hits";
	o << left << setw(10) << "Covered%";
	o << endl;
	o << left << setw(10);
	o << left << setw(20) << "----------------";
	o << left << setw(10) << "------";
	o << left << setw(10) << "-----";
	o << left << setw(10) << "------";
	o << left << setw(10) << "----";
	o << left << setw(10) << "-------";
	o << endl;

	if(toggle_flag_){
		toggle_num_->dump(o, "Toggle");
	}

	if(stat_flag_){
		stat_num_->dump(o, "Statement");
	}

	if(branch_flag_)
	{
		branch_num_->dump(o, "Branch");
	}

	if(path_flag_){
		path_num_->dump(o, "Path");
	}

	if(fsm_flag_){
		o << left << setw(10);
		o << left << setw(20) << "Fsm";
		o << left << setw(10) << "none";
		o << left << setw(10) << "none";
		o << left << setw(10) << "none";
		o << left << setw(10) << "none";
		o << left << setw(10) << "none";
		o << endl;
	}

	if(combine_flag_){
		o << left << setw(10);
		o << left << setw(20) << "Combine";
		o << left << setw(10) << "none";
		o << left << setw(10) << "none";
		o << left << setw(10) << "none";
		o << left << setw(10) << "none";
		o << left << setw(10) << "none";
		o << endl;
	}
}

void FinalRep::dump_toggle(ostream& o)
{
	o << "================================Toggle Details================================" << endl;
	map<Module*, ModuleRep*>::iterator pos;
	for(pos = reports_.begin(); pos != reports_.end(); pos++)
	{
		pos->second->dump_toggle(o);
		toggle_num_ ->add(pos->second->toggle_num_);
	}
	o << left << setw(10) << "Total";
	o << left << setw(10) << "Toggle Coverage = " << double(toggle_num_->hit_*100)/double(toggle_num_->total_) 
	<< "%(" << toggle_num_->hit_ << " hit of " << toggle_num_->total_ << " bits" << ")" << endl;
	o << endl;
}

void FinalRep::dump_fsm(ostream& o)
{
	o << "================================Fsm Details================================" << endl;
	map<Module*, ModuleRep*>::iterator pos;
	for(pos = reports_.begin(); pos != reports_.end(); pos++)
	{
		pos->second->dump_fsm(o);
	}
	o << endl;
}

void FinalRep::dump_stat(ostream& o)
{
	o << "================================Statement Details================================" << endl;
	map<Module*, ModuleRep*>::iterator pos;
	for(pos = reports_.begin(); pos != reports_.end(); pos++)
	{
		pos->second->dump_stat(o);
		stat_num_ ->add(pos->second->stat_num_);
	}
	o << left << setw(10) << "Total";
	o << left << setw(10) << "Statement Coverage = " << double(stat_num_->hit_*100)/double(stat_num_->total_) 
	<< "%(" << stat_num_->hit_ << " hit of " << stat_num_->total_ << " lines" << ")" << endl;
	o << endl;
}

void FinalRep::dump_path(ostream& o)
{
	o << "================================Path Details================================" << endl;
	map<Module*, ModuleRep*>::iterator pos;
	for(pos = reports_.begin(); pos != reports_.end(); pos++)
	{
		pos->second->dump_path(o);
		path_num_ ->add(pos->second->path_num_);
	}
	o << left << setw(10) << "Total";
	o << left << setw(10) << "Path Coverage = " << double(path_num_->hit_*100)/double(path_num_->total_) 
	<< "%(" << path_num_->hit_ << " hit of " << path_num_->total_ << " paths" << ")" << endl;
	o << endl;
}

void FinalRep::dump_branch(ostream& o)
{
	o << "================================Branch Details================================" << endl;
	map<Module*, ModuleRep*>::iterator pos;
	for(pos = reports_.begin(); pos != reports_.end(); pos++)
	{
		pos->second->dump_branch(o);
		branch_num_ ->add(pos->second->branch_num_);
	}
	o << left << setw(10) << "Total";
	o << left << setw(10) << "Branch Coverage = " << double(branch_num_->hit_*100)/double(branch_num_->total_) 
	<< "%(" << branch_num_->hit_ << " hit of " << branch_num_->total_ << " branchs" << ")" << endl;
	o << endl;
}

void FinalRep::dump_combine(ostream& o)
{
	o << "================================Combine Details================================" << endl;
	map<Module*, ModuleRep*>::iterator pos;
	for(pos = reports_.begin(); pos != reports_.end(); pos++)
	{
		pos->second->dump_combine(o);
	}
	o << endl;
}
