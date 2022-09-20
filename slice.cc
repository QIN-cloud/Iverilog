//#pragma warning(disable:4786)

#include <iostream>
#include <string>
#include <string.h>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <set>
#include "slice.h"

using namespace std;

#define VAR_LEN 50
#define NODE_MAX 2500

using namespace std;

typedef set<string> SET_STR;

typedef set<string>::iterator SET_PTR;

class link;

class aggregate
{
public:
	SET_STR elements;
	int count;
//	char temp[50];
	
//	char * x(int i);
	aggregate();
	~aggregate();
	aggregate(aggregate &other);
	aggregate operator +(aggregate b);
	aggregate operator *(aggregate &b);
	aggregate operator -(aggregate &b);
	bool operator ==(aggregate &b);
	void operator =(const aggregate& b);
	bool operator <=(aggregate &b);
	bool operator >=(string ele);
	void add_element(string element);
	//	bool exist(const char e[VAR_LENGTH]);
	//	void delete_element(char element[VAR_LENGTH]);
	void delete_all(void);
	void output(void);
};

/*
char * aggregate::x(int i)
{
	SET_PTR sp;
	sp = elements.begin();
	int j;
	for(j = 0; j < i ; j ++ )//sp != b.elements.end() ; sp++)
	{
		if(sp == elements.end())
			return temp;
		sp++;//if(temp.elements.find(*sp) == temp.elements.end())
	}
	if(sp != elements.end())
		strcpy(temp,(*sp).c_str());
	return temp;
}
*/

aggregate::aggregate()
{
	elements.clear();
	count = 0;
}

aggregate::~aggregate()
{
	elements.clear();
	count = 0;
}

aggregate::aggregate(aggregate & other)
{
	elements.clear();
	elements = other.elements;
	count = other.count;
}

aggregate aggregate::operator +(aggregate b)
{
	aggregate temp;
	temp = (*this);
	SET_PTR sp;
	for(sp = b.elements.begin(); sp != b.elements.end() ; sp++)
		temp.elements.insert(*sp);
	temp.count = temp.elements.size();
	return temp;
}
aggregate aggregate:: operator *(aggregate &b)
{
	aggregate temp;
	SET_PTR sp;
	for(sp = b.elements.begin(); sp != b.elements.end() ; sp++)
		if(elements.find(*sp) != elements.end())
			temp.elements.insert(*sp);
	temp.count = temp.elements.size();
	return temp;
}

aggregate aggregate:: operator -(aggregate &b)
{
	aggregate temp;
	temp = (*this);
	SET_PTR sp;
	for(sp = b.elements.begin(); sp != b.elements.end() ; sp++)
		if(temp.elements.find(*sp) != temp.elements.end() )
			temp.elements.erase(*sp);
	temp.count = temp.elements.size();
	return temp;
}

bool aggregate::operator ==(aggregate &b)
{
	if(count != b.count )
		return false;
	SET_PTR sp;
	for(sp = elements.begin(); sp != elements.end() ; sp++)
		if(b.elements.find(*sp) == b.elements.end())
			return false;
		return true;
}

void aggregate::operator =(const aggregate& b)
{
	elements = b.elements;
	count = b.count;
}

bool aggregate::operator <=(aggregate &b)
{
	SET_PTR sp;
	for(sp = elements.begin(); sp != elements.end() ; sp++)
		if(b.elements.find(*sp) == b.elements.end())
			return false;
		return true;
}

bool aggregate::operator >=(string ele)
{
	if(elements.find(ele) == elements.end())
		return false;
	return true;
}

void aggregate::add_element(string element)
{
	elements.insert(element);
	count = elements.size();
}

void aggregate::output(void)
{
	SET_PTR sp;
	for(sp = elements.begin(); sp != elements.end() ; sp++)
		cout << *sp << " , ";
	cout << endl;
}

void aggregate::delete_all(void)
{
	elements.clear();
	count = 0;
}


class node
{
public:
	aggregate def,ref,control,R;
	
	bool sel;			//�Ƿ�ѡ��
	
	bool control_sel;	//�Ƿ�ѡ�� ���ƽڵ�
	
	bool work_through;	//�Ƿ� �߹���������
	
	bool end_node;		//�Ƿ��� ��ֹ�ڵ�
	
	int next;			//ָ���� ���ƽڵ㣨��always,initial�ڵ㣩ָ���� ��ֹ�ڵ㣨always,initial�ڵ㣩
	
