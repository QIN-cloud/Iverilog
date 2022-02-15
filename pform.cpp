#include "config.h"

#include "compiler.h"
#include "pform.h"
#include "parse_misc.h"
#include "parse_api.h"
#include "PEvent.h"
#include "PUdp.h"
#include "PData.h"
#include <list>
#include <map>
#include <assert.h>
#include <typeinfo>
#include <sstream>
#include <strstream>

map<string,Module*> pform_modules;
map<string,PUdp*> pform_primitives;

/*
 * The lexor accesses the vl_* variables.
 */
string vl_file = "";

extern int VLparse();

static Module*pform_cur_module = 0;

/*
 * These variables track the current time scale, as well as where the
 * timescale was set. This supports warnings about tangled timescales.
 */
static int pform_time_unit = 0;
static int pform_time_prec = 0;

static char*pform_timescale_file = 0;
static unsigned pform_timescale_line = 0;

/*
 * The scope stack and the following functions handle the processing
 * of scope. As I enter a scope, the push function is called, and as I
 * leave a scope the pop function is called. Entering tasks, functions
 * and named blocks causes scope to be pushed and popped. The module
 * name is not included it this scope stack.
 *
 * The hier_name function, therefore, returns the name path of a
 * function relative the current function.
 */

static hname_t scope_stack;

void pform_push_scope(char*name)
{
      scope_stack.append(name);
}

void pform_pop_scope()
{
      char*tmp = scope_stack.remove_tail_name();
      assert(tmp);
      free(tmp);
}

static hname_t hier_name(const char*tail)
{
      hname_t name = scope_stack;
      name.append(tail);
      return name;
}

/*
 * This function evaluates delay expressions. The result should be a
 * simple constant that I can interpret as an unsigned number.
static unsigned long evaluate_delay(PExpr*delay)
{
      PENumber*pp = dynamic_cast<PENumber*>(delay);
      if (pp == 0) {
	    VLerror("Sorry, delay expression is too complicated.");
	    return 0;
      }

      return pp->value().as_ulong();
}
*/

void pform_startmodule(const char*name, const char*file, unsigned lineno,
		       svector<named_pexpr_t*>*attr)
{
      assert( pform_cur_module == 0 );


      pform_cur_module = new Module(name);
      pform_cur_module->time_unit = pform_time_unit;
      pform_cur_module->time_precision = pform_time_prec;

      pform_cur_module->set_file(file);
      pform_cur_module->set_lineno(lineno);

	  if (attr) {
	      for (unsigned idx = 0 ;  idx < attr->count() ;  idx += 1) {
		      named_pexpr_t*tmp = (*attr)[idx];
		      pform_cur_module->attributes[tmp->name] = tmp->parm;
	      }
      }
}

/*
 * This function is called by the parser to make a simple port
 * reference. This is a name without a .X(...), so the internal name
 * should be generated to be the same as the X.
 */
Module::port_t* pform_module_port_reference(char*name,
					    const char*file,
					    unsigned lineno)
{
      Module::port_t*ptmp = new Module::port_t;
      PEIdent*tmp = new PEIdent(hname_t(name));
      tmp->set_file(file);
      tmp->set_lineno(lineno);
      ptmp->name = name;
      ptmp->expr = svector<PEIdent*>(1);
      ptmp->expr[0] = tmp;

      return ptmp;
}

void pform_module_set_ports(svector<Module::port_t*>*ports)
{
      assert(pform_cur_module);

	/* The parser parses ``module foo()'' as having one
	   unconnected port, but it is really a module with no
	   ports. Fix it up here. */
      if (ports && (ports->count() == 1) && ((*ports)[0] == 0)) {
	    delete ports;
	    ports = 0;
      }

      if (ports != 0) {
	    pform_cur_module->ports = *ports;
	    delete ports;
      }
}

void pform_endmodule(const char*name)
{
      assert(pform_cur_module);
      assert(strcmp(name, pform_cur_module->mod_name()) == 0);
      pform_cur_module->adjust_decl();
      pform_modules[name] = pform_cur_module;
      pform_cur_module = 0;
}

/*
bool pform_expression_is_constant(const PExpr*ex)
{
      return ex->is_constant(pform_cur_module);
}
*/

MIN_TYP_MAX min_typ_max_flag = TYP;
unsigned min_typ_max_warn = 10;

PExpr* pform_select_mtm_expr(PExpr*min, PExpr*typ, PExpr*max)
{
      PExpr*res = 0;

      switch (min_typ_max_flag) {
	  case MIN:
	    res = min;
	    delete typ;
	    delete max;
	    break;
	  case TYP:
	    res = typ;
	    delete min;
	    delete max;
	    break;
	  case MAX:
	    res = max;
	    delete min;
	    delete typ;
	    break;
      }

      if (min_typ_max_warn > 0) {
	    cerr << res->get_line() << ": warning: choosing ";
	    switch (min_typ_max_flag) {
		case MIN:
		  cerr << "min";
		  break;
		case TYP:
		  cerr << "typ";
		  break;
		case MAX:
		  cerr << "max";
		  break;
	    }

	    cerr << " expression." << endl;
	    min_typ_max_warn -= 1;
      }

      return res;
}

