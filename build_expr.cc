#include <PExpr.h>
#include <PGate.h>
#include  "ivl_assert.h"

void PGAssign::build_expr(map<PExpr*, set<PExpr*> >& exprs) const
{
    pin(1)->build_expr(exprs, nullptr);
}

void PExpr::build_expr(map<PExpr*, set<PExpr*> >& exprs, PEBinary* binary)
{
    return;
}

void PEFNumber::build_expr(map<PExpr*, set<PExpr*> >& exprs, PEBinary* binary)
{
    if(binary){
        exprs[binary].insert(this);
    }
}

void PEIdent::build_expr(map<PExpr*, set<PExpr*> >& exprs, PEBinary* binary)
{
    if(binary){
        exprs[binary].insert(this);
    }
}

void PEUnary::build_expr(map<PExpr*, set<PExpr*> >& exprs, PEBinary* binary)
{
    if(binary){
        exprs[binary].insert(this);
    }
    expr_->build_expr(exprs, nullptr);
}

void PEBinary::build_expr(map<PExpr*, set<PExpr*> >& exprs, PEBinary* binary)
{
    if((op_ == 'a' || op_ == 'o')){
        if(binary && op_ == binary->get_op()){
            left_->build_expr(exprs, binary);
            right_->build_expr(exprs, binary);
        }
        else{
            left_->build_expr(exprs, this);
            right_->build_expr(exprs, this);
            if(binary)
                exprs[binary].insert(this);
        }
    }
    else{
        left_->build_expr(exprs, nullptr);
        right_->build_expr(exprs, nullptr);
        if(binary)
            exprs[binary].insert(this);
    }
}

void PETernary::build_expr(map<PExpr*, set<PExpr*> >& exprs, PEBinary* binary)
{
    expr_->build_expr(exprs, nullptr);
    tru_->build_expr(exprs, nullptr);
    fal_->build_expr(exprs, nullptr);
    exprs[this].insert(expr_);
}