	char kind[30];		//�ڵ�����
	
	bool b_exist;		//�ýڵ��Ƿ񱻸�ֵ�����Ƿ� ʹ��
	
	link *pre_link;
	
	char self[6];		//�������
	
	int num_of_pre(int);	//�ýڵ��ǰ�̽ڵ������㺯��
	
	set<string> funcs, mods;

	int lineno;
	
};

class link
{
public:
	link *next;
	node *pre_line;
};

class pipe
{public:
node *ele[500];
int num;

pipe(void)
{
	num=0;
}
void push(node *e);
node *pop(void);


};

void pipe::push(node *e)
{
	ele[num]=e;
	num++;
}

node *pipe::pop(void)
{
	int i;
	node *temp;
	temp=ele[0];
	if(num==0)return NULL;
	num--;
	for(i=0;i<num;i++)
		ele[i]=ele[i+1];
	return temp;
}

void tree_view(node *ab_end,node *root_end,node *me);

int node::num_of_pre(int type)
{
	link *temp;
	bool all_are_control=true;
	bool only_one_is_control=false;
	int num=0;
	if(type==1){				//���ص���������control����Ŀ
		temp=this->pre_link;
		if(temp==NULL)return 0;
		while(temp!=NULL)
		{
			num++;
			temp=temp->next;
		}
		return num;
	}
	if(type==2)
	{						//�����ҽ���һ�ڵ㲻����controlʱ������1
		temp=this->pre_link;			//�����нڵ㶼����controlʱ������0
		if(temp==NULL)return 0;			//�����нڵ㶼������controlʱ������2
		while(temp!=NULL)
		{
			if(!(this->control>=(temp->pre_line->self)))only_one_is_control=true;//return 1;
			else all_are_control=false;
			temp=temp->next;
		}
		if(all_are_control)return 2;
		if(!all_are_control&&only_one_is_control)return 1;
		return 0;
		
	}
}




node n[NODE_MAX];

int num_of_node;
int num_of_side;
int line_to_sli;
int top_num=NODE_MAX-1;

int lines_to_slice[NODE_MAX];
int var_ptr=0;

pipe p_PDG;

char filename[20];

bool finish;

void add_DEF_REF(node *n_be_add,node *n_to_add);
void make_gra(int,int);
void cal_S(node *ab_end,node *root_end,node *me);
void add_control(node *n_block);
void reset(void);
void reset(node *n_block);
void sel_lines_in_block(node *);
void add_pipe(node *);
void get_var_lines(char* var);
void reCal_al_in(node *);
void work()
{
	int i,j;
	node *temp;
	
	for(j=0;j<var_ptr;j++)
	{
		line_to_sli=lines_to_slice[j];
		n[line_to_sli].R=n[line_to_sli].ref+n[line_to_sli].R;
		//n[line_to_sli].sel=true;
		finish=false;
		while(!finish)
		{
			finish=true;
			if((!strcmp(n[line_to_sli].kind,"PROCESS"))||(!strcmp(n[line_to_sli].kind,"ASSIGN")))
				temp=&n[line_to_sli];
			else
				temp=&n[n[line_to_sli].next];
			reset();		//��λȫ�� �߹� ���
			tree_view(&n[line_to_sli],&n[0],&n[line_to_sli]);
						
			reset(temp);
			cal_S(&n[line_to_sli],&n[0],&n[line_to_sli]);
			sel_lines_in_block(temp);
			add_control(temp);
			reCal_al_in(temp);
			add_pipe(temp);
			temp=p_PDG.pop();
			while(temp!=NULL)
			{
				tree_view(&n[temp->next],&n[0],&n[temp->next]);
				reset(temp);
				cal_S(&n[temp->next],&n[0],&n[temp->next]);
				sel_lines_in_block(temp);
				add_control(temp);
				reCal_al_in(temp);
				add_pipe(temp);
				temp=p_PDG.pop();
			}
		}
	}
	
	/*
	cout<<"final"<<'\n';
	
	 for(i=1;i<=num_of_node;i++){
	 if(n[i].sel)cout<<i<<'\n';
	 }
	 */
	
}

void get_var_lines(char* var)
{
	int i;
	for(i=top_num+1;i<NODE_MAX;i++)
	{
		lines_to_slice[var_ptr]=i;
		n[i].R.add_element(var);
		var_ptr++;
	}
}

