#include "vcdvar.h"
#include <iomanip>
#include <algorithm>

void VcdVar::dump(ostream& o)
{
	o << name << "[" << msb << ":" << lsb <<"]";
	o << " cur_var = ";
	cur_val.dump(o);
	o << " pre_val = ";
	pre_val.dump(o);
	o << " sim_val = ";
	sim_val.dump(o);
	o << endl; 
}

VcdVar::VcdVar()
{

}

VcdVar::~VcdVar()
{

}

BranchTree::BranchTree(Module* md, TreeType type)
{
	module_ = md;
	type_ = type;
}

BranchTree::~BranchTree()
{
	
}

BrCondit::BrCondit(Module* md, TreeType type) : BranchTree(md, type)
{

}

BrCondit::~BrCondit()
{
	delete tru_;
	if(fal_) delete fal_;
}

BrCase::BrCase(Module* md, TreeType type) : BranchTree(md, type)
{
	
}

BrCase::~BrCase()
{
	for(pair<PExpr*, BranchTree*> item : items_) {
		delete item.second;
	}
}

BrBlock::BrBlock(Module* md, TreeType type) : BranchTree(md, type)
{

}

BrBlock::~BrBlock()
{
	for(BranchTree* stat : stats_) {
		delete stat;
	}
}

BrLeaf::BrLeaf(Module* md, TreeType type) : BranchTree(md, type)
{

}

BrLeaf::~BrLeaf()
{

}

unsigned BrCondit::get_total()
{
	unsigned n1, n2;
	n1 = tru_->get_total();
	if(fal_) n2 = fal_->get_total();
	else n2 = 1;
	return (n1+n2);
}

unsigned BrCondit::get_hit(VcdScope* scope)
{
	unsigned n1, n2;
	n1 = tru_->get_hit(scope);
	if(fal_) n2 = fal_->get_hit(scope);
	else n2 = 1;
	return (n1+n2);
}

unsigned BrCase::get_total()
{
	unsigned res = 0;
	for(pair<PExpr*, BranchTree*> item : items_) {
		res += item.second->get_total();
	}
	return res;
}

unsigned BrCase::get_hit(VcdScope* scope)
{
	unsigned res = 0;
	for(pair<PExpr*, BranchTree*> item : items_) {
		res += item.second->get_hit(scope);
	}
	return res;
}

unsigned BrBlock::get_total()
{
	unsigned res = 0;
	for(BranchTree* stat : stats_) {
		res += stat->get_total();
	}
	return res;
}

unsigned BrBlock::get_hit(VcdScope* scope)
{
	unsigned res = 0;
	for(BranchTree* stat : stats_) {
		res += stat->get_hit(scope);
	}
	return res;
}

unsigned BrLeaf::get_total()
{
	return 1;
}

unsigned BrLeaf::get_hit(VcdScope* scope)
{
	if(cover_.find(scope) == cover_.end())
		return 0;
	return 1;
}

void BrCondit::add(vector<unsigned>& values, VcdScope* scope, unsigned& idx)
{
	cout << typeid(this).name() << " " << idx << endl;
	assert(idx < values.size());
	cover_[scope] = true;
	if(values[idx]) tru_->add(values, scope, ++idx);
	else fal_->add(values, scope, ++idx);
}

void BrCase::add(vector<unsigned>& values, VcdScope* scope, unsigned& idx)
{
	cout << typeid(this).name() << " " << idx << endl;
	for(size_t i = 0; i < items_.size(); i++) {
		if(items_[i].first)
			cout << i << " " << (*(items_[i].first)) << " " << items_[i].second << endl;
		else
			cout << i << " " << "default" << " " << items_[i].second << endl;
	}
	assert(idx < values.size());
	cover_[scope] = true;
	assert(values[idx] < items_.size());
	items_[values[idx]].second->add(values, scope, ++idx);
}

void BrBlock::add(vector<unsigned>& values, VcdScope* scope, unsigned& idx)
{
	cout << typeid(this).name() << " " << idx << " " << this << endl;
	cover_[scope] = true;
	for(BranchTree* stat : stats_) {
		stat->add(values, scope, idx);
	}
}

void BrLeaf::add(vector<unsigned>& values, VcdScope* scope, unsigned& idx)
{
	cout << typeid(this).name() << " " << idx << " " << this << endl;
	cover_[scope] = true;
}

void BrCondit::dump_summary(ostream& o, VcdScope* scope)
{
	string type;
	switch (type_) {
	case TreeType::IF :
		type = "IF";
		break;
	case TreeType::TERNARY :
		type = "TERNARY";
		break;
	default:
		break;
	}
	dump_summary_line(o, type, cond_->get_lineno(), get_total(), get_hit(scope));
}

