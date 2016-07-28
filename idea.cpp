//@	{
//@	"targets":
//@		[{
//@		"name":"idea","type":"application"
//@		,"pkgconfig_libs":["Qt5Widgets"]
//@		}]
//@	}

#include "project.hpp"
#include "project_treeview.hpp"
#include "project_treeviewevents.hpp"
#include "document_contentview.hpp"

#include <QApplication>
#include <QVBoxLayout>
#include <QSplitter>

int main(int argc,char** argv)
	{
	QApplication app(argc,argv);
	QCoreApplication::setApplicationName("IDEa");

	QWidget mainwin;
	mainwin.resize(1024,768);
	QVBoxLayout vbox;
	QSplitter hbox(&mainwin);

	Idea::Document_ContentView v(mainwin);

	Idea::Project_TreeViewEvents tree_events;
	Idea::Project_TreeView proj_tree(mainwin,tree_events);
	proj_tree.documentViewAttach(v);
	Idea::Project proj(".",proj_tree);
	proj.viewAttach(proj_tree);

	hbox.addWidget(proj_tree.widget());
	hbox.addWidget(v.widget());

	hbox.setStretchFactor(0,0);
	hbox.setStretchFactor(1,1);

	vbox.setMargin(2);
	vbox.addWidget(&hbox);

	mainwin.setLayout(&vbox);
	mainwin.show();

	auto ret=app.exec();
	
	proj.documentsSave();
	return ret;
	}
