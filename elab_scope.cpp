#include "config.h"
#include "compiler.h"
#include <iostream>

/*
 * Elaboration happens in two passes, generally. The first scans the
 * pform to generate the NetScope tree and attach it to the Design
 * object. The methods in this source file implement the elaboration
 * of the scopes.
 */

#include "Module.h"
#include "PData.h"
#include "PEvent.h"
#include "PExpr.h"
#include "PGate.h"
#include "PTask.h"
#include "PWire.h"
#include "Statement.h"
#include "netlist.h"
#include "util.h"
#include <typeinfo>
#include <assert.h>

bool Module::elaborate_scope(Design*des, NetScope*scope) const
{
	// Generate all the parameters that this instance of this
	// module introduces to the design. This loop elaborates the
	// parameters, but doesn't evaluate references to
	// parameters. This scan practically locates all the
	// parameters and puts them in the parameter table in the
	// design.

	// No expressions are evaluated, yet. For now, leave them in
	// the pform and just place a NetEParam placeholder in the
	// place of the elaborated expression.

      typedef map<string,param_expr_t>::const_iterator mparm_it_t;
      typedef map<hname_t,PExpr*>::const_iterator hparm_it_t;


	// This loop scans the parameters in the module, and creates
	// stub parameter entries in the scope for the parameter name.

      for (mparm_it_t cur = parameters.begin()
		 ; cur != parameters.end() ;  cur ++) {

	    NetEParam*tmp = new NetEParam;
	    tmp->set_line(*((*cur).second.expr));
	    if ((*cur).second.msb)
		  tmp->cast_signed( (*cur).second.signed_flag );

	    scope->set_parameter((*cur).first, tmp, 0, 0, false);
      }

      for (cur = localparams.begin()
		 ; cur != localparams.end() ;  cur ++) {

	    NetEParam*tmp = new NetEParam;
	    tmp->set_line(*((*cur).second.expr));
	    if ((*cur).second.msb)
		  tmp->cast_signed( (*cur).second.signed_flag );

	    scope->set_parameter((*cur).first, tmp, 0, 0, false);
      }


	// Now scan the parameters again, this time elaborating them
	// for use as parameter values. This is after the previous
	// scan so that local parameter names can be used in the
	// r-value expressions.

      for (cur = parameters.begin()
		 ; cur != parameters.end() ;  cur ++) {

	    PExpr*ex = (*cur).second.expr;
	    assert(ex);

	    NetExpr*val = ex->elaborate_pexpr(des, scope);
	    NetExpr*msb = 0;
	    NetExpr*lsb = 0;
	    bool signed_flag = false;

	      /* If the parameter declaration includes msb and lsb,
		 then use them to calculate a width for the
		 result. Then make sure the constant expression of the
		 parameter value is coerced to have the correct
		 and defined width. */
	    if ((*cur).second.msb) {
		  msb = (*cur).second.msb ->elaborate_pexpr(des, scope);
		  assert(msb);
		  lsb = (*cur).second.lsb ->elaborate_pexpr(des, scope);
		  signed_flag = (*cur).second.signed_flag;
	    }

	    val->cast_signed(signed_flag);
	    val = scope->set_parameter((*cur).first, val,
				       msb, lsb, signed_flag);
	    assert(val);
	    delete val;
      }

      for (cur = localparams.begin()
		 ; cur != localparams.end() ;  cur ++) {

	    PExpr*ex = (*cur).second.expr;
	    assert(ex);

	    NetExpr*val = ex->elaborate_pexpr(des, scope);
	    val = scope->set_parameter((*cur).first, val, 0, 0, false);
	    assert(val);
	    delete val;
      }

	// Run through the defparams for this module, elaborate the
	// expressions in this context and save the result is a table
	// for later final override.

	// It is OK to elaborate the expressions of the defparam here
	// because Verilog requires that the expressions only use
	// local parameter names. It is *not* OK to do the override
	// here because the parameter receiving the assignment may be
	// in a scope not discovered by this pass.

      for (hparm_it_t cur2 = defparms.begin()
		 ; cur2 != defparms.end() ;  cur2++ ) {

	    PExpr*ex = (*cur2).second;
	    assert(ex);

	    NetExpr*val = ex->elaborate_pexpr(des, scope);
	    if (val == 0) continue;
	    scope->defparams[(*cur2).first] = val;
      }

	  // Evaluate the attributes. Evaluate them in the scope of the
	  // module that the attribute is attached to. Is this correct?
      unsigned nattr;
      attrib_list_t*attr = evaluate_attributes(attributes, nattr, des, scope);
	  
      for (unsigned idx = 0 ;  idx < nattr ;  idx += 1)
		  scope->attribute(attr[idx].key, attr[idx].val);
	  
      delete[]attr;

	// Tasks introduce new scopes, so scan the tasks in this
	// module. Create a scope for the task and pass that to the
	// elaborate_scope method of the PTask for detailed
	// processing.

      typedef map<string,PTask*>::const_iterator tasks_it_t;

      for (tasks_it_t cur3 = tasks_.begin()
		 ; cur3 != tasks_.end() ;  cur3++ ) {

	    NetScope*task_scope = new NetScope(scope, (*cur3).first.c_str(),
					       NetScope::TASK);
	    (*cur3).second->elaborate_scope(des, task_scope);
      }


	// Functions are very similar to tasks, at least from the
	// perspective of scopes. So handle them exactly the same
	// way.

      typedef map<string,PFunction*>::const_iterator funcs_it_t;

      for (funcs_it_t cur4 = funcs_.begin()
		 ; cur4 != funcs_.end() ;  cur4++ ) {

	    NetScope*func_scope = new NetScope(scope, (*cur4).first.c_str(),
					       NetScope::FUNC);
	    (*cur4).second->elaborate_scope(des, func_scope);
      }


	// Gates include modules, which might introduce new scopes, so
	// scan all of them to create those scopes.

      typedef list<PGate*>::const_iterator gates_it_t;

      for (gates_it_t gcur = gates_.begin()
		 ; gcur != gates_.end() ;  gcur ++ ) {

	    (*gcur) -> elaborate_scope(des, scope);
      }


	// initial and always blocks may contain begin-end and
	// fork-join blocks that can introduce scopes. Therefore, I
	// get to scan processes here.

      typedef list<PProcess*>::const_iterator proc_it_t;

      for (proc_it_t pcur = behaviors_.begin()
		 ; pcur != behaviors_.end() ;  pcur ++ ) {

	    (*pcur) -> statement() -> elaborate_scope(des, scope);
      }

	// Scan through all the named events in this scope. We do not
	// need anything more then the current scope to do this
	// elaboration, so do it now. This allows for normal
	// elaboration to reference these events.

      for (map<string,PEvent*>::const_iterator et = events.begin()
		 ; et != events.end() ;  et ++ ) {

	    (*et).second->elaborate_scope(des, scope);
      }

      for (map<hname_t,PData*>::const_iterator dcur = datum.begin()
		 ; dcur != datum.end() ;  dcur ++ ) {

	    (*dcur).second->elaborate_scope(des, scope);
      }

      return des->errors == 0;
}