void BrCase::dump_summary(ostream& o, VcdScope* scope)
{
	string type;
	switch (type_) {
	case TreeType::CASE :
		type = "CASE";
		break;
	case TreeType::CASEX :
		type = "CASEX";
		break;
	case TreeType::CASEZ :
		type = "CASEZ";
		break;
	default:
		break;
	}
	dump_summary_line(o, type, cond_->get_lineno(), get_total(), get_hit(scope));
}

void BrBlock::dump_summary(ostream& o, VcdScope* scope)
{

}

void BrLeaf::dump_summary(ostream& o, VcdScope* scope)
{

}

void BrCondit::dump(ostream& o, VcdScope* scope)
{
	bool found = cover_.find(scope) != cover_.end();
	if(type_ == TERNARY) {
		dump_line_detail(o, false, false, cond_);
	} else {
		Filelines* file_ = scope->pdesign_->get_lines();
		string lineinf = (*file_)[scope->module_->get_file()][lineno_-1];
		dump_line_detail(o, lineno_, false, false, lineinf);
	}
	tru_->dump(o, scope);
	if(fal_) fal_->dump(o, scope);
	else {
		string error = "MISSING ELSE";
		dump_line_detail(o, lineno_, true, true, error);
	}
}

void BrCase::dump(ostream& o, VcdScope* scope)
{
	Filelines* file_ = scope->pdesign_->get_lines();
	bool found = cover_.find(scope) != cover_.end();
	string lineinf = (*file_)[scope->module_->get_file()][lineno_-1];
	dump_line_detail(o, lineno_, false, false, lineinf);
	for(pair<PExpr*, BranchTree*> item : items_){
		item.second->dump(o, scope);
		if(item.second == (--items_.end())->second && item.first) {
			string error = "MISSING DEFAULT";
			dump_line_detail(o, lineno_, true, true, error);
		}
	}
}

void BrBlock::dump(ostream& o, VcdScope* scope)
{
	for(BranchTree* stat : stats_){
		stat->dump(o, scope);
	}
}

void BrLeaf::dump(ostream& o, VcdScope* scope)
{
	bool found = cover_.find(scope) != cover_.end();
	if(type_ == EXPRLEAF)
		dump_line_detail(o, true, found, node_.expr_);
	else {
		Filelines* file_ = scope->pdesign_->get_lines();
		string lineinf = (*file_)[scope->module_->get_file()][node_.lineno-1];
		dump_line_detail(o, node_.lineno, true, found, lineinf);
	}
}

ScopeRep::ScopeRep(bool t, bool f, bool l, bool p, bool b, bool c, VcdScope* scope)
{
	cover_t = t;
	cover_f = f;
	cover_l = l;
	cover_p = p;
	cover_b = b;
	cover_c = c;
	scope_ = scope;
}

void ScopeRep::initial()
{
	if(cover_t) toggle_initial();
	if(cover_f) fsm_initial();
	if(cover_l) line_initial();
	if(cover_p) path_initial();
	if(cover_b) branch_initial();
	if(cover_c) cond_initial();
}

void ScopeRep::toggle_initial()
{
	map<string, VcdVar*>::iterator pos = scope_->defines_.begin();
	for(; pos != scope_->defines_.end(); pos++) {
		ToggleRep* rep = new ToggleRep(pos->second);
		toggle_[pos->second] = rep;
	}
}

void ScopeRep::fsm_initial()
{
	for(VcdVar* var : scope_->fsm_vars_) {
		FsmRep* rep = new FsmRep(var);
		fsm_[var] = rep;
	}
}

void ScopeRep::line_initial()
{
	if(!scope_->module_->sync_cfgs_.empty() || !scope_->module_->combine_cfgs_.empty()) {
		for(unsigned i = 0; i < scope_->module_->get_cfg()->cfgs->count(); i++) {
			Cfg* cfg = (*(scope_->module_->get_cfg()->cfgs))[i];
			LineRep* rep = new LineRep(cfg, scope_);
			line_[cfg->id] = rep;
		}
	}
}

void ScopeRep::path_initial()
{
	if(!scope_->module_->sync_cfgs_.empty() || !scope_->module_->combine_cfgs_.empty()) {
		for(unsigned i = 0; i < scope_->module_->get_cfg()->cfgs->count(); i++) {
			Cfg* cfg = (*(scope_->module_->get_cfg()->cfgs))[i];
			PathRep* rep = new PathRep(cfg, scope_);
			path_[cfg->id] = rep;
		}
	}
}

void ScopeRep::branch_initial()
{
	cout << scope_->module_->branchs_.size() << endl;
	map<unsigned, BranchTree*>::iterator pos = scope_->module_->branchs_.begin();
	for(; pos != scope_->module_->branchs_.end(); pos++) {
		BranchRep* rep = new BranchRep(pos->second, scope_);
		branch_[pos->first] = rep;
	}
}

void ScopeRep::cond_initial()
{
	map<PExpr*, set<PExpr*> >::iterator pos = scope_->module_->exprs_.begin();
	for(; pos != scope_->module_->exprs_.end(); pos++) {
		CondRep* rep = new CondRep(pos->first, pos->second);
		cond_[pos->first] = rep;
	}
}

