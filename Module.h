#ifndef IVL_Module_H
#define IVL_Module_H
/*
<<<<<<< Updated upstream
 * Copyright (c) 1998-2021 Stephen Williams (steve@icarus.com)
=======
 * Copyright (c) 1998-2019 Stephen Williams (steve@icarus.com)
>>>>>>> Stashed changes
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */


# include  <list>
# include  <map>
# include  <vector>
# include  <utility>
<<<<<<< Updated upstream
=======
# include  "svector.h"
>>>>>>> Stashed changes
# include  "StringHeap.h"
# include  "HName.h"
# include  "named.h"
# include  "PScope.h"
# include  "PNamedItem.h"
# include  "netlist.h"
# include  "pform_types.h"
<<<<<<< Updated upstream
=======
# include  "slice.h"
# include  "PDesign.h"
>>>>>>> Stashed changes
class PExpr;
class PEIdent;
class PGate;
class PGenerate;
class PModport;
class PSpecPath;
class PTask;
class PFunction;
class PWire;
class PProcess;
class Design;
class NetScope;
<<<<<<< Updated upstream
=======
class ModuleNode;
class PData;
class dd_var;

struct cond_time_expr;

typedef struct vcd_var{
	string name_;
	bool little_endia;
	verinum pre_val_;
	verinum cur_val_;
}vcd_var;

typedef map<string, vcd_var> vcd_vars;
>>>>>>> Stashed changes

/*
 * A module is a named container and scope. A module holds a bunch of
 * semantic quantities such as wires and gates. The module is
 * therefore the handle for grasping the described circuit.
 *
 * SystemVerilog introduces program blocks and interfaces. These have
 * much in common with modules, so the Module class is used to represent
 * these containers as well.
 */

class Module : public PScopeExtra, public PNamedItem {

	/* The module ports are in general a vector of port_t
	   objects. Each port has a name and an ordered list of
	   wires. The name is the means that the outside uses to
	   access the port, the wires are the internal connections to
<<<<<<< Updated upstream
	   the port. In SystemVerilog, input ports may also have a
	   default value. */
    public:
      struct port_t {
	    perm_string name;
	    std::vector<PEIdent*> expr;
	    PExpr*default_value;
=======
	   the port. */
    public:
      struct port_t {
	    perm_string name;
	    vector<PEIdent*> expr;
>>>>>>> Stashed changes
      };

    public:
	/* The name passed here is the module name, not the instance
	   name. This name must be a permallocated string. */
      explicit Module(LexicalScope*parent, perm_string name);
      ~Module();

	/* Initially false. This is set to true if the module has been
	   declared as a library module. This makes the module
	   ineligible for being chosen as an implicit root. It has no
	   other effect. */
      bool library_flag;

      bool is_cell;

	/* This is true if the module represents a program block
	   instead of a module/cell. Program blocks have content
	   restrictions and slightly modify scheduling semantics. */
      bool program_block;

	/* This is true if the module represents a interface
	   instead of a module/cell. Interfaces have different
	   content restrictions and some extra allowed items. */
      bool is_interface;

      enum UCDriveType { UCD_NONE, UCD_PULL0, UCD_PULL1 };
      UCDriveType uc_drive;

	/* specparams are simpler than other parameters, in that they
	   can have a range, but not an explicit type. The restrictions
	   are enforced by the parser. */
<<<<<<< Updated upstream
      std::map<perm_string,param_expr_t*>specparams;
=======
      map<perm_string,param_expr_t*>specparams;
>>>>>>> Stashed changes

	/* The module also has defparam assignments which don't create
	   new parameters within the module, but may be used to set
	   values within this module (when instantiated) or in other
	   instantiated modules. */
<<<<<<< Updated upstream
      typedef std::pair<pform_name_t,PExpr*> named_expr_t;
      std::list<named_expr_t>defparms;
      static std::list<named_expr_t>user_defparms;
=======
      typedef pair<pform_name_t,PExpr*> named_expr_t;
      list<named_expr_t>defparms;
      static list<named_expr_t>user_defparms;
>>>>>>> Stashed changes

        /* Parameters may be overridden at instantiation time;
           the overrides do not contain explicit parameter names,
           but rather refer to parameters in the order they
           appear in the instantiated module.  Therefore a
           list of names in module-order is needed to pass from
           a parameter-index to its name. */
<<<<<<< Updated upstream
      std::list<perm_string> param_names;

	/* This is an array of port descriptors, which is in turn a
	   named array of PEident pointers. */
      std::vector<port_t*> ports;

      std::map<perm_string,PExpr*> attributes;

	/* The module has a list of generate schemes that appear in
	   the module definition. These are used at elaboration time. */
      std::list<PGenerate*> generate_schemes;
=======
      list<perm_string> param_names;

	/* This is an array of port descriptors, which is in turn a
	   named array of PEident pointers. */
      vector<port_t*> ports;

