//@	{"targets":[{"name":"project_treeviewevents.o","type":"object"}]}

#include "project_treeviewevents.hpp"
#include "project.hpp"
#include <cstring>

using namespace Idea;

void Project_TreeViewEvents::itemSelected(const Project_TreeView& view)
	{
	auto item=view.itemSelectedGet();
	if(!item.is_dir)
		{view.projectGet().documentCurrentSet(item.name);}
	}

static std::string itemNameGenerate(const Project_TreeView::Item& item
	,const char* name)
	{
	std::string item_name(item.is_dir?item.name:item.in_dir);
	if(strcmp(item.in_dir,".")==0 && !item.is_dir)
		{item_name.clear();}
	if(item_name.size()!=0)
		{item_name+='/';}
	item_name+=name;
	return std::move(item_name);
	}

void Project_TreeViewEvents::fileCreate(const Project_TreeView& view
	,const char* name)
	{
	auto item_name=itemNameGenerate(view.itemSelectedGet(),name);
	view.projectGet().documentCreate(item_name.c_str());
	}

void Project_TreeViewEvents::directoryCreate(const Project_TreeView& view,const char* name)
	{
	auto item_name=itemNameGenerate(view.itemSelectedGet(),name);
	view.projectGet().directoryCreate(item_name.c_str());
	}

void Project_TreeViewEvents::itemDelete(const Project_TreeView& view)
	{
	auto item=view.itemSelectedGet();
	if(item.is_dir)
		{view.projectGet().directoryRemove(item.name);}
	else
		{view.projectGet().fileRemove(item.name);}
	}
