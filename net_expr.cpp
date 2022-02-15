#include "config.h"
#include "netlist.h"
#include "compiler.h"
#include <iostream>

NetExpr::TYPE NetExpr::expr_type() const
{
      return ET_VECTOR;
}

/*
 * Create an add/sub node from the two operands. Make a best guess of
 * the 
 */
NetEBAdd::NetEBAdd(char op, NetExpr*l, NetExpr*r)
: NetEBinary(op, l, r)
{
      NetEConst* tmp;

	/* Catch the special case that one of the operands is an
	   unsized constant number. If so, then we should set the
	   width of that number to the size of the other operand, plus
	   one. This expands the expression to account for the largest
	   possible result.

	   The set_width applied to a constant value will only
	   truncate the constant so far as it can still hold its
	   logical value, so this is safe to do. */
      if ( (tmp = dynamic_cast<NetEConst*>(r))
	   && (! tmp->has_width())
	   && (tmp->expr_width() > l->expr_width()) ) {

	    unsigned target_width = l->expr_width() + 1;
	    r->set_width(target_width);

	      /* Note: This constant value will not gain a defined
		 width from this. Make sure. */
	    assert(! r->has_width() );

      } else if ( (tmp = dynamic_cast<NetEConst*>(l))
	   && (! tmp->has_width())
	   && (tmp->expr_width() > r->expr_width()) ) {

	    unsigned target_width = r->expr_width() + 1;
	    l->set_width(target_width);

	      /* Note: This constant value will not gain a defined
		 width from this. Make sure. */
	    assert(! l->has_width() );

      }

	/* Now that we have the operand sizes the way we like, or as
	   good as we are going to get them, set the size of myself. */
      if (r->expr_width() > l->expr_width()) {

	    expr_width(r->expr_width());

      } else {
	    expr_width(l->expr_width());
      }

      cast_signed(l->has_sign() && r->has_sign());
}

NetEBAdd::~NetEBAdd()
{
}

NetExpr* NetEBAdd::dup_expr() const
{
      NetEBAdd*result = new NetEBAdd(op_, left_->dup_expr(),
				     right_->dup_expr());
      return result;
}

NetExpr::TYPE NetEBAdd::expr_type() const
{
      if (left_->expr_type() == ET_REAL)
	    return ET_REAL;

      if (right_->expr_type() == ET_REAL)
	    return ET_REAL;

      return ET_VECTOR;
}

/*
 * Create a comparison operator with two sub-expressions.
 *
 * Handle the special case of an unsized constant on the left or right
 * side by resizing the number to match the other
 * expression. Otherwise, the netlist will have to allow the
 * expressions to have different widths.
 */
NetEBComp::NetEBComp(char op, NetExpr*l, NetExpr*r)
: NetEBinary(op, l, r)
{
	if (NetEConst*tmp = dynamic_cast<NetEConst*>(r)) do {
		
		if (tmp->has_width())
			break;
		
		if (l->expr_width() == 0)
			break;
		
		if (tmp->expr_width() == l->expr_width())
			break;
		
		tmp->set_width(l->expr_width());
		
	} while (0);
	
	if (NetEConst*tmp = dynamic_cast<NetEConst*>(l)) do {
		
		if (tmp->has_width())
			break;
		
		if (r->expr_width() == 0)
			break;
		
		if (tmp->expr_width() == r->expr_width())
			break;
		
		tmp->set_width(r->expr_width());
		
	} while (0);
	
	
	expr_width(1);
}

NetEBComp::~NetEBComp()
{
}

bool NetEBComp::has_width() const
{
      return true;
}

NetEBDiv::NetEBDiv(char op, NetExpr*l, NetExpr*r)
: NetEBinary(op, l, r)
{
      unsigned w = l->expr_width();
      if (r->expr_width() > w)
	    w = r->expr_width();

      expr_width(w);
      cast_signed(l->has_sign() && r->has_sign());
}