void PGModule::elaborate_scope_mod_(Design*des, Module*mod, NetScope*sc) const
{
      if (get_name() == "") {
	    cerr << get_line() << ": error: Instantiation of module "
		 << mod->mod_name() << " requires an instance name." << endl;
	    des->errors += 1;
	    return;
      }

	// Missing module instance names have already been rejected.
      assert(get_name() != "");

	// Check for duplicate scopes. Simply look up the scope I'm
	// about to create, and if I find it then somebody beat me to
	// it.

      if (sc->child(get_name())) {
	    cerr << get_line() << ": error: Instance/Scope name " <<
		  get_name() << " already used in this context." <<
		  endl;
	    des->errors += 1;
	    return;
      }

	// check for recursive instantiation by scanning the current
	// scope and its parents. Look for a module instantiation of
	// the same module, but farther up in the scope.

      for (NetScope*scn = sc ;  scn ;  scn = scn->parent()) {
	    if (scn->type() != NetScope::MODULE)
		  continue;

	    if (strcmp(mod->mod_name(), scn->module_name()) != 0)
		  continue;

	    cerr << get_line() << ": error: You cannot instantiate "
		 << "module " << mod->mod_name() << " within itself." << endl;

	    cerr << get_line() << ":      : The offending instance is "
		 << sc->name() << "." << get_name() << " within "
		 << scn->name() << "." << endl;

	    des->errors += 1;
	    return;
      }

	// Create the new scope as a MODULE with my name.
      NetScope*my_scope = new NetScope(sc, get_name().c_str(), NetScope::MODULE);
      my_scope->set_module_name(mod->mod_name());

	// Set time units and precision.
      my_scope->time_unit(mod->time_unit);
      my_scope->time_precision(mod->time_precision);
      des->set_precision(mod->time_precision);

	// This call actually arranges for the description of the
	// module type to process this instance and handle parameters
	// and sub-scopes that might occur. Parameters are also
	// created in that scope, as they exist. (I'll override them
	// later.)
      mod->elaborate_scope(des, my_scope);

	// Look for module parameter replacements. The "replace" map
	// maps parameter name to replacement expression that is
	// passed. It is built up by the ordered overrides or named
	// overrides.

      typedef map<string,PExpr*>::const_iterator mparm_it_t;
      map<string,PExpr*> replace;


	// Positional parameter overrides are matched to parameter
	// names by using the param_names list of parameter
	// names. This is an ordered list of names so the first name
	// is parameter 0, the second parameter 1, and so on.

      if (overrides_) {
	    assert(parms_ == 0);
	    list<string>::const_iterator cur = mod->param_names.begin();
	    unsigned idx = 0;
	    for (;;) {
		  if (idx >= overrides_->count())
			break;
		  if (cur == mod->param_names.end())
			break;

		  replace[*cur] = (*overrides_)[idx];

		  idx += 1;
		  cur ++;
	    }
      }

	// Named parameter overrides carry a name with each override
	// so the mapping into the replace list is much easier.
      if (parms_) {
	    assert(overrides_ == 0);
	    for (unsigned idx = 0 ;  idx < nparms_ ;  idx += 1)
		  replace[parms_[idx].name] = parms_[idx].parm;

      }


	// And here we scan the replacements we collected. Elaborate
	// the expression in my context, then replace the sub-scope
	// parameter value with the new expression.

      for (mparm_it_t cur = replace.begin()
		 ; cur != replace.end() ;  cur ++ ) {

	    PExpr*tmp = (*cur).second;
	    NetExpr*val = tmp->elaborate_pexpr(des, sc);
	    bool flag = my_scope->replace_parameter((*cur).first, val);
	    if (! flag) {
		  cerr << val->get_line() << ": warning: parameter "
		       << (*cur).first << " not found in "
		       << sc->name() << "." << endl;
	    }
      }
}

