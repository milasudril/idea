//@	{
//@	"targets":
//@		[{
//@		"name":"idea","type":"application"
//@		,"pkgconfig_libs":["Qt5Widgets"]
//@		}]
//@	}

#include "project.hpp"
#include "project_treeview.hpp"

#include <QApplication>

int main(int argc,char** argv)
	{
	QApplication app(argc,argv);
	Idea::Project_TreeView proj_tree;
	Idea::Project proj(".",proj_tree);
	proj.viewAttach(proj_tree);

	return app.exec();
	}
