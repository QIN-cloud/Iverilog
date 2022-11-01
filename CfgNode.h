#ifndef __Cfg_Node_H
#define __Cfg_Node_H

#include <set>
#include <string>
#include <valarray>
#include "cfg.h"
#include "svector.h"

using namespace std;

class PExpr;
struct Cfg;
struct Module_Cfgs;

typedef struct Node{
	set<string> refs_, defs_;
	set<int, less<int> >infls_, dsuccs_;
	string type;
	string assign_type;
	int lineno_;
	set<string> funcs_;
	set<string> mods_;
	svector<PExpr*> expr_;
	list<PExpr*> caseitem_;
}Node;

std::ostream& operator << (std::ostream&, const Node&);

class CfgNode{
  public:
    CfgNode();
    CfgNode(int ln);
    ~CfgNode();

    unsigned ref_count();
    unsigned def_count();
    string find_ref(string& ref);
    string find_def(string& def);
    set<int, less<int> >& get_infls();
    set<int, less<int> >& get_dirsuccs();
    unsigned infl_count();
    unsigned dsucc_count();
    int find_infl(int infl);
    int find_dirsucc(int dsucc);
    
    void add_ref(string& ref);
    void add_def(string& def);
    void add_refs(svector<string>& refs);
    void add_refs(set<string>& refs);
    void add_defs(set<string>& defs);
    void add_defs(svector<string>& defs);
    void add_infl(svector<unsigned>&infl);
	void add_funcs(set<string>& funcs);
    void add_mods(set<string>& mods);
	
	virtual set<string>& get_funcs(){return funcs_;};
	virtual set<string>& get_mods(){return mods_;};	
    virtual set<string>& get_refs();
    virtual set<string>& get_defs();
    virtual void add_dsucc(int dsucc);
    virtual void dump(std::ostream&o);

    virtual void Ndump(std::ostream&o, int&nNo);
    virtual void Pdump(std::ostream&o, int&pNo);

    virtual svector<Node*>& construct_node(int);

	svector<PExpr*> get_expr(){return expr_;};
	void set_expr(svector<PExpr*> expr){expr_ = svector<PExpr*>(expr);};
	void set_expr(PExpr* expr){expr_ = svector<PExpr*>(expr_,expr);};
	virtual void set_caseitem(list<PExpr*> caseitem){caseitem_ = list<PExpr*>(caseitem);};
  virtual void set_caseitem(valarray<PExpr*> caseitem){
    for(unsigned i = 0; i < caseitem.size(); ++i)
      caseitem_.push_back(caseitem[i]);
  };
	list<PExpr*> get_caseitem(){return caseitem_;};

	void set_casetype(int t){casetype = t;}
	int get_casetype(){return casetype;}

	void set_assign_type(string s){assign_type_ = s;};
	string get_assign_type(){return assign_type_;};
    
    void set_lineno(int lineno);
    int get_lineno();
    
  protected:
    set<string> refs_, defs_, funcs_, mods_;
    set<int, less<int> >infls_, dsuccs_;
    int lineno_;
	svector<PExpr*> expr_;
	list<PExpr*> caseitem_;
	string assign_type_;
	int casetype;//0 for nomal case, 1 for casex, 2 for casez;
};

class BlockNode : public CfgNode {
  public:
      BlockNode();
      BlockNode(int ln);
      ~BlockNode();

      void add_node(CfgNode* node, int type);
      CfgNode* get_node(int idx);
      svector<CfgNode*>& get_nodes();
      int find_node(CfgNode& node);
      unsigned nodes_count(){return nodes_.count();}
      
      virtual set<string>& get_funcs();
	  virtual set<string>& get_mods();
	  virtual void add_dsucc(int dsucc);
      virtual set<string>& get_refs();      
      virtual set<string>& get_defs();      
      virtual void dump(std::ostream&o);

      virtual void Ndump(std::ostream&o, int&nNo);
      virtual void Pdump(std::ostream&o, int&pNo);

      virtual svector<Node*>& construct_node(int);

	  virtual void set_caseitem(list<PExpr*> caseitem){nodes_[0]->set_caseitem(caseitem);};

  private:
      CfgNode* root_;
      svector<CfgNode*> nodes_;
};

class CaseNode : public CfgNode {
  public :
    CaseNode();
    CaseNode(int ln);
    ~CaseNode();


    void add_node(CfgNode* node);
    svector<CfgNode*>& get_nodes();
    CfgNode* get_node(int idx);
    unsigned path_count();

    virtual set<string>& get_funcs();
	virtual set<string>& get_mods();
	virtual void add_dsucc(int dsucc);    
    virtual set<string>& get_refs();    
    virtual set<string>& get_defs();    
    virtual void dump(std::ostream&o);

    virtual void Ndump(std::ostream&o, int&nNo);
    virtual void Pdump(std::ostream&o, int&pNo);

    virtual svector<Node*>& construct_node(int);

	virtual void set_caseitem(list<PExpr*> caseitem){paths_[0]->set_caseitem(caseitem);};

  private:
    svector<CfgNode*> paths_;
};

class CondNode : public CfgNode{
  public:
    CondNode();
    CondNode(int ln);
    ~CondNode();

    void add_node(CfgNode* node, int type);    
    CfgNode* get_node(int type);

    virtual set<string>& get_funcs();
	virtual set<string>& get_mods();
	virtual void add_dsucc(int dsucc);    
    virtual set<string>& get_refs();    
    virtual set<string>& get_defs();    
    virtual void dump(std::ostream&o);

    virtual void Ndump(std::ostream&o, int&nNo);
    virtual void Pdump(std::ostream&o, int&pNo);

    virtual svector<Node*>& construct_node(int);

