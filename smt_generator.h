#ifndef SMT_GENERATOR_H
#define SMT_GENERATOR_H
#include "Module.h"
#include "AnalysisControler.h"
#include "idp.h"
#include <unordered_map>
#include <list>
#include <vector>

#define NULLTIME 0
#define BASE_WIDTH 16

#define SMT_INT 0
#define SMT_BOOL -1
#define SMT_NULL -2
#define SMT_WRONG -3
#define SMT_MAX 64

class InstanModule;
class SmtGenerator;
class PGate;
class NetDesign;
class NetSymbol;
class NetInstan;
class BitSet;

static unordered_map<char, const char*> SMT_Vec_Add = {{'+', "bvadd"}, {'-', "bvsub"}};
static unordered_map<char, const char*> SMT_Vec_Div = {{'/', "bvudiv"}, {'%', "bvurem"}};
static unordered_map<char, const char*> SMT_Vec_Bits = {{'^', "bvxor"}, {'|', "bvor"}, {'&', "bvand"}, {'O', "bvnor"}, {'A', "bvnand"}, {'X', "bvxor"}};
static unordered_map<char, const char*> SMT_Vec_Comp = {{'>', "bvugt"}, {'<', "bvult"}, {'L', "bvule"}, {'G', "bvuge"}, {'e', "="}, {'n', "distinct"}};
static unordered_map<char, const char*> SMT_Int_Comp = {{'>', ">"}, {'<', "<"}, {'L', "<="}, {'G', ">="}, {'e', "="}, {'n', "distinct"}};
static unordered_map<char, const char*> SMT_Bool_Logic = {{'a', "and"}, {'o', "or"}};
static unordered_map<char, const char*> SMT_Vec_Mult = {{'*', "bvmul"}};

static const u_int16_t base_num[16] = {0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff, 0x1ff, 0x3ff, 0x7ff,0xfff, 0x1fff, 0x3fff, 0x7fff, 0xffff};
static const u_int16_t full_num_except_bit[16] = {0xfffe, 0xfffd, 0xfffb, 0xfff7, 0xffef, 0xffdf, 0xffbf, 0xff7f, 0xfeff, 0xfdff, 0xfbff, 0xf7ff,0xefff, 0xdfff, 0xbfff, 0x7fff};
static const u_int16_t low_fill_zero_bit[16] = {0xfffe, 0xfffb, 0xfff8, 0xfff0, 0xffe0, 0xffb0, 0xff80, 0xff00, 0xfe00, 0xfb00, 0xf800, 0xf000, 0xe000, 0xb000, 0x8000, 0x0000};

struct BitSet{
    BitSet(int base, unsigned width);
    vector<u_int16_t> signal_bit;
    int base;
    unsigned width;
    void dump();
    void initial();
    void full_initial();
    void set(int left, int right);
    void set(u_int16_t& n, unsigned left, unsigned right);
    bool need_new_state(int left, int right);
    bool need_new_state(int i, unsigned left, unsigned right);
    list<pair<int, int> > un_smt_assert_part();
};

struct SmtDefine{
    enum UpdateType{TIMEUPDATE, SPACEUPDATE, LINKUPDATE, TOPINPUT};
    pair<unsigned, unsigned> state;
    pair<unsigned, unsigned> laststate;
    NetSymbol* symbol;
    BitSet* bit_vec;
    size_t getWidth();
    string getName();
    string getLastName();
    void initial(ostream& out, NetSymbol* symbol);
    void update(ostream &out, UpdateType udtype, unsigned udtime, SmtDefine* port, int msb, int lsb);
};

class InstanModule{
public:
    InstanModule(InstanModule* prelevel, NetInstan* instan, ostream& out);
    ~InstanModule();
    void update(ostream& out, unsigned time);
    void startSynchroLogic(ostream& out, unordered_map<SmtDefine*, bool>& change, unsigned& tempid);
    void startCombineLogic(ostream& out, unsigned& tempid);
    void getValueForInput(ostream& out, unsigned time, SmtGenerator* generator);
    void updatePort(ostream& out, PExpr* port1, PExpr* port2, bool input);
    void initialPort(ostream& out, PExpr* port1, PExpr* port2);
    void initialOutput(ostream& out);
public:
    NetInstan* instan;
    unordered_map<string, SmtDefine*> define;
    list<InstanModule*> nextlevel;
    InstanModule* lastlevel;
};

class SmtGenerator{
public:
    SmtGenerator(NetDesign* design);
    ~SmtGenerator();
    void generateSmt(ostream& out);
    void startSynchroLogic(ostream& out, unsigned& tempid);
    void startCombineLogic(ostream& out, unsigned& tempid);
    void getValueForInput(ostream& out, unsigned time);
    void updateInput(ostream& out, unsigned time);
    void initialOutput(ostream& out);
    void assertForClk(ostream& out);
    void assertForRst(ostream& out);
    void declareClk(ostream& out, unsigned time);
    inline void set_design(NetDesign* design){this->design = design;}
public:
    NetDesign* design;
    InstanModule* root;
    const char* clk;
    const char* rst;
    unsigned time;
};

extern void declareConstBool(ostream& out, string name);
extern void declareConstBitVec(ostream& out, string name, unsigned width);
extern void assertSingelStatement(ostream& out, string left, string right, string op);
extern void assertStatement(ostream& out, list<pair<string, bool> >& condit, ostringstream& expr);
extern void assertConditStatement(ostream& out, ostringstream& expr, int width, string name);
extern void assertEqual(ostringstream& out, ostringstream& expr1, int width1, ostringstream& expr2, int width2, string op);
extern void assertInitial(ostream& out, SmtDefine* var);
extern void assertVarState(ostream& out, int msb, int lsb, SmtDefine* var);
extern void bv_to_int(ostringstream& expr, ostringstream& target);
extern void int_to_bv(ostringstream& expr, unsigned width, ostringstream& target);
extern void bv_to_bv(ostringstream& expr, unsigned width, unsigned finalwidth, ostringstream& target);
extern void bv_int_bv(ostringstream& expr, unsigned width, ostringstream& target);
extern void bool_to_bv(ostringstream& expr, ostringstream& target);
extern void extract(ostringstream& expr, int msi, int lsi, ostringstream& target);
extern void bv_or_int_to_bool(ostringstream& expr, unsigned width, ostringstream& target);
extern void bv_or_bool_to_int(ostringstream& expr, unsigned width, ostringstream& target);
extern void bv_compare_zero(ostringstream& expr, string op, unsigned width, ostringstream& target);
extern void int_compare_zero(ostringstream& expr, string op, ostringstream& target);

#endif