NetEBDiv::~NetEBDiv()
{
}

NetExpr* NetEBDiv::dup_expr() const
{
      NetEBDiv*result = new NetEBDiv(op_, left_->dup_expr(),
				       right_->dup_expr());
      return result;
}

NetExpr::TYPE NetEBDiv::expr_type() const
{
      if (left_->expr_type() == ET_REAL)
	    return ET_REAL;

      if (right_->expr_type() == ET_REAL)
	    return ET_REAL;

      return ET_VECTOR;
}

NetEBMult::NetEBMult(char op, NetExpr*l, NetExpr*r)
: NetEBinary(op, l, r)
{
	expr_width(l->expr_width() + r->expr_width());
	cast_signed(l->has_sign() && r->has_sign());
	
	/* If it turns out that this is not a signed expression, then
	   cast the signedness out of the operands as well. */
	if (! has_sign()) {
		l->cast_signed(false);
		r->cast_signed(false);
	}
}

NetEBMult::~NetEBMult()
{
}

NetExpr* NetEBMult::dup_expr() const
{
      NetEBMult*result = new NetEBMult(op_, left_->dup_expr(),
				       right_->dup_expr());
      return result;
}

NetExpr::TYPE NetEBMult::expr_type() const
{
      if (left_->expr_type() == ET_REAL)
	    return ET_REAL;

      if (right_->expr_type() == ET_REAL)
	    return ET_REAL;

      return ET_VECTOR;
}

NetEBShift::NetEBShift(char op, NetExpr*l, NetExpr*r)
: NetEBinary(op, l, r)
{
      expr_width(l->expr_width());

	// The >>> is signed if the left operand is signed.
      if (op == 'R') cast_signed(l->has_sign());
}

NetEBShift::~NetEBShift()
{
}

bool NetEBShift::has_width() const
{
      return left_->has_width();
}

NetExpr* NetEBShift::dup_expr() const
{
      NetEBShift*result = new NetEBShift(op_, left_->dup_expr(),
					 right_->dup_expr());
      return result;
}

NetEConcat::NetEConcat(unsigned cnt, NetExpr* r)
: parms_(cnt), repeat_(r)
{
      if (repeat_ == 0) {
	    repeat_calculated_ = true;
	    repeat_value_ = 1;
      } else {
	    repeat_calculated_ = false;
      }

      expr_width(0);
}

NetEConcat::~NetEConcat()
{
      for (unsigned idx = 0 ;  idx < parms_.count() ;  idx += 1)
	    delete parms_[idx];
}

bool NetEConcat::has_width() const
{
      return true;
}

void NetEConcat::seti(unsigned idx, NetExpr*e)
{
      assert(idx < parms_.count());
      //assert(parms_[idx] == 0);
      parms_[idx] = e;
      expr_width( expr_width() + e->expr_width() );
}

NetExpr* NetEConcat::dup_expr() const
{
      NetEConcat*dup = new NetEConcat(parms_.count(), repeat_);
      for (unsigned idx = 0 ;  idx < parms_.count() ;  idx += 1)
	    if (parms_[idx]) {
		  NetExpr*tmp = parms_[idx]->dup_expr();
		  assert(tmp);
		  dup->parms_[idx] = tmp;
	    }


      dup->expr_width(expr_width());
      return dup;
}

unsigned NetEConcat::repeat()
{
      if (repeat_calculated_)
	    return repeat_value_;

      assert(repeat_);

      if (! dynamic_cast<NetEConst*>(repeat_)) {
	    NetExpr*tmp = repeat_->eval_tree();
	    if (tmp != 0) {
		  delete repeat_;
		  repeat_ = tmp;
	    }
      }

      NetEConst*repeat_const = dynamic_cast<NetEConst*>(repeat_);

	/* This should not be possible, as it was checked earlier to
	   assure that this is a constant expression. */
      if (repeat_const == 0) {
	    cerr << get_line() << ": internal error: repeat expression "
		 << "is not a compile time constant." << endl;
	    cerr << get_line() << ":               : Expression is: "
		 << *repeat_ << endl;
	    repeat_calculated_ = true;
	    repeat_value_ = 1;
	    return 1;
      }

      repeat_calculated_ = true;
      repeat_value_ = repeat_const->value().as_ulong();

      delete repeat_;
      repeat_ = 0;

      return repeat_value_;
}