	virtual void set_caseitem(list<PExpr*> caseitem){root_->set_caseitem(caseitem);};

  private:
    CfgNode* root_;
    CfgNode* if_;
    CfgNode* else_;
};

class EventNode : public CfgNode{
  public:
    EventNode();
    EventNode(int ln);
    ~EventNode();
    void add_node(CfgNode* node, int type);    
    CfgNode* get_node(int type);

    virtual set<string>& get_funcs();
	virtual set<string>& get_mods();
	virtual void add_dsucc(int dsucc);    
    virtual set<string>& get_refs();    
    virtual set<string>& get_defs();    
    virtual void dump(std::ostream&o);

    virtual void Ndump(std::ostream&o, int&nNo);
    virtual void Pdump(std::ostream&o, int&pNo);

    virtual svector<Node*>& construct_node(int);

	virtual void set_caseitem(list<PExpr*> caseitem){root->set_caseitem(caseitem);};

  private:
    CfgNode* root;
    CfgNode* st;
};

class LoopNode : public CfgNode{
  public:
    LoopNode();
    LoopNode(int ln);
    ~LoopNode();
    void add_node(CfgNode* node, int type);    
    CfgNode* get_node(int type);

    virtual set<string>& get_funcs();
	virtual set<string>& get_mods();
	virtual void add_dsucc(int dsucc);    
    virtual set<string>& get_refs();    
    virtual set<string>& get_defs();    
    virtual void dump(std::ostream&o);

    virtual void Ndump(std::ostream&o, int&nNo);
    virtual void Pdump(std::ostream&o, int&pNo);

    virtual svector<Node*>& construct_node(int);

	virtual void set_caseitem(list<PExpr*> caseitem){root_->set_caseitem(caseitem);};

  private:
    CfgNode* root_;
    CfgNode* st_;
};

class ProcessNode : public CfgNode {
  public:
    ProcessNode();
    ProcessNode(int ln);
    ~ProcessNode();
    void add_node(CfgNode* node, int type);
    CfgNode* get_node(int type);
    
	virtual set<string>& get_funcs();
	virtual set<string>& get_mods();
    virtual set<string>& get_refs(){set<string>* zero = new set<string>; return *zero;};
    virtual set<string>& get_defs(){set<string>* zero = new set<string>; return *zero;};
    virtual void add_dsucc(int dsucc){};
    virtual void dump(std::ostream&o);

    virtual void Ndump(std::ostream&o, int&nNo);
    virtual void Pdump(std::ostream&o, int&pNo);

    virtual svector<Node*>& construct_node(int);

	  unsigned get_id(){return id_;};
	  void set_id(unsigned id){id_ = id;};

  	void set_sync(bool sync){sync_ = sync;}
	  bool get_sync(){return sync_;}

    void set_always(bool always){always_ = always;}
    bool get_always(){return always_;}
    
  private:
    CfgNode* root_;
    CfgNode* st_;
	  unsigned id_;
	  bool sync_;
    bool always_;
};

class GenerateNode : public CfgNode {
  public:
    GenerateNode():CfgNode(){}
    GenerateNode(int ln):CfgNode(ln){}
    ~GenerateNode(){}
    void add_node(CfgNode* node, int type);
    void add_node(ProcessNode* node, int type);
    void add_node(GenerateNode* node);
    CfgNode* get_node(int type);
    
  //virtual set<string>& get_funcs();
  //virtual set<string>& get_mods();
    virtual set<string>& get_refs(){set<string>* zero = new set<string>; return *zero;};
    virtual set<string>& get_defs(){set<string>* zero = new set<string>; return *zero;};
    virtual void add_dsucc(int dsucc){};
    virtual void dump(std::ostream&o);

    virtual void Ndump(std::ostream&o, int&nNo);
    //virtual void Pdump(std::ostream&o, int&pNo);

    virtual svector<Node*>& construct_node(int);
    
  private:
    svector<ProcessNode*> procs_, funcs_, tasks_;
    svector<GenerateNode*> generates_;
    svector<CfgNode*> assigns_;
    
    CfgNode* root_;
    CfgNode* st_;
};

class ModuleNode : public CfgNode {
  public:
    ModuleNode();
    ModuleNode(int ln);
    ~ModuleNode();
    void add_node(ProcessNode* node, int type); //type indicate to add node to which vector, 0 for proc, 1 for func and 2 for assign
    void add_node(CfgNode* node);
    void add_node(GenerateNode* node);
    svector<ProcessNode*> get_procnodes();
    svector<ProcessNode*> get_funcnodes();
    svector<ProcessNode*> get_tasknodes();
    svector<GenerateNode*> get_generatenodes();
    svector<CfgNode*> get_assignnodes();
    unsigned nodes_count(int type);
    CfgNode* get_procnode(int idx, int type);
    CfgNode* get_assignnode(int idx);
    int find_node(int lineno, int type);

    Cfg* build_cfg(ProcessNode*);
	Module_Cfgs* build_cfgs();
    
	virtual set<string>& get_funcs();
	virtual set<string>& get_mods();
    virtual set<string>& get_refs(){set<string>* zero = new set<string>; return *zero;};
    virtual set<string>& get_defs(){set<string>* zero = new set<string>; return *zero;};
    virtual void add_dsucc(int dsucc){};
    virtual void dump(std::ostream&o);

    virtual void Ndump(std::ostream&o, int&nNo);
    virtual void Pdump(std::ostream&o, int&pNo);

    virtual svector<Node*>& construct_node(int);
  private:
    svector<ProcessNode*> procs_, funcs_, tasks_;
    svector<GenerateNode*> generates_;
    svector<CfgNode*> assigns_;
};

#endif
