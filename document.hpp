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
					virtual void documentRemoved(Document& document)=0;
				};

			class View
				{
				public:
					virtual View& documentSet(Document& document)=0;
					virtual Document& documentGet() const noexcept=0;
				};

			struct Selection
				{
				int line_from;
				int index_from;
				int line_to;
				int index_to;
				};

			Document(const char* filename,Monitor& monitor);

			void remove();
			
			void reload();

			bool dirtyIs() const noexcept
				{return m_dirty;}

			void filenameSet(const char* filename_new);

			const char* filenameGet() const noexcept
				{return m_filename.c_str();}

			Document& contentSet(const uint8_t* ptr_data,size_t n);

			Document& selectionSet(const Selection& sel) noexcept
				{
				m_sel=sel;
				return *this;
				}

			const Selection& selectionGet() const noexcept
				{return m_sel;}

			const uint8_t* begin() const noexcept
				{return m_content.data();}

			const uint8_t* end() const noexcept
				{return begin() + length();}

			size_t length() const noexcept
				{return m_content.size();}


			void contentSave();

			void contentSaveAs(const char* filename_new);

			void contentSaveCopy(const char* filename_new);

			bool textfileIs() const noexcept;

		private:
			Monitor* r_monitor;
			std::string m_filename;
			std::vector<uint8_t> m_content;
			Selection m_sel;
			bool m_dirty;
		};
	}

#endif
