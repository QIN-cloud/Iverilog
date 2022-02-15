#include "vcdvar.h"
#include "verinum.h"

vcd_var::vcd_var()
{
	toggle01_stat.clear();
	toggle10_stat.clear();
}

void vcd_var::toggle01()
{
	for(unsigned idx = 0; idx < width; ++idx)
	{
		if(toggle01_stat[idx])
			continue;
		if(((pre_val_[idx] == verinum::V0) || (pre_val_[idx] == verinum::Vx)) && (cur_val_[idx] == verinum::V1))
			toggle01_stat[idx] = true;
	}
}

void vcd_var::toggle10()
{
	for(unsigned idx = 0; idx < width; ++idx)
	{
		if(toggle10_stat[idx])
			continue;
		if(((pre_val_[idx] == verinum::V1) || (pre_val_[idx] == verinum::Vx)) && (cur_val_[idx] == verinum::V0))
			toggle10_stat[idx] = true;
	}
}

void vcd_var::initial_toggle_stat()
{
	for(unsigned idx = 0; idx < width; ++idx)
	{
		toggle01_stat[idx] = false;
		toggle10_stat[idx] = false;
	}
}

void vcd_var::toggle_report(ostream& report)
{
	report << "Signal : " << name_ << " Toggle 0 -> 1 bits:\n";
	report << "---------------------------------\n";
	for(unsigned idx = 0; idx < width; ++idx)
	{
		if(toggle01_stat[idx])
		{
			if(little_endia)
				report << idx - lsb << endl;
			else
				report << lsb - idx << endl;
		}
	}
	report << "---------------------------------\n";

	report << "Signal : " << name_ << " Toggle 1 -> 0 bits:\n";
	report << "---------------------------------\n";
	for(idx = 0; idx < width; ++idx)
	{
		if(toggle10_stat[idx])
		{
			if(little_endia)
				report << idx - lsb << endl;
			else
				report << lsb - idx << endl;
		}
	}
	report << "---------------------------------\n";
}

fsm_stat::fsm_stat()
{
	covered_state.clear();
	covered_trans.clear();
}

void fsm_stat::add_covered_state(verinum state)
{
	bool found = false;
	for(vector<verinum>::iterator vpos = covered_state.begin(); vpos != covered_state.end(); ++vpos)
	{
		if((*vpos) == state)
		{
			found = true;
			break;
		}
	}
	if(!found)
		covered_state.push_back(state);
}

void fsm_stat::add_covered_tran(covered_tran ct)
{
	bool found = false;
	for(vector<covered_tran>::iterator vpos = covered_trans.begin(); vpos != covered_trans.end(); ++vpos)
	{
		if(((*vpos).from == ct.from) && ((*vpos).to == ct.to))
		{
			found = true;
			break;
		}
	}
	if(!found)
		covered_trans.push_back(ct);
}

void fsm_stat::report_fsm(ostream& report)
{
	report << "Covered States : " << endl; 
	report << "----------------------------------------" << endl;
	for(vector<verinum>::iterator spos = covered_state.begin(); spos != covered_state.end(); ++spos)
	{
		report << *spos << endl;
	}
	
	report << "----------------------------------------" << endl;
	report << "Covered State Transitions: " << endl; 
	report << "----------------------------------------" << endl;
	for(vector<covered_tran>::iterator tpos = covered_trans.begin(); tpos != covered_trans.end(); ++tpos)
	{
		report << (*tpos).from << "->" << (*tpos).to << endl;
	}
	report << "----------------------------------------" << endl;
}

stmt_stat::stmt_stat()
{
	covered_stmts.clear();
}

void stmt_stat::add_stmt(set<unsigned, less<unsigned> > stmt)
{
		for(set<unsigned, less<unsigned> >::iterator spos = stmt.begin(); spos != stmt.end(); ++spos)
		{
			covered_stmts.insert(*spos);
		}
}

void stmt_stat::report_stmt(ostream& report)
{
	report << "Covered Statements:" << endl;
	report << "----------------------------------------" << endl;
	for(set<unsigned, less<unsigned> >::iterator mpos = covered_stmts.begin(); mpos != covered_stmts.end(); ++mpos)
	{
		report << *mpos << endl;
	}
	report << "----------------------------------------" << endl;
}

path_stat::path_stat()
{
	covered_paths.clear();
}

void path_stat::add_path(set<unsigned, less<unsigned> > path)
{
	bool found = false;
	for(vector<set<unsigned, less<unsigned> > >::iterator vpos = covered_paths.begin(); vpos != covered_paths.end(); ++vpos)
	{
		if((*vpos).size() == path.size())
		{
			if(equal((*vpos), path))
			{
				found = true;
				break;
			}
		}
	}
	if(!found)
		covered_paths.push_back(path);
}

bool path_stat::equal(set<unsigned, less<unsigned> > p1, set<unsigned, less<unsigned> > p2)
{
	set<unsigned, less<unsigned> >::iterator ppos1 = p1.begin();
	set<unsigned, less<unsigned> >::iterator ppos2 = p2.begin();
	while((ppos1 != p1.end()) && (ppos2 != p2.end()))
	{
		if((*ppos1) == (*ppos2))
		{
			++ppos1;
			++ppos2;
		}
		else
		{
			return false;
		}
	}
	return true;
}

void path_stat::report_path(ostream& report)
{
	report << "Covered Paths:" <<endl;
	report << "----------------------------------------" << endl;
	for(vector<set<unsigned, less<unsigned> > >::iterator mpos = covered_paths.begin(); mpos != covered_paths.end(); ++mpos)
	{
		for(set<unsigned, less<unsigned> >::iterator spos = (*mpos).begin(); spos != (*mpos).end(); ++spos)
		{
			report << *spos << ", ";
		}
		report << endl;
		report << "----------------------------------------" << endl;
	}
	report << "----------------------------------------" << endl;
}