ScopeRep::~ScopeRep()
{
	if(cover_t) {
		map<VcdVar*, ToggleRep*>::iterator pos = toggle_.begin();
		for(; pos != toggle_.end(); pos++) {
			delete pos->second;
		}
	}
	if(cover_f) {
		map<VcdVar*, FsmRep*>::iterator pos = fsm_.begin();
		for(; pos != fsm_.end(); pos++) {
			delete pos->second;
		}
	}
	if(cover_l) {
		map<unsigned, LineRep*>::iterator pos = line_.begin();
		for(; pos != line_.end(); pos++) {
			delete pos->second;
		}
	}
	if(cover_p) {
		map<unsigned, PathRep*>::iterator pos = path_.begin();
		for(; pos != path_.end(); pos++) {
			delete pos->second;
		}
	}
	if(cover_b) {
		map<unsigned, BranchRep*>::iterator pos = branch_.begin();
		for(; pos != branch_.end(); pos++) {
			delete pos->second;
		}
	}
	if(cover_c) {
		map<PExpr*, CondRep*>::iterator pos = cond_.begin();
		for(; pos != cond_.end(); pos++) {
			delete pos->second;
		}
	}
}

void ScopeRep::add_tgl_report(set<VcdVar*> vars)
{
	for(VcdVar* var : vars) {
		assert(toggle_.find(var) != toggle_.end());
		toggle_[var]->add();
	}
}

void ScopeRep::add_fsm_report(VcdVar* var)
{
	assert(fsm_.find(var) != fsm_.end());
	fsm_[var]->add();
}

void ScopeRep::add_line_report(Cfg* cfg, set<unsigned>& lines)
{
	assert(line_.find(cfg->id) != line_.end());
	line_[cfg->id]->add(lines);
}

void ScopeRep::add_path_report(Cfg* cfg, set<unsigned>& lines)
{
	assert(path_.find(cfg->id) != path_.end());
	path_[cfg->id]->add(lines);
}

void ScopeRep::add_branch_report(unsigned lineno, vector<unsigned>& values)
{
	cout << lineno << " ";
	cout << "<";
	for(unsigned value : values) {
		cout << " [" << value << "]";
	}
	cout << " >" << endl;
	assert(branch_.find(lineno) != branch_.end());
	branch_[lineno]->add(values);
}

void ScopeRep::add_cond_report(map<PExpr*, map<PExpr*, bool> > values)
{
	map<PExpr*, map<PExpr*, bool> >::iterator pos = values.begin();
	for(; pos != values.end(); pos++) {
		assert(cond_.find(pos->first) != cond_.end());
		cond_[pos->first]->add(pos->second);
	}
}

void ScopeRep::dump(ostream& o)
{
	if(cover_t)
		dump_tgl_report(o);
	if(cover_f)
		dump_fsm_report(o);
	if(cover_l)
		dump_line_report(o);
	if(cover_p)
		dump_path_report(o);
	if(cover_b)
		dump_branch_report(o);
	if(cover_c)
		dump_cond_report(o);
}

void ScopeRep::dump_tgl_report(ostream& o)
{
	unsigned port_num = 0;
	unsigned signal_num = 0;
	map<VcdVar*, ToggleRep*>::iterator pos;
	for(pos = toggle_.begin(); pos != toggle_.end(); pos++){
		if(pos->first->type == VcdVar::SIGNAL) signal_num++;
		else port_num++;
	}
	dump_coverage_line(o, scope_, "Toggle");
	dump_summary_line(o, "Type", "Total", "Covered", "Persent");
	dump_summary_line(o, "----", "-----", "-------", "-------");
	dump_tgl_summary(o, "Total");
	if(port_num) dump_tgl_summary(o, "Port");
	if(signal_num) dump_tgl_summary(o, "Signal");
	if(port_num) dump_tgl_detail(o, "Port");
	if(signal_num) dump_tgl_detail(o, "Signal");
	if(port_num) dump_tgl_bit(o, "Port");
	if(signal_num) dump_tgl_bit(o, "Signal");
	dump_line(o, "\n");
}

void ScopeRep::dump_tgl_summary(ostream& o, string type)
{
	unsigned total = 0; 
	unsigned pos_covered = 0;
	unsigned neg_covered = 0;
	unsigned num = 0;
	unsigned cover_num = 0;
	map<VcdVar*, ToggleRep*>::iterator pos = toggle_.begin();
	for(; pos != toggle_.end(); pos++) {
		if(type == "Total"
		|| (type == "Port" && pos->first->type != VcdVar::SIGNAL)
		|| (type == "Signal" && pos->first->type == VcdVar::SIGNAL)) {
			unsigned pos_add, neg_add;
			pos_add = pos->second->get_pos_num();
			neg_add = pos->second->get_neg_num();
			total += pos->first->width;
			pos_covered += pos_add;
			neg_covered += neg_add;
			if(pos_add == pos->first->width && neg_add == pos->first->width)
				cover_num++;
			num++;
		}
	}
	dump_summary_line(o, type + "s", num, cover_num);
	dump_summary_line(o, type + " Bits", total*2, pos_covered+neg_covered);
	dump_summary_line(o, type + " Bits 0->1", total, pos_covered);
	dump_summary_line(o, type + " Bits 1->0", total, neg_covered);
}

