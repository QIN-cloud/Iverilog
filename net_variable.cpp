#include "config.h"
#include "netlist.h"

NetVariable::NetVariable(const char*name)
{
      name_ = name;
      scope_ = 0;
      snext_ = 0;
}

NetVariable::~NetVariable()
{
}

const char*NetVariable::basename() const
{
      return name_;
}

NetScope* NetVariable::scope()
{
      return scope_;
}

const NetScope* NetVariable::scope() const
{
      return scope_;
}

NetEVariable::NetEVariable(NetVariable*v)
: var_(v)
{
}

NetEVariable::~NetEVariable()
{
}

NetExpr::TYPE NetEVariable::expr_type() const
{
      return NetExpr::ET_REAL;
}

const NetVariable* NetEVariable::variable() const
{
      return var_;
}