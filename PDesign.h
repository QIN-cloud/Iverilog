#ifndef __Design_H
#define __Design_H

#include <map>
#include <string>
#include <queue>
#include <list>
#include <stack>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <unordered_map>
#include "Module.h"
#include "svector.h"
#include "StringHeap.h"
#include "PGate.h"
#include "vcdvar.h"
#include "verinum.h"
#include "netlist.h"
using namespace std;

class Module;
class ModuleNode;
class PUdp;
class Design;
class PGate;
class VcdVar;
class VcdScope;
class verinum;
class FinalRep;

/* 
*  Each Module will be represented by a node, the module information and 
*  the module instantiated in this module are stored in this node.
*  The ModuleTree is used to represent the hierarchical relationship between 
*  modules. Since the module in the vcd file is saved with an instantiation name. 
*  By accessing the ModuleTree, the type of the module can be obtained during 
*  the process of vcd file parsing.
*/
class ModuleTreeNode{
public:
      ModuleTreeNode(Module* module): module_(module){};
      Module* get_module(){return module_;}
      
      //Module instantiations.
      //For example, there is a module instantiation like "xram xram1(...)",
      //establish a mapping relationship between instatiated xram1 and xram.
      map<perm_string, ModuleTreeNode*> next_;

private:
      Module* module_;
};


/* 
*  This class is used to control the process of coverage analysis. Including follows:
*  The extraction of variables in vcd file.
*  Record and update variable values during the process of parsing the vcd file.
*  Select the corresponding module process for the condition variable value changing.
*  Replay for cfgs and save to a report.
*/
class PDesign {
public:
    PDesign();
    ~PDesign();
    
    void set_modules(map<perm_string, Module*> modules);
    void set_udps(map<perm_string, PUdp*>& udps);
    void set_design(Design* design);

    map<perm_string, Module*> get_modules();
    map<perm_string, PUdp*> get_udps();
    
    svector<ModuleNode*>* build_nodes(PDesign& de, std::string mainmodule);
	svector<ModuleNode*>* build_nodes(PDesign& de);
    
	void build_nodes();

    void dump(ostream&o);

    /*Start Coverage Analysis.*/
    void function_cover(perm_string top_module, perm_string select_module, const char* fsm_selects, const char* vcd_file, ostream& target_file,
    bool toggle, bool fsm, bool statement, bool path, bool branch, bool combine);

private:

    /*Build module node tree and conditional expressions table.*/
    void build_before_cover(const char* fsm_selects);

    /*Split the selected variables using in fsm analysis.*/
    void fsm_var_parse(const char* fsm_selects);

    /*It's possible that mutiple module types have the same module name of instantiation,
    for example "moduleA test(....);  moduleB test(....)" , so we need to store the 
    module types for every module name by building a module_tree.*/
    ModuleTreeNode* build_module_tree(Module* module_);

    /*Parse the definition lines before the "$enddefinitions".*/
    void vcd_parse_def( FILE* vcd );

    /*Skip the useless lines in definition.*/
    void vcd_parse_def_ignore( FILE* vcd );

    /*Parse a variable defined line such as "$var reg 8 ! a [7:0] $end".
    Format as $var port-type symbol var-name [msb:lsb] $end.*/
    void vcd_parse_def_var( FILE* vcd );

    /*Parse a instantiated module defined line such as "$scope module top $end".
    Format as $scope scope-type instantiated-name $end.*/
    void vcd_parse_def_scope( FILE* vcd );

    /*Parse the vaule change lines after the "$enddefinitions".*/
    void vcd_parse_sim(FILE* vcd);

    /*Skip the useless lines in value changing.*/
    void vcd_parse_sim_ignore( FILE* vcd );

    /*Parse a value record line of mutiple bits such as "b11101101 &".
    Format as changed-value symbol.*/
    void vcd_parse_sim_vector( FILE* vcd, char* value );

    /*Parse a value record line of single bit such as "1!".
    Format as changed-value link to the symbol.*/
    void set_symbol_char( char* sym, char value );

    /*Update variable values when meeting the "#Time" lines.*/
    void update_vcd_vars();
    
    /*Collect the variable bit changed at current time.*/
    void toggle_timestep();
    
    /*Collect the states and state transitions for all variables.*/
    void fsm_timestep();
    
    /*When we get all values of variables at this time, we select the cfgs which should
    be replayed and collect result for statement, path, branch and combine coverage. */
    void sim_timestep();

    /*Evaluate the sensitive list and decide whether to replay the process.*/
    bool execute_process(Cfg_Node* node, VcdScope* vs, Cfg* cfg);

    /*Replay a Cfg and get a list of linenos where the line is executed.*/
    void cfg_replay(Cfg* cfg, VcdScope* scope, set<string>& defs);

    /*Evaluate the conditional expression, then select the next node by evaluating value.*/
    int eval_cond_expr(Cfg_Node* node, VcdScope* vs, Cfg* cfg, bool combine);

    /*After replaying a cfg, we get a list of linenos named paths, this will be the result 
    of statement, path and branch coverage.*/
    void add_path(Module* md, Cfg* cfg, const set<unsigned>& path);

    /*Dump the final coverage analysis report.*/
    void report_coverage(ostream& o);
    
    /*Dump the module and module gates structure.*/
    void dump_module_tree(ostream& o);

    /*Dump the variables and symbols prasing in vcd file.*/
    void dump_symbol_vars(ostream& o);

    /*Dump the cfgs and instantiation modules for every module type.*/
    void dump_module_vars(ostream& o);

    /*Dump the value of all variables at current moment.*/
    void dump_everytime_vars(ostream& o);

    /*Dump the dates before starting the simulation replay process, excluding 
    the conditon expressions and their cfgs in every modules, vcd symbols and 
    the variables they matched,module instantiate informations.*/
    void dump_def_information();

private:
    Design* des;
    map<perm_string, Module*> modules_;
    map<perm_string, PUdp*> udps_; 
    map<perm_string, ModuleTreeNode*> mt_nodes;    //Each module type only has one ModuleTreeNode.
    ofstream evaluate_out;                         //Record the value of condit expression at every time.                     
    ofstream control_out;                            //Record the value changed.

/*Function Coverage Analysis Signs.*/
private:
    bool toggle_;
    bool fsm_;
    bool statement_;
    bool path_;
    bool branch_;
    bool combine_; 
    Module* top_;
    map<Module*, bool> select_mod;

/*Variables used in the part of definitions parsing.*/
private:
    bool                   found_in_vcd;                                        //Flag whether the module exists in vcd file.
    int                    scope_count;                                         //Scope level in definition parsing.
    VcdScope*              current_scope;                                       //Current module instantiate in the vcd parsing process.
    map<string, bool>      fsm_names;                                           //The variable names selected in fsm cover.
    stack<ModuleTreeNode*> enter_scopes;                                        //Overhead ModuleTreeNodes that are searched in dfs. 

/*Variables used in the part of value changes parsing.*/
private:    
    FinalRep* report;                                                           //Coverage report.
    int cur_sim_time;                                                           //Current time of the simulation replay process.
    map<VcdVar*, bool> fsm_vars;                                                //Record the vcd_var in fsm variables.
    map<string,verinum> variety_symbols;                                        //Recording of symbols having value changing at a moment.
    unordered_map<string, vector<pair<string, VcdScope*> > >  symbol_vars;      //Form as {symbol, vector {var name, instantiated module} }.
};
#endif