unsigned NetEConcat::repeat() const
{
      assert(repeat_calculated_);
      return repeat_value_;
}

NetECReal::NetECReal(const verireal&val)
: value_(val)
{
}

NetECReal::~NetECReal()
{
}

const verireal& NetECReal::value() const
{
      return value_;
}

bool NetECReal::has_width() const
{
      return false;
}

NetExpr* NetECReal::dup_expr() const
{
      NetECReal*tmp = new NetECReal(value_);
      tmp->set_line(*this);
      return tmp;
}

NetExpr::TYPE NetECReal::expr_type() const
{
      return ET_REAL;
}

NetECRealParam::NetECRealParam(NetScope*s, const char*n, const verireal&v)
: NetECReal(v), scope_(s), name_(n)
{
}

NetECRealParam::~NetECRealParam()
{
}

const char* NetECRealParam::name() const
{
      return name_;
}

const NetScope* NetECRealParam::scope() const
{
      return scope_;
}

NetEParam::NetEParam()
: des_(0), scope_(0)
{
}

NetEParam::NetEParam(Design*d, NetScope*s, const hname_t&n)
: des_(d), scope_(s), name_(n)
{
}

NetEParam::~NetEParam()
{
}

bool NetEParam::has_width() const
{
      return false;
}

NetExpr* NetEParam::dup_expr() const
{
      NetEParam*tmp = new NetEParam(des_, scope_, name_);
      tmp->set_line(*this);
      return tmp;
}

NetESelect::NetESelect(NetExpr*exp, NetExpr*base, unsigned wid)
: expr_(exp), base_(base)
{
      expr_width(wid);
}

NetESelect::~NetESelect()
{
      delete expr_;
      delete base_;
}

const NetExpr*NetESelect::sub_expr() const
{
      return expr_;
}

const NetExpr*NetESelect::select() const
{
      return base_;
}

bool NetESelect::has_width() const
{
      return true;
}

bool NetESelect::set_width(unsigned w)
{
      if (expr_width() == 1)
	    return true;
      else
	    return false;
}

NetESFunc::NetESFunc(const char*n, NetExpr::TYPE t,
		     unsigned width, unsigned np)
: name_(0), type_(t)
{
      name_ = lex_strings.add(n);
      expr_width(width);
      nparms_ = np;
      parms_ = new NetExpr*[np];
      for (unsigned idx = 0 ;  idx < nparms_ ;  idx += 1)
	    parms_[idx] = 0;
}

NetESFunc::~NetESFunc()
{
      for (unsigned idx = 0 ;  idx < nparms_ ;  idx += 1)
	    if (parms_[idx]) delete parms_[idx];

      delete[]parms_;
	/* name_ string ls lex_strings allocated. */
}

const char* NetESFunc::name() const
{
      return name_;
}

unsigned NetESFunc::nparms() const
{
      return nparms_;
}

void NetESFunc::parm(unsigned idx, NetExpr*v)
{
      assert(idx < nparms_);
      if (parms_[idx])
	    delete parms_[idx];
      parms_[idx] = v;
}

const NetExpr* NetESFunc::parm(unsigned idx) const
{
      assert(idx < nparms_);
      return parms_[idx];
}

NetExpr* NetESFunc::parm(unsigned idx)
{
      assert(idx < nparms_);
      return parms_[idx];
}

NetExpr::TYPE NetESFunc::expr_type() const
{
      return type_;
}