void ScopeRep::dump_tgl_detail(ostream&o, string type)
{
	dump_item_line(o, type + " Details");
	dump_toggle_detail(o, "Name", "Toggle", "Toggle 0->1", "Toggle 1->0", "Direction");
	dump_toggle_detail(o, "----", "------", "-----------", "-----------", "---------");
	bool flag = type == "Signal"; 
	map<VcdVar*, ToggleRep*>::iterator pos = toggle_.begin();
	for(; pos != toggle_.end(); pos++) {
		if(pos->first->type == VcdVar::SIGNAL && flag
		|| pos->first->type != VcdVar::SIGNAL && !flag)
			pos->second->dump_detail(o);
	}
}

void ScopeRep::dump_tgl_bit(ostream& o, string type)
{
	dump_item_line(o, type + " Bits");
	dump_toggle_bit(o, "Name", "Bits");
	dump_toggle_bit(o, "----", "----");
	bool flag = type == "Signal"; 
	map<VcdVar*, ToggleRep*>::iterator pos = toggle_.begin();
	for(; pos != toggle_.end(); pos++) {
		if(pos->first->type == VcdVar::SIGNAL && flag
		|| pos->first->type != VcdVar::SIGNAL && !flag)
			pos->second->dump_bit(o);
	}
}

void ScopeRep::dump_fsm_report(ostream& o)
{
	if(!fsm_.empty()) {
		dump_coverage_line(o, scope_, "Fsm");
		map<VcdVar*, FsmRep*>::iterator pos = fsm_.begin();
		for(; pos != fsm_.end(); pos++) {
			pos->second->dump_state(o);
			dump_depart_line(o);
			pos->second->dump_transition(o);
			dump_depart_line(o);
		}
		dump_line(o, "\n");
	}
}

void ScopeRep::dump_line_summary(ostream& o)
{
	unsigned hit = 0;
	unsigned total = 0;
	map<unsigned, LineRep*>::iterator pos = line_.begin();
	for(; pos != line_.end(); pos++) {
		pos->second->dump_summary(o);
		hit += pos->second->get_hit();
		total += pos->second->get_total();
	}
	dump_summary_line(o, "TOTAL", 0, total, hit);
}

void ScopeRep::dump_line_detail(ostream& o)
{
	map<unsigned, LineRep*>::iterator pos = line_.begin();
	for(; pos != line_.end(); pos++) {
		pos->second->dump(o);
	}
}

void ScopeRep::dump_line_report(ostream& o)
{
	if(!line_.empty()) {
		dump_coverage_line(o, scope_, "Line");
		dump_summary_line(o, "Type", "Lineno", "Total", "Covered", "Persent");
		dump_summary_line(o, "----", "------", "-----", "-------", "-------");
		dump_line_summary(o);
		dump_depart_line(o);
		dump_line_detail(o);
	}
}

void ScopeRep::dump_path_summary(ostream& o)
{
	unsigned hit = 0;
	unsigned total = 0;
	map<unsigned, PathRep*>::iterator pos = path_.begin();
	for(; pos != path_.end(); pos++) {
		pos->second->dump_summary(o);
		hit += pos->second->get_hit();
		total += pos->second->get_total();
	}
	dump_summary_line(o, "TOTAL", 0, total, hit);
}

void ScopeRep::dump_path_detail(ostream& o)
{
	map<unsigned, PathRep*>::iterator pos = path_.begin();
	for(; pos != path_.end(); pos++) {
		pos->second->dump(o);
		dump_line(o, "\n");
	}
}

void ScopeRep::dump_path_report(ostream& o)
{
	if(!path_.empty()) {
		dump_coverage_line(o, scope_, "Path");
		dump_summary_line(o, "Type", "Lineno", "Total", "Covered", "Persent");
		dump_summary_line(o, "----", "------", "-----", "-------", "-------");
		dump_path_summary(o);
		dump_depart_line(o);
		dump_path_detail(o);
	}
}

void ScopeRep::dump_branch_summary(ostream& o)
{
	unsigned total = 0;
	unsigned hit = 0;
	map<unsigned, BranchRep*>::iterator pos = branch_.begin();
	for(; pos != branch_.end(); pos++) {
		pos->second->dump_summary(o);
		total += pos->second->get_total();
		hit += pos->second->get_hit();
	}
	dump_summary_line(o, "TOTAL", 0, total, hit);
	dump_line(o, "\n");
}

