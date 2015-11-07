/*
	LinkList.h 模板类声明及定义 于2012/4/17
	CopyRight 1.0 by jeckral
*/
#ifndef DataStruct_LinkList_Base_H_ 
#define DataStruct_LinkList_Base_H_ 
#include <assert.h>
#define ERROR 0
#define OK    1
#define ERROR 0
#define TRUE 1
#define  FALSE  0
typedef int Status;
typedef int Boolean;

template <typename ElemType>
class LinkList
{
	public :
			class LinkNode
			{
				public:
					ElemType data;
					LinkNode *next;
			};
	typedef LinkNode*  NodePointer;
	//***************************
	void clear();
	Status InsertList(int i,ElemType e);
	NodePointer gethead()const{return head;}
	int getlength()const; 
	LinkList<ElemType> operator= (LinkList<ElemType> rightL);
	bool ListEmpty()const;
	int LocateElem(ElemType e,Status(*compare)(const ElemType& ,const ElemType&))const;
	Status PriorElem(ElemType e,ElemType &e0)const;
	Status NextElem(ElemType e,ElemType &e0)const;
	Status DeleteList(ElemType e);
	Status DeleteList(NodePointer s);
	Status DeleteRepeat();
	void adverse();
	 Status GetElem(int i,ElemType& e)const;
	//****************************
	LinkList();
	virtual ~LinkList();
	LinkList(const LinkList<ElemType>& otherL);
	private :
		NodePointer head;  //不带头结点
};

//************操作实现****************
template <typename ElemType>
void LinkList<ElemType>::clear()
{
		NodePointer p,q;
		q=NULL,p=head;
		while(p){
			q=p;
			p=p->next;
			delete q;
			q=NULL;
		}
		head =NULL;
}

template <typename ElemType>
Status LinkList<ElemType>::InsertList(int i,ElemType e)
{
	int j=1;
	NodePointer p=head;
	NodePointer s;
	if(i<1)
		return ERROR;
	s=new LinkNode;
	assert(s!=0);
	s->data=e;
	if(i==1){
		s->next=head;
		head=s;
	}
	else{
		while(p&&j<i-1){
			j++;
			p=p->next;
		}
		if(!p)
			return ERROR;
		s->next=p->next;
		p->next=s;
	}
	return OK;
}
template <typename ElemType>
Status LinkList<ElemType> ::DeleteList(ElemType e)
{
	NodePointer p=head,q=head;
	while(e!=p->data){
			q=p;
			p=p->next;
	}
	if(!p)
		return ERROR;
	if(p==head)
		head=p->next;
	q->next=p->next;
	p=NULL;
	return OK;
}
//added delete 
template <typename ElemType>
Status LinkList<ElemType> ::DeleteList(NodePointer s)
{
	NodePointer p=head,q=head;
	while(s!=p){
		q=p;
		p=p->next;
	}
	if(!p)
		return ERROR;
	if(p==head)
		head=p->next;
	q->next=p->next;
	delete p ;
	p=NULL;
	return OK;
}

template <typename ElemType>
int LinkList<ElemType>::getlength()const 
{
	int n=0;
	NodePointer p=head;
	while(p){
		++n;
		p=p->next;
	}
	return n;
}
template <typename ElemType>
LinkList<ElemType> LinkList<ElemType>::operator=(LinkList<ElemType> rightL)
{
	NodePointer p=NULL;
	NodePointer rp=rightL.gethead();
	NodePointer s;
	if(this!=&rightL){
		clear();
		while(rp){
			s=new LinkNode;
			assert(s!=0);
			s->data=rp->data;
			if(!head)
				head=s;
			else
				p->next=s;
			p=s;
			rp=rp->next;
		}
		if(p)
			p->next=NULL;
	}
	return *this;
}

template <typename ElemType>
bool LinkList<ElemType>:: ListEmpty()const 
{
	if(!head)
		return TRUE;
	else
		return FALSE;
}
template <typename ElemType>
int LinkList<ElemType>::LocateElem(ElemType e,Status(*compare)(const ElemType&,const ElemType&))const 
{
	NodePointer p=head;
	int j=1;
	while(j<=getlength()&&!(*compare)(p->data,e)){
		j++;
		p=p->next;
	}
	return j;
}
template <typename ElemType>
Status LinkList<ElemType>::PriorElem(ElemType e,ElemType &e0)const 
{
	if(!head || !head->next)
		return ERROR;
	NodePointer q, p=head;
	q=head->next;
	while(e!=q->data && q){
		p=q;
		q=q->next;
	}
	if(!p)
		return ERROR;
	e0=p->data;
	return TRUE;
}
template <typename ElemType>
Status LinkList<ElemType>::NextElem(ElemType e,ElemType &e0) const
{
	NodePointer q,p=head;
	q=head->next;
	while(e!=p->data && p->next){
			p=p->next;
			q=p->next;
	}
	if(!q)
		return ERROR;
	e0=q->data;
	return OK;
}
template <typename ElemType>
Status LinkList<ElemType>::DeleteRepeat()
{
	ElemType e;
	if(!head|| !head->next)
		 return ERROR;
	NodePointer q=head,p=head->next;
	while(p){
		while(q!=p){
			e=q->data;
			if(p->data==e){
				q=q->next;
				DeleteList(e);				
			}
			else
				q=q->next;
		}
		q=head;
		p=p->next;
	}
	return OK;
}
template <typename ElemType>
void LinkList<ElemType>:: adverse()
{
	NodePointer p,q,r;
	if(!head)
		return ;
	r=NULL,p=head,q=p->next;
	while(p){
		p->next=r;
		r=p;
		p=q;
		if(q)
			q=q->next;
	}
	head=r;
}
template <typename ElemType>
Status LinkList<ElemType>:: GetElem(int i,ElemType &e)const 
{
	NodePointer p=head;
	if (i<1|| i>getlength())
		return ERROR;
	while(p&&i>1){
		p=p->next;
		i--;
	}
	if(!p)
		return ERROR;
	e=p->data;
	return OK;
}

//*******************系统调用函数*********************
template <typename ElemType>
LinkList<ElemType>::LinkList()
{
	head=NULL;
}

template <typename ElemType>
LinkList<ElemType>::~LinkList()
{
	if(head != NULL)
		clear();
}

template <typename ElemType>
LinkList<ElemType>::LinkList(const LinkList& otherL)
{
	NodePointer p;
	NodePointer op=otherL.head;
	NodePointer s;
	head=p=NULL;
	while(op){
		s=new LinkNode;
		assert(s!=0);
		s->data=op->data;
		if(!head)
			head=s;
		else
			p->next=s;
		p=s;
		op=op->next;
	}
	if(head)
		p->next=NULL;
}
#endif /*LINKLIST.H*/