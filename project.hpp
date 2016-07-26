//@	{
//@	 "targets":[{"name":"project.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"project.o","rel":"implementation"}]
//@	}

#ifndef IDEA_PROJECT_HPP
#define IDEA_PROJECT_HPP

#include "document.hpp"
#include <string>
#include <map>

namespace Idea
	{
	class Document;
	class Project:public Document::Monitor
		{
		public:
			class View
				{
				public:
					virtual View& projectSet(Project& project)=0;
					virtual View& documentCurrentSet(Project& project)=0;
				};

			Project(const char* project_root,View& view);

			const char* rootGet() const noexcept
				{return m_root.c_str();}

			Project& documentCreate(const char* filename);

			Project& viewAttach(View& view);

			Project& documentCurrentSet(const char* filename);

			Project& documentCurrentSet(Document& document);

			void statusChanged(const Document& document);

			void filenameChanged(const Document& document,const char* filename_old);

			void documentCreated(Document&& document_new);

			Document& documentCurrentGet() const noexcept
				{return *r_doc_current;}


		private:
			std::string m_root;
			std::map<std::string,Document> m_documents;
			Document* r_doc_current;
			View* r_view;
		};
	}

#endif
