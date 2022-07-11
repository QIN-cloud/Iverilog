#include "smt_generator.h"
#include <sstream>
#include <fstream>

void declareConstBitVec(ostream &out, string name, unsigned width)
{
    out << "(declare-const " << name << " (_ BitVec " << width << ") )\n";
}

void assertSingelStatement(ostream& out, string left, string right, string op)
{
    out << "(assert (" << op << " " << left << " " << right << ") )\n";
}

void assertStatement(ostream& out, list<pair<string, bool> >& condit, ostringstream& expr)
{
    out << "(assert ";
    if(condit.empty()) {
        out << expr.str();
    } else if(condit.size() == 1) {
        out << "(==> ";
        if(condit.front().second)
                out << " " << condit.front().first;
            else
                out << " " << "(not " << condit.front().first << ")";
        out << " " << expr.str() << ")"; 
    } else {
        out << "(==> (and";
        for(pair<string, bool> cond : condit) {
            if(cond.second)
                out << " " << cond.first;
            else
                out << " " << "(not " << cond.first << ")";
        }
        out << ") " << expr.str() << " )";
    }
    out << ") ";
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

string SmtDefine::getName()
{
    ostringstream ss;
    ss << location->levelname_ << "." << name;
    ss << "_" << state.first << "_" << state.second;
    return ss.str();
}

string SmtDefine::getLastName()
{
    ostringstream ss;
    ss << location->levelname_ << "." << name;
    ss << "_" << laststate.first << "_" << laststate.second;
    return ss.str();
}

void SmtDefine::initial(RefVar* var, ostream& out, InstanModule* im)
{  
    location = im;
    name = var->name;
    bitselect.first = var->msb;
    bitselect.second = var->lsb;
    state.first = 0;
    state.second = 0;
    laststate = state;
    type = var->ptype == "PINPUT" ? INPUT :
           var->ptype == "POUTPUT" ? OUTPUT :
           var->ptype == "PINOUT" ? INOUT : NOTPORT;
    cout << im->module_->pscope_name() << endl;
    declareConstBitVec(out, getName(), getWidth());
}

size_t SmtDefine::getWidth()
{
    return (bitselect.first - bitselect.second);
}

void SmtDefine::update(ostream &out, UpdateType udtype, unsigned udtime, SmtDefine* port)
{
    string name1 = getName();
    if (udtype == TIMEUPDATE) {
        state.first = udtime;
        state.second = 0;
    } else {
        laststate = state;
        state.second += 1;
    }
    string name2 = getName();
    declareConstBitVec(out, name2, getWidth());
    if(udtype == TIMEUPDATE)
        assertSingelStatement(out, name1, name2, "=");
    else {
        if(port)
            assertSingelStatement(out, name2, port->getName(), "=");
        for(SmtDefine* var : linkport) {
            update(out, udtype, udtime, this);
        }
    }
}

SmtGenerator::SmtGenerator()
{

}

SmtGenerator::~SmtGenerator()
{

}

InstanModule::InstanModule(Module* md, ostream& out, InstanModule* lastlevel, string type)
{
    module_ = md;
    type_ = type;
    if(lastlevel)
        levelname_ = lastlevel->levelname_ + "." + type_;
    else
        levelname_ = type;
    map<string, RefVar*>::iterator pos = md->vartab_.begin();
    for(; pos != md->vartab_.end(); pos++){
        SmtDefine* defvar = new SmtDefine;
        pos->second->dump(cout);
        defvar->initial(pos->second, out, this);
        symbol_[pos->first] = defvar;
    }
}

InstanModule::~InstanModule()
{

}

InstanModule* SmtGenerator::buildInstanModule(Module* md, ostream& out, Design* design, InstanModule* lastlevel, string type)
{
    InstanModule* im = new InstanModule(md, out, lastlevel, type);
        im->levelname_ = im->module_->pscope_name();
    for(PGate* gate : md->get_gates()) {
        if(gate->ty == PGate::MODULE) {
            InstanModule* nextim = buildInstanModule(module_[gate->get_type()], out, design, im, (gate->get_name().str()));
        }
    }
    return im;
}

void SmtGenerator::generateSmt(Module* topmodule, ostream& out, Design* design)
{
    map<perm_string, Module*>::iterator pos = module_.begin();
    for(; pos != module_.end(); pos++) {
        pos->second->build_vartab(design);
        pos->second->set_cfg_process();
    }
    top_ = buildInstanModule(topmodule, out, design, nullptr, topmodule->pscope_name().str());
}