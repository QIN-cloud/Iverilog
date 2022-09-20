#include "smt_generator.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include "PGate.h"

void declareConstBool(ostream& out, string name)
{
    out << "(declare-const " << name << " Bool" << ")" << endl;
}

void declareConstBitVec(ostream &out, string name, unsigned width)
{
    out << "(declare-const " << name << " (_ BitVec " << width << "))" << endl;
}

void assertSingelStatement(ostream& out, string left, string right, string op)
{
    out << "(assert (" << op << " " << left << " " << right << "))" << endl;
}

void assertStatement(ostream& out, list<pair<string, bool> >& condit, ostringstream& expr)
{
    out << "(assert ";
    if(condit.empty()) {
        out << expr.str();
    } else if(condit.size() == 1) {
        out << "(=> ";
        if(condit.front().second)
            out << " " << condit.front().first;
        else
            out << " " << "(not " << condit.front().first << ")";
        out << " " << expr.str() << ")";
    } else {
        out << "(=> (and";
        for(pair<string, bool> cond : condit) {
            if(cond.second)
                out << " " << cond.first;
            else
                out << " " << "(not " << cond.first << ")";
        }
        out << ") " << expr.str() << " )";
    }
    out << ")" << endl;
}

void assertConditStatement(ostream& out, ostringstream& expr, int width, string name)
{
    ostringstream b_expr;
    if(width != SMT_BOOL)
        bv_or_int_to_bool(expr, width, b_expr);
    else
        b_expr << expr.str();
    assertSingelStatement(out, name, b_expr.str(), "=");
}

void assertEqual(ostringstream& out, ostringstream& expr1, int width1, ostringstream& expr2, int width2, string op)
{
    ostringstream i_expr1, i_expr2;
    bv_or_bool_to_int(expr1, width1, i_expr1);
    bv_or_bool_to_int(expr2, width2, i_expr2);
    out << "(" << op << " " << i_expr1.str() << " " << i_expr2.str() << ")";
}

void assertInitial(ostream& out, SmtDefine* var)
{
    string value(var->getWidth(), '0');
    out << "(assert (= " << var->getName() << " #b" << value << ") )" << endl;
}

void assertVarState(ostream& out, int msb, int lsb, SmtDefine* var)
{
    ostringstream s1, s2, o1, o2;
    s1 << var->getName();
    s2 << var->getLastName();
    extract(s1, msb, lsb, o1);
    extract(s2, msb, lsb, o2);
    out << "(assert (= " << o1.str() << " " << o2.str() << ") )" << endl;
}

string SmtDefine::getName()
{
    ostringstream ss;
    ss << symbol->get_idp()->get_level_name() << "." << symbol->get_name();
    ss << "_" << state.first << "_" << state.second;
    return ss.str();
}

string SmtDefine::getLastName()
{
    ostringstream ss;
    ss << symbol->get_idp()->get_level_name() << "." << symbol->get_name();
    ss << "_" << laststate.first << "_" << laststate.second;
    return ss.str();
}

void SmtDefine::initial(ostream& out, NetSymbol* symbol)
{
    this->symbol = symbol;
    state.first = 0;
    state.second = 0;
    laststate = state;
    declareConstBitVec(out, getName(), getWidth());
    bit_vec = new BitSet(symbol->get_lsb(), symbol->get_width());
}

size_t SmtDefine::getWidth()
{
    return symbol->get_width();
}

void SmtDefine::update(ostream &out, UpdateType udtype, unsigned udtime, SmtDefine* port, int msb, int lsb)
{
    if(udtype == TIMEUPDATE) {
        laststate = state;
        string name1 = getName();
        state.first = udtime;
        state.second = 0;
        string name2 = getName();
        declareConstBitVec(out, name2, getWidth());
        assertSingelStatement(out, name1, name2, "=");
        bit_vec->full_initial();
    } else if(udtype == SPACEUPDATE) {
        if(bit_vec->need_new_state(msb, lsb)) {
            list<pair<int, int> > update_bits = bit_vec->un_smt_assert_part();
            for(auto bit_select : update_bits) {
                assertVarState(out, bit_select.second, bit_select.first, this);
            }
            laststate = state;
            state.second += 1;
            declareConstBitVec(out, getName(), getWidth());
            bit_vec->initial();
        }
        bit_vec->set(msb, lsb);
    } else if(udtype == LINKUPDATE){
        laststate = state;
        state.second += 1;
        declareConstBitVec(out, getName(), getWidth());
        bit_vec->full_initial();
    } else {
        laststate = state;
        state.second += 1;
        declareConstBitVec(out, getName(), getWidth());
        bit_vec->full_initial();
    }
}