void ScopeRep::dump_branch_detail(ostream& o)
{
	map<unsigned, BranchRep*>::iterator pos = branch_.begin();
	for(; pos != branch_.end(); pos++) {
		dump_branch_line(o, pos->first);
		dump_depart_line(o);
		pos->second->dump(o);
		dump_line(o, "\n");
	}
}

void ScopeRep::dump_branch_report(ostream& o)
{
	if(!branch_.empty()) {
		dump_coverage_line(o, scope_, "Branch");
		dump_summary_line(o, "Type", "Lineno", "Total", "Covered", "Persent");
		dump_summary_line(o, "----", "------", "-----", "-------", "-------");
		dump_branch_summary(o);
		dump_branch_detail(o);
	}
}

void ScopeRep::dump_cond_summary(ostream& o)
{
	unsigned hit = 0;
	unsigned total = 0;
	map<PExpr*, CondRep*>::iterator pos = cond_.begin();
	for(; pos != cond_.end(); pos++) {
		hit += pos->second->get_hit();
		total += pos->second->get_total();
	}
	dump_summary_line(o, "TOTAL", total, hit);
}

void ScopeRep::dump_cond_detail(ostream& o)
{
	map<PExpr*, CondRep*>::iterator pos = cond_.begin();
	for(; pos != cond_.end(); pos++) {
		pos->second->dump(o);
		dump_line(o,"");
	}
}

void ScopeRep::dump_cond_report(ostream& o)
{
	if(!cond_.empty()) {
		dump_coverage_line(o, scope_, "Cond");
		dump_summary_line(o, "Lineno", "Total", "Covered", "Persent");
		dump_summary_line(o, "------", "-----", "-------", "-------");
		dump_cond_summary(o);
		dump_depart_line(o);
		dump_line(o, "\n");
		dump_cond_detail(o);
	}
}

ToggleRep::ToggleRep(VcdVar* var)
{
	var_ = var;
	pos_bits_ = vector<bool> (var->width, false);
	neg_bits_ = vector<bool> (var->width, false);
}

ToggleRep::~ToggleRep()
{

}

void ToggleRep::add()
{
	for(unsigned idx = 0; idx < var_->width; ++idx) {
		//Toggle bit change from 0 to 1
		if((var_->pre_val[idx] == verinum::V0) && (var_->cur_val[idx] == verinum::V1)) {
			pos_bits_[idx] = true;
		}
		//Toggle bit change from 1 to 0
		if((var_->pre_val[idx] == verinum::V1) && (var_->cur_val[idx] == verinum::V0)) {
			neg_bits_[idx] = true;
		}
	}
}

unsigned ToggleRep::get_pos_num()
{
	return count(pos_bits_.begin(), pos_bits_.end(), true);
}

unsigned ToggleRep::get_neg_num()
{
	return count(neg_bits_.begin(), neg_bits_.end(), true);
}

void ToggleRep::dump_detail(ostream& o)
{
	ostringstream name;
	name << var_->name;
	bool posedge = get_pos_num()==var_->width;
	bool negedge = get_neg_num()==var_->width;
	dump_toggle_detail(o, name, posedge, negedge, var_->type);
}

void ToggleRep::dump_bit(ostream& o)
{
	ostringstream name, bits;
	name << var_->name;
	for(unsigned i = 0; i < var_->width; i++) {
		if(pos_bits_[i] && neg_bits_[i]) bits << "*";
		else if(pos_bits_[i]) bits << "1";
		else if(neg_bits_[i]) bits << "0";
		else bits << "?";
	}
	dump_toggle_bit(o, name, bits);
}

FsmRep::FsmRep(VcdVar* var)
{
	var_ = var;
}

FsmRep::~FsmRep()
{

}

void FsmRep::add()
{
	//Add State.
	if(states_.find(var_->cur_val) == states_.end())
		states_[var_->cur_val] = true;
	//Add Transition.
	if(var_->pre_val.is_defined()) {
		pair<verinum, verinum> tran = make_pair(var_->pre_val, var_->cur_val);
		if(trans_.find(tran) == trans_.end())
			trans_[tran] = true;
	}
}

unsigned FsmRep::get_state()
{
	return states_.size();
}

unsigned FsmRep::get_transition()
{
	return trans_.size();
}

void FsmRep::dump_state(ostream& o)
{
	dump_line(o, "State Details for FSM::" + var_->name);
	dump_depart_line(o);
	unsigned i = 1;
	map<verinum, bool>::iterator pos = states_.begin();
	for(; pos != states_.end(); pos++) {
		o << "State[" << i++ << "] : ";
		dump_fsm_state(o, pos->first);
		dump_line(o, " ");
	}
}

