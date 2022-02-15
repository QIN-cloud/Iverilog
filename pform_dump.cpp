#include "config.h"

/*
 * This file provides the pform_dump function, that dumps the module
 * passed as a parameter. The dump is as much as possible in Verilog
 * syntax, so that a human can tell that it really does describe the
 * module in question.
 */
#include "pform.h"
#include "PEvent.h"
#include "PData.h"
#include <iostream>
#include <iomanip>
#include <typeinfo>

ostream& operator << (ostream&out, const PExpr&obj)
{
      obj.dump(out);
      return out;
}

ostream& operator << (ostream&o, const PDelays&d)
{
      d.dump_delays(o);
      return o;
}

ostream& operator<< (ostream&o, PGate::strength_t str)
{
      switch (str) {
	  case PGate::HIGHZ:
	    o << "highz";
	    break;
	  case PGate::WEAK:
	    o << "weak";
	    break;
	  case PGate::PULL:
	    o << "pull";
	    break;
	  case PGate::STRONG:
	    o << "strong";
	    break;
	  case PGate::SUPPLY:
	    o << "supply";
	    break;
	  default:
	    assert(0);
      }
      return o;
}

void PExpr::dump(ostream&out) const
{
      out << typeid(*this).name();
}

void PEConcat::dump(ostream&out) const
{
      if (repeat_)
	    out << "{" << *repeat_;

      if (parms_.count() == 0) {
	    out << "{}";
	    return;
      }

      out << "{";
      if (parms_[0]) out << *parms_[0];
      for (unsigned idx = 1 ;  idx < parms_.count() ;  idx += 1) {
	    out << ", ";
	    if (parms_[idx]) out << *parms_[idx];
      }

      out << "}";

      if (repeat_) out << "}";
}

void PECallFunction::dump(ostream &out) const
{
      out << path_ << "(";

      if (parms_.count() > 0) {
	    if (parms_[0]) parms_[0]->dump(out);
	    for (unsigned idx = 1; idx < parms_.count(); ++idx) {
		  out << ", ";
		  if (parms_[idx]) parms_[idx]->dump(out);
	    }
      }
      out << ")";
}

void PEEvent::dump(ostream&out) const
{
      switch (type_) {
	  case PEEvent::ANYEDGE:
	    break;
	  case PEEvent::POSEDGE:
	    out << "posedge ";
	    break;
	  case PEEvent::NEGEDGE:
	    out << "negedge ";
	    break;
	  case PEEvent::POSITIVE:
	    out << "positive ";
	    break;
      }
      out << *expr_;

}

void PEFNumber::dump(ostream &out) const
{
      out << value();
}

void PENumber::dump(ostream&out) const
{
      out << value();
}

void PEIdent::dump(ostream&out) const
{
      out << path_;
      if (msb_) {
	    out << "[" << *msb_;
	    if (lsb_) {
		  out << ":" << *lsb_;
	    }
	    out << "]";
      }
}

void PEString::dump(ostream&out) const
{
      out << "\"" << text_ << "\"";
}

void PETernary::dump(ostream&out) const
{
      out << "(" << *expr_ << ")?(" << *tru_ << "):(" << *fal_ << ")";
}

void PEUnary::dump(ostream&out) const
{
      out << op_ << "(" << *expr_ << ")";
}

void PEBinary::dump(ostream&out) const
{
      out << "(" << *left_ << ")";
      switch (op_) {
	  case 'a':
	    out << "&&";
	    break;
	  case 'e':
	    out << "==";
	    break;
	  case 'E':
	    out << "===";
	    break;
	  case 'l':
	    out << "<<";
	    break;
	  case 'n':
	    out << "!=";
	    break;
	  case 'N':
	    out << "!==";
	    break;
	  case 'r':
	    out << ">>";
	    break;
	  default:
	    out << op_;
	    break;
      }
      out << "(" << *right_ << ")";
}


