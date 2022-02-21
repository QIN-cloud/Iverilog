#ifndef IVL_Module_H
#define IVL_Module_H
/* 
 * Copyright (c) 1998-2019 Stephen Williams (steve@icarus.com)
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
# include  "svector.h"
# include  "StringHeap.h"
# include  "HName.h"
# include  "named.h"
# include  "PScope.h"
# include  "PNamedItem.h"
# include  "netlist.h"
# include  "pform_types.h"
# include  "slice.h"
# include  "PDesign.h"
# include  "cfg.h"
# include  "vcdvar.h"
# include  "testpath.h"
# include  <fstream>

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
class PDesign;
class Design;
class NetScope;
class PData;
class VcdVar;
struct cond_expr;

typedef map<string, VcdVar*> vcd_vars;

/* A struct stores the paths of a Cfg node. */
struct PathNode{
public:
      PathNode(Cfg_Node* node) : node_(node){};
      ~PathNode();
      Cfg_Node* node_;
      vector<string> paths_;
};

/* This scope represents a instantiation module by this module type.*/
class VcdScope{
public:
      VcdScope(Module* module):module_(module){};
      ~VcdScope();
      void dump(ostream& o) const;
      Module* module_;                    //Module type.
      string name_;                       //Instatntiated name.
      vcd_vars vars;                      //Variriables in this vcd scope.
};


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
	   the port. */
      public:
            struct port_t {
                  perm_string name;
                  vector<PEIdent*> expr;
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
            map<perm_string,param_expr_t*>specparams;

            /* The module also has defparam assignments which don't create
            new parameters within the module, but may be used to set
            values within this module (when instantiated) or in other
            instantiated modules. */
            typedef pair<pform_name_t,PExpr*> named_expr_t;
            list<named_expr_t>defparms;
            static list<named_expr_t>user_defparms;

            /* Parameters may be overridden at instantiation time;
            the overrides do not contain explicit parameter names,
            but rather refer to parameters in the order they
            appear in the instantiated module.  Therefore a
            list of names in module-order is needed to pass from
            a parameter-index to its name. */
            list<perm_string> param_names;

            /* This is an array of port descriptors, which is in turn a
            named array of PEident pointers. */
            vector<port_t*> ports;

            map<perm_string,PExpr*> attributes;

            /* The module has a list of generate schemes that appear in
            the module definition. These are used at elaboration time. */
            list<PGenerate*> generate_schemes;

            /* Nested modules are placed here, and are not elaborated
            unless they are instantiated, implicitly or explicitly. */
            std::map<perm_string,Module*> nested_modules;

            /* An interface can contain one or more named modport lists.
            The parser will ensure these don't appear in modules or
            program blocks. */
            map<perm_string,PModport*> modports;

            list<PSpecPath*> specify_paths;

            /* The mod_name() is the name of the module type.*/
            perm_string mod_name() const { return pscope_name(); }

            void add_gate(PGate*gate);

            unsigned port_count() const;
            const vector<PEIdent*>& get_port(unsigned idx) const;
            unsigned find_port(const char*name) const;

            /* Return port name ("" for undeclared port)*/
            perm_string get_port_name(unsigned idx) const;

            PGate* get_gate(perm_string name);
            PTask* get_task(const string&name);
            PTask* get_task(perm_string name);
            PWire* get_wire(perm_string name);

            const list<PGate*>& get_gates() const;

            const map<perm_string, PWire*> get_wires() const;

            void dump(ostream&out) const;

            bool elaborate(Design*, NetScope*scope) const;

            typedef map<perm_string,PExpr*> replace_t;

            bool elaborate_scope(Design*, NetScope*scope, const replace_t&rep);

            bool elaborate_sig(Design*, NetScope*scope) const;

            SymbolType symbol_type() const;

      public:
            ModuleNode* build_node(PDesign& de);

            /* Build cfgs for processes in this module. */
            void build_cfgs();           

            /* Search the variables in condition expressions and record the process id. */
            void build_var_cfgs();

            /* Build variable table*/
            void build_vartable(Design*);

            /* Build cond expression table. */
            void build_cetable();

            /* Generate paths for every process. */
            void build_paths();

            /* Using DFS to generate paths for start node of a cfg.*/
            PathNode* gen_path(Cfg_Node* node, svector<Cfg_Node*>* root, unsigned index);

            /* Write random paths acorrding to seeds.*/
            void random_path(ostream& o, string seeds) const;

            /* Dump the cfgnodes for every cfg. */
            void dump_cfg(ostream& o) const; 

            /* Dump the condit variable and process id,then dump the vcd scopes instantiated of this module. */
            void dump_vars(ostream& o) const;

            /* Dump the variables of instatiated scopes during the simulation replay. */      
            void dump_values(ostream& o)const; 

            void dump_slice(std::ostream& out, slicer* s) const;

            void dump_paths(ostream& o)const;

            inline void set_cfg(Module_Cfgs* cfg){cfg_ = cfg;}

            inline void set_modulenode(ModuleNode* mn){mn_  = mn;}

            inline Module_Cfgs* get_cfg(){return cfg_;};

            inline list<cond_expr*>& get_cetable(){return cetab_;};

            inline set<Var*>& get_vartable(){return vartab_;}

            vector<VcdScope*> vcd_scopes;        /* Mutiple instantiated scopes of this module type. */
            
            map<string, set<unsigned>> var_cfgs; /* Save condit variable and process id, form as { var, set{ process id } }. */

            map<Cfg_Node*, PathNode*> path_nodes_;

            map<Cfg*, vector<set<unsigned> > > routes_;

      private:
            void dump_specparams_(ostream&out, unsigned indent) const;
            list<PGate*>     gates_;                    /* Gates include assign lines and module instantiated. */
            ModuleNode*      mn_;                       /* The cfgnode build by this module. */
            Module_Cfgs*     cfg_;                      /* The Cfgs build by module. */
            set<Var*>        vartab_;                   /* Defined variables. */
		    list<cond_expr*> cetab_;                    /* Condit expressions. */
            map<unsigned, vector<string> > paths_;      /* All paths for every process. */

      private: // Not implemented
            Module(const Module&);
            Module& operator= (const Module&);
};

#endif /* IVL_Module_H */