      map<perm_string,PExpr*> attributes;

	/* The module has a list of generate schemes that appear in
	   the module definition. These are used at elaboration time. */
      list<PGenerate*> generate_schemes;
>>>>>>> Stashed changes

	/* Nested modules are placed here, and are not elaborated
	   unless they are instantiated, implicitly or explicitly. */
      std::map<perm_string,Module*> nested_modules;

	/* An interface can contain one or more named modport lists.
           The parser will ensure these don't appear in modules or
           program blocks. */
<<<<<<< Updated upstream
      std::map<perm_string,PModport*> modports;

      std::list<PSpecPath*> specify_paths;
=======
      map<perm_string,PModport*> modports;

      list<PSpecPath*> specify_paths;
>>>>>>> Stashed changes

	// The mod_name() is the name of the module type.
      perm_string mod_name() const { return pscope_name(); }

      void add_gate(PGate*gate);

      unsigned port_count() const;
<<<<<<< Updated upstream
      const std::vector<PEIdent*>& get_port(unsigned idx) const;
=======
      const vector<PEIdent*>& get_port(unsigned idx) const;
>>>>>>> Stashed changes
      unsigned find_port(const char*name) const;

      // Return port name ("" for undeclared port)
      perm_string get_port_name(unsigned idx) const;

<<<<<<< Updated upstream
      PExpr* get_port_default_value(unsigned idx) const;

      PGate* get_gate(perm_string name);

      const std::list<PGate*>& get_gates() const;

      void dump(std::ostream&out) const;
      bool elaborate(Design*, NetScope*scope) const;

      typedef std::map<perm_string,PExpr*> replace_t;
=======
      PGate* get_gate(perm_string name);
      PTask* get_task(const string&name);
      PTask* get_task(perm_string name);

      const list<PGate*>& get_gates() const;

      void dump(ostream&out) const;
      bool elaborate(Design*, NetScope*scope) const;

      typedef map<perm_string,PExpr*> replace_t;
>>>>>>> Stashed changes
      bool elaborate_scope(Design*, NetScope*scope, const replace_t&rep);

      bool elaborate_sig(Design*, NetScope*scope) const;

      SymbolType symbol_type() const;
<<<<<<< Updated upstream

      bool can_be_toplevel() const;

    private:
      void dump_specparams_(std::ostream&out, unsigned indent) const;
      std::list<PGate*> gates_;
=======
      //2021.2.24
      PWire* get_wire(perm_string name) const;
      const map<perm_string,PWire*>& get_wires() const;
      void dump_slice(std::ostream&out, slicer* s) const;
      ModuleNode* build_node(PDesign& de);
      set<string>& get_funcs();
      set<string>& get_mods();
      void set_modulenode(ModuleNode* mn){mn_ = mn;};
      ModuleNode* get_modulenode(){return mn_;};
      
      void set_cfg(Module_Cfgs* cfg){cfg_ = cfg;}
      Module_Cfgs* get_cfg(){return cfg_;};
      void build_cfgs();
      ostream& dump_cfg(ostream&);
      
      void build_cetable();
      list<cond_expr*>& get_cetable(){return cetab_;};
      ostream& dump_cetable(ostream& o);
      
      void build_ddmodel();
      list<dd_var*>& get_ddmodel(){return ddmodel_;}
      ostream& dump_ddmodel(ostream& o);
      
      void build_exprtable();
      list<assign_expr*>& get_exprtable(){return aetab_;};
      ostream& dump_exprtable(ostream& o);
      
      void test(ostream& o);
      
      void coverage(Design*, const char* vcd_file);

      void adjust_assigntype();

      void find_all_path(const char*);

    private:
      void dump_specparams_(ostream&out, unsigned indent) const;
      list<PGate*> gates_;
      //2021.2.24
      void vcd_parse_def_ignore( FILE* vcd );
      void vcd_parse_def_var( FILE* vcd );
      void vcd_parse_def_scope( FILE* vcd );
      void vcd_parse_def( FILE* vcd );
      //void vcd_parse_sim( Design* des, FILE* vcd );
      void vcd_parse_sim_ignore( FILE* vcd );
      void vcd_parse_sim_vector( FILE* vcd, char* value );
      void set_symbol_char( char* sym, char value );
      //void sim_timestep(Design* des);
      //void dump_vcd_vars();
      //int eval_cond_expr(Design* des, Cfg_Node* node);

      ModuleNode* mn_;
      Module_Cfgs* cfg_;
      list<cond_expr*> cetab_;
      list<assign_expr*> aetab_;
      list<dd_var*> ddmodel_;
      bool enter_scope;
      int scope_count;
      bool found_in_vcd;
      int cur_sim_time;

      vcd_vars vcd_vars_;//var id in vcd, hierachy name of signal;
>>>>>>> Stashed changes

    private: // Not implemented
      Module(const Module&);
      Module& operator= (const Module&);
};

#endif /* IVL_Module_H */
