#ifndef IVL_PDelays_H
#define IVL_PDelays_H
/*
<<<<<<< Updated upstream
 * Copyright (c) 1999-2021 Stephen Williams (steve@icarus.com)
=======
 * Copyright (c) 1999-2014 Stephen Williams (steve@icarus.com)
>>>>>>> Stashed changes
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

# include  "svector.h"
<<<<<<< Updated upstream
=======
# include  "PExpr.h"
>>>>>>> Stashed changes
# include  <string>
# include  <list>
# include  <iostream>

class Design;
class NetScope;
class NetExpr;
class PExpr;

class Design;
class NetScope;
class NetExpr;
class PExpr;

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
<<<<<<< Updated upstream
      void set_delays(const std::list<PExpr*>*del, bool delete_flag=true);
=======
      void set_delays(const list<PExpr*>*del, bool delete_flag=true);
>>>>>>> Stashed changes

      unsigned delay_count() const;

      void eval_delays(Design*des, NetScope*scope,
		       NetExpr*&rise_time,
		       NetExpr*&fall_time,
		       NetExpr*&decay_time,
		       bool as_nets_flag =false) const;

<<<<<<< Updated upstream
      void dump_delays(std::ostream&out) const;
=======
      void dump_delays(ostream&out) const;
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
>>>>>>> Stashed changes

    private:
      PExpr* delay_[3];
      bool delete_flag_;

    private: // not implemented
      PDelays(const PDelays&);
      PDelays& operator= (const PDelays&);
};

ostream& operator << (ostream&o, const PDelays&);

#endif /* IVL_PDelays_H */