void pform_make_udp(const char*name, list<string>*parms,
		    svector<PWire*>*decl, list<string>*table,
		    Statement*init_expr,
		    const char*file, unsigned lineno)
{
      unsigned local_errors = 0;
      assert(parms->size() > 0);

	/* Put the declarations into a map, so that I can check them
	   off with the parameters in the list. If the port is already
	   in the map, merge the port type. I will rebuild a list
	   of parameters for the PUdp object. */
      map<string,PWire*> defs;
      for (unsigned idx = 0 ;  idx < decl->count() ;  idx += 1) {

	    hname_t pname = (*decl)[idx]->path();

	    if (PWire*cur = defs[pname.peek_name(0)]) {
		  bool rc = true;
		  assert((*decl)[idx]);
		  if ((*decl)[idx]->get_port_type() != NetNet::PIMPLICIT) {
			rc = cur->set_port_type((*decl)[idx]->get_port_type());
			assert(rc);
		  }
		  if ((*decl)[idx]->get_wire_type() != NetNet::IMPLICIT) {
			rc = cur->set_wire_type((*decl)[idx]->get_wire_type());
			assert(rc);
		  }

	    } else {
		  defs[pname.peek_name(0)] = (*decl)[idx];
	    }
      }


	/* Put the parameters into a vector of wire descriptions. Look
	   in the map for the definitions of the name. In this loop,
	   the parms list in the list of ports in the port list of the
	   UDP declaration, and the defs map maps that name to a
	   PWire* created by an input or output declaration. */
      svector<PWire*> pins (parms->size());
      svector<string> pin_names (parms->size());
      { list<string>::iterator cur;
        unsigned idx;
        for (cur = parms->begin(), idx = 0
		   ; cur != parms->end()
		   ; idx++, cur++) {
	      pins[idx] = defs[*cur];
	      pin_names[idx] = *cur;
	}
      }

	/* Check that the output is an output and the inputs are
	   inputs. I can also make sure that only the single output is
	   declared a register, if anything. The possible errors are:

	      -- an input port (not the first) is missing an input
	         declaration.

	      -- An input port is declared output.

	*/
      assert(pins.count() > 0);
      do {
	    if (pins[0] == 0) {
		  cerr << file<<":"<<lineno << ": error: "
		       << "Output port of primitive " << name
		       << " missing output declaration." << endl;
		  cerr << file<<":"<<lineno << ":      : "
		       << "Try: output " << pin_names[0] << ";"
		       << endl;
		  error_count += 1;
		  local_errors += 1;
		  break;
	    }
	    if (pins[0]->get_port_type() != NetNet::POUTPUT) {
		  cerr << file<<":"<<lineno << ": error: "
		       << "The first port of a primitive"
		       << " must be an output." << endl;
		  cerr << file<<":"<<lineno << ":      : "
		       << "Try: output " << pin_names[0] << ";"
		       << endl;
		  error_count += 1;
		  local_errors += 1;
		  break;;
	    }
      } while (0);

      for (idx = 1 ;  idx < pins.count() ;  idx += 1) {
	    if (pins[idx] == 0) {
		  cerr << file<<":"<<lineno << ": error: "
		       << "Port " << (idx+1)
		       << " of primitive " << name << " missing"
		       << " input declaration." << endl;
		  cerr << file<<":"<<lineno << ":      : "
		       << "Try: input " << pin_names[idx] << ";"
		       << endl;
		  error_count += 1;
		  local_errors += 1;
		  continue;
	    }
	    if (pins[idx]->get_port_type() != NetNet::PINPUT) {
		  cerr << file<<":"<<lineno << ": error: "
		       << "Port " << (idx+1)
		       << " of primitive " << name << " in an input port"
		       << " with an output declaration." << endl;
		  cerr << file<<":"<<lineno << ":      : "
		       << "Try: input " << pin_names[idx] << ";"
		       << endl;
		  error_count += 1;
		  local_errors += 1;
		  continue;
	    }
	    assert(pins[idx]->get_wire_type() != NetNet::REG);
      }
      if (local_errors > 0) {
	    delete parms;
	    delete decl;
	    delete table;
	    delete init_expr;
	    return;
      }

	/* Interpret and check the table entry strings, to make sure
	   they correspond to the inputs, output and output type. Make
	   up vectors for the fully interpreted result that can be
	   placed in the PUdp object. */
      svector<string> input   (table->size());
      svector<char>   current (table->size());
      svector<char>   output  (table->size());
      { unsigned idx = 0;
        for (list<string>::iterator cur = table->begin()
		   ; cur != table->end()
		   ; cur ++, idx += 1) {
	      string tmp = *cur;
	      assert(tmp.find(':') == (pins.count() - 1));

	      input[idx] = tmp.substr(0, pins.count()-1);
	      tmp = tmp.substr(pins.count()-1);

	      if (pins[0]->get_wire_type() == NetNet::REG) {
		    assert(tmp[0] == ':');
		    assert(tmp.size() == 4);
		    current[idx] = tmp[1];
		    tmp = tmp.substr(2);
	      }

	      assert(tmp[0] == ':');
	      assert(tmp.size() == 2);
	      output[idx] = tmp[1];
	}
      }

	// Put the primitive into the primitives table
      if (pform_primitives[name]) {
	    VLerror("UDP primitive already exists.");

      } else {
	    PUdp*udp = new PUdp(name, parms->size());

	      // Detect sequential udp.
	    if (pins[0]->get_wire_type() == NetNet::REG)
		  udp->sequential = true;

	      // Make the port list for the UDP
	    for (unsigned idx = 0 ;  idx < pins.count() ;  idx += 1)
		  udp->ports[idx] = pins[idx]->path().peek_name(0);

	    udp->tinput   = input;
	    udp->tcurrent = current;
	    udp->toutput  = output;

	    pform_primitives[name] = udp;
      }


	/* Delete the excess tables and lists from the parser. */
      delete parms;
      delete decl;
      delete table;
}

