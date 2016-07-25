//@	{"targets":[{"name":"project.o","type":"object"}]}

#include "project.hpp"
#include "project_view.hpp"

using namespace Idea;

Project::Project(const char* project_root):m_root(project_root)
	{}

Project& Project::fileCreate(const char* filename)
	{
	return *this;
	}

Project& Project::viewAttach(Project_View& view)
	{
	r_view=&view;
	r_view->projectSet(*this);
	return *this;
	}