SmtGenerator::SmtGenerator(NetDesign* design)
{
    this->design = design;
    root = nullptr;
}

SmtGenerator::~SmtGenerator()
{

}

InstanModule::InstanModule(InstanModule* lastlevel, NetInstan* instan, ostream& out)
{
    this->instan = instan;
    this->lastlevel = lastlevel;
    unordered_map<string, NetSymbol*> symbols = instan->get_define();
    for(auto pos : symbols) {
        SmtDefine* var = new SmtDefine;
        var->initial(out, pos.second);
        define[pos.first] = var;
        assertInitial(out, var);
    }
    map<string, NetInstan*>::iterator npos = instan->next_level.begin();
    for(; npos != instan->next_level.end(); npos++) {
        InstanModule* next = new InstanModule(this, npos->second, out);
        nextlevel.push_back(next);
    }
}

InstanModule::~InstanModule()
{

}

void InstanModule::update(ostream& out, unsigned time)
{
    unordered_map<string, SmtDefine*>::iterator pos = define.begin();
    for(; pos != define.end(); pos++)
    {
        pos->second->update(out, SmtDefine::UpdateType::TIMEUPDATE, time, nullptr, 0, 0);
        pos->second->bit_vec->full_initial();
    }
    for(InstanModule* instan : nextlevel)
    {
        instan->update(out, time);
    }
}

void InstanModule::startSynchroLogic(ostream& out, unordered_map<SmtDefine*, bool>& change, unsigned& tempid)
{
    out << "\n;" << "Update inputs for " << instan->get_level_name()  << endl;
    if(!instan->input_port.empty())
    {
        for(pair<PExpr*, PExpr*> in : instan->input_port)
        {
            updatePort(out, in.first, in.second, true);
        }
    }
    out << "\n; " << instan->get_level_name() << " " << "synchrous generate" << endl;
    for(size_t i = 0; i < instan->get_module()->get_cfg()->cfgs->count(); i++)
    {
        Cfg* cfg = (*(instan->get_module()->get_cfg()->cfgs))[i];
        if(cfg->sync)
        {
            out << ";" << instan->get_module()->get_file() << " : " << cfg->lineno << endl;
            cfg->proc->dump_design_smt(out, this, change, tempid);
        }
    }
    for(InstanModule* instan : nextlevel)
    {
        instan->startSynchroLogic(out, change, tempid);
    }
}

void InstanModule::startCombineLogic(ostream& out, unsigned& tempid)
{
    for(InstanModule* instan : nextlevel)
    {
        instan->startCombineLogic(out, tempid);
    }
    out << "\n; " << instan->get_level_name() << " " << "assign generate" << endl;
    unordered_map<SmtDefine*, bool> change;
    for(PGAssign* ag : instan->get_module()->assigns_)
    {
        out << ";";
        ag->dump(out);
        ag->dump_design_smt(out, this);
    }
    out << "\n; " << instan->get_level_name() << " " << "combine generate" << endl;
    for(list<Cfg*> cfgset : instan->get_module()->cfg_list)
    {
        Cfg* cfg = nullptr;
        if(cfgset.size() == 1)
        {
            cfg = cfgset.front();
            if(!cfg->sync)
            {
                out << "\n; " << instan->get_module()->get_file() << " : " << cfg->lineno << endl;
                cfg->proc->dump_design_smt(out, this, change, tempid);
            }
        }
        if(cfg)
        {
            out << "\n; " << instan->get_level_name() << " " << "assign generate" << endl;
            for(PGAssign* ag : instan->get_module()->assigns_)
            {
                out << ";";
                ag->dump(out);
                ag->dump_design_smt(out, this);
            }
        }
    }
    out << "\n;" << "Update outputs for " << instan->get_level_name() << endl;
    if(!instan->output_port.empty())
    {
        for(pair<PExpr*, PExpr*> ou : instan->output_port)
        {
            updatePort(out, ou.second, ou.first, false);
        }
    }
}

void InstanModule::getValueForInput(ostream& out, unsigned time, SmtGenerator* generator)
{
    unordered_map<string, SmtDefine*>::iterator pos = define.begin();
    for(; pos != define.end(); pos++)
    {
        for(unsigned i = 0; i < time; i++)
        {
            if(i && (i % 2 ) == 0 && pos->first != generator->clk)
                out << "(get-value(" << instan->get_level_name() << "." << pos->first << "_" << i << "_0))" << endl;
        }
    }
}

