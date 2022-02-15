#include "config.h"

#include "PEvent.h"

PEvent::PEvent(const string&n)
: name_(n)
{
}

PEvent::~PEvent()
{
}

string PEvent::name() const
{
      return name_;
}