void FsmRep::dump_transition(ostream& o)
{
	dump_line(o, "Transition Details for FSM::" + var_->name);
	dump_depart_line(o);
	unsigned i = 1;
	map<pair<verinum, verinum>, bool >::iterator pos = trans_.begin();
	for(; pos != trans_.end(); pos++) {
		o << "Transition[" << i++ << "] : ";
		dump_fsm_transtate(o, pos->first.first, pos->first.second);
	}
}

LineRep::LineRep(Cfg* cfg, VcdScope* scope)
{
	cfg_ = cfg;
	file_ = scope->pdesign_->get_lines();
	module_ = scope->module_;
	initial();
}

LineRep::~LineRep()
{

}

void LineRep::initial()
{
	for(unsigned i = 0; i < cfg_->root->count(); i++) {
		Cfg_Node* node = (*(cfg_->root))[i];
		if((node->type.find("ISCONTROL") == 0
		 && node->type.compare("ISCONTROL.EVENT") != 0)
		 || node->assign_type.find("NOCONTROL") == 0) {
			stats_[node->lineno] = false;
		}
	}
}

void LineRep::add(set<unsigned>& lines)
{
	for(unsigned lineno : lines) {
		if(stats_.find(lineno) != stats_.end())
		stats_[lineno] = true;
	}
}

unsigned LineRep::get_total()
{
	return stats_.size();
}

unsigned LineRep::get_hit()
{
	unsigned res = 0;
	unordered_map<unsigned, bool>::iterator pos;
	for(pos = stats_.begin(); pos != stats_.end(); pos++) {
		if(pos->second)	res++;
	}
	return res;
}

void LineRep::dump_summary(ostream& o)
{
	dump_summary_line(o, "ALWAYS", cfg_->lineno, get_total(), get_hit());
}

void LineRep::dump(ostream& o)
{
	unsigned min_line = INT32_MAX;
	unsigned max_line = 0;
	for(unsigned i = 0; i < cfg_->root->count(); i++) {
		Cfg_Node* node = (*(cfg_->root))[i];
		if(node->lineno > 0) {
			min_line = min_line < node->lineno ? min_line : node->lineno;
			max_line = max_line > node->lineno ? max_line : node->lineno;
		}
	}
	min_line--;
	for(unsigned i = min_line; i <= max_line; i++) {
		if(stats_.find(i+1) == stats_.end())
			dump_line_detail(o, i+1, false, false, (*file_)[module_->get_file()][i]);
		else
			dump_line_detail(o, i+1, true, stats_[i+1], (*file_)[module_->get_file()][i]);
	}
}

PathRep::PathRep(Cfg* cfg, VcdScope* scope)
{
	cfg_ = cfg;
	scope_ = scope;
	file_ = scope->pdesign_->get_lines();
	paths_ = vector<bool>(scope_->module_->paths_[cfg_->id].size(), false);
}

PathRep::~PathRep()
{

}

unsigned PathRep::get_total()
{
	return paths_.size();
}

unsigned PathRep::get_hit()
{
	return count(paths_.begin(), paths_.end(), true);
}

void PathRep::add(set<unsigned>& lines)
{
	if(get_hit() == get_total())
		return;
	bool found = false;
	for(unsigned i = 0; i < scope_->module_->routes_[cfg_].size(); i++) {
		if(equal(lines, scope_->module_->routes_[cfg_][i])) {
			paths_[i] = true;
			found = true;
			break;
		}
	}
	if(!found) {
		cerr << "Problem in path coverage" << endl;
	}
}

void PathRep::dump_summary(ostream& o)
{
	dump_summary_line(o, "ALWAYS", cfg_->lineno, get_total(), get_hit());
}

void PathRep::dump(ostream& o)
{
	dump_path_line(o, cfg_->lineno);
	dump_line(o, "\n");
	for(unsigned i = 0; i < paths_.size(); i++) {
		ostringstream s1;
		string res = paths_[i] ? "Covered" : "Not Covered";
		s1 << "Path[" << i << "] " << res;
		dump_line(o, s1);
		dump_depart_line(o);
		set<unsigned>& path = scope_->module_->routes_[cfg_][i];
		for(unsigned lineno : path) {
			ostringstream s2;
			s2 << left << setw(10) << (lineno) << (*(file_))[scope_->module_->get_file()][lineno-1];
			dump_line(o, s2);
		}
		dump_line(o, "\n");
	}
}

BranchRep::BranchRep(BranchTree* tree, VcdScope* scope)
{
	tree_ = tree;
	scope_ = scope;
}

BranchRep::~BranchRep()
{

}

void BranchRep::add(vector<unsigned>& values)
{
	unsigned idx = 0;
	tree_->add(values, scope_, idx);
}

unsigned BranchRep::get_hit()
{
	return tree_->get_hit(scope_);
}

unsigned BranchRep::get_total()
{
	return tree_->get_total();
}

void BranchRep::dump(ostream& o)
{
	tree_->dump(o, scope_);
}

void BranchRep::dump_summary(ostream& o)
{
	tree_->dump_summary(o, scope_);
}

