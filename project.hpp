//@	{
//@	 "targets":[{"name":"project.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"project.o","rel":"implementation"}]
//@	}

#ifndef IDEA_PROJECT_HPP
#define IDEA_PROJECT_HPP

#include <string>
#include <map>

namespace Idea
	{
	class Project_View;
	class Project
		{
		public:
			Project(const char* project_root);

			const char* rootGet() const noexcept
				{return m_root.c_str();}

			Project& fileCreate(const char* filename);

			Project& viewAttach(Project_View& view);

		private:
			std::string m_root;
			std::map<std::string,std::string> m_documents;
			std::string* r_doc_current;
			Project_View* r_view;
		};
	}

#endif