void PWire::dump(ostream&out) const
{
      unsigned idx;  
	  map<string,PExpr*>::const_iterator idxi;
	  switch (port_type_) {
	  case NetNet::PIMPLICIT:
	    out << "    input";
	    break;
	  case NetNet::PINPUT:
	    out << "    input";
	    break;
	  case NetNet::POUTPUT:
	    out << "    output";
	    break;
	  case NetNet::PINOUT:
	    out << "    inout";
	    break;
	  case NetNet::NOT_A_PORT:
	    goto l1;
      }

      assert(msb_.count() == lsb_.count());
      for (idx = 0 ;  idx < msb_.count() ;  idx += 1) {

	    if (msb_[idx] == 0) {
		  assert(lsb_[idx] == 0);
		  //out << " <scalar>";

	    } else {
		  if (lsb_[idx])
			out << " [" << *msb_[idx] << ":" << *lsb_[idx] << "]";
		  else
			out << " [" << *msb_[idx] << "]";
	    }
      }

      out << " " << hname_.peek_name(hname_.component_count()-1);

      out << ";" << endl;
      
      //
l1:      switch(type_){
	      case NetNet::IMPLICIT:
		  //out<<"    IMPLICIT";
		  goto l2;
	      case NetNet::IMPLICIT_REG:
		  //out<<"    IMPLICIT_REG";
		  goto l2;
 	      case NetNet::INTEGER:
		  out<<"    integer";
		  break;
	      case NetNet::WIRE:
		  out<<"    wire";
		  break;
	      case NetNet::TRI:
		  out<<"    tri";
		  break;
	      case NetNet::TRI1:
		  out<<"    tri1";
	      case NetNet::SUPPLY0:
		  out<<"    supply0";
		  break;
	      case NetNet::SUPPLY1:
		  out<<"    supply1";
		  break;
	      case NetNet::WAND:
		  out<<"    wand";
		  break;
	      case NetNet::TRIAND:
		  out<<"    triand";
		  break;
	      case NetNet::TRI0:
		  out<<"    tri0";
		  break;
	      case NetNet::WOR:
		  out<<"    wor";
		  break;
	      case NetNet::TRIOR:
		  out<<"    trior";
		  break;
	      case NetNet::REG:
		  out<<"    reg";
		  break;
      }

      if (signed_) {
	    out << " signed";
      }

      assert(msb_.count() == lsb_.count());
      for (idx = 0 ;  idx < msb_.count() ;  idx += 1) {

	    if (msb_[idx] == 0) {
		  assert(lsb_[idx] == 0);
		  //out << " <scalar>";

	    } else {
		  if (lsb_[idx])
			out << " [" << *msb_[idx] << ":" << *lsb_[idx] << "]";
		  else
			out << " [" << *msb_[idx] << "]";
	    }
      }

      out << " " << hname_.peek_name(hname_.component_count()-1);

	// If the wire has indices, dump them.
      if (lidx_ || ridx_) {
	    out << "[";
	    if (lidx_) out << *lidx_;
	    if (ridx_) out << ":" << *ridx_;
	    out << "]";
      }

      out << ";" << endl;
      for (idxi = attributes.begin()
		 ; idxi != attributes.end()
		 ; idxi ++) {
	    out << "        " << (*idxi).first;
	    if ((*idxi).second)
		  out << " = " << *(*idxi).second;
	    out << endl;
      }
l2:;
}

void PGate::dump_pins(ostream&out) const
{
      if (pin_count()) {
	    if (pin(0)) out << *pin(0);
			      
	    for (unsigned idx = 1 ;  idx < pin_count() ;  idx += 1) {
		  out << ", ";
		  if (pin(idx)) out << *pin(idx);
	    }
      }
}

void PDelays::dump_delays(ostream&out) const
{
      if (delay_[0] && delay_[1] && delay_[2])
	    out << "#(" << *delay_[0] << "," << *delay_[1] << "," <<
		  *delay_[2] << ")";
      else if (delay_[0])
	    out << "#" << *delay_[0];
}

void PGate::dump_delays(ostream&out) const
{
      delay_.dump_delays(out);
}

void PGate::dump(ostream&out) const
{
      out << "    " << typeid(*this).name() << " ";
      delay_.dump_delays(out);
      out << " " << get_name() << "(";
      dump_pins(out);
      out << ");" << endl;

}

void PGAssign::dump(ostream&out) const
{
    switch(strength0()){
	case PGate::HIGHZ:
	  out << "    assign ( highz0 ";
	  break;
	case PGate::WEAK:
	  out << "    assign (weak0 ";
	  break;
	case PGate::PULL:
	  out << "    assign (pull0 ";
	  break;
	case PGate::STRONG:
	  out << "    assign (strong0 ";
	  break;
	case PGate::SUPPLY:
	  out << "    assign (supply0 ";
	  break;
	case PGate::NO_STRENGTH:
	  out << "    assign ";  
      }
      switch(strength1()){
	case HIGHZ:
	  out << "highz1) ";
	  break;
	case WEAK:
	  out << "weak1) ";
	  break;
	case PULL:
	  out << "pull1) ";
	  break;
	case STRONG:
	  out << "strong1) ";
	  break;
	case SUPPLY:
	  out << "supply1) ";
	  break;
      }

      dump_delays(out);
      out << " " << *pin(0) << " = " << *pin(1) << ";" << endl;
}

