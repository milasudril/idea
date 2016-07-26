//@	{
//@	"targets":
//@		[{
//@		"name":"project_treeview.o","type":"object"
//@		,"pkgconfig_libs":["Qt5Widgets"]
//@		}]
//@	}

#include "project_treeview.hpp"

#include <QPushButton>
#include <QFileSystemModel>
#include <QTreeView>
#include <QHeaderView>

using namespace Idea;

class Project_TreeView::Impl:public QWidget
	{
	public:
		Impl(EventHandler& event_handler,Project_TreeView& owner):
			r_owner(owner),r_event_handler(&event_handler)
			{
			m_tree.setModel(&m_fs);
			m_tree.setHeaderHidden(1);
			m_tree.header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

			connect(m_tree.selectionModel(),&QItemSelectionModel::selectionChanged
				,[this](const QItemSelection& sel_new,const QItemSelection& sel_old)
					{
					auto fileinfo=m_fs.fileInfo(sel_new.indexes().first());

				//	auto path_absolute=m_fs.rootPath();
					auto in_dir_full=QDir(m_fs.rootDirectory());
					m_dir=in_dir_full.relativeFilePath( fileinfo.absolutePath() ).toLocal8Bit();
					m_filename=in_dir_full.relativeFilePath(fileinfo.absoluteFilePath()).toLocal8Bit();
					m_isdir=fileinfo.isDir();
					r_event_handler->itemSelected(r_owner);
					});

			m_tree.show();
			}

		void rootSet(const char* root_dir)
			{
			m_fs.setRootPath(root_dir);
			m_tree.setRootIndex(m_fs.index(root_dir));
			}

		Item itemSelectedGet() const noexcept
			{
			return {m_dir.constData(),m_filename.constData(),m_isdir};
			}

	private:
		QFileSystemModel m_fs;
		QTreeView m_tree;
		QByteArray m_dir;
		QByteArray m_filename;
		bool m_isdir;

		Project_TreeView& r_owner;
		EventHandler* r_event_handler;
	};

Project_TreeView::Project_TreeView(EventHandler& event_handler):
	m_impl(new Project_TreeView::Impl(event_handler,*this))
	{
	}

Project_TreeView::~Project_TreeView(){}

Project_TreeView& Project_TreeView::projectSet(Project& project)
	{
	m_impl->rootSet(project.rootGet());
	return *this;
	}

Project_TreeView& Project_TreeView::documentCurrentSet(Project& project)
	{
	return *this;
	}

Project_TreeView::Item Project_TreeView::itemSelectedGet() const noexcept
	{
	return m_impl->itemSelectedGet();
	}
