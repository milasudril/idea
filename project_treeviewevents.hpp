//@	{
//@	 "targets":[{"name":"project_treeviewevents.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"project_treeviewevents.o","rel":"implementation"}]
//@	}

#ifndef IDEA_PROJECT_TREEVIEWEVENTS_HPP
#define IDEA_PROJECT_TREEVIEWEVENTS_HPP

#include "project_treeview.hpp"

namespace Idea
	{
	class Project_TreeViewEvents:public Project_TreeView::EventHandler
		{
		public:
			void itemSelected(const Project_TreeView& view);
			void fileCreate(const Project_TreeView& view,const char* name);
			void directoryCreate(const Project_TreeView& view,const char* name);
			void itemDelete(const Project_TreeView& view);
			void itemDeleted(const Project_TreeView& view,const Project_TreeView::Item& item);
		};
	}

#endif
