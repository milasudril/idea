//@	{
//@	 "targets":[{"name":"document_contentview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"document_contentview.o","rel":"implementation"}]
//@	}

#ifndef DOCUMENT_CONTENTVIEW_HPP
#define DOCUMENT_CONTENTVIEW_HPP

#include "document.hpp"
#include <memory>

class QWidget;

namespace Idea
	{
	class Document_ContentView:public Document::View
		{
		public:
			Document_ContentView(QWidget& parent);
			~Document_ContentView();

			Document_ContentView& documentSet(Document& document);
			Document& documentGet() const noexcept;

			QWidget* widget();

		private:
			class Impl;
			std::unique_ptr<Impl> m_impl;
		};
	};

#endif
