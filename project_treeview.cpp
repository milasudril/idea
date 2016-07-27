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
#include <QInputDialog>
#include <QMessageBox>

using namespace Idea;

class Project_TreeView::Impl:public QWidget
	{
	public:
		Impl(QWidget& parent,EventHandler& event_handler,Project_TreeView& owner):
			QWidget(&parent),m_tree(this),m_newfile(this),m_newdir(this),m_rm(this)
			,r_owner(owner)
			,r_event_handler(&event_handler),r_doc_view(nullptr)
			{
			m_tree.setModel(&m_fs);
			m_tree.setHeaderHidden(1);
			m_tree.header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
			m_box.addWidget(&m_tree);
			m_box.setMargin(4);
			m_newfile.setText("✚📄");
			m_newdir.setText("✚📁");
			m_rm.setText("✗");
			m_projman.addWidget(&m_newfile);
			m_projman.addWidget(&m_newdir);
			m_projman.addWidget(&m_rm);
			m_box.addLayout(&m_projman);
			setLayout(&m_box);

		//	If no item is selected, use the project root directory as selected item.
			m_isdir=1;

			connect(m_tree.selectionModel(),&QItemSelectionModel::selectionChanged
				,[this](const QItemSelection& sel_new,const QItemSelection& sel_old)
					{
					auto fileinfo=m_fs.fileInfo(sel_new.indexes().first());
					auto in_dir_full=QDir(m_fs.rootDirectory());
					m_dir=in_dir_full.relativeFilePath( fileinfo.absolutePath() ).toLocal8Bit();
					m_filename=in_dir_full.relativeFilePath(fileinfo.absoluteFilePath()).toLocal8Bit();
					m_isdir=fileinfo.isDir();
					r_event_handler->itemSelected(r_owner);
					});

			connect(&m_newfile,&QPushButton::clicked,[this](bool selected)
				{
				QInputDialog dlg(this);
				dlg.setLabelText("Choose a filename");
				dlg.setWindowTitle("Create a new file");
				if(dlg.exec())
					{
					r_event_handler->fileCreate(r_owner,dlg.textValue().toUtf8().constData());
					}
				});

			connect(&m_newdir,&QPushButton::clicked,[this](bool selected)
				{
				QInputDialog dlg(this);
				dlg.setLabelText("Choose a directory name");
				dlg.setWindowTitle("Create a new directory");
				if(dlg.exec())
					{
					r_event_handler->directoryCreate(r_owner,dlg.textValue().toUtf8().constData());
					}
				});

			connect(&m_rm,&QPushButton::clicked,[this](bool selected)
				{
				if(m_filename.size()!=0)
					{
					QMessageBox msgbox(this);
					msgbox.setWindowTitle("Deleting item");
					QString str("Are you sure you want to delete ");
					str+=m_filename;
					str+="?";

					msgbox.setText(str);
					msgbox.setIcon(QMessageBox::Warning);
					msgbox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
					if(msgbox.exec()==QMessageBox::Yes)
						{r_event_handler->itemDelete(r_owner);}
					}
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
			
			auto& doc=proj.documentCurrentGet();
			if(r_doc_view!=nullptr)
				{r_doc_view->documentSet(doc);}
		
		//	Select current document in the tree view
			auto filename=doc.filenameGet();
			auto index_a=m_fs.index(filename,0);
			auto index_b=m_fs.index(filename,m_fs.columnCount() - 1);

			auto selection=m_tree.selectionModel();
			QSignalBlocker blocker(selection);
			if(selection->hasSelection())
				{selection->clearSelection();}
			selection->select(QItemSelection(index_a,index_b),QItemSelectionModel::Select);
			}

	private:
		QFileSystemModel m_fs;
		QVBoxLayout m_box;
		QTreeView m_tree;
		QHBoxLayout m_projman;
		QPushButton m_newfile;
		QPushButton m_newdir;
		QPushButton m_rm;


	//	Properties about selected item
		QByteArray m_dir;
		QByteArray m_filename;
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
