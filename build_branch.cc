#include<Module.h>
#include<Statement.h>
#include<PExpr.h>
#include<PGate.h>
#include<vcdvar.h>

void Module::build_branchs()
{
    for(PGAssign* assign : assigns_) {
        assign->build_branch(this, branchs_);
    }
    for(PProcess* proc : behaviors) {
        proc->build_branch(this, branchs_, nullptr);
    }
}

void PGAssign::build_branch(Module* md, map<unsigned, BranchTree*>& branchs)
{
    pin(1)->build_branch(md, branchs, nullptr);
}

BranchTree* PExpr::build_branch(Module* md, map<unsigned, BranchTree*>& branchs, BranchTree* root)
{
    BrLeaf* node = nullptr;
    if(root) {
        node = new BrLeaf(md, BranchTree::EXPRLEAF);
        node->node_.expr_ = this;
    }
    return node;
}

BranchTree* PETernary::build_branch(Module* md, map<unsigned, BranchTree*>& branchs, BranchTree* root)
{
    BrCondit* node = new BrCondit(md, BranchTree::TERNARY);
    node->cond_ = expr_;
    node->lineno_ = get_lineno();
    node->tru_ = tru_->build_branch(md, branchs, node);
    node->fal_ = fal_->build_branch(md, branchs, node);
    if(!root) branchs[get_lineno()] = node;
    return node;
}

void PProcess::build_branch(Module* md, map<unsigned, BranchTree*>& branchs, BranchTree* root)
{
    if(statement_) statement_->build_branch(md, branchs, root);
}

BranchTree* Statement::build_branch(Module* md, map<unsigned, BranchTree*>& branchs, BranchTree* root)
{
    return nullptr;
}

BranchTree* PEventStatement::build_branch(Module* md, map<unsigned, BranchTree*>& branchs, BranchTree* root)
{
    if(statement_) statement_->build_branch(md, branchs, root);
    return nullptr;
}

BranchTree* PBlock::build_branch(Module* md, map<unsigned, BranchTree*>& branchs, BranchTree* root)
{
    BrBlock* node = nullptr;
    cout << get_lineno() << " " << root << endl;
    if(root) {
        node = new BrBlock(md, BranchTree::BLOCK);
        for(Statement* st : list_) {
            if(PAssign_* ag = dynamic_cast<PAssign_*>(st)) {
                if(node->stats_.empty()) {
                    BranchTree* sn = ag->build_branch(md, branchs, node);
                    node->stats_.push_back(sn);
                }
            } else {
                BranchTree* sn = st->build_branch(md, branchs, node);
                if(node->stats_.front()->type_ == BranchTree::STATLEAF) {
                    BranchTree* temp = node->stats_.front();
                    node->stats_.pop_back();
                    delete temp;
                }
                node->stats_.push_back(sn);
            }
        }
    } else {
        for(Statement* st : list_) {
            st->build_branch(md, branchs, root);
        }
    }
    return node;
}

BranchTree* PAssign_::build_branch(Module* md, map<unsigned, BranchTree*>& branchs, BranchTree* root)
{
    BrLeaf* node = nullptr;
    if(root) {
        node = new BrLeaf(md, BranchTree::STATLEAF);
        node->node_.lineno = get_lineno();
    }
    return node;
}

BranchTree* PCondit::build_branch(Module* md, map<unsigned, BranchTree*>& branchs, BranchTree* root)
{
    BrCondit* node = new BrCondit(md, BranchTree::IF);
    node->lineno_ = get_lineno();
    node->cond_ = expr_;
    node->tru_ = if_->build_branch(md, branchs, node);
    if(else_) {
        node->fal_ = else_->build_branch(md, branchs, node);
    } else {
        node->fal_ = nullptr;
        cerr << get_fileline() << " MISSING ELSE!" << endl;
    }
    if(!root) branchs[get_lineno()] = node;
    return node;
}

BranchTree* PCase::build_branch(Module* md, map<unsigned, BranchTree*>& branchs, BranchTree* root)
{
    BranchTree::TreeType type;
    switch (type_) {
    case NetCase::EQ :
        type = BranchTree::CASE;
        break;
    case NetCase::EQX : 
        type = BranchTree::CASEX;
        break;
    case NetCase::EQZ :
        type = BranchTree::CASEZ;
        break;
    default:
        break;
    }
    BrCase* node = new BrCase(md, type);
    node->cond_ = expr_;
    node->lineno_ = get_lineno();
    for(unsigned i = 0; i < items_->count(); i++) {
        BranchTree* sn = (*(items_))[i]->stat->build_branch(md, branchs, node);
        if(!(*(items_))[i]->expr.empty()) {
            if(i == (items_->count()-1)) {
                cerr << get_fileline() << " MISSING DEFAULT" << endl;
            }
            node->items_.push_back(make_pair((*(items_))[i]->expr.front(), sn));
        } else {
            node->items_.push_back(make_pair(nullptr, sn));
        }
    }
    if(!root) branchs[get_lineno()] = node;
    return node;
}