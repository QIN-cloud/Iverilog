#ifndef SMT_GENERATOR_H
#define SMT_GENERATOR_H
#include "Module.h"
#include "PDesign.h"
#include  "testpath.h"
#include <unordered_map>
#include <list>


#define NULLTIME 0

class InstanModule;
class SmtGenerator;

struct SmtDefine{
    enum VarPortType{INPUT, OUTPUT, INOUT, NOTPORT};
    enum UpdateType{TIMEUPDATE, SPACEUPDATE};
    string name;
    pair<unsigned, unsigned> bitselect;
    pair<unsigned, unsigned> state;
    pair<unsigned, unsigned> laststate;
    VarPortType type;
    InstanModule* location;
    list<SmtDefine*> linkport;
    size_t getWidth();
    string getName();
    string getLastName();
    void initial(RefVar* var, ostream& out, InstanModule* im);
    void update(ostream& out, UpdateType udtype, unsigned udtime, SmtDefine* port);
};

class InstanModule{
public:
    InstanModule(Module* md, ostream& out, InstanModule* lastlevel, string type);
    ~InstanModule();
    void generateSmt(ostream& out);
public:
    string levelname_;
    string type_;
    Module* module_;
    list<InstanModule*> nextlevel;
    unordered_map<string, SmtDefine*> symbol_;
};

class SmtGenerator{
public:
    SmtGenerator();
    ~SmtGenerator();
    void generateSmt(Module* md, ostream& out, Design* design);
    InstanModule* buildInstanModule(Module* topmodule, ostream& out, Design* design, InstanModule* lastlevel, string type);
    inline void setModule(map<perm_string, Module*>& pform_modules){module_ = pform_modules;}
private:    
    InstanModule* top_;
    map<perm_string, Module*> module_;
};

extern void declareConstBitVec(ostream& out, string name, unsigned width);
extern void assertSingelStatement(ostream& out, string left, string right, string op);
extern void assertStatement(ostream& out, list<pair<string, bool> >& condit, ostringstream& expr);
extern void assertConditStatement(ostream& out, ostringstream& expr, int width, string name);
extern void assertEqual(ostringstream& out, ostringstream& expr1, int width1, ostringstream& expr2, int width2, string op);
#endif