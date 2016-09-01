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
#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexerbatch.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexermatlab.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexertex.h>
#include <Qsci/qscilexerxml.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QApplication>
#include <QFontDatabase>
#include <QMessageBox>
#include <QPushButton>
#include <QPixmap>
#include <QLabel>

using namespace Idea;

class Document_ContentView::Impl:public QWidget
	{
	public:
		Impl(QWidget& parent):QWidget(&parent),r_document(nullptr)
			,m_filename(this),m_save(this),m_reload(this),m_delete(this)
			,m_scintilla(this),m_lexer(nullptr),m_pixview(this)
			{
			m_scintilla.setMarginLineNumbers(1,1);
			m_scintilla.setFolding(QsciScintilla::CircledTreeFoldStyle);
			m_scintilla.setMarginsFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
			m_scintilla.setMarginWidth(1,"9999");
			m_scintilla.setIndentationsUseTabs(1);
			m_scintilla.setTabWidth(4);
			m_scintilla.setIndentationGuides(1);
			m_scintilla.setUtf8(1);

			filenameCatchChangeEvent();
			m_save.setText("⛁");
			connect(&m_save,&QToolButton::clicked,[this](bool checked)
				{
				if(r_document!=nullptr)
					{
					documentCopy();
					r_document->contentSave();
					}
				});

			m_reload.setText("⟲");
			connect(&m_reload,&QToolButton::clicked,[this](bool checked)
				{
				if(r_document!=nullptr)
					{
					r_document->reload();
					auto doc_temp=r_document;
					r_document=nullptr;
					documentSet(*doc_temp);
					}
				});
		
			m_delete.setText("✗");
			deleteCatchClicked();

			m_toolbar.addWidget(&m_filename);
			m_toolbar.addWidget(&m_save);
			m_toolbar.addWidget(&m_reload);
			m_toolbar.addWidget(&m_delete);
			m_toolbar.setMargin(0);
			m_toolbar.setSpacing(2);
			m_box.addLayout(&m_toolbar);
			m_box.addWidget(&m_scintilla);
			r_view_current=&m_scintilla;
			m_box.setMargin(4);
			setLayout(&m_box);
			m_lexer=nullptr;
			}
//👓🔎
		~Impl()
			{delete m_lexer;}

		void documentSet(Document& document)
			{
			if(r_document!=nullptr)
				{documentCopy();}

			r_document=&document;

		//	Choose widget depending on the document content.
		//	This simplistic solution works fine for two kinds (here: text, and image)
		//	Do not add more document types unless a vtable dispatch is introduced
			if(r_view_current!=nullptr)
				{
				r_view_current->hide();
				m_box.removeWidget(r_view_current);
				r_view_current=nullptr;
				}
			if(r_document->textfileIs())
				{
				scintillaConfig();
				r_view_current=&m_scintilla;
				}
			else
			if(pixviewConfig())
				{r_view_current=&m_pixview;}
			
			if(r_view_current!=nullptr)
				{
				m_box.addWidget(r_view_current);
				r_view_current->show();
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
		QToolButton m_reload;
		QToolButton m_delete;
		QsciScintilla m_scintilla;
		QsciLexer* m_lexer;
		QLabel m_pixview;

		QWidget* r_view_current;

		void documentCopy()
			{
			if(r_document->textfileIs())
				{
				auto swap=m_scintilla.text().toUtf8();
				r_document->contentSet(reinterpret_cast<const uint8_t*>(swap.constData())
					,swap.size());
				Document::Selection sel;
				m_scintilla.getSelection(&sel.line_from,&sel.index_from
					,&sel.line_to,&sel.index_to);
				r_document->selectionSet(sel);
				}
			}

		bool pixviewConfig()
			{
			QPixmap pixmap;
			if(pixmap.loadFromData(r_document->begin(),r_document->length()))
				{
				m_pixview.setScaledContents(1);
				m_pixview.setPixmap(pixmap);
			//	m_pixview.setMinimumSize(QSize(0,0));
				m_pixview.setSizePolicy(QSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored));
			//	m_pixview.setContentType(QSizePolicy::DefaultType);
				return 1;
				}
			return 0;
			}

		void scintillaConfig()
			{
			QByteArray tmp(reinterpret_cast<const char*>(r_document->begin())
				,r_document->length());
			m_scintilla.setText(tmp);
			auto sel=r_document->selectionGet();
			m_scintilla.setSelection(sel.line_from,sel.index_from,sel.line_to,sel.index_to);

			auto pos=strrchr(r_document->filenameGet(),'.');
			m_scintilla.setWrapMode(QsciScintilla::WrapNone);
			if(pos!=NULL)
				{
				delete m_lexer;
				m_lexer=nullptr;
				if(strcmp(pos,".cpp")==0 || strcmp(pos,".hpp")==0
					|| strcmp(pos,".h")==0)
					{m_lexer=new QsciLexerCPP;}
				else
				if(strcmp(pos,".json")==0 || strcmp(pos,".js")==0)
					{m_lexer=new QsciLexerJavaScript;}
				else
				if(strcmp(pos,".sh")==0)
					{m_lexer=new QsciLexerBash;}
				else
				if(strcmp(pos,".bat")==0)
					{m_lexer=new QsciLexerBatch;}
				else
				if(strcmp(pos,".css")==0)
					{m_lexer=new QsciLexerCSS;}
				else
				if(strcmp(pos,".html")==0 || strcmp(pos,".htm")==0)
					{
					m_lexer=new QsciLexerHTML;
					m_scintilla.setWrapMode(QsciScintilla::WrapWord);
					}
				else
				if(strcmp(pos,".xml")==0 || strcmp(pos,".xsl")==0 ||
					strcmp(pos,".xslt")==0 || strcmp(pos,".xsd")==0 )
					{
					m_lexer=new QsciLexerXML;
					m_scintilla.setWrapMode(QsciScintilla::WrapWord);
					}
				else
				if(strcmp(pos,".md")==0)
					{m_scintilla.setWrapMode(QsciScintilla::WrapWord);}
				else
				if(strcmp(pos,".m")==0)
					{m_lexer=new QsciLexerMatlab;}
				else
				if(strcmp(pos,".py")==0)
					{m_lexer=new QsciLexerPython;}
				else
				if(strcmp(pos,".tex")==0)
					{
					m_lexer=new QsciLexerTeX;
					m_scintilla.setWrapMode(QsciScintilla::WrapWord);
					}

				if(m_lexer!=nullptr)
					{m_lexer->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont),-1);}
				else
					{m_scintilla.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));}
				m_scintilla.setLexer(m_lexer);
				}
			}

		void filenameCatchChangeEvent()
			{
			connect(&m_filename,&QLineEdit::returnPressed,[this]()
				{
				if(r_document==nullptr)
					{return;}
				documentCopy();
				auto filename=m_filename.text().toUtf8();
				if(filename.size()!=0 && r_document->filenameGet()!=filename)
					{
					if(*( r_document->filenameGet() )=='\0')
						{
						r_document->contentSaveAs(filename.constData());
						return;
						}

					QMessageBox msgbox(this);
					msgbox.setIcon(QMessageBox::Question);
					msgbox.setWindowTitle("Filename change");
					msgbox.setText("The filename was edited. Pick an action.");
					auto rename=msgbox.addButton("Rename",QMessageBox::ActionRole);
					auto saveas=msgbox.addButton("Save as",QMessageBox::ActionRole);
					auto savecopy=msgbox.addButton("Save a copy",QMessageBox::ActionRole);
					msgbox.addButton(QMessageBox::Cancel);
					msgbox.exec();

					auto button_clicked=msgbox.clickedButton();
					if(button_clicked==rename)
						{r_document->filenameSet(filename.constData());}
					else
					if(button_clicked==saveas)
						{r_document->contentSaveAs(filename.constData());}
					else
					if(button_clicked==savecopy)
						{r_document->contentSaveCopy(filename.constData());}
					else
						{m_filename.setText(r_document->filenameGet());}
					}
				});
			}

		void deleteCatchClicked()
			{
			connect(&m_delete,&QToolButton::clicked,[this](bool checked)
				{
				if(r_document!=nullptr)
					{
					if(*(r_document->filenameGet())!='\0')
						{
						QMessageBox msgbox(this);
						msgbox.setWindowTitle("Deleting file");
						QString str("Are you sure you want to delete ");
						str+=r_document->filenameGet();
						str+="?";
						msgbox.setText(str);
						msgbox.setIcon(QMessageBox::Warning);
						msgbox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
						if(msgbox.exec()==QMessageBox::Yes)
							{
							auto doc_current=r_document;
							r_document=nullptr;
							doc_current->remove();
							}
						}
					}
				});
			}
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
