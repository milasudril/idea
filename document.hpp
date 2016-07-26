//@	{
//@	 "targets":[{"name":"document.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"document.o","rel":"implementation"}]
//@	}

#ifndef IDEA_DOCUMENT_HPP
#define IDEA_DOCUMENT_HPP

#include <cstdint>
#include <vector>
#include <string>

namespace Idea
	{
	class Document
		{
		public:
			class Monitor
				{
				public:
					virtual void statusChanged(const Document& document)=0;
					virtual void filenameChanged(const Document& document,const char* filename_old)=0;
					virtual void documentCreated(Document&& document_new)=0;
				};

			Document(const char* filename,Monitor& monitor);

			bool dirtyIs() const noexcept
				{return m_dirty;}

			Document& filenameSet(const char* filename_new);

			const char* filenameGet() const noexcept
				{return m_filename.c_str();}

			Document& contentSet(const uint8_t* ptr_data,size_t length);

			const uint8_t* begin() const noexcept
				{return m_content.data();}

			const uint8_t* end() const noexcept
				{return begin() + length();}

			size_t length() const noexcept
				{return m_content.size();}


			void contentSave();

			/**Create a copy of current document, rename current document, and
			 * save it.
			*/
			void contentSaveAs(const char* filename_new);

			/**Create a copy of current document, and rename and save the copy.
			*/
			void contentSaveCopy(const char* filename_new);

		private:
			Monitor* r_monitor;
			std::string m_filename;
			std::vector<uint8_t> m_content;
			bool m_dirty;
		};
	}

#endif