/*
 * Elaborate the datum within the module. This variable may be
 * within a subscope (i.e., a function or task) so use the components
 * of the name to find the precise scope where this item goes.
 */
void PData::elaborate_scope(Design*des, NetScope*scope) const
{
      NetScope*sub_scope = scope;
      for (unsigned idx = 0 ;  idx < (hname_.component_count()-1); idx += 1) {
	    sub_scope = sub_scope->child(hname_.peek_name(idx));

	    if (sub_scope == 0) {
		  cerr << get_line() << ": internal error: "
		       << "Could not find sub-scope "
		       << hname_.peek_name(idx) << " of "
		       << hname_ << " in module " << scope->name()
		       << endl;
		  des->errors += 1;
		  return;
	    }
      }

      const char*basename = hname_.peek_tail_name();
      NetVariable*tmp = new NetVariable(lex_strings.add(basename));
      tmp->set_line(*this);
      sub_scope->add_variable(tmp);
}

/*
 * The isn't really able to create new scopes, but it does create the
 * event name in the current scope, so can be done during the
 * elaborate_scope scan. Note that the name_ of the PEvent object has
 * no hierarchy, but neither does the NetEvent, until it is stored in
 * the NetScope object.
 */
void PEvent::elaborate_scope(Design*des, NetScope*scope) const
{
      NetEvent*ev = new NetEvent(name_.c_str());
      ev->set_line(*this);
      scope->add_event(ev);
}

