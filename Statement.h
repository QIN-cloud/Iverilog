#ifndef __Statement_H
#define __Statement_H

#include <string>
#include <map>
#include <set>
#include "svector.h"
#include "PDelays.h"
#include "PExpr.h"
#include "HName.h"
#include "LineInfo.h"
#include "slice.h"

class PExpr;
class Statement;
class PEventStatement;
class CfgNode;
class ProcessNode;
class PDesign;
class Design;

/*
 * The PProcess is the root of a behavioral process. Each process gets
 * one of these, which contains its type (initial or always) and a
 * pointer to the single statement that is the process. A module may
 * have several concurrent processes.
 */
class PProcess : public LineInfo {

    public:
      enum Type { PR_INITIAL, PR_ALWAYS };

      PProcess(Type t, Statement*st)
      : type_(t), statement_(st) { }

      virtual ~PProcess();

      Type type() const { return type_; }
      Statement*statement() { return statement_; }

      map<string,PExpr*> attributes;
      
      ProcessNode* build_node(PDesign& de);

	  bool is_synchronous(){return sync_;};
	  void set_synchronous(bool sync) {sync_ = sync;};
	  unsigned get_id(){return id_;};
	  void set_id(unsigned id){id_ = id;};

      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer* s) const;
	  virtual set<string>& get_funcs();

    private:
      Type type_;
      Statement*statement_;

  	  bool sync_;
	  unsigned id_;
};

/*
 * The PProcess is a process, the Statement is the actual action. In
 * fact, the Statement class is abstract and represents all the
 * possible kinds of statements that exist in Verilog.
 */
class Statement : public LineInfo {

    public:
      Statement() { }
      virtual ~Statement() =0;
      
      virtual CfgNode* build_node(PDesign& de);
      virtual svector<unsigned>& get_linenos();
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;
      virtual void elaborate_scope(Design*des, NetScope*scope) const;

	protected:
	  set<string> funcs_;

};

/*
 * Assignment statements of the various forms are handled by this
 * type. The rvalue is an expression. The lvalue needs to be figured
 * out by the parser as much as possible.
 */
class PAssign_  : public Statement {
    public:
      explicit PAssign_(PExpr*lval, PExpr*ex);
      explicit PAssign_(PExpr*lval, PExpr*de, PExpr*ex);
      explicit PAssign_(PExpr*lval, PEventStatement*de, PExpr*ex);
      virtual ~PAssign_() =0;

      const PExpr* lval() const { return lval_; }
      const PExpr* rval() const { return rval_; }

    protected:
      PExpr* delay_;
      PEventStatement*event_;
      PExpr* lval_;
      PExpr* rval_;

	  NetAssign_* elaborate_lval(Design*, NetScope*scope) const;
};

class PAssign  : public PAssign_ {

    public:
      explicit PAssign(PExpr*lval, PExpr*ex);
      explicit PAssign(PExpr*lval, PExpr*de, PExpr*ex);
      explicit PAssign(PExpr*lval, PEventStatement*de, PExpr*ex);
      ~PAssign();

      virtual CfgNode* build_node(PDesign& de);
      virtual void dump(std::ostream&out, unsigned ind) const;      
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;

    private:
};

class PAssignNB  : public PAssign_ {

    public:
      explicit PAssignNB(PExpr*lval, PExpr*ex);
      explicit PAssignNB(PExpr*lval, PExpr*de, PExpr*ex);
      ~PAssignNB();

      virtual CfgNode* build_node(PDesign& de);
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;
};

/*
 * A block statement is an ordered list of statements that make up the
 * block. The block can be sequential or parallel, which only affects
 * how the block is interpreted. The parser collects the list of
 * statements before constructing this object, so it knows a priori
 * what is contained.
 */
class PBlock  : public Statement {

    public:
      enum BL_TYPE { BL_SEQ, BL_PAR };

      explicit PBlock(const string&n, BL_TYPE t, const svector<Statement*>&st);
      explicit PBlock(BL_TYPE t, const svector<Statement*>&st);
      explicit PBlock(BL_TYPE t);
      ~PBlock();

      BL_TYPE bl_type() const { return bl_type_; }

      virtual CfgNode* build_node(PDesign& de);
      virtual svector<unsigned>& get_linenos();
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;
      virtual void elaborate_scope(Design*des, NetScope*scope) const;

    private:
      string name_;
      const BL_TYPE bl_type_;
      svector<Statement*>list_;
};

class PCallTask  : public Statement {

    public:
      explicit PCallTask(const hname_t&n, const svector<PExpr*>&parms);
      ~PCallTask();

      const hname_t& path() const;

      unsigned nparms() const { return parms_.count(); }

      PExpr*&parm(unsigned idx)
	    { assert(idx < parms_.count());
	      return parms_[idx];
	    }

      PExpr* parm(unsigned idx) const
	    { assert(idx < parms_.count());
	      return parms_[idx];
	    }

      virtual CfgNode* build_node(PDesign& de);
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

      virtual NetProc* elaborate(Design*des, NetScope*scope) const;

    private:
      NetProc* elaborate_sys(Design*des, NetScope*scope) const;
      NetProc* elaborate_usr(Design*des, NetScope*scope) const;

      hname_t path_;
      svector<PExpr*> parms_;
};

class PCase  : public Statement {

    public:
      //enum TYPE { EQ, EQX, EQZ };
      struct Item {
	    svector<PExpr*>expr;
	    Statement*stat;
      };

      PCase(NetCase::TYPE, PExpr*ex, svector<Item*>*);
      ~PCase();

      virtual CfgNode* build_node(PDesign& de);
      virtual svector<unsigned>& get_linenos();
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;
      virtual void elaborate_scope(Design*des, NetScope*scope) const;

