#ifndef __PDelays_H
#define __PDelays_H

#include "svector.h"
#include <string>
#include <iostream>
#include "PExpr.h"

#ifdef __GNUC__
#if __GNUC__ > 2
using namespace std;
#endif
#endif

/*
 * Various PForm objects can carry delays. These delays include rise,
 * fall and decay times. This class arranges to carry the triplet.
 */
class PDelays {

    public:
      PDelays();
      ~PDelays();

	/* Set the delay expressions. If the delete_flag is true, then
	   this object takes ownership of the expressions, and will
	   delete it in the destructor. */
      void set_delay(PExpr*);
      void set_delays(const svector<PExpr*>*del, bool delete_flag=true);

      void dump_delays(std::ostream&out) const;

	  void eval_delays(Design*des, NetScope*scope,
		       unsigned long&rise_time,
		       unsigned long&fall_time,
		       unsigned long&decay_time) const;
      
      svector<string>& get_vars()
      {
      	  svector<string>* tmp = new svector<string>(0);
      	  for(unsigned idx = 0; idx < 3; ++idx)
      	  {
      	      if(delay_[idx] !=0)
      	  	  tmp = new svector<string>(*tmp, delay_[idx]->get_vars());
      	  }
      	  return *tmp;
      };

    private:
      PExpr* delay_[3];
      bool delete_flag_;

    private: // not implemented
      PDelays(const PDelays&);
      PDelays& operator= (const PDelays&);
};

std::ostream& operator << (std::ostream&o, const PDelays&);

#endif