void add_pipe(node *node_al)
{
	link *temp;
	node *pre;
	
	temp=node_al->pre_link;
	while(temp!=NULL)
	{
		pre=temp->pre_line;
		pre->R=pre->R+node_al->R;
		if(!pre->work_through&&!((node_al->R*n[pre->next].def)==n[0].control))
			//��û�о�������ǰ�̽ڵ��def��ýڵ��R֮����Ϊ��
			p_PDG.push(&n[pre->next]);
		temp=temp->next;
	}
	
}

void reCal_al_in(node *n_block)
{
	SET_PTR i;
	for(i = n_block->control.elements.begin(); i != n_block->control.elements.end();i++)
	{
		string t = *i;
		if(n[atoi(t.c_str())].sel)
			n_block->R=n_block->R + n[atoi(t.c_str())].R;
	}
		
}

void sel_lines_in_block(node *n_al_in)
{
	SET_PTR i,j;
	for(i = n_al_in->control.elements.begin() ; i != n_al_in->control.elements.end() ;i++)
	{		//��� �� always initial �ڵ������ӽڵ㱻ѡ��
		if(n[atoi((*i).c_str())].sel){
			//printf("111111111111111111\n");
			n_al_in->sel=true;					//�� �� always initial �ڵ� ѡ��
		}
	}
	for(i = n_al_in->control.elements.begin() ; i != n_al_in->control.elements.end() ;i++)
		for(j = n_al_in->control.elements.begin() ; j != n_al_in->control.elements.end() ;j++)
			if((n[atoi((*i).c_str())].sel)&&
				(n[atoi((*j).c_str())].control>=(n[atoi((*i).c_str())].self)))
			{
				//printf("3333333333333333\n");
				n[atoi((*j).c_str())].sel=true;
			}
			
}