void InstanModule::updatePort(ostream& out, PExpr* l_expr, PExpr* r_expr, bool input)
{
    assert(lastlevel);
    ostringstream l_str;
    ostringstream r_str;
    InstanModule* instan1 = input ? lastlevel : this;
    InstanModule* instan2 = input ? this : lastlevel;
    int l_width, r_width;
    l_width = l_expr->dump_design_smt(out, l_str, instan1);
    if(input)
    {
        set<string> change = r_expr->get_var_names();
        for(string s : change)
        {
            assert(define.find(s) != define.end());
            SmtDefine* var = define[s];
            var->update(out, SmtDefine::UpdateType::LINKUPDATE, 0, nullptr, 0, 0);
        }
    }
    else
    {
        set<string> change = r_expr->get_var_names();
        for(string s : change)
        {
            assert(lastlevel->define.find(s) != lastlevel->define.end());
            SmtDefine* var = lastlevel->define[s];
            if(var->state.second == 0) {
                var->update(out, SmtDefine::UpdateType::LINKUPDATE, 0, nullptr, 0, 0);
            }
        }
    }

    r_width = r_expr->dump_design_smt(out, r_str, instan2);
    assert(l_width > SMT_INT && r_width > SMT_INT);

    ostringstream l_equal;
    if(l_width != r_width)
        bv_to_bv(l_str, l_width, r_width, l_equal);
    else
        l_equal << l_str.str();

    out << "(assert " << "(= " << r_str.str() << " " << l_equal.str() << ") )" << endl;
}

void InstanModule::initialPort(ostream& out, PExpr* l_expr, PExpr* r_expr)
{
    assert(lastlevel);
    ostringstream l_str;
    ostringstream r_str;
    int l_width = l_expr->dump_design_smt(out, l_str, lastlevel);
    int r_width = r_expr->dump_design_smt(out, r_str, this);
    ostringstream l_equal;
    if(l_width != r_width)
        bv_to_bv(l_str, l_width, r_width, l_equal);
    else
        l_equal << l_str.str();
    out << "(assert (= " << r_str.str() << " " <<  l_equal.str() << ") )" << endl;
    if(lastlevel->lastlevel == nullptr)
    {
        out << "(assert (= " << l_equal.str() << " #b";
        for(size_t i = 0; i < r_width; i++)
        {
            out << "0";
        }
        out << "))" << endl;
    }
}

void InstanModule::initialOutput(ostream& out)
{
    if(!nextlevel.empty())
    {
        for(InstanModule* instan : nextlevel)
        {
            instan->initialOutput(out);
        }
    }
    if(!instan->output_port.empty())
    {
        for(auto ou : instan->output_port)
        {
            initialPort(out, ou.first, ou.second);
        }
    }
}

void SmtGenerator::generateSmt(ostream& out)
{
    cout << "Build Instance for Module..." << endl;
    root = new InstanModule(nullptr, design->get_root(), out);
    unsigned tempid = 0;
    for(unsigned i = 0; i < time; i++) {
        cout << "Smt For #Time" << i << "" << endl;
        out << "\n;#Time" << i << "" << endl;
        if(i){
            root->update(out, i);
        }
        declareClk(out, i);
        updateInput(out, i);
        startSynchroLogic(out, tempid);
        startCombineLogic(out, tempid);
    }
    if(clk) assertForClk(out);
    if(rst) assertForRst(out);
    out << "(check-sat)" << endl;
    getValueForInput(out, time);
}

void SmtGenerator::assertForClk(std::ostream &out) {
    bool flag = false;
    for(unsigned i = 0; i < time; i++) {
        out << "(assert (= " << root->instan->get_level_name() << "." << clk << "_" << i << "_1 " << "#b" << flag << ") )" << endl;
        flag = ! flag;
    }
}

void SmtGenerator::assertForRst(std::ostream &out) {
    out << "(assert (= " << root->instan->get_level_name() << "." << rst << "_" << 1 << "_1 " << "#b0" << " ) )" << endl;
    out << "(assert (= " << root->instan->get_level_name() << "." << rst << "_" << 3 << "_1 " << "#b1" << " ) )" << endl;
}

void SmtGenerator::declareClk(std::ostream &out, unsigned int time) {
    root->define[clk]->update(out, SmtDefine::UpdateType::TOPINPUT, 0, 0, 0, 0);
}

void SmtGenerator::startSynchroLogic(ostream& out, unsigned& tempid)
{
    unordered_map<SmtDefine*, bool> change;
    root->startSynchroLogic(out, change, tempid);
}

void SmtGenerator::startCombineLogic(ostream& out, unsigned& tempid)
{
    root->startCombineLogic(out, tempid);
}