/*
 * This function attaches a range to a given name. The function is
 * only called by the parser within the scope of the net declaration,
 * and the name that I receive only has the tail component.
 */
static void pform_set_net_range(const char*name,
				const svector<PExpr*>*range,
				bool signed_flag)
{

      PWire*cur = pform_cur_module->get_wire(hier_name(name));
      if (cur == 0) {
	    VLerror("error: name is not a valid net.");
	    return;
      }

      if (range == 0) {
	      /* This is the special case that we really mean a
		 scalar. Set a fake range. */
	    cur->set_range(0, 0);

      } else {
	    assert(range->count() == 2);
	    assert((*range)[0]);
	    assert((*range)[1]);
	    cur->set_range((*range)[0], (*range)[1]);
      }
      cur->set_signed(signed_flag);
}

void pform_set_net_range(list<char*>*names,
			 svector<PExpr*>*range,
			 bool signed_flag)
{
      assert((range == 0) || (range->count() == 2));

      for (list<char*>::iterator cur = names->begin()
		 ; cur != names->end()
		 ; cur ++ ) {
	    char*txt = *cur;
	    pform_set_net_range(txt, range, signed_flag);
	    free(txt);
      }

      delete names;
      if (range)
	    delete range;
}

/*
 * This is invoked to make a named event. This is the declaration of
 * the event, and not necessarily the use of it.
 */
static void pform_make_event(const char*name, const char*fn, unsigned ln)
{
      PEvent*event = new PEvent(name);
      event->set_file(fn);
      event->set_lineno(ln);
      pform_cur_module->events[name] = event;
}

void pform_make_events(list<char*>*names, const char*fn, unsigned ln)
{
      list<char*>::iterator cur;
      for (cur = names->begin() ;  cur != names->end() ;  cur++) {
	    char*txt = *cur;
	    pform_make_event(txt, fn, ln);
	    free(txt);
      }

      delete names;
}

/*
 * pform_makegates is called when a list of gates (with the same type)
 * are ready to be instantiated. The function runs through the list of
 * gates and calls the pform_makegate function to make the individual gate.
 */
void pform_makegate(PGBuiltin::Type type,
		    struct str_pair_t str,
		    svector<PExpr*>* delay,
		    const lgate&info,
		    svector<named_pexpr_t*>*attr)
{
      if (info.parms_by_name) {
	    cerr << info.file << ":" << info.lineno << ": Gates do not "
		  "have port names." << endl;
	    error_count += 1;
	    return;
      }

      PGBuiltin*cur = new PGBuiltin(type, info.name, info.parms, delay);
      if (info.range[0])
	    cur->set_range(info.range[0], info.range[1]);

      if (attr) {
	    for (unsigned idx = 0 ;  idx < attr->count() ;  idx += 1) {
		  named_pexpr_t*tmp = (*attr)[idx];
		  cur->attributes[tmp->name] = tmp->parm;
	    }
      }

      cur->strength0(str.str0);
      cur->strength1(str.str1);
      cur->set_file(info.file);
      cur->set_lineno(info.lineno);

      pform_cur_module->add_gate(cur);
}

void pform_makegates(PGBuiltin::Type type,
		     struct str_pair_t str,
		     svector<PExpr*>*delay,
		     svector<lgate>*gates,
		     svector<named_pexpr_t*>*attr)
{
      for (unsigned idx = 0 ;  idx < gates->count() ;  idx += 1) {
	    pform_makegate(type, str, delay, (*gates)[idx], attr);
      }

      if (attr) {
	    for (unsigned idx = 0 ;  idx < attr->count() ;  idx += 1) {
		  named_pexpr_t*cur = (*attr)[idx];
		  delete cur;
	    }
	    delete attr;
      }

      delete gates;
}

/*
 * A module is different from a gate in that there are different
 * constraints, and sometimes different syntax. The X_modgate
 * functions handle the instantaions of modules (and UDP objects) by
 * making PGModule objects.
 */