slicer program_slicing(svector<ModuleNode*>* mns, const char* var)
{

	//DWORD ti = GetTickCount();
	int i,j;
	unsigned idx;
	char sub[255];
	
	int line_num,max_line=0;
	
	set<string>::iterator pos;
	set<int, less<int> >::iterator ipos;
	
	//static 
	slicer temp;
	//-Sx  -MDecoder_ARM_stage1 -v{ExistState,} err2.v >out.txt
	for(i=1;i<NODE_MAX;i++)
		//itoa(i,n[i].self,10);
		sprintf(n[i].self, "%d", i);
	for(idx = 0; idx < mns->count(); ++idx)
	{
		svector<Node*> nodes((*mns)[idx]->construct_node(0));
		
		for(unsigned i = 0; i < nodes.count(); ++i)
		{
			line_num=nodes[i]->lineno_;
			n[line_num].funcs=nodes[i]->funcs_;
			n[line_num].mods=nodes[i]->mods_;
			/*
			�����жϸý���Ƿ��Ѿ����ڣ��������ӡ���������ڣ�
			����������PROCESS����Խ�����Ͳ����Ķ���
			�Ժ�һ�������Ҫ����always��@�����ͬһ��ʱ�������
			*/
			if((n[line_num].b_exist!=true)||(strcmp(n[line_num].kind,"PROCESS")))
			{
				n[line_num].b_exist=true;
				strcpy(n[line_num].kind,nodes[i]->type.c_str());
			}
			if(line_num>max_line)
				max_line=line_num;
			n[line_num].lineno = nodes[i]->lineno_;
			for(pos = nodes[i]->refs_.begin(); pos != nodes[i]->refs_.end(); ++pos)
				n[line_num].ref.add_element((*pos));
			for(pos = nodes[i]->defs_.begin(); pos != nodes[i]->defs_.end(); ++pos)
				n[line_num].def.add_element((*pos));
			ipos = nodes[i]->dsuccs_.begin();
			if(ipos==nodes[i]->dsuccs_.end())
				n[line_num].end_node=true;
			/* 
			   Add this code to deal with the last statement in a loop body when
			   the loop body is the last statement in a block, which will make the 
			   last statement to be end_node			   
			*/
			else if((nodes[i]->dsuccs_.size() == 1) && (*ipos <= nodes[i]->lineno_))
				n[line_num].end_node=true;
			else
				for(;ipos != nodes[i]->dsuccs_.end(); ++ipos)
					//�ж�ĳ������DIRSUCC�Ƿ�����Լ�����if(...) then begin�������
					if (*ipos!=line_num)
						//���������ǰ�̽��ָ�롣
						make_gra(*ipos,line_num);
					for(ipos = nodes[i]->infls_.begin(); ipos != nodes[i]->infls_.end(); ++ipos)
					{
						char tmp[255];
						//itoa((*ipos), tmp, 10);
						sprintf(tmp, "%d", (*ipos));
						n[line_num].control.add_element(tmp);
					}
		}
	}
	
	int t_exist;
	//��Block�����begin��ĵ�һ����䣬����ǰ��ָ�롣
	for(idx = 0; idx < mns->count(); ++idx)
	{
		svector<Node*> nodes((*mns)[idx]->construct_node(0));
		
		for(unsigned i = 0; i < nodes.count(); ++i)
		{
			if(nodes[i]->dsuccs_.size()>0)
			{
				for(ipos=nodes[i]->dsuccs_.begin();ipos!=nodes[i]->dsuccs_.end();ipos++)
				{
					if(((n[*ipos].b_exist)&&((*ipos)==nodes[i]->lineno_))||(!n[*ipos].b_exist))
					{
						t_exist=*ipos+1;
						//cout<<n[t_exist].b_exist<<endl;
						while(!n[t_exist].b_exist && t_exist < NODE_MAX)
						{
							if(t_exist < NODE_MAX-1)
								t_exist++;
							else
								break;
						}
						make_gra(t_exist,nodes[i]->lineno_);
					}
				}
			}
		}
	}
	
	num_of_node=max_line;
	//��process�е���������def��ref�ӵ�PROCESSͷ��Ϣ�С�
	for(i=1;i<=num_of_node;i++)
		if((!strcmp(n[i].kind,"PROCESS"))||(!strcmp(n[i].kind,"ASSIGN")))
			add_DEF_REF(&n[i],&n[i]);
	//�½����������㣬ʹ��ָ���Process��������ֹ��㡣
	for(i=1;i<=num_of_node;i++)
	{
		if((!strcmp(n[i].kind,"PROCESS"))||(!strcmp(n[i].kind,"ASSIGN")))
		{
			for(SET_PTR j = n[i].control.elements.begin() ; j != n[i].control.elements.end(); j++)
			{
				if(n[atoi((*j).c_str())].end_node)
					make_gra(top_num,atoi((*j).c_str()));
			}
			if(n[i].control.count==0)
				make_gra(top_num,i);
			n[top_num].end_node=true;
			n[i].next=top_num;
			//itoa(top_num,sub,10);
			sprintf(sub, "%d", top_num);
			n[i].control.add_element(sub);
			n[top_num].next=i;
			top_num--;
		}
	}
	//����PDGͼ��
	for(i=1;i<=num_of_node;i++)
		for(j=1;j<=num_of_node;j++)
			if(((!strcmp(n[i].kind,"PROCESS"))||(strcmp(n[i].kind,"ASSIGN")))&&
				((!strcmp(n[j].kind,"PROCESS"))||(!strcmp(n[j].kind,"ASSIGN")))&&
				(!((n[i].def*n[j].ref)==n[0].control))&&(i!=j))
				make_gra(j,n[i].next);
			
	int sptr1=0, sptr2;
	//ȡ���������Ҫ��Ƭ���ź�����
	cout<<"var: "<<var<<endl;
	string stmp(var);
	while((sptr2=stmp.find(",", sptr1 ))!=-1)
	{
		const char* tmp0 = stmp.substr(sptr1,sptr2-sptr1).c_str();
		char* tmp = new char[strlen(tmp0)];
		strcpy(tmp, tmp0);
		get_var_lines(tmp);
		sptr1=sptr2+1;
	}
	//the last variable
	if(sptr2==-1)
	{
		const char* tmp2 = stmp.substr(sptr1,stmp.length()-sptr1).c_str();
		char* tmp1 = new char[strlen(tmp2)];
		strcpy(tmp1, tmp2); //缓冲溢出s
		get_var_lines(tmp1);
	}
	//��Ƭ	
	work();	
	for(i=1;i<=num_of_node;i++)
	{
		n[i].def.delete_all();
		n[i].ref.delete_all();
	}
	for(idx = 0; idx < mns->count(); ++idx)
	{
		svector<Node*> nodes((*mns)[idx]->construct_node(0));
		
		for(unsigned i = 0; i < nodes.count(); ++i)
		{
			line_num=nodes[i]->lineno_;
			for(pos = nodes[i]->refs_.begin(); pos != nodes[i]->refs_.end(); ++pos)
				n[line_num].ref.add_element((*pos).c_str());
			for(pos = nodes[i]->defs_.begin(); pos != nodes[i]->defs_.end(); ++pos)
				n[line_num].def.add_element((*pos).c_str());
		}
	}
	temp.slice.clear();
	temp.vars.clear();

	/*
	for(i = 1; i <= num_of_node; i++)
		printf("%d\n", n[i].sel);
	*/
	for(i = 1;i<=num_of_node;i++)
	{
		if(n[i].sel)
		{
			SET_PTR j;
			//cout<<"slice insert"<<i<<endl;
			temp.slice.insert(i);
			for(j = n[i].def.elements.begin() ; j != n[i].def.elements.end(); j++)
				temp.vars.insert(*j);
			for(j = n[i].ref.elements.begin() ; j != n[i].ref.elements.end(); j++)
				temp.vars.insert(*j);
			for(pos = n[i].funcs.begin(); pos != n[i].funcs.end(); ++pos)
				temp.funcs.insert(*pos);
			for(pos = n[i].mods.begin(); pos != n[i].mods.end(); ++pos)
				temp.mods.insert(*pos);
		}
	}
	cout<<"4444444444"<<endl;	
	//cout << endl << "Slicing Time: " << GetTickCount() - ti << "ms" << endl;
	return temp;
}



