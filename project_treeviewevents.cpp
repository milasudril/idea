//@	{"targets":[{"name":"project_treeviewevents.o","type":"object"}]}

#include "project_treeviewevents.hpp"
#include "project.hpp"

using namespace Idea;

void Project_TreeViewEvents::itemSelected(const Project_TreeView& view)
	{
	auto item=view.itemSelectedGet();
	printf("Item selected %s %s %d\n",item.in_dir,item.name,item.is_dir);
	}
