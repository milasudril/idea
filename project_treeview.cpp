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
#include <QVBoxLayout>

using namespace Idea;

class Project_TreeView::Impl:public QWidget
	{
	public:
		Impl(QWidget& parent,EventHandler& event_handler,Project_TreeView& owner):
			QWidget(&parent),m_tree(this),r_owner(owner)
			,r_event_handler(&event_handler),r_doc_view(nullptr)
			{
			m_tree.setModel(&m_fs);
			m_tree.setHeaderHidden(1);
			m_tree.header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
			m_box.addWidget(&m_tree);
			m_box.setMargin(4);
			setLayout(&m_box);

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
			}

		void projectSet(Project& project)
			{
			r_project=&project;
			auto root_dir=project.rootGet();
			m_fs.setRootPath(root_dir);
			m_tree.setRootIndex(m_fs.index(root_dir));
			}

		Project& projectGet() const noexcept
			{
			return *r_project;
			}

		Item itemSelectedGet() const noexcept
			{
			return {m_dir.constData(),m_filename.constData(),m_isdir};
			}

		QWidget* widget() noexcept
			{return this;}


		void documentViewAttach(Document::View& view)
			{
			r_doc_view=&view;
			}

		void documentCurrentSet(Project& proj)
			{
			if(r_doc_view!=nullptr)
				{r_doc_view->documentSet(proj.documentCurrentGet());}
			}

	private:
		QFileSystemModel m_fs;
		QTreeView m_tree;
		QByteArray m_dir;
		QByteArray m_filename;
		QVBoxLayout m_box;
		bool m_isdir;

		Project_TreeView& r_owner;
		EventHandler* r_event_handler;
		Project* r_project;
		Document::View* r_doc_view;
	};

Project_TreeView::Project_TreeView(QWidget& parent,EventHandler& event_handler):
	m_impl(new Project_TreeView::Impl(parent,event_handler,*this))
	{
	}

Project_TreeView::~Project_TreeView(){}

Project_TreeView& Project_TreeView::projectSet(Project& project)
	{
	m_impl->projectSet(project);
	return *this;
	}

Project& Project_TreeView::projectGet() const
	{
	return m_impl->projectGet();
	}

Project_TreeView& Project_TreeView::documentCurrentSet(Project& project)
	{
	m_impl->documentCurrentSet(project);
	return *this;
	}

Project_TreeView::Item Project_TreeView::itemSelectedGet() const noexcept
	{
	return m_impl->itemSelectedGet();
	}

QWidget* Project_TreeView::widget()
	{return m_impl->widget();}

Project_TreeView& Project_TreeView::documentViewAttach(Document::View& view)
	{
	m_impl->documentViewAttach(view);
	return *this;
	}
