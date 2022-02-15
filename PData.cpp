#include "config.h"
#include "PData.h"

PData::PData(const hname_t&h)
: hname_(h)
{
}

PData::~PData()
{
}

const hname_t&PData::name() const
{
      return hname_;
}