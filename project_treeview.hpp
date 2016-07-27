//@	{
//@	 "targets":[{"name":"project_treeview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"project_treeview.o","rel":"implementation"}]
//@	}

#ifndef IDEA_PROJECT_TREEVIEW_HPP
#define IDEA_PROJECT_TREEVIEW_HPP

#include "project.hpp"
#include "document.hpp"
#include <memory>

class QWidget;

namespace Idea
	{
	class Project;
	class Project_TreeView:public Project::View
		{
		public:
			struct Item
				{
				const char* in_dir;
				const char* name;
				bool is_dir;
				};

			class EventHandler
				{
				public:
					virtual void itemSelected(const Project_TreeView& view)=0;
					virtual void fileCreate(const Project_TreeView& view,const char* name)=0;
					virtual void directoryCreate(const Project_TreeView& view,const char* name)=0;
					virtual void itemDelete(const Project_TreeView& view)=0;
				};

			Project_TreeView(QWidget& parent,EventHandler& event_handler);
			~Project_TreeView();

			Project_TreeView& projectSet(Project& project);
			Project& projectGet() const;
			Project_TreeView& documentCurrentSet(Project& project);
			Project_TreeView& documentViewAttach(Document::View& view);

			Item itemSelectedGet() const noexcept;

			QWidget* widget();

		private:
			struct Impl;
			std::unique_ptr<Impl> m_impl;
		};
	}

#endif