static void pform_make_modgate(const char*type,
			       const string&name,
			       struct parmvalue_t*overrides,
			       svector<PExpr*>*wires,
			       PExpr*msb, PExpr*lsb,
			       const char*fn, unsigned ln)
{
      PGModule*cur = new PGModule(type, name, wires);
      cur->set_file(fn);
      cur->set_lineno(ln);
      cur->set_range(msb,lsb);

      if (overrides && overrides->by_name) {
	    unsigned cnt = overrides->by_name->count();
	    named<PExpr*>*byname = new named<PExpr*>[cnt];

	    for (unsigned idx = 0 ;  idx < cnt ;  idx += 1) {
		  named_pexpr_t*curp = (*overrides->by_name)[idx];
		  byname[idx].name = curp->name;
		  byname[idx].parm = curp->parm;
	    }

	    cur->set_parameters(byname, cnt);

      } else if (overrides && overrides->by_order) {
	    cur->set_parameters(overrides->by_order);
      }

      pform_cur_module->add_gate(cur);
}

static void pform_make_modgate(const char*type,
			       const string&name,
			       struct parmvalue_t*overrides,
			       svector<named_pexpr_t*>*bind,
			       PExpr*msb, PExpr*lsb,
			       const char*fn, unsigned ln)
{
      unsigned npins = bind->count();
      named<PExpr*>*pins = new named<PExpr*>[npins];
      for (unsigned idx = 0 ;  idx < npins ;  idx += 1) {
	    named_pexpr_t*curp = (*bind)[idx];
	    pins[idx].name = curp->name;
	    pins[idx].parm = curp->parm;
      }

      PGModule*cur = new PGModule(type, name, pins, npins);
      cur->set_file(fn);
      cur->set_lineno(ln);
      cur->set_range(msb,lsb);

      if (overrides && overrides->by_name) {
	    unsigned cnt = overrides->by_name->count();
	    named<PExpr*>*byname = new named<PExpr*>[cnt];

	    for (unsigned idx = 0 ;  idx < cnt ;  idx += 1) {
		  named_pexpr_t*curp = (*overrides->by_name)[idx];
		  byname[idx].name = curp->name;
		  byname[idx].parm = curp->parm;
	    }

	    cur->set_parameters(byname, cnt);

      } else if (overrides && overrides->by_order) {

	    cur->set_parameters(overrides->by_order);
      }

      pform_cur_module->add_gate(cur);
}

void pform_make_modgates(const char*type,
			 struct parmvalue_t*overrides,
			 svector<lgate>*gates)
{

      for (unsigned idx = 0 ;  idx < gates->count() ;  idx += 1) {
	    lgate cur = (*gates)[idx];

	    if (cur.parms_by_name) {
		  pform_make_modgate(type, cur.name, overrides,
				     cur.parms_by_name,
				     cur.range[0], cur.range[1],
				     cur.file, cur.lineno);

	    } else if (cur.parms) {

		    /* If there are no parameters, the parser will be
		       tricked into thinking it is one empty
		       parameter. This fixes that. */
		  if ((cur.parms->count() == 1) && (cur.parms[0][0] == 0)) {
			delete cur.parms;
			cur.parms = new svector<PExpr*>(0);
		  }
		  pform_make_modgate(type, cur.name, overrides,
				     cur.parms,
				     cur.range[0], cur.range[1],
				     cur.file, cur.lineno);

	    } else {
		  svector<PExpr*>*wires = new svector<PExpr*>(0);
		  pform_make_modgate(type, cur.name, overrides,
				     wires,
				     cur.range[0], cur.range[1],
				     cur.file, cur.lineno);
	    }
      }

      delete gates;
}

PGAssign* pform_make_pgassign(PExpr*lval, PExpr*rval,
			      svector<PExpr*>*del,
			      struct str_pair_t str)
{
      svector<PExpr*>*wires = new svector<PExpr*>(2);
      (*wires)[0] = lval;
      (*wires)[1] = rval;

      PGAssign*cur;

      if (del == 0)
	    cur = new PGAssign(wires);
      else
	    cur = new PGAssign(wires, del);

      cur->strength0(str.str0);
      cur->strength1(str.str1);

      pform_cur_module->add_gate(cur);
      return cur;
}

void pform_make_pgassign_list(svector<PExpr*>*alist,
			      svector<PExpr*>*del,
			      struct str_pair_t str,
			      const char* fn,
			      unsigned lineno)
{
	PGAssign*tmp;
	for (unsigned idx = 0 ;  idx < alist->count()/2 ;  idx += 1) {
	      tmp = pform_make_pgassign((*alist)[2*idx],
					(*alist)[2*idx+1],
					del, str);
	      tmp->set_file(fn);
	      tmp->set_lineno(lineno);
	}
}

/*
 * this function makes the initial assignment to a register as given
 * in the source. It handles the case where a reg variable is assigned
 * where it it declared:
 *
 *    reg foo = <expr>;
 *
 * This is equivilent to the combination of statements:
 *
 *    reg foo;
 *    initital foo = <expr>;
 *
 * and that is how it is parsed. This syntax is not part of the
 * IEEE1364-1994 standard, but is approved by OVI as enhancement
 * BTF-B14.
 */