void PGBuiltin::dump(ostream&out) const
{
      switch (type()) {
	  case PGBuiltin::BUFIF0:
	    out << "    bufif0 ";
	    break;
	  case PGBuiltin::BUFIF1:
	    out << "    bufif1 ";
	    break;
	  case PGBuiltin::NOTIF0:
	    out << "    bufif0 ";
	    break;
	  case PGBuiltin::NOTIF1:
	    out << "    bufif1 ";
	    break;
	  case PGBuiltin::NAND:
	    out << "    nand ";
	    break;
	  case PGBuiltin::NMOS:
	    out << "    nmos ";
	    break;
	  case PGBuiltin::RNMOS:
	    out << "    rnmos ";
	    break;
	  case PGBuiltin::RPMOS:
	    out << "    rpmos ";
	    break;
	  case PGBuiltin::PMOS:
	    out << "    pmos ";
	    break;
	  default:
	    out << "    builtin gate ";
      }

      switch(strength0()){
	case PGate::HIGHZ:
	  out << "    assign ( highz0 ";
	  break;
	case PGate::WEAK:
	  out << "    assign (weak0 ";
	  break;
	case PGate::PULL:
	  out << "    assign (pull0 ";
	  break;
	case PGate::STRONG:
	  out << "    assign (strong0 ";
	  break;
	case PGate::SUPPLY:
	  out << "    assign (supply0 ";
	  break;
	case PGate::NO_STRENGTH:
	  out << "    assign ";  
      }
      switch(strength1()){
	case HIGHZ:
	  out << "highz1) ";
	  break;
	case WEAK:
	  out << "weak1) ";
	  break;
	case PULL:
	  out << "pull1) ";
	  break;
	case STRONG:
	  out << "strong1) ";
	  break;
	case SUPPLY:
	  out << "supply1) ";
	  break;
      }
      dump_delays(out);
      out << " " << get_name();

      if (msb_) {
	    out << " [" << *msb_ << ":" << *lsb_ << "]";
      }

      out << "(";
      dump_pins(out);
      out << ");" << endl;
}

void PGModule::dump(ostream&out) const
{
      out << "    " << type_ << " ";

	// If parameters are overridden by order, dump them.
      if (overrides_) {
	    assert(parms_ == 0);
            out << "#(";
	    out << *((*overrides_)[0]);
	    for (unsigned idx = 1 ;  idx < overrides_->count() ;  idx += 1) {
	          out << "," << *((*overrides_)[idx]);
	    }
	    out << ") ";
      }

	// If parameters are overridden by name, dump them.
      if (parms_) {
	    assert(overrides_ == 0);
	    out << "#(";
	    out << "." << parms_[0].name << "(" << *parms_[0].parm << ")";
	    for (unsigned idx = 1 ;  idx < nparms_ ;  idx += 1) {
		  out << ", ." << parms_[idx].name << "(" <<
			*parms_[idx].parm << ")";
	    }
	    out << ") ";
      }

      out << get_name();

	// If the module is arrayed, print the index expressions.
      if (msb_ || lsb_) {
	    out << "[";
	    if (msb_) out << *msb_;
	    out << ":";
	    if (lsb_) out << *lsb_;
	    out << "]";
      }

      out << "(";
      if (pins) {
	    out << "." << pins[0].name << "(";
	    if (pins[0].parm) out << *pins[0].parm;
	    out << ")";
	    for (unsigned idx = 1 ;  idx < npins_ ;  idx += 1) {
		  out << ", ." << pins[idx].name << "(";
		  if (pins[idx].parm)
			out << *pins[idx].parm;
		  out << ")";
	    }
      } else {
	    dump_pins(out);
      }
      out << ");" << endl;
}

void Statement::dump(ostream&out, unsigned ind) const
{
	/* I give up. I don't know what type this statement is,
	   so just print the C++ typeid and let the user figure
	   it out. */
      out << setw(ind) << "";
      out << "/* " << get_line() << ": " << typeid(*this).name()
	  << " */ ;" << endl;
}

void PAssign::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "";
      if(delay_!=0)
	out << *lval() << " = " << delay_ << " " << *rval() << ";";
      else
	out << *lval() << " = " << " " << *rval() << ";";
      out << "  /* " << get_line() << " */" << endl;
}