CondRep::CondRep(PExpr* cond, set<PExpr*> items)
{
	assert(items.size() < INT32_WIDTH);
	cond_ = cond;
	for(PExpr* item : items) {
		items_.push_back(item);
	}
	if(items.size() == 1) type_ = SINGEL;
	else {
		PEBinary* expr = dynamic_cast<PEBinary*>(cond);
		assert(expr);
		switch (expr->get_op()) {
		case 'a':
			type_ = AND;
			break;
		case 'o':
			type_ = OR;
			break;
		default:
			cerr << expr->get_fileline() << "Option " << expr->get_op() << " is unsupported now!" << endl;
			expr->dump(cerr);
			exit(1);
			break;
		}
	}	
}

CondRep::~CondRep()
{

}

void CondRep::add(map<PExpr*, bool>& values)
{
	unsigned val = 0;
	for(unsigned i = 0; i < items_.size(); i++) {
		val += values[items_[i]] ? 1 << i : 0;
	}
	combs_[val] = true;
}

unsigned CondRep::get_total()
{
	return (items_.size() + 1);
}

unsigned CondRep::get_hit()
{
	unsigned n = 0;
	if(type_ == SINGEL) {
		n += combs_.find(0) != combs_.end() ? 1 : 0;
		n += combs_.find(1) != combs_.end() ? 1 : 0;
	} else if(type_ == AND) {
		unsigned res = (1 << items_.size()) - 1;
		n += combs_.find(res) != combs_.end() ? 1 : 0;
		for(unsigned i = 0; i < items_.size(); i++) {
			if(combs_.find(res - (1 << i)) != combs_.end())
				n++;
		}
	} else {
		n += combs_.find(0) != combs_.end() ? 1 : 0;
		for(unsigned i = 0; i < items_.size(); i++) {
			if(combs_.find(1 << i) != combs_.end())
				n++;
		}
	}
	return n;
}

void CondRep::dump_summary(ostream& o)
{
	dump_summary_line(o, to_string(cond_->get_lineno()), get_total(), get_hit());
}

void CondRep::dump(ostream& o)
{
	dump_line(o, "EXPRESSION LINE " + to_string(cond_->get_lineno()));
	ostringstream c;
	cond_->dump(c);
	dump_line(o, c);
	dump_depart_line(o);
	for(unsigned i = 0; i < items_.size(); i++) {
		ostringstream s;
		s << "[" << i+1 << "] ";
		items_[i]->dump(s);
		dump_line(o, s); 
	}
	dump_depart_line(o);
	dump_cond_line(o, items_.size());
	if(type_ == CondType::SINGEL) {
		dump_cond_detail(o, type_, 1, 0, combs_.find(0)!= combs_.end());
		dump_cond_detail(o, type_, 1, 1, combs_.find(1)!= combs_.end());
	} else {
		unsigned base = type_ == CondType::AND ? (1<<items_.size())-1 : 0;
		if(type_ == CondType::OR)
			dump_cond_detail(o, type_, items_.size(), 0, combs_.find(base) != combs_.end());
		for(unsigned i = 0; i < items_.size(); i++) {
			unsigned value = type_ == CondType::AND ? base - (1 << i) : 1 << i; 
			dump_cond_detail(o, type_, items_.size(), i+1, combs_.find(value) != combs_.end());
		}
		if(type_ == CondType::AND)
			dump_cond_detail(o, type_, items_.size(), 0, combs_.find(base) != combs_.end());
	}
}

void dump_coverage_line(ostream& o, VcdScope* scope, string type)
{
	dump_depart_line(o);
	o << type << " Coverage for Module " << scope->module_->pscope_name() << " " << scope->name_ << endl;
	dump_depart_line(o);
}

void dump_depart_line(ostream& o)
{
	for(unsigned i = 0; i < 100; i++) o << "_";
	o << endl;
}

void dump_summary_line(ostream& o, string s1, string s2, string s3, string s4)
{
	o << left << setw(20) << s1;
	o << left << setw(10) << s2;
	o << left << setw(10) << s3;
	o << left << setw(10) << s4;
	o << endl;
}

void dump_summary_line(ostream& o, string head, unsigned total, unsigned covered)
{
	o.setf(ios::fixed);
	o << setprecision(2);
	o << left << setw(20) << head;
	o << left << setw(10) << total;
	o << left << setw(10) << covered;
	o << left << setw(10) << double(covered*100)/double(total);
	o << endl;
}

void dump_summary_line(ostream& o, string s1, string s2, string s3, string s4, string s5)
{
	o << left << setw(10) << s1;
	o << left << setw(10) << s2;
	o << left << setw(10) << s3;
	o << left << setw(10) << s4;
	o << left << setw(10) << s5;
	o << endl;
}

