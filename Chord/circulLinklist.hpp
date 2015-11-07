/*
	CirCulLinkList.h  循环双链表头文件 2012/4/17
	CopyRight 1.0
*/
#ifndef DataStruct_CirculLinkList_Base_H_
#define DataStruct_CirculLinkList_Base_H_
#include <assert.h>
#define ERROR 0
#define OK    1
#define ERROR 0
#define TRUE 1
#define  FALSE  0
typedef int Status;
typedef int Boolean;

template <typename ElemType>
class CirculLinkList
{
	public:
		class LinkNode
		{
			public:
			ElemType data;
			LinkNode * next;
		};
		typedef LinkNode * NodePointer;
	//********************************************
		void Clear();
		Status InsertList(int i,const ElemType&,NodePointer* p);
		int getlength();
		Status ListEmpty();
		void movehead(int m);
		NodePointer Gethead()const{return head;}
		Status DeleteList( int i,ElemType & e);
		Status DeleteList(NodePointer s);
	//**********************************************
		CirculLinkList ();
		virtual ~CirculLinkList();
		CirculLinkList (const CirculLinkList & otherL);
	private: 
		NodePointer head;
};
template <typename ElemType>
void CirculLinkList<ElemType>::Clear()
{
	NodePointer p;
	while(head!=head->next){
		p=head->next;
		head->next=p->next;
		delete p;
	}
	delete head;
	head=NULL;
}
template <typename ElemType>
Status CirculLinkList<ElemType>::ListEmpty()
{
	if(!head)
		return TRUE;
	return FALSE;
}
template <typename ElemType>
int CirculLinkList<ElemType>::getlength()
{
	int i=1;
	if(!head)
		return 0;
	NodePointer p=head->next;
	while(p!=head){
		++i;
		p=p->next;
	}
	return i;
}
template <typename ElemType>
Status CirculLinkList<ElemType>::InsertList(int i,const ElemType& e,NodePointer* pp) //这里对InsertList进行了调整，让其返回插入元素的在环中的指针
{
	NodePointer p=head,q=head,s;
	int j=1;
	if(i<1 || i>getlength()+1){
		if(pp!=NULL)
			*pp =NULL;
		return ERROR;	
	}
	s=new LinkNode;
	assert(s!=0);
	s->data=e;
	if(i==1){
		head=s;
		s->next=head;
	}
	else if(i==getlength()+1){
		while(q->next!=head)
				q=q->next;
		q->next=s;
		s->next=head;
	}
	else{
		while(p &&j<i-1 ){
			j++;
			p=p->next;
		}
		if(!p){
			*pp = NULL;
			return ERROR;
		}
		s->next=p->next;
		p->next=s;
	}
	if(pp!=NULL)
		*pp = s;
	return OK;
}
template <typename ElemType>
Status CirculLinkList<ElemType>::DeleteList(int i,ElemType &e)
{
	NodePointer p=head,q=NULL;
	int j=1;
	if(i<1)
		return ERROR;
	while(j<getlength()){
		j++;
		p=p->next;
	}
	j=0;
	while(j<i-1){
		j++;
		p=p->next;
	}//该循环让p指向被删除结点的前驱
	q=p->next;
	if(q==head)
		head=q->next;
	p->next=q->next;
	e=q->data;	
//	free(q);
	delete q;
	q = NULL;
	return OK;
}

template <typename ElemType>
Status CirculLinkList<ElemType>::DeleteList(NodePointer s)
{
	if(!head)
		return 0;
	NodePointer p=head->next;
	NodePointer q=head; //q指向p的前驱
	while(p!=head){
		if(p == s){ //如果是待删除节点
			q->next = s->next;
			delete s;	//删除该节点
			s = NULL;
			return 1;
		}
		q=p;
		p=p->next;
	}
	if(s == head){
		head =s->next;
		q->next =s->next;
		delete s;
		s = NULL;
	}
	return 1;
}
template <typename ElemType>
void CirculLinkList<ElemType>::movehead(int m)
{
	int i;
	for(i=1;i<m;i++)
		head=head->next;
}
//*****************************
template <typename ElemType>
CirculLinkList<ElemType>::CirculLinkList()
{
	head=NULL;
}
template <typename ElemType>
CirculLinkList<ElemType>::~CirculLinkList()
{
	if(head !=NULL){
		Clear();
	}
}
template <typename ElemType>
CirculLinkList<ElemType>:: CirculLinkList(const CirculLinkList<ElemType >& otherL)
{
	NodePointer p,s,q=NULL;
	p=otherL.head;
	while(p->next!=otherL.head){
		s=new LinkNode ;
		assert(s!=0);
		s->data=p->data;
		if(!head)
			head=s;
		else
			q->next=s;
		q=s;
		p=p->next;
	}
	s=new LinkNode;
	assert(s!=0);
	s->data=p->data;
	q=s;
	q->next=head;
}
#endif /*CIRCULLINKLIST.H*/