void PAssignNB::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "";
      if(delay_ != 0)
        out << *lval() << " <= " << delay_ << " " << *rval() << ";";
      else
        out << *lval() << " <= " << " " << *rval() << ";";
      out << "  /* " << get_line() << " */" << endl;
}

void PBlock::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "" << "begin";
      if (name_.length())
	    out << " : " << name_;
      out << endl;

      for (unsigned idx = 0 ;  idx < list_.count() ;  idx += 1) {
	    if (list_[idx])
		  list_[idx]->dump(out, ind+2);
	    else
		  out << setw(ind+2) << "" << "/* NOOP */ ;" << endl;
      }

      out << setw(ind) << "" << "end" << endl;
}

void PCallTask::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "" << path_;

      if (parms_.count() > 0) {
	    out << "(";
	    if (parms_[0])
		  out << *parms_[0];

	    for (unsigned idx = 1 ;  idx < parms_.count() ;  idx += 1) {
		  out << ", ";
		  if (parms_[idx])
			out << *parms_[idx];
	    }
	    out << ")";
      }

      out << "; /* " << get_line() << " */" << endl;
}

void PCase::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "";
      switch (type_) {
	  case NetCase::EQ:
	    out << "case";
	    break;
	  case NetCase::EQX:
	    out << "casex";
	    break;
	  case NetCase::EQZ:
	    out << "casez";
	    break;
      }
      out << " (" << *expr_ << ") /* " << get_line() << " */" << endl;

      for (unsigned idx = 0 ;  idx < items_->count() ;  idx += 1) {
	    PCase::Item*cur = (*items_)[idx];

	    if (cur->expr.count() == 0) {
		  out << setw(ind+2) << "" << "default:";

	    } else {
		  out << setw(ind+2) << "" << *cur->expr[0];
			
		  for(unsigned e = 1 ; e < cur->expr.count() ; e += 1)
			out << ", " << *cur->expr[e];

		  out << ":";
	    }

	    if (cur->stat) {
		  out << endl;
		  cur->stat->dump(out, ind+6);
	    } else {
		  out << " ;" << endl;
	    }
      }

      out << setw(ind) << "" << "endcase" << endl;
}

void PCondit::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "" << "if (" << *expr_ << ")" << endl;
      if (if_)
	    if_->dump(out, ind+3);
      else
	    out << setw(ind) << ";" << endl;
      if (else_) {
	    out << setw(ind) << "" << "else" << endl;
	    else_->dump(out, ind+3);
      }
}

void PCAssign::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "" << "assign " << *lval_ << " = " << *expr_
	  << "; /* " << get_line() << " */" << endl;
}

void PDeassign::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "" << "deassign " << *lval_ << "; /* "
	  << get_line() << " */" << endl;
}

void PDelayStatement::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "" << "#" << *delay_ << " /* " <<
	    get_line() << " */";
      if (statement_) {
	    out << endl;
	    statement_->dump(out, ind+2);
      } else {
	    out << " /* noop */;" << endl;
      }
}

void PDisable::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "" << "disable " << scope_ << "; /* "
	  << get_line() << " */" << endl;
}

void PEventStatement::dump(ostream&out, unsigned ind) const
{
      if (expr_.count() == 0) {
	    out << setw(ind) << "" << "@* ";

      } else {
	    out << setw(ind) << "" << "@(" << *(expr_[0]);
	    if (expr_.count() > 1)
		  for (unsigned idx = 1 ;  idx < expr_.count() ;  idx += 1)
			out << " or " << *(expr_[idx]);

	    out << ")";
      }

      if (statement_) {
	    out << endl;
	    statement_->dump(out, ind+2);
      } else {
	    out << " ;" << endl;
      }
}

void PForce::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "" << "force " << *lval_ << " = " << *expr_
	  << "; /* " << get_line() << " */" << endl;
}

void PForever::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "" << "forever /* " << get_line() << " */" << endl;

      statement_->dump(out, ind+3);
}

void PForStatement::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "" << "for (" << *name1_ << " = " << *expr1_
	  << "; " << *cond_ << "; " << *name2_ << " = " << *expr2_ <<
	    ")" << endl;

      statement_->dump(out, ind+3);
}

