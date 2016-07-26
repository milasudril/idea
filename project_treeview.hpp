//@	{
//@	 "targets":[{"name":"project_treeview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"project_treeview.o","rel":"implementation"}]
//@	}

#ifndef IDEA_PROJECT_TREEVIEW_HPP
#define IDEA_PROJECT_TREEVIEW_HPP

#include "project.hpp"
#include <memory>

namespace Idea
	{
	class Project;
	class Project_TreeView:public Project::View
		{
		public:
			class EventHandler
				{
				public:
					virtual void itemSelected(const Project_TreeView& view)=0;
				};

			Project_TreeView();
			~Project_TreeView();

			Project_TreeView& projectSet(Project& project);
			Project_TreeView& documentCurrentSet(Project& project);

		private:
			struct Impl;
			std::unique_ptr<Impl> m_impl;
		};
	}

#endif