void pform_make_reginit(const struct vlltype&li,
			const char*name, PExpr*expr)
{
      const hname_t sname = hier_name(name);
      PWire*cur = pform_cur_module->get_wire(sname);
      if (cur == 0) {
	    VLerror(li, "internal error: reginit to non-register?");
	    delete expr;
	    return;
      }

      PEIdent*lval = new PEIdent(sname);
      lval->set_file(li.text);
      lval->set_lineno(li.first_line);
      PAssign*ass = new PAssign(lval, expr);
      ass->set_file(li.text);
      ass->set_lineno(li.first_line);
      PProcess*top = new PProcess(PProcess::PR_INITIAL, ass);
      top->set_file(li.text);
      top->set_lineno(li.first_line);

      pform_cur_module->add_behavior(top);
}

/*
 * This function is used by the parser when I have port definition of
 * the form like this:
 *
 *     input wire signed [7:0] nm;
 *
 * The port_type, type, signed_flag and range are known all at once,
 * so we can create the PWire object all at once instead of piecemeal
 * as is done for the old method.
 */
void pform_module_define_port(const struct vlltype&li,
			      const char*nm,
			      NetNet::PortType port_type,
			      NetNet::Type type,
			      bool signed_flag,
			      svector<PExpr*>*range,
			      svector<named_pexpr_t*>*attr)
{
      hname_t name = hier_name(nm);
      PWire*cur = pform_cur_module->get_wire(name);
      if (cur) {
	    strstream msg;
	    msg << name << " definition conflicts with "
		<< "definition at " << cur->get_line()
		<< "." << ends;
	    VLerror(msg.str());
	    return;
      }


      cur = new PWire(name, type, port_type);
      cur->set_file(li.text);
      cur->set_lineno(li.first_line);

      cur->set_signed(signed_flag);

      if (range == 0) {
	    cur->set_range(0, 0);

      } else {
	    assert(range->count() == 2);
	    assert((*range)[0]);
	    assert((*range)[1]);
	    cur->set_range((*range)[0], (*range)[1]);
      }

	  if (attr) {
	      for (unsigned idx = 0 ;  idx < attr->count() ;  idx += 1) {
		      named_pexpr_t*tmp = (*attr)[idx];
		      cur->attributes[tmp->name] = tmp->parm;
	      }
      }

      pform_cur_module->add_wire(cur);
}

/*
 * This function makes a single signal (a wire, a reg, etc) as
 * requested by the parser. The name is unscoped, so I attach the
 * current scope to it (with the scoped_name function) and I try to
 * resolve it with an existing PWire in the scope.
 *
 * The wire might already exist because of an implicit declaration in
 * a module port, i.e.:
 *
 *     module foo (bar...
 *
 *         reg bar;
 *
 * The output (or other port direction indicator) may or may not have
 * been seen already, so I do not do any checking with it yet. But I
 * do check to see if the name has already been declared, as this
 * function is called for every declaration.
 */
void pform_makewire(const vlltype&li, const char*nm,
		    NetNet::Type type, NetNet::PortType pt,
		    svector<named_pexpr_t*>*attr)
{
      hname_t name = hier_name(nm);
      PWire*cur = pform_cur_module->get_wire(name);
      if (cur) {
	    if ((cur->get_wire_type() != NetNet::IMPLICIT)
		&& (cur->get_wire_type() != NetNet::IMPLICIT_REG)) {
		  ostringstream msg;
		  msg << name << " previously defined at " <<
			cur->get_line() << ".";
		  VLerror(msg.str().c_str());
	    } else {
		  bool rc = cur->set_wire_type(type);
		  if (rc == false) {
			ostringstream msg;
			msg << name << " definition conflicts with "
			    << "definition at " << cur->get_line()
			    << ".";
			VLerror(msg.str().c_str());
		  }
	    }

	    cur->set_file(li.text);
	    cur->set_lineno(li.first_line);
	    return;
      }

      cur = new PWire(name, type, pt);
      cur->set_file(li.text);
      cur->set_lineno(li.first_line);

      if (attr) {
	    for (unsigned idx = 0 ;  idx < attr->count() ;  idx += 1) {
		  named_pexpr_t*tmp = (*attr)[idx];
		  cur->attributes[tmp->name] = tmp->parm;
	    }
      }

      pform_cur_module->add_wire(cur);
}

void pform_makewire(const vlltype&li,
		    svector<PExpr*>*range,
		    bool signed_flag,
		    list<char*>*names,
		    NetNet::Type type,
		    NetNet::PortType pt,
		    svector<named_pexpr_t*>*attr)
{
      for (list<char*>::iterator cur = names->begin()
		 ; cur != names->end()
		 ; cur ++ ) {
	    char*txt = *cur;
	    pform_makewire(li, txt, type, pt, attr);
	    pform_set_net_range(txt, range, signed_flag);
	    free(txt);
      }

      delete names;
      if (range)
	    delete range;
}