void PFunction::elaborate_scope(Design*des, NetScope*scope) const
{
      assert(scope->type() == NetScope::FUNC);

      if (statement_)
	    statement_->elaborate_scope(des, scope);
}

void PTask::elaborate_scope(Design*des, NetScope*scope) const
{
      assert(scope->type() == NetScope::TASK);

      if (statement_)
	    statement_->elaborate_scope(des, scope);
}


/*
 * The base statement does not have sub-statements and does not
 * introduce any scope, so this is a no-op.
 */
void Statement::elaborate_scope(Design*, NetScope*) const
{
}

/*
 * When I get a behavioral block, check to see if it has a name. If it
 * does, then create a new scope for the statements within it,
 * otherwise use the current scope. Use the selected scope to scan the
 * statements that I contain.
 */
void PBlock::elaborate_scope(Design*des, NetScope*scope) const
{
      NetScope*my_scope = scope;

      if (name_ != "") {
	    my_scope = new NetScope(scope, name_.c_str(), bl_type_==BL_PAR
				    ? NetScope::FORK_JOIN
				    : NetScope::BEGIN_END);
      }

      for (unsigned idx = 0 ;  idx < list_.count() ;  idx += 1)
	    list_[idx] -> elaborate_scope(des, my_scope);

}

/*
 * The case statement itself does not introduce scope, but contains
 * other statements that may be named blocks. So scan the case items
 * with the elaborate_scope method.
 */
void PCase::elaborate_scope(Design*des, NetScope*scope) const
{
      assert(items_);
      for (unsigned idx = 0 ;  idx < (*items_).count() ;  idx += 1) {
	    assert( (*items_)[idx] );

	    if (Statement*sp = (*items_)[idx]->stat)
		  sp -> elaborate_scope(des, scope);
      }
}

/*
 * The conditional statement (if-else) does not introduce scope, but
 * the statements of the clauses may, so elaborate_scope the contained
 * statements.
 */
void PCondit::elaborate_scope(Design*des, NetScope*scope) const
{
      if (if_)
	    if_ -> elaborate_scope(des, scope);

      if (else_)
	    else_ -> elaborate_scope(des, scope);
}

/*
 * Statements that contain a further statement but do not
 * intrinsically add a scope need to elaborate_scope the contained
 * statement.
 */
void PDelayStatement::elaborate_scope(Design*des, NetScope*scope) const
{
      if (statement_)
	    statement_ -> elaborate_scope(des, scope);
}

/*
 * Statements that contain a further statement but do not
 * intrinsically add a scope need to elaborate_scope the contained
 * statement.
 */
void PEventStatement::elaborate_scope(Design*des, NetScope*scope) const
{
      if (statement_)
	    statement_ -> elaborate_scope(des, scope);
}

/*
 * Statements that contain a further statement but do not
 * intrinsically add a scope need to elaborate_scope the contained
 * statement.
 */
void PForever::elaborate_scope(Design*des, NetScope*scope) const
{
      if (statement_)
	    statement_ -> elaborate_scope(des, scope);
}

/*
 * Statements that contain a further statement but do not
 * intrinsically add a scope need to elaborate_scope the contained
 * statement.
 */
void PForStatement::elaborate_scope(Design*des, NetScope*scope) const
{
      if (statement_)
	    statement_ -> elaborate_scope(des, scope);
}

/*
 * Statements that contain a further statement but do not
 * intrinsically add a scope need to elaborate_scope the contained
 * statement.
 */
void PRepeat::elaborate_scope(Design*des, NetScope*scope) const
{
      if (statement_)
	    statement_ -> elaborate_scope(des, scope);
}

/*
 * Statements that contain a further statement but do not
 * intrinsically add a scope need to elaborate_scope the contained
 * statement.
 */
void PWhile::elaborate_scope(Design*des, NetScope*scope) const
{
      if (statement_)
	    statement_ -> elaborate_scope(des, scope);
}