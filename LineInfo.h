#ifndef __LineInfo_H
#define __LineInfo_H

#include <string>

using namespace std;

/*
 * This class holds line information for an internal object.
 *
 * Note that the file names are C-style strings that are allocated by
 * the lexor (which parses the line directives) and are never
 * deallocated. We can therefore safely store the pointer and never
 * delete the string, even if LineInfo objects are destroyed.
 */

class LineInfo {
    public:
      LineInfo() : file_(0), lineno_(0) { }
      ~LineInfo();

      string get_line() const;
      int get_lineno() const;
      string get_filename() const;

      void set_line(const LineInfo&that);

      void set_file(const char*f) { file_ = f; }
      void set_lineno(unsigned n) { lineno_ = n; }

    private:
      const char* file_;
      unsigned lineno_;
};

#endif