void pform_makewire(const vlltype&li,
		    svector<PExpr*>*range,
		    bool signed_flag,
		    svector<PExpr*>*delay,
		    str_pair_t str,
		    net_decl_assign_t*decls,
		    NetNet::Type type)
{
      net_decl_assign_t*first = decls->next;
      decls->next = 0;

      while (first) {
	    net_decl_assign_t*next = first->next;

	    pform_makewire(li, first->name, type, NetNet::NOT_A_PORT, 0);
	    pform_set_net_range(first->name, range, signed_flag);

	    hname_t name = hier_name(first->name);
	    PWire*cur = pform_cur_module->get_wire(name);
	    if (cur != 0) {
		  PEIdent*lval = new PEIdent(hname_t(first->name));
		  lval->set_file(li.text);
		  lval->set_lineno(li.first_line);
		  pform_make_pgassign(lval, first->expr, delay, str);
	    }

	    free(first->name);
	    delete first;
	    first = next;
      }
}

void pform_set_port_type(const char*nm, NetNet::PortType pt,
			 const char*file, unsigned lineno)
{
      hname_t name = hier_name(nm);
      PWire*cur = pform_cur_module->get_wire(name);
      if (cur == 0) {
	    cur = new PWire(name, NetNet::IMPLICIT, NetNet::PIMPLICIT);
	    cur->set_file(file);
	    cur->set_lineno(lineno);
	    pform_cur_module->add_wire(cur);
      }

      switch (cur->get_port_type()) {
	  case NetNet::PIMPLICIT:
	    if (! cur->set_port_type(pt))
		  VLerror("error setting port direction.");
	    break;

	  case NetNet::NOT_A_PORT:
	    cerr << file << ":" << lineno << ": error: "
		 << "port " << name << " is not in the port list."
		 << endl;
	    error_count += 1;
	    break;

	  default:
	    cerr << file << ":" << lineno << ": error: "
		 << "port " << name << " already has a port declaration."
		 << endl;
	    error_count += 1;
	    break;
      }

}

/*
 * This function is called by the parser to create task ports. The
 * resulting wire (which should be a register) is put into a list to
 * be packed into the task parameter list.
 *
 * It is possible that the wire (er, register) was already created,
 * but we know that if the name matches it is a part of the current
 * task, so in that case I just assign direction to it.
 *
 * The following example demonstrates some of the issues:
 *
 *   task foo;
 *      input a;
 *      reg a, b;
 *      input b;
 *      [...]
 *   endtask
 *
 * This function is called when the parser matches the "input a" and
 * the "input b" statements. For ``a'', this function is called before
 * the wire is declared as a register, so I create the foo.a
 * wire. For ``b'', I will find that there is already a foo.b and I
 * just set the port direction. In either case, the ``reg a, b''
 * statement is caught by the block_item non-terminal and processed
 * there.
 *
 * Ports are implicitly type reg, because it must be possible for the
 * port to act as an l-value in a procedural assignment. It is obvious
 * for output and inout ports that the type is reg, because the task
 * only contains behavior (no structure) to a procedural assignment is
 * the *only* way to affect the output. It is less obvious for input
 * ports, but in practice an input port receives its value as if by a
 * procedural assignment from the calling behavior.
 *
 * This function also handles the input ports of function
 * definitions. Input ports to function definitions have the same
 * constraints as those of tasks, so this works fine. Functions have
 * no output or inout ports.
 */
svector<PWire*>*pform_make_task_ports(NetNet::PortType pt,
				      bool signed_flag,
				      svector<PExpr*>*range,
				      list<char*>*names,
				      const char* file,
				      unsigned lineno)
{
      assert(names);
      svector<PWire*>*res = new svector<PWire*>(0);
      for (list<char*>::iterator cur = names->begin()
		 ; cur != names->end() ; cur ++ ) {

	    char*txt = *cur;
	    hname_t name = hier_name(txt);

	      /* Look for a preexisting wire. If it exists, set the
		 port direction. If not, create it. */
	    PWire*curw = pform_cur_module->get_wire(name);
	    if (curw) {
		  curw->set_port_type(pt);
	    } else {
		  curw = new PWire(name, NetNet::IMPLICIT_REG, pt);
		  curw->set_file(file);
		  curw->set_lineno(lineno);
		  pform_cur_module->add_wire(curw);
	    }

	    curw->set_signed(signed_flag);

	      /* If there is a range involved, it needs to be set. */
	    if (range)
		  curw->set_range((*range)[0], (*range)[1]);

	    svector<PWire*>*tmp = new svector<PWire*>(*res, curw);

	    free(txt);
	    delete res;
	    res = tmp;
      }

      if (range)
	    delete range;
      delete names;
      return res;
}

void pform_set_task(const string&name, PTask*task)
{
      pform_cur_module->add_task(name, task);
}

/*
 * This function is called to fill out the definition of the function
 * with the trappings that are discovered after the basic function
 * name is parsed.
 */
