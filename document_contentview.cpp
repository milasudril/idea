//@	{
//@	 "targets":
//@		[{
//@		 "name":"document_contentview.o","type":"object"
//@		,"pkgconfig_libs":["Qt5Widgets"]
//@		,"dependencies":[{"ref":"qt5scintilla2","rel":"external"}]
//@		}]
//@	}

#include "document_contentview.hpp"

#include <QWidget>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerjavascript.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QApplication>
#include <QFontDatabase>

using namespace Idea;

class Document_ContentView::Impl:public QWidget
	{
	public:
		Impl(QWidget& parent):QWidget(&parent),r_document(nullptr)
			,m_filename(this),m_save(this),m_delete(this),m_scintilla(this)
			{
			m_scintilla.setMarginLineNumbers(1,1);
			m_scintilla.setFolding(QsciScintilla::CircledTreeFoldStyle);
			m_scintilla.setMarginsFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
			m_scintilla.setMarginWidth(1,"9999");
			m_scintilla.setIndentationsUseTabs(1);
			m_scintilla.setTabWidth(4);
			m_scintilla.setIndentationGuides(1);
			m_save.setText("⛁");
			m_delete.setText("✗");
			m_toolbar.addWidget(&m_filename);
			m_toolbar.addWidget(&m_save);
			m_toolbar.addWidget(&m_delete);
			m_toolbar.setMargin(0);
			m_toolbar.setSpacing(2);
			m_box.addLayout(&m_toolbar);
			m_box.addWidget(&m_scintilla);
			m_box.setMargin(4);
			setLayout(&m_box);
			m_lexer=nullptr;
			}

		~Impl()
			{delete m_lexer;}

		void documentSet(Document& document)
			{
			if(r_document!=nullptr)
				{
				auto swap=m_scintilla.text().toUtf8();
				r_document->contentSet(reinterpret_cast<const uint8_t*>(swap.constData())
					,swap.size());
				}

			r_document=&document;
			QByteArray tmp(reinterpret_cast<const char*>(r_document->begin())
				,r_document->length());
			m_scintilla.setText(QString::fromUtf8(tmp));

			auto pos=strrchr(document.filenameGet(),'.');
			if(pos!=NULL)
				{
				if(strcmp(pos,".cpp")==0 || strcmp(pos,".hpp")==0)
					{
					delete m_lexer;
					m_lexer=new QsciLexerCPP;

					}
				else
				if(strcmp(pos,".json")==0 || strcmp(pos,".js")==0)
					{
					delete m_lexer;
					m_lexer=new QsciLexerJavaScript;
					}
				m_lexer->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont),-1);
				m_scintilla.setLexer(m_lexer);
				}
			m_filename.setText(r_document->filenameGet());
			}

		Document& documentGet() const noexcept
			{return *r_document;}

		QWidget* widget() noexcept
			{return this;}

	private:
		Document* r_document;
		QVBoxLayout m_box;
		QHBoxLayout m_toolbar;
		QLineEdit m_filename;
		QToolButton m_save;
		QToolButton m_delete;
		QsciScintilla m_scintilla;
		QsciLexer* m_lexer;
	};

Document_ContentView::Document_ContentView(QWidget& parent):
	m_impl(new Impl(parent))
	{}

Document_ContentView::~Document_ContentView()
	{}

Document_ContentView& Document_ContentView::documentSet(Document& document)
	{
	m_impl->documentSet(document);
	return *this;
	}

Document& Document_ContentView::documentGet() const noexcept
	{
	return m_impl->documentGet();
	}

QWidget* Document_ContentView::widget()
	{return m_impl->widget();}
