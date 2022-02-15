#ifndef __named_H
#define __named_H

#include <string>

/*
 * There are lots of places where names are attached to objects. This
 * simple template expresses the lot.
 */

template <class T> struct named {
      string name;
      T parm;
};

#endif