void dump_summary_line(ostream& o, string head, unsigned lineno, unsigned total, unsigned covered)
{
	o.setf(ios::fixed);
	o << setprecision(2);
	o << left << setw(10) << head;
	o << left << setw(10) << lineno;
	o << left << setw(10) << total;
	o << left << setw(10) << covered;
	o << left << setw(10) << double(covered*100)/double(total);
	o << endl;
}

void dump_toggle_detail(ostream& o, string s1, string s2, string s3, string s4, string s5)
{
	o << left << setw(50) << s1;
	o << left << setw(10) << s2;
	o << left << setw(12) << s3;
	o << left << setw(12) << s4;
	o << left << setw(10) << s5;
	o << endl;
}

void dump_toggle_detail(ostream& o, ostringstream& name, bool posedge, bool negedge, VcdVar::Type type)
{
	o << left << setw(50) << name.str();
	o << left << setw(10) << (posedge && negedge ? "YES" : "NO");
	o << left << setw(12) << (posedge ? "YES" : "NO");
	o << left << setw(12) << (negedge ? "YES" : "NO");
	o << left << setw(10);
	switch (type) {
	case VcdVar::INPUT :
		o << "INPUT";
		break;
	case VcdVar::OUTPUT : 
		o << "OUTPUT";
		break;
	case VcdVar::INOUT : 
		o << "INOUT";
		break;
	default:
		o << "NULL";
		break;
	}
	o << endl;
}

void dump_item_line(ostream& o, string item)
{
	dump_depart_line(o);
	o << item << endl;
	dump_depart_line(o);
}

void dump_line(ostream& o, string s)
{
	o << s << endl;
}

void dump_line(ostream& o, ostringstream& s)
{
	o << s.str() << endl;
}

void dump_toggle_bit(ostream& o, string s1, string s2)
{
	o << left << setw(50) << s1;
	o << s2 << endl;
}

void dump_toggle_bit(ostream& o, ostringstream& name, ostringstream& bits)
{
	o << left << setw(50) << name.str();
	o << bits.str() << endl;
}

void dump_fsm_state(ostream& o, const verinum& v)
{
	o << "{";
	v.dump(o);
	o << "}";
}

void dump_fsm_transtate(ostream& o, const verinum& pre_val, const verinum& cur_val)
{
	dump_fsm_state(o, pre_val);
	o << "->";
	dump_fsm_state(o, cur_val);
	o << endl;
}

void dump_line_detail(ostream& o, unsigned lineno, bool select, bool hit, string& s)
{
	string s1 = select ? (hit ? "1/1" : "0/1") : " ";
	string s2 = s1 == " " ? " " : "==>";
	o << left << setw(8) << lineno;
	o << left << setw(10) << s1;
	o << left << setw(10) << s2;
	o << s << endl;
}

void dump_line_detail(ostream& o, bool select, bool hit, PExpr* expr)
{
	string s1 = select ? (hit ? "1/1" : "0/1") : " ";
	string s2 = s1 == " " ? " " : "==>";
	o << left << setw(10) << s1;
	o << left << setw(10) << s2;
	expr->dump(o);
	o << endl;
}

void dump_path_line(ostream& o, unsigned lineno)
{
	o << "PATH COVERAGE DETAIL FOR ALWAYS IN LINE " << lineno << endl;
}

void dump_cond_line(ostream& o, unsigned num)
{
	for(unsigned i = 0; i < num; i++) {
		string s = "-" + to_string(i+1) + "-";
		o << left << setw(5) << s;
	}
	o << "Status" << endl;
}

void dump_cond_detail(ostream& o, CondRep::CondType type, unsigned num, unsigned idx, bool covered)
{
	ostringstream s;
	string status = covered ? "Covered" : "Not Covered";
	if(type == CondRep::SINGEL) {
		s << left << setw(5) << idx << status;
		dump_line(o, s);
	} else {
		bool state;
		for(unsigned i = 0; i < num; i++) {
			if(!idx || idx != (i+1))
				state = type == CondRep::AND;
			else
				state = type == CondRep::OR;
			s << left << setw(5) << state;
	}
		s << status;
		dump_line(o, s);
	}
}

void dump_branch_detail(ostream& o, unsigned lineno, string head, bool covered, ostringstream& expr)
{
	o << left << setw(10) << lineno;
	o << left << setw(10) << head;
	o << left << setw(15) << (covered ? "Covered" : "Not Covered");
	o << expr.str() << endl;
}

void dump_branch_line(ostream& o, unsigned lineno)
{
	o << "BRANCH COVEREGE DETAIL IN LINE " << lineno << endl;
}

bool equal(const set<unsigned>& l1, const set<unsigned>& l2)
{
	if(l1.size() != l2.size()) return false;
	set<unsigned>::iterator pos1 = l1.begin();
	set<unsigned>::iterator pos2 = l2.begin();
	while(pos1 != l1.end() && pos2 != l2.end()) {
		if((*pos1) != (*pos2)) return false;
		pos1++;
		pos2++;
	}
	return true;
}