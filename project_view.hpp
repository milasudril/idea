//@	{"targets":[{"name":"project_view.hpp","type":"include"}]}

#ifndef IDEA_PROJECT_VIEW_HPP
#define IDEA_PROJECT_VIEW_HPP

namespace Idea
	{
	class Project;
	class Project_View
		{
		public:
			virtual Project_View& projectSet(Project& project)=0;
		};
	}

#endif
