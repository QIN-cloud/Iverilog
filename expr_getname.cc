# include "PExpr.h"
# include "ivl_assert.h"

set<string> merge(set<string> l, set<string> r);

set<string> PExpr::get_var_names()
{
    set<string> null_set;
    return null_set;
}

set<string> PEConcat::get_var_names()
{
    set<string> res;
    for(unsigned i = 0; i < parms_.size(); i++)
    {
        set<string> temp = parms_[i]->get_var_names();
        res = merge(res,temp);
    }
    return res;
}

set<string> PEIdent::get_var_names()
{
    set<string> res;
    res.insert(path_.back().name.str());
    return res;
}

set<string> PEUnary::get_var_names()
{
    return expr_->get_var_names();
}

set<string> PEBinary::get_var_names()
{
    set<string> lnames = left_->get_var_names();
    set<string> rnames = right_->get_var_names();
    return merge(lnames, rnames);
}

set<string> PETernary::get_var_names()
{
    set<string> expr_names = expr_->get_var_names();
    set<string> true_names = tru_->get_var_names();
    set<string> false_names = fal_->get_var_names();
    return merge(merge(true_names, false_names), expr_names);
}

set<string> merge(set<string> l, set<string> r)
{
    set<string> res;
    for(string s : l){
        res.insert(s);
    }
    for(string s : r){
        res.insert(s);
    }
    return res;
}
