//@	{
//@	 "targets":[{"name":"document.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"document.o","rel":"implementation}]
//@	"}

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
			Document(const char* filename);

			bool dirtyIs() const noexcept
				{return m_dirty;}

			Document& filenameSet(const char* filename_new);

			const char* filenameGet() const noexcept
				{return m_filename.c_str();}

			Document& contentSet(const uint8_t* ptr_data,size_t length);

			void contentSave();

		private:
			std::string m_filename;
			std::vector<uint8_t> m_content;
			bool m_dirty;
		};
	}

#endif
