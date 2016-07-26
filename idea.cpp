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

#include <QApplication>

int main(int argc,char** argv)
	{
	QApplication app(argc,argv);
	Idea::Project_TreeViewEvents tree_events;
	Idea::Project_TreeView proj_tree(tree_events);
	Idea::Project proj(".",proj_tree);
	proj.viewAttach(proj_tree);

	return app.exec();
	}
