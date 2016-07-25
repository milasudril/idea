//@	{
//@	"targets":
//@		[{
//@		"name":"project_treeview.o","type":"object"
//@		,"pkgconfig_libs":["Qt5Widgets"]
//@		}]
//@	}

#include "project_treeview.hpp"
#include "project.hpp"

#include <QPushButton>
#include <QFileSystemModel>
#include <QTreeView>
#include <QHeaderView>

using namespace Idea;

class Project_TreeView::Impl:public QWidget
	{
	public:
		Impl()
			{
			m_tree.setModel(&m_fs);
			m_tree.setHeaderHidden(1);
			m_tree.header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
			m_tree.show();
			}

		void rootSet(const char* root_dir)
			{
			m_fs.setRootPath(root_dir);
			m_tree.setRootIndex(m_fs.index(root_dir));
			}

	private:
		QFileSystemModel m_fs;
		QTreeView m_tree;
	};

Project_TreeView::Project_TreeView():m_impl(new Project_TreeView::Impl)
	{
	}

Project_TreeView::~Project_TreeView(){}

Project_View& Project_TreeView::projectSet(Project& project)
	{
	m_impl->rootSet(project.rootGet());
	return *this;
	}
