//@	{
//@	 "targets":[{"name":"project_treeview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"project_treeview.o","rel":"implementation"}]
//@	}

#ifndef IDEA_PROJECT_TREEVIEW_HPP
#define IDEA_PROJECT_TREEVIEW_HPP

#include "project_view.hpp"
#include <memory>

namespace Idea
	{
	class Project;
	class Project_TreeView:public Project_View
		{
		public:
			class EventHandler
				{
				public:
					virtual void itemSelected(const Project_TreeView& view)=0;
				};

			Project_TreeView();
			~Project_TreeView();

			Project_View& projectSet(Project& project);

		private:
			struct Impl;
			std::unique_ptr<Impl> m_impl;
		};
	}

#endif