    private:
	  NetCase::TYPE type_;
      PExpr*expr_;

      svector<Item*>*items_;

    private: // not implemented
      PCase(const PCase&);
      PCase& operator= (const PCase&);
};

class PCAssign  : public Statement {

    public:
      explicit PCAssign(PExpr*l, PExpr*r);
      ~PCAssign();

      virtual CfgNode* build_node(PDesign& de);
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;

    private:
      PExpr*lval_;
      PExpr*expr_;
};

class PCondit  : public Statement {

    public:
      PCondit(PExpr*ex, Statement*i, Statement*e);
      ~PCondit();

      virtual CfgNode* build_node(PDesign& de);
      virtual svector<unsigned>& get_linenos();
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;
      virtual void elaborate_scope(Design*des, NetScope*scope) const;

    private:
      PExpr*expr_;
      Statement*if_;
      Statement*else_;

    private: // not implemented
      PCondit(const PCondit&);
      PCondit& operator= (const PCondit&);
};

class PDeassign  : public Statement {

    public:
      explicit PDeassign(PExpr*l);
      ~PDeassign();

      virtual CfgNode* build_node(PDesign& de);
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;

    private:
      PExpr*lval_;
};

class PDelayStatement  : public Statement {

    public:
      PDelayStatement(PExpr*d, Statement*st);
      ~PDelayStatement();

      virtual CfgNode* build_node(PDesign& de);
      virtual svector<unsigned>& get_linenos();
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;
      virtual void elaborate_scope(Design*des, NetScope*scope) const;

    private:
      PExpr*delay_;
      Statement*statement_;
};

/*
 * This represends the parsing of a disable <scope> statement.
 */
class PDisable  : public Statement {

    public:
      explicit PDisable(const hname_t&sc);
      ~PDisable();

      //virtual CfgNode* build_node(PDesign& de);
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;

    private:
      hname_t scope_;
};

/*
 * The event statement represents the event delay in behavioral
 * code. It comes from such things as:
 *
 *      @name <statement>;
 *      @(expr) <statement>;
 *      @* <statement>;
 */
class PEventStatement  : public Statement {

    public:

      explicit PEventStatement(const svector<PEEvent*>&ee);
      explicit PEventStatement(PEEvent*ee);
	// Make an @* statement.
      explicit PEventStatement(void);

      ~PEventStatement();

      void set_statement(Statement*st);

      virtual CfgNode* build_node(PDesign& de);
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;
      virtual void elaborate_scope(Design*des, NetScope*scope) const;

	  // This method is used to elaborate, but attach a previously
	  // elaborated statement to the event.
      NetProc* elaborate_st(Design*des, NetScope*scope, NetProc*st) const;

	  NetProc* elaborate_wait(Design*des, NetScope*scope, NetProc*st) const;
    private:
      svector<PEEvent*>expr_;
      Statement*statement_;
};

class PForce  : public Statement {

    public:
      explicit PForce(PExpr*l, PExpr*r);
      ~PForce();

      virtual CfgNode* build_node(PDesign& de);
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;

    private:
      PExpr*lval_;
      PExpr*expr_;
};

class PForever : public Statement {
    public:
      explicit PForever(Statement*s);
      ~PForever();

      virtual CfgNode* build_node(PDesign& de);
      virtual svector<unsigned>& get_linenos();
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;
      virtual void elaborate_scope(Design*des, NetScope*scope) const;

    private:
      Statement*statement_;
};

class PForStatement  : public Statement {

    public:
      PForStatement(PExpr*n1, PExpr*e1, PExpr*cond,
		    PExpr*n2, PExpr*e2, Statement*st);
      ~PForStatement();

      virtual CfgNode* build_node(PDesign& de);
      virtual svector<unsigned>& get_linenos();
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;
      virtual void elaborate_scope(Design*des, NetScope*scope) const;

    private:
      PExpr* name1_;
      PExpr* expr1_;

      PExpr*cond_;

      PExpr* name2_;
      PExpr* expr2_;

      Statement*statement_;
};

class PNoop  : public Statement {

    public:
      PNoop() { }
};

class PRepeat : public Statement {
    public:
      explicit PRepeat(PExpr*expr, Statement*s);
      ~PRepeat();

      virtual CfgNode* build_node(PDesign& de);
      virtual svector<unsigned>& get_linenos();
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;
      virtual void elaborate_scope(Design*des, NetScope*scope) const;

    private:
      PExpr*expr_;
      Statement*statement_;
};

class PRelease  : public Statement {

    public:
      explicit PRelease(PExpr*l);
      ~PRelease();

      virtual CfgNode* build_node(PDesign& de);
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;

    private:
      PExpr*lval_;
};

/*
 * The PTrigger statement sends a trigger to a named event. Take the
 * name here.
 */
class PTrigger  : public Statement {

    public:
      explicit PTrigger(const hname_t&ev);
      ~PTrigger();

      virtual CfgNode* build_node(PDesign& de);
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;

    private:
      hname_t event_;
};

class PWhile  : public Statement {

    public:
      PWhile(PExpr*e1, Statement*st);
      ~PWhile();

      virtual CfgNode* build_node(PDesign& de);
      virtual svector<unsigned>& get_linenos();
      virtual void dump(std::ostream&out, unsigned ind) const;
	  virtual void dump_slice(std::ostream&out, unsigned ind, slicer*) const;
	  virtual set<string>& get_funcs();

	  virtual NetProc* elaborate(Design*des, NetScope*scope) const;
      virtual void elaborate_scope(Design*des, NetScope*scope) const;

    private:
      PExpr*cond_;
      Statement*statement_;
};

#endif
