#ifndef __Module_H
#define __Module_H

#include <list>
#include <map>
#include "svector.h"
#include "HName.h"
#include "named.h"
#include "LineInfo.h"
#include "PTask.h"
#include "slice.h"
#include <string>
#include "vcdvar.h"

class PDesign;
class PEvent;
class PExpr;
class PEIdent;
class PGate;
class PTask;
class PFunction;
class PWire;
class PProcess;
class ModuleNode;
class PData;
class Design;
class dd_var;

struct cond_time_expr;

typedef struct vcd_id{
	string name_;
	long lsb, msb;
}vcd_id;

typedef map<string, vcd_var> vcd_vars;

/*
* A module is a named container and scope. A module holds a bunch of
* semantic quantities such as wires and gates. The module is
* therefore the handle for grasping the described circuit.
*/

class Module : public LineInfo {
/* The module ports are in general a vector of port_t
objects. Each port has a name and an ordered list of
wires. The name is the means that the outside uses to
access the port, the wires are the internal connections to
the port. 
	*/
public:
	struct port_t {
		string name;
		svector<PEIdent*> expr;
	};
	
    public:
		explicit Module(const char*name);
		~Module();
		
		
		/* The module has parameters that are evaluated when the
		module is elaborated. During parsing, I put the parameters
		into this map. */
		struct param_expr_t {
			PExpr*expr;
			PExpr*msb;
			PExpr*lsb;
			bool signed_flag;
		};
		map<string,param_expr_t>parameters;
		map<string,param_expr_t>localparams;
		
		/* specparams are simpler then other params, in that they have
		no type information. They are merely constant
		expressions. */
		map<string,PExpr*>specparams;
		
		/* The module also has defparam assignments which don't create
		new parameters within the module, but may be used to set
		values within this module (when instantiated) or in other
		instantiated modules. */
		map<hname_t,PExpr*>defparms;
		
        /* Parameters may be overridden at instantiation time;
		the overrides do not contain explicit parameter names,
		but rather refer to parameters in the order they
		appear in the instantiated module.  Therefore a
		list of names in module-order is needed to pass from
		a parameter-index to its name. */
		list<string> param_names;
		
		/* This is an array of port descriptors, which is in turn a
		named array of PEident pointers. */
		svector<port_t*> ports;
		
		/* Keep a table of named events declared in the module. */
		map<string,PEvent*>events;
		
		/* Keep a table of datum variables declared in the module. */
		map<hname_t,PData*>datum;

		map<string,PExpr*> attributes;
		
		/* These are the timescale for this module. The default is
		set by the `timescale directive. */
		int time_unit, time_precision;
		
		const char*mod_name() const { return name_; }
		
		void add_gate(PGate*gate);
		
		// The add_wire method adds a wire by name, but only if the
		// wire name doesn't already exist. Either way, the result is
		// the existing wire or the pointer passed in.
		PWire* add_wire(PWire*wire);
		
		void add_behavior(PProcess*behave);
		void add_task(const string&name, PTask*def);
		void add_function(const string&name, PFunction*def);
		
		unsigned port_count() const;
		const svector<PEIdent*>& get_port(unsigned idx) const;
		string& get_port_name(unsigned idx);
		unsigned find_port(const string&) const;
		
		// Find a wire by name. This is used for connecting gates to
		// existing wires, etc.
		PWire* get_wire(const hname_t&name) const;
		PGate* get_gate(const string&name);
		PTask* get_task(const string&name);
		
		const map<hname_t,PWire*>& get_wires() const;
		const list<PGate*>& get_gates() const;
		const list<PProcess*>& get_behaviors() const;
		void adjust_decl();
		
		void dump(std::ostream&out) const;
		void dump_slice(std::ostream&out, slicer* s) const;
		ModuleNode* build_node(PDesign& de);
		set<string>& get_funcs();
		set<string>& get_mods();
		