void PFunction::dump(ostream&out, unsigned ind) const
{
      //out << setw(ind) << "" << "output " << out_->path() << ";" << endl;
      if (ports_)
	    for (unsigned idx = 0 ;  idx < ports_->count() ;  idx += 1) {
		  out << setw(ind) << "";
		  out << "input ";
		  out << (*ports_)[idx]->path().peek_name((*ports_)[idx]->path().component_count()-1) << ";" << endl;
	    }

      if (statement_)
	    statement_->dump(out, ind);
      else
	    out << setw(ind) << "" << "/* NOOP */" << endl;
}

void PRelease::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "" << "release " << *lval_ << "; /* "
	  << get_line() << " */" << endl;
}

void PRepeat::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "" << "repeat (" << *expr_ << ")" << endl;

      statement_->dump(out, ind+3);
}

void PTask::dump(ostream&out, unsigned ind) const
{
	if (ports_)
		for (unsigned idx = 0 ;  idx < ports_->count() ;  idx += 1) {
			out << setw(ind) << "";
			switch ((*ports_)[idx]->get_port_type()) {
			case NetNet::PINPUT:
				out << "input ";
				break;
			case NetNet::POUTPUT:
				out << "output ";
				break;
			case NetNet::PINOUT:
				out << "inout ";
				break;
			default:
				assert(0);
				break;
			}
			out << (*ports_)[idx]->path().peek_name((*ports_)[idx]->path().component_count()-1) << ";" << endl;
		}

      if(decls_.size() > 0)
	      for (map<hname_t,PWire*>::const_iterator decl = decls_.begin()
                       ; decl != decls_.end()
                       ; decl ++ ) {
			      (*decl).second->dump(out);
	      }

      if (statement_)
	    statement_->dump(out, ind);
      else
	    out << setw(ind) << "" << "/* NOOP */" << endl;
}

void PTrigger::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "" << "-> " << event_ << ";" << endl;
}

void PWhile::dump(ostream&out, unsigned ind) const
{
      out << setw(ind) << "" << "while (" << *cond_ << ")" << endl;

      statement_->dump(out, ind+3);
}

void PProcess::dump(ostream&out, unsigned ind) const
{
      switch (type_) {
	  case PProcess::PR_INITIAL:
	    out << setw(ind) << "" << "initial";
	    break;
	  case PProcess::PR_ALWAYS:
	    out << setw(ind) << "" << "always";
	    break;
      }

      out << " /* " << get_line() << " */" << endl;

      for (map<string,PExpr*>::const_iterator idx = attributes.begin()
		 ; idx != attributes.end() ; idx++ ) {

	    out << setw(ind+2) << "" << "(* " << (*idx).first;
	    if ((*idx).second) {
		  out << " = " << *(*idx).second;
	    }
	    out << " *)" << endl;
      }

      statement_->dump(out, ind+2);
}

