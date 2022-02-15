#ifndef __PTask_H
#define __PTask_H

#include "LineInfo.h"
#include "svector.h"
#include <string>
#include "PWire.h"
#include "HName.h"
#include <map>
#include <set>

class PDesign;
class PWire;
class Statement;
class ProcessNode;
class Design;

/*
 * The PTask holds the parsed definitions of a task.
 */
class PTask  : public LineInfo {

    public:
      explicit PTask();
      ~PTask();

      void set_ports(svector<PWire *>*p);
      void set_statement(Statement *s);
      unsigned port_count();
      PWire* get_port(unsigned idx);

      void dump(std::ostream&, unsigned) const;
      ProcessNode* build_node(PDesign& de);

      void add_decl(PWire* decl);
	  set<string>& get_funcs();

	  // Tasks introduce scope, to need to be handled during the
	  // scope elaboration pass. The scope passed is my scope,
	  // created by the containing scope. I fill it in with stuff if
	  // I need to.
      void elaborate_scope(Design*des, NetScope*scope) const;
	  
	  // Bind the ports to the regs that are the ports.
      void elaborate_sig(Design*des, NetScope*scope) const;
	  
	  // Elaborate the statement to finish off the task definition.
      void elaborate(Design*des, NetScope*scope) const;
      
    private:
      svector<PWire*>*ports_;
      map<hname_t, PWire*>decls_;
      Statement*statement_;

    private: // Not implemented
      PTask(const PTask&);
      PTask& operator=(const PTask&);
};

/*
 * The function is similar to a task (in this context) but there is a
 * single output port and a set of input ports. The output port is the
 * function return value.
 */
class PFunction : public LineInfo {

    public:
      explicit PFunction();
      ~PFunction();

      void set_ports(svector<PWire *>*p);
      void set_statement(Statement *s);
      void set_output(PWire*);

      void add_decl(PWire* decl);
	      
      void dump(std::ostream&, unsigned) const;
      ProcessNode* build_node(PDesign& de);
	  set<string>& get_funcs();

	  void elaborate_scope(Design*des, NetScope*scope) const;
	  
	  /* elaborate the ports and return value. */
      void elaborate_sig(Design *des, NetScope*) const;
	  
	  /* Elaborate the behavioral statement. */
      void elaborate(Design *des, NetScope*) const;

    private:
      PWire*out_;
      svector<PWire *> *ports_;
      map<hname_t, PWire*>decls_;
      Statement *statement_;
};

#endif
