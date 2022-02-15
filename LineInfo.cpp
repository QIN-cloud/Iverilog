#include "config.h"

#include "LineInfo.h"
#include <cstdio>

LineInfo::~LineInfo()
{
}

string LineInfo::get_line() const
{
      char buf[8];
      sprintf(buf, "%u", lineno_);
      return string(file_? file_ : "") + ":" + buf;
}

void LineInfo::set_line(const LineInfo&that)
{
      file_ = that.file_;
      lineno_ = that.lineno_;
}

int LineInfo::get_lineno() const
{
	return lineno_;
}

string LineInfo::get_filename() const
{
	return string(file_);
}