void Module::dump(ostream&out) const
{
	if (attributes.begin() != attributes.end()) {
		out << "(* ";
		for (map<string,PExpr*>::const_iterator idx = attributes.begin();
		idx != attributes.end() ; idx++ ) {
			if (idx != attributes.begin()) {
				out << " , ";
			}
			out << (*idx).first;
			if ((*idx).second) {
				out << " = " << *(*idx).second;
			}
		}
		out << " *)  ";
	}

    out << "module " << name_ ;
    if(ports.count() < 1)
      out<< ";" << endl;
    else{
	out<<"(";
	for(unsigned i = 0; i < ports.count(); i += 1){
	  port_t*c = ports[i];
	  if(i == (ports.count() -1))
   	    out<<c->name<<");"<<endl;
	  else
	    out<<c->name<<", "; 
	}
	}

      /*
      for (unsigned idx = 0 ;  idx < ports.count() ;  idx += 1) {
	    port_t*cur = ports[idx];

	    if (cur == 0) {
		  out << "    unconnected" << endl;
		  continue;
	    }

	    out << "    ." << cur->name << "(" << *cur->expr[0];
	    for (unsigned wdx = 1 ;  wdx < cur->expr.count() ;  wdx += 1) {
		  out << ", " << *cur->expr[wdx];
	    }

	    out << ")" << endl;
      }
      */

      typedef map<string,param_expr_t>::const_iterator parm_iter_t;
      typedef map<hname_t,PExpr*>::const_iterator parm_hiter_t;
      for (parm_iter_t cur = parameters.begin()
		 ; cur != parameters.end() ; cur ++) {
	    out << "    parameter ";
	    if ((*cur).second.signed_flag)
		  out << "signed ";
	    if ((*cur).second.msb)
		  out << "[" << *(*cur).second.msb << ":"
		      << *(*cur).second.lsb << "] ";
	    out << (*cur).first << " = ";
	    if ((*cur).second.expr)
		  out << *(*cur).second.expr << ";" << endl;
	    else
		  out << "/* ERROR */;" << endl;
      }

      for (cur = localparams.begin()
		 ; cur != localparams.end() ; cur ++) {
	    out << "    localparam ";
	    if ((*cur).second.msb)
		  out << "[" << *(*cur).second.msb << ":"
		      << *(*cur).second.lsb << "] ";
	    out << (*cur).first << " = ";
	    if ((*cur).second.expr)
		  out << *(*cur).second.expr << ";" << endl;
	    else
		  out << "/* ERROR */;" << endl;
      }

      for (parm_hiter_t hcur = defparms.begin()
		 ; hcur != defparms.end() ;  hcur ++) {
	    out << "    defparam " << (*hcur).first << " = ";
	    if ((*hcur).second)
		  out << *(*hcur).second << ";" << endl;
	    else
		  out << "/* ERROR */;" << endl;
      }

      for (map<string,PEvent*>::const_iterator ecur = events.begin()
		 ; ecur != events.end() ;  ecur ++ ) {
	    PEvent*ev = (*ecur).second;
	    out << "    event " << ev->name() << "; // "
		<< ev->get_line() << endl;
      }

	  for (map<hname_t,PData*>::const_iterator dcur = datum.begin()
		 ; dcur != datum.end() ;  dcur ++ ) {
	    PData*tmp = (*dcur).second;
	    out << "    real " << tmp->name() << "; // "
		<< tmp->get_line() << endl;
      }

	// Iterate through and display all the wires.
      for (map<hname_t,PWire*>::const_iterator wire = wires_.begin()
		 ; wire != wires_.end()
		 ; wire ++ ) {

	    //if((*wire).first.component_count() <= 1)
	    (*wire).second->dump(out);
      }

	// Dump the task definitions.
      typedef map<string,PTask*>::const_iterator task_iter_t;
      for (task_iter_t tcur = tasks_.begin()
		 ; tcur != tasks_.end() ; tcur ++) {
	    out << "    task " << (*tcur).first << ";" << endl;
	    (*tcur).second->dump(out, 6);
	    out << "    endtask;" << endl;
      }

	// Dump the function definitions.
      typedef map<string,PFunction*>::const_iterator func_iter_t;
      for (func_iter_t fcur = funcs_.begin()
		 ; fcur != funcs_.end() ; fcur ++) {
	    out << "    function " << (*fcur).first << ";" << endl;
	    (*fcur).second->dump(out, 6);
	    out << "    endfunction;" << endl;
      }


	// Iterate through and display all the gates
      for (list<PGate*>::const_iterator gate = gates_.begin()
		 ; gate != gates_.end()
		 ; gate ++ ) {

	    (*gate)->dump(out);
      }


      for (list<PProcess*>::const_iterator behav = behaviors_.begin()
		 ; behav != behaviors_.end()
		 ; behav ++ ) {

	    (*behav)->dump(out, 4);
      }

      out << "endmodule" << endl;
}

void pform_dump(ostream&out, Module*mod)
{
      mod->dump(out);
}

void PUdp::dump(ostream&out) const
{
      out << "primitive " << name_ << "(" << ports[0];
      for (unsigned idx = 1 ;  idx < ports.count() ;  idx += 1)
	    out << ", " << ports[idx];
      out << ");" << endl;

      if (sequential)
	    out << "    reg " << ports[0] << ";" << endl;

      out << "    table" << endl;
      for (idx = 0 ;  idx < tinput.count() ;  idx += 1) {
	    out << "     ";
	    for (unsigned chr = 0 ;  chr < tinput[idx].length() ;  chr += 1)
		  out << " " << tinput[idx][chr];

	    if (sequential)
		  out << " : " << tcurrent[idx];

	    out << " : " << toutput[idx] << " ;" << endl;
      }
      out << "    endtable" << endl;

      //if (sequential)
	//    out << "    initial " << ports[0] << " = 1'b" << initial
	//	<< ";" << endl;

	// Dump the attributes for the primitive as attribute
	// statements.
      for (map<string,PExpr*>::const_iterator idxi = attributes.begin()
		 ; idxi != attributes.end()
		 ; idxi++) {
	    out << "    attribute " << (*idxi).first;
	    if ((*idxi).second)
		  out << " = " << *(*idxi).second;
	    out << endl;
      }

      out << "endprimitive" << endl;
}