		bool elaborate(Design*, NetScope*scope) const;
		
		bool elaborate_scope(Design*, NetScope*scope) const;
		
		bool elaborate_sig(Design*, NetScope*scope) const;
		
		void traversal_proc();
		
		void set_modulenode(ModuleNode* mn){mn_ = mn;};
		ModuleNode* get_modulenode(){return mn_;};
		
		void set_cfg(Module_Cfgs* cfg){cfg_ = cfg;}
		Module_Cfgs* get_cfg(){return cfg_;};
		void build_cfgs();
		ostream& dump_cfg(ostream&);
		
		void build_vartable(Design*);
		set<Var*>& get_vartable(){return vartab_;}
		void set_var_space(set<string>&);
		void set_var_time(int);
		ostream& dump_vartable(ostream&);
		
		void build_cetable();
		list<cond_expr*>& get_cetable(){return cetab_;};
		cond_time_expr gen_ce(int, int);
		ostream& dump_cetable(ostream& o);
		
		void build_ddmodel();
		list<dd_var*>& get_ddmodel(){return ddmodel_;}
		ostream& dump_ddmodel(ostream& o);
		
		void build_exprtable();
		list<assign_expr*>& get_exprtable(){return aetab_;};
		ostream& dump_exprtable(ostream& o);
		
		void test(ostream& o);
		
		void adjust_assigntype();

		void find_all_path(const char*);

		map<int, map<int, string> > getpropathes();
		void dumpprocpathes(ostream&);
		void dumpprocpath(ostream&, int procid, int pathid);
		void genpathcom(ostream&, int simucycle);

		void pathcover(Design*, const char* vcd_file, ostream&, int);
		void togglecover(const char* vcd_file, ostream& report);
		void fsmcover(const char* vcd_file, string var, ostream& report);

		private:
			void vcd_parse_def_ignore( FILE* vcd );
			void vcd_parse_def_var( FILE* vcd );
			void vcd_parse_def_scope( FILE* vcd );
			void vcd_parse_def( FILE* vcd );
			void vcd_parse_sim( Design* des, FILE* vcd, ostream& report, int type);
			void vcd_parse_toggle(FILE* vcd_handle);
			void vcd_parse_sim_ignore( FILE* vcd );
			void vcd_parse_sim_vector( FILE* vcd, char* value );
			void set_symbol_char( char* sym, char value );
			void sim_timestep(Design* des, ostream&out, int type);
			void toggle_timestep();
			void dump_vcd_vars();
			int eval_cond_expr(Design* des, Cfg_Node* node);
			void report_toggle(ostream& report);

			void vcd_parse_fsm(FILE* vcd_handle);
			void fsm_timestep();
			void report_fsm(ostream& report);

			void report_path_stmt(int type, ostream& report);
		private:
			char* name_;
			
			map<hname_t,PWire*> wires_;
			list<PGate*> gates_;
			list<PProcess*> behaviors_;
			map<string,PTask*> tasks_;
			map<string,PFunction*> funcs_;
			ModuleNode* mn_;
			Module_Cfgs* cfg_;
			
			set<Var*> vartab_;
			list<cond_expr*> cetab_;
			list<assign_expr*> aetab_;
			list<dd_var*> ddmodel_;

			map<int, map<int, string> > procpathes;
			
			void build_dd_model(string TF, dd_var* ddn, int cfgno, int cfgidx, Cfg_Node* n);
			
			vcd_vars vcd_vars_;//var id in vcd, hierachy name of signal;
			map<string, vcd_id> vcd_ids_;

			map<string, fsm_stat> fsm;

			map<unsigned, stmt_stat> statements;
			map<unsigned, path_stat> paths;
			
			bool enter_scope;
			int scope_count;
			bool found_in_vcd;
			int cur_sim_time;
			
		private: // Not implemented
			Module(const Module&);
			Module& operator= (const Module&);
};

#endif