void pform_set_function(const char*name, NetNet::Type ntype,
			svector<PExpr*>*ra, PFunction *func)
{
      if (ntype == NetNet::IMPLICIT)
	    ntype = NetNet::REG;

	/* Form into path_return a hierarchical name for the synthetic
	   return value for the function. The return value is the same
	   name as the function, so if the function name is "foo", the
	   wire is "foo.foo". */
      hname_t path_return (name);
      path_return.append(name);

      PWire*out = new PWire(path_return, ntype, NetNet::POUTPUT);
      if (ra) {
	    assert(ra->count() == 2);
	    out->set_range((*ra)[0], (*ra)[1]);
	    delete ra;
      }

	/* If the return type of the function is INTEGER, then
	   generate a range for it. */
      if (ntype == NetNet::INTEGER) {
	    out->set_signed(true);
      }

      pform_cur_module->add_wire(out);
      func->set_output(out);
      pform_cur_module->add_function(name, func);
}

void pform_set_attrib(const char*name, const string&key, char*value)
{
      hname_t path (name);

      if (PWire*cur = pform_cur_module->get_wire(path)) {
	    cur->attributes[key] = new PEString(value);

      } else if (PGate*cur = pform_cur_module->get_gate(name)) {
	    cur->attributes[key] = new PEString(value);

      } else {
	    free(value);
	    VLerror("Unable to match name for setting attribute.");

      }
}

/*
 * Set the attribute of a TYPE. This is different from an object in
 * that this applies to every instantiation of the given type.
 */
void pform_set_type_attrib(const string&name, const string&key,
			   char*value)
{
      map<string,PUdp*>::const_iterator udp = pform_primitives.find(name);
      if (udp == pform_primitives.end()) {
	    VLerror("type name is not (yet) defined.");
	    free(value);
	    return;
      }

      (*udp).second ->attributes[key] = new PEString(value);
}

/*
 * This function attaches a memory index range to an existing
 * register. (The named wire must be a register.
 */
void pform_set_reg_idx(const char*name, PExpr*l, PExpr*r)
{
      PWire*cur = pform_cur_module->get_wire(hier_name(name));
      if (cur == 0) {
	    VLerror("internal error: name is not a valid memory for index.");
	    return;
      }

      cur->set_memory_idx(l, r);
}

void pform_set_parameter(const string&name, bool signed_flag,
			 svector<PExpr*>*range, PExpr*expr)
{
      assert(expr);
      pform_cur_module->parameters[name].expr = expr;

      if (range) {
	    assert(range->count() == 2);
	    assert((*range)[0]);
	    assert((*range)[1]);
	    pform_cur_module->parameters[name].msb = (*range)[0];
	    pform_cur_module->parameters[name].lsb = (*range)[1];
      } else {
	    pform_cur_module->parameters[name].msb = 0;
	    pform_cur_module->parameters[name].lsb = 0;
      }
      pform_cur_module->parameters[name].signed_flag = signed_flag;

      pform_cur_module->param_names.push_back(name);
}

void pform_set_localparam(const string&name, PExpr*expr)
{
      assert(expr);
      pform_cur_module->localparams[name].expr = expr;
      pform_cur_module->localparams[name].msb  = 0;
      pform_cur_module->localparams[name].lsb  = 0;
      pform_cur_module->localparams[name].signed_flag = false;
}

void pform_set_defparam(const hname_t&name, PExpr*expr)
{
      assert(expr);
      pform_cur_module->defparms[name] = expr;
}

void pform_set_specparam(const string&name, PExpr*expr)
{
      assert(expr);
      pform_cur_module->specparams[name] = expr;
}

/*
 * XXXX Not implemented yet.
 */
extern void pform_make_specify_path(list<char*>*src, char pol,
				    bool full_flag, list<char*>*dst)
{
      delete src;
      delete dst;
}

void pform_set_port_type(const struct vlltype&li,
			 list<char*>*names,
			 svector<PExpr*>*range,
			 bool signed_flag,
			 NetNet::PortType pt)
{
      for (list<char*>::iterator cur = names->begin()
		 ; cur != names->end()
		 ; cur ++ ) {
	    char*txt = *cur;
	    pform_set_port_type(txt, pt, li.text, li.first_line);
	    if (range)
		  pform_set_net_range(txt, range, signed_flag);
	    free(txt);
      }

      delete names;
      if (range)
	    delete range;
}

static void pform_set_reg_integer(const char*nm)
{
      hname_t name = hier_name(nm);
      PWire*cur = pform_cur_module->get_wire(name);
      if (cur == 0) {
	    cur = new PWire(name, NetNet::INTEGER, NetNet::NOT_A_PORT);
	    cur->set_signed(true);
	    pform_cur_module->add_wire(cur);
      } else {
	    bool rc = cur->set_wire_type(NetNet::INTEGER);
	    assert(rc);
	    cur->set_signed(true);
      }
      assert(cur);

      cur->set_signed(true);
}

void pform_set_reg_integer(list<char*>*names)
{
      for (list<char*>::iterator cur = names->begin()
		 ; cur != names->end()
		 ; cur ++ ) {
	    char*txt = *cur;
	    pform_set_reg_integer(txt);
	    free(txt);
      }
      delete names;
}

