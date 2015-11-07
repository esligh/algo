#ifndef _Kadmelia_node_class_def_h_
#define _Kadmelia_node_class_def_h_
#include "interface_rpc.h"
#include "router.h"
#include "filedict.h"
#include <windows.h>

class Node: private interface_rpc
{
public:
		Node();
		Node(const nodeinfo& info);
		~Node();
		const Vnode* GetFileDestLst(const hash_value& hv);
		bool StorePairKV(const hash_value& key,const destination& value);
		bool StorePairKV(const hash_value& key,const Vnode& nodelst);
		bool DelPairKV(const hash_value& key,const destination& value); //±£¡Ù
		bool AddNeighbour(const hash_value& hv,const neighbour& someone);
		bool UpdateRouter(const neighbour& someone);
		bool GetDestLst(const hash_value& ,Vnode&);
		//////////////////////////////////////////////////////////////////////////
		const nodeinfo& getlocalinfo()const;
		hash_value getlocalhash()const;
		size_t getdictsize()const;
		smallInt getroutersize()const;
		bool isonline()const;
		void setonline(bool b);
		hash_value Node::hash_node(const nodeinfo& info);
		Router& get_router(){return router_;}
		FileDict& get_dict(){return filedict_;}
		bool schedule();
		//////////////////////////////////////////////////////////////////////////
		/*interface_rpc method*/
		virtual bool rpc_ping(const Node& sender);
		virtual bool rpc_store(const Node& sender,const hash_value& key,const destination& dest);
		virtual bool rpc_find_value(const Node& node,const hash_value& key,Vnode& dest);
		virtual bool rpc_find_node(const Node& sender,const hash_value& id,NeighbourLst& nodelst);
		bool update(const nodeinfo& node);
		bool CopyNeighbourLst(const NeighbourLst& lst);
		bool get_closest_neighbour(const Node& node,const hash_value& id,NeighbourLst& lst);
		bool get_closest_neighbour(const hash_value& id,NeighbourLst& lst);
		bool GetNeightbourByCount(const hash_value& id,NeighbourLst& lst);
		bool share_torrent(std::string& fname);
		bool find_key(const hash_value& key,Vnode& nodelst);
		bool find_key(const std::string& name,Vnode& nodelst);
		bool transer_kv(const hash_value& key,const Node& to);
		///////////////////////////////////////////////////////////////////////////////////
		void PrintRouter();
		void PrintDict();
		//////////////////////////////////////////////////////////////////////////
private:
		bool get_closer(NeightbourSet& set,NeighbourLst& lst);
private:
	nodeinfo local_;
	Router router_;
	FileDict filedict_;
	bool bOnline;
};
bool compare(const neighbour& left,const neighbour& right);
#endif