void add_DEF_REF(node *n_be_add,node *n_to_add)			//�ݹ����
{
	SET_PTR i;
	int c;
	c = n_be_add->control.count;
	for(i = n_be_add->control.elements.begin(); i != n_be_add->control.elements.end() ; i++){
		n_to_add->ref = n_to_add->ref + n[atoi((*i).c_str())].ref;
		n_to_add->def = n_to_add->def + n[atoi((*i).c_str())].def;
		n_to_add->control.add_element(n[atoi((*i).c_str())].self);
		n[atoi((*i).c_str())].next = atoi(n_to_add->self);
		//if(!(n[atoi(n_be_add->control.x(i))].control==n[0].control))
		//	add_DEF_REF(&n[atoi(n_be_add->control.x(i))],n_to_add);
	}
}


void reset(void)				//�����нڵ�� �߹� ��� ��λ
{
	for(int i=0;i<NODE_MAX;i++)
		n[i].work_through=false;
}

void reset(node *n_block)		//��ĳһalways,initial����е��������� �߹� ��� ��λ
{
	SET_PTR i;
	for(i = n_block->control.elements.begin() ; i != n_block->control.elements.end();i++)
		n[atoi((*i).c_str())].work_through=false;
}


void tree_view(node *ab_end,node *root_end,node *me)			//���ĳһ�����е����
{
	node *last,*pre;
	
	aggregate temp_set;
	
	bool go_once;
	
	last=me;

	root_end->work_through=true;
	
	if(!strcmp(last->kind,"PROCESS")||(!strcmp(last->kind,"ASSIGN")))
	{
		temp_set=last->R;
		last->R=root_end->R+last->R;
		if(!(temp_set==last->R))
			finish=false;
		return;
	}
	else
	{
		if(((last->def)*(root_end->R))==n[0].control)
		{
			temp_set=last->R;
			last->R=root_end->R+last->R;
			if(!(temp_set==last->R))
				finish=false;
		}
		else
		{
			temp_set=last->R;
			last->R=(root_end->R-last->def)+last->ref+last->R;
			if(!(temp_set==last->R))
				finish=false;
		}
	}
	
	if(me->work_through)return;
	
	me->work_through=true;
	
	//cout<<last->self<<'\n';
	
	link *temp;
	while(true)
	{
		if(last->num_of_pre(1)>1)
		{		//ǰ�̵���Ŀ����1
			if(last->num_of_pre(2)==1)
			{	//��һ��ǰ�̲�������control
				temp=last->pre_link;
				go_once=false;
				while(temp!=NULL)
				{
					if(!(last->control>=(temp->pre_line->self))&&!go_once)
					{
						pre=temp->pre_line;
						go_once=true;
					}
					else
						tree_view(ab_end,last,(temp->pre_line));
					temp=temp->next;
				}
			}
			else
			{					//ǰ��ȫ���ǣ����ǣ�����control
				temp=last->pre_link;
				while(temp!=NULL)
				{
					tree_view(ab_end,last,(temp->pre_line));
					temp=temp->next;
				}
				break;
			}
			
		}
		else 
			if(last->num_of_pre(1)==1)
			{	//ǰ�̵���ĿΪ1
				pre=(last->pre_link->pre_line);
				
			}
			else
			{							//��û��ǰ��ʱ
				break;
				
			}
			
			if(!strcmp(pre->kind,"PROCESS"))
			{				//modified !!!!!
				temp_set=pre->R;
				pre->R=pre->R+last->R;
				if(!(temp_set==pre->R))
					finish=false;
			}
			else
			{
				if(((last->R)*(pre->def))==n[0].control)
				{
					temp_set=pre->R;
					pre->R=last->R+pre->R;
					if(!(temp_set==pre->R))
						finish=false;
				}
				else
				{
					temp_set=pre->R;
					pre->R=(last->R-pre->def)+pre->ref+pre->R;
					if(!(temp_set==pre->R))
						finish=false;
				}
			}
			
			if((pre==ab_end)||(pre==root_end)||
				(pre->work_through)||(!strcmp(pre->kind,"PROCESS"))||(!strcmp(pre->kind,"ASSIGN"))) break;
			pre->work_through=true;
			last=pre;
			//	cout<<last->self<<'\n';
	}
}

void cal_S(node *ab_end,node *root_end,node *me)	//����S��Ҳ��ĳһģ����Ӧ���������
{
	node *last,*pre;
	
	bool go_once;
	
	last=me;

	root_end->work_through=true;
	
	if(!((root_end->R*last->def)==n[0].control))
		last->sel=true;
	
	if(me->work_through)return;
	
	if(!strcmp(last->kind,"PROCESS")||(!strcmp(last->kind,"ASSIGN")))return;
	
	me->work_through=true;
	
	link *temp;
	while(true)
	{
		if(last->num_of_pre(1)>1)
		{		//ǰ�̵���Ŀ����1
			if(last->num_of_pre(2)==1)
			{	//��һ��ǰ�̲�������control
				temp=last->pre_link;
				go_once=false;
				while(temp!=NULL)
				{
					if(!(last->control>=(temp->pre_line->self))&&!go_once)
					{
						pre=temp->pre_line;
						go_once=true;
					}
					else
						cal_S(ab_end,last,(temp->pre_line));
					temp=temp->next;
				}
			}
			else
			{					//ÿһ��ǰ�̶�������control
				temp=last->pre_link;
				while(temp!=NULL)
				{
					cal_S(ab_end,last,(temp->pre_line));
					temp=temp->next;
				}
				break;
			}
			
		}
		else 
			if(last->num_of_pre(1)==1)
			{	//ǰ�̵���ĿΪ1
				pre=(last->pre_link->pre_line);
				
			}
			else
			{							//��û��ǰ��ʱ
				break;
				
			}
			
			if(!((last->R*pre->def)==n[0].control))
				pre->sel=true;
			
			if((pre==ab_end)||(pre==root_end)||
				(pre->work_through)||(!strcmp(pre->kind,"PROCESS"))||(!strcmp(pre->kind,"ASSIGN"))) break;
			pre->work_through=true;
			last=pre;
	}
}

void add_control(node *n_block)
{
	SET_PTR i;
	for(i = n_block->control.elements.begin();i != n_block->control.elements.end(); i++)
	{
		string tmp = n[atoi((*i).c_str())].kind;
		if(n[atoi((*i).c_str())].sel&&!n[atoi((*i).c_str())].control_sel
			&&(tmp.find("ISCONTROL") == 0))
		{
			n[atoi((*i).c_str())].R=n[atoi((*i).c_str())].ref+
				n[atoi((*i).c_str())].R;
			finish=false;				//�˴����иĶ�����ѭ�����ܽ���
			n[atoi((*i).c_str())].control_sel=true;		//ÿһ��controlֻ�������ӽ�һ��
		}
	}
}

void make_gra(int a,int b)			//b is the prelink of a , b��a��ǰ��
{
	if(n[a].pre_link==NULL)
	{
		link *temp;
		temp=new link;
		n[a].pre_link=temp;
		temp->next=NULL;
		temp->pre_line=&n[b];
	}
	else
	{
		link *temp;
		link *t;
		temp=n[a].pre_link;
		while(temp->next!=NULL)
			temp=temp->next;
		t=new link;
		temp->next=t;
		t->pre_line=&n[b];
		t->next=NULL;
	}
}