static void pform_set_reg_time(const char*nm)
{
      hname_t name = hier_name(nm);
      PWire*cur = pform_cur_module->get_wire(name);
      if (cur == 0) {
	    cur = new PWire(name, NetNet::REG, NetNet::NOT_A_PORT);
	    pform_cur_module->add_wire(cur);
      } else {
	    bool rc = cur->set_wire_type(NetNet::REG);
	    assert(rc);
      }
      assert(cur);

}

void pform_set_reg_time(list<char*>*names)
{
      for (list<char*>::iterator cur = names->begin()
		 ; cur != names->end()
		 ; cur ++ ) {
	    char*txt = *cur;
	    pform_set_reg_time(txt);
	    free(txt);
      }
      delete names;
}

svector<PWire*>* pform_make_udp_input_ports(list<char*>*names)
{
      svector<PWire*>*out = new svector<PWire*>(names->size());

      unsigned idx = 0;
      for (list<char*>::iterator cur = names->begin()
		 ; cur != names->end()
		 ; cur ++ ) {
	    char*txt = *cur;
	    PWire*pp = new PWire(txt, NetNet::IMPLICIT, NetNet::PINPUT);
	    (*out)[idx] = pp;
	    idx += 1;
	    free(txt);
      }

      delete names;
      return out;
}

PProcess* pform_make_behavior(PProcess::Type type, Statement*st,
			      svector<named_pexpr_t*>*attr)
{
      PProcess*pp = new PProcess(type, st);

      if (attr) {
	    for (unsigned idx = 0 ;  idx < attr->count() ;  idx += 1) {
		  named_pexpr_t*tmp = (*attr)[idx];
		  pp->attributes[tmp->name] = tmp->parm;
	    }
	    delete attr;
      }

      pform_cur_module->add_behavior(pp);
      return pp;
}


FILE*vl_input = 0;
extern void reset_lexor();

int pform_parse(const char*path, FILE*file)
{
      vl_file = path;
      if (file == 0) {

	    if (strcmp(path, "-") == 0)
		  vl_input = stdin;
	    else
		  vl_input = fopen(path, "r");
	    if (vl_input == 0) {
		  cerr << "Unable to open " <<vl_file << "." << endl;
		  return 11;
	    }

      } else {
	    vl_input = file;
      }

      reset_lexor();
      error_count = 0;
      warn_count = 0;
      int rc = VLparse();

      if (file == 0)
	    fclose(vl_input);

      if (rc) {
	    cerr << "I give up." << endl;
	    error_count += 1;
      }

      return error_count;
}

/*
 * The lexor calls this function to set the active timescale when it
 * detects a `timescale directive. The function saves the directive
 * values (for use by modules) and if warnings are enabled checks to
 * see if some modules have no timescale.
 */
void pform_set_timescale(int unit, int prec,
			 const char*file, unsigned lineno)
{
      bool first_flag = true;

      assert(unit >= prec);
      pform_time_unit = unit;
      pform_time_prec = prec;

      if (pform_timescale_file) {
	    free(pform_timescale_file);
	    first_flag = false;
      }

      pform_timescale_file = strdup(file);
      pform_timescale_line = lineno;
}

static void pform_make_datum(const char*name, const char*fn, unsigned ln)
{
      hname_t hname = hier_name(name);
      PData*datum = new PData(hname);
      datum->set_file(fn);
      datum->set_lineno(ln);
      pform_cur_module->datum[hname] = datum;
}

void pform_make_reals(list<char*>*names, const char*fn, unsigned ln)
{
      list<char*>::iterator cur;
      for (cur = names->begin() ;  cur != names->end() ;  cur++) {
	    char*txt = *cur;
	    pform_make_datum(txt, fn, ln);
	    free(txt);
      }

      delete names;
}

bool pform_expression_is_constant(const PExpr*ex)
{
      return ex->is_constant(pform_cur_module);
}

verinum* pform_verinum_with_size(verinum*siz, verinum*val,
				 const char*file, unsigned lineno)
{
      assert(siz->is_defined());
      unsigned long size = siz->as_ulong();

      verinum::V pad;

      switch (val->get(val->len()-1)) {
	  case verinum::Vz:
	    pad = verinum::Vz;
	    break;
	  case verinum::Vx:
	    pad = verinum::Vx;
	    break;
	  default:
	    pad = verinum::V0;
	    break;
      }

      verinum*res = new verinum(pad, size);

      unsigned copy = val->len();
      if (res->len() < copy)
	    copy = res->len();

      for (unsigned idx = 0 ;  idx < copy ;  idx += 1) {
	    res->set(idx, val->get(idx));
      }

      res->has_sign(val->has_sign());

      bool trunc_flag = false;
      for (idx = copy ;  idx < val->len() ;  idx += 1) {
	    if (val->get(idx) != pad) {
		  trunc_flag = true;
		  break;
	    }
      }

      if (trunc_flag) {
	    cerr << file << ":" << lineno << ": warning: Numeric constant "
		 << "truncated to " << copy << " bits." << endl;
      }

      delete siz;
      delete val;
      return res;
}