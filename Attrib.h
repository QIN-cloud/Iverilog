#ifndef __Attrib_H
#define __Attrib_H

#include <string>
#include "verinum.h"

/*
 * This class keeps a map of key/value pairs. The map can be set from
 * an STL map, or by setting individual mappings.
 */
class Attrib {

    public:
      Attrib();
      ~Attrib();

      const verinum&attribute(const string&key) const;
      void attribute(const string&key, const verinum&value);
      bool has_compat_attributes(const Attrib&that) const;


	/* Provide a means of iterating over the entries in the map. */
      unsigned       attr_cnt() const;
      const char*    attr_key(unsigned idx) const;
      const verinum& attr_value(unsigned idx) const;


    private:
      struct cell_ {
	    string  key;
	    verinum val;
      };

      unsigned nlist_;
      struct cell_*list_;

    private: // not implemented
      Attrib(const Attrib&);
      Attrib& operator= (const Attrib&);
};

#endif
