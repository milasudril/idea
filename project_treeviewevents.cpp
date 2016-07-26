//@	{"targets":[{"name":"project_treeviewevents.o","type":"object"}]}

#include "project_treeviewevents.hpp"
#include "project.hpp"

using namespace Idea;

void Project_TreeViewEvents::itemSelected(const Project_TreeView& view)
	{
	auto item=view.itemSelectedGet();
	if(!item.is_dir)
		{view.projectGet().documentCurrentSet(item.name);}
	}