void SmtGenerator::getValueForInput(ostream& out, unsigned time)
{
    root->getValueForInput(out, time, this);
}

void SmtGenerator::updateInput(ostream& out, unsigned time)
{
    out << ";" << "Update the Top module inputs" << endl;
    for(auto it : root->define)
    {
        if(it.first != clk) {
            it.second->update(out, SmtDefine::UpdateType::TOPINPUT, 0, 0, 0, 0);
            it.second->bit_vec->full_initial();
            if(time % 2 == 0)
                out << "(assert (= " << it.second->getName() << " " << it.second->getLastName() << ") )" << endl;
        }
    }
}

void SmtGenerator::initialOutput(ostream& out)
{
    root->initialOutput(out);
}

BitSet::BitSet(int base, unsigned int width) {
    this->base = base;
    this->width = width;
    unsigned size = width / BASE_WIDTH;
    if(width % BASE_WIDTH) size++;
    signal_bit = vector<uint16_t>(size, UINT16_MAX);
}

void BitSet::dump() {
    for(size_t i = 0; i < signal_bit.size(); i++) {
        cout << " [" << i << "] : ";
        for(size_t j = 0; j < BASE_WIDTH; j++) {
            cout << ((signal_bit[i] | full_num_except_bit[j]) == UINT16_MAX);
        }
    }
    cout << endl;
}

void BitSet::initial() {
    for(size_t i = 0; i < signal_bit.size(); i++) {
        signal_bit[i] = 0;
    }
}

void BitSet::full_initial() {
    for(size_t i = 0; i < signal_bit.size(); i++) {
        signal_bit[i] = UINT16_MAX;
    }
}

void BitSet::set(int left, int right) {
    assert(left >= right);
    unsigned l = left - base;
    unsigned r = right - base;
    for(size_t i = 0; i < signal_bit.size(); i++) {
        unsigned cur_l = (i + 1) * BASE_WIDTH - 1;
        unsigned cur_r = i * BASE_WIDTH;
        if(cur_r > l) break;
        if(cur_l < r) continue;
        unsigned select_l = l > cur_l ? BASE_WIDTH - 1 : l % BASE_WIDTH;
        unsigned select_r = r < cur_r ? 0 : r % BASE_WIDTH;
        set(signal_bit[i], select_l, select_r);
    }
}

void BitSet::set(u_int16_t& n, unsigned int left, unsigned int right) {
    assert(left >= right && left < (base + width));
    unsigned len = left - right;
    u_int16_t shift_n = base_num[len] << right;
    n |= shift_n;
}

bool BitSet::need_new_state(int left, int right) {
    assert(left >= right);
    unsigned l = left - base;
    unsigned r = right - base;
    bool result = false;
    for(size_t i = 0; i < signal_bit.size(); i++) {
        unsigned cur_l = (i + 1) * BASE_WIDTH - 1;
        unsigned cur_r = i * BASE_WIDTH;
        if(cur_r > l) break;
        if(cur_l < r) continue;
        unsigned select_l = l > cur_l ? BASE_WIDTH - 1 : l % BASE_WIDTH;
        unsigned select_r = r < cur_r ? 0 : r % BASE_WIDTH;
        result |= need_new_state(i, select_l, select_r);
    }
    return result;
}

bool BitSet::need_new_state(int i, unsigned int left, unsigned int right) {
    assert(left >= right && left < BASE_WIDTH);
    if(signal_bit[i] == UINT16_MAX) return true;
    unsigned n = left - right;
    u_int16_t select_n = (signal_bit[i] >> right) & base_num[n] ;
    return (select_n != 0);
}

list<pair<int, int> > BitSet::un_smt_assert_part() {
    list<pair<int, int> > result;
    unsigned idx = 0;
    unsigned record = 0;
    bool flag = false;
    size_t i = 0;
    size_t j = 0;
    while(idx < width) {
        if(j == BASE_WIDTH) {
            j = 0;
            i++;
        }
        if(!signal_bit[i]) {
            if(!flag) {
                flag = true;
                record = idx;
            }
            idx = (idx + BASE_WIDTH) >= width ? width : idx + BASE_WIDTH;
            j = BASE_WIDTH;
        } else {
            if((signal_bit[i] | full_num_except_bit[j]) == UINT16_MAX) {
                if(flag) {
                    flag = false;
                    result.push_back(make_pair(record, idx - 1));
                }
            } else {
                if(!flag) {
                    flag = true;
                    record = idx;
                }
            }
            j++;
            idx++;
        }
    }
    if(flag) result.push_back(make_pair(record, idx -1));
    return result;
}
