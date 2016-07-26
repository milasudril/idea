//@	{"targets":[{"name":"document.o","type":"object"}]}

#include "document.hpp"
#include <cstdio>
#include <memory>
#include <cstring>

using namespace Idea;

static void file_close(FILE* file)
	{
	if(file!=NULL)
		{fclose(file);}
	}

typedef std::unique_ptr<FILE,decltype(&file_close)> FileHandle;

static std::vector<uint8_t> fileLoad(const char* filename)
	{
	FileHandle f(fopen(filename,"rb"),file_close);
	auto x=f.get();
	if(x==NULL)
		{return std::vector<uint8_t>();}

	std::vector<uint8_t> ret;
	int ch_in;
	while((ch_in=getc(x))!=EOF)
		{ret.push_back(static_cast<uint8_t>(ch_in));}

	return std::move(ret);
	}

Document::Document(const char* filename,Monitor& monitor):r_monitor(&monitor)
,m_filename(filename),m_content(fileLoad(filename))
	{
	m_dirty=0;
	}

Document& Document::filenameSet(const char* filename_new)
	{
	auto filename_old=m_filename;
	if(m_filename!=filename_new)
		{
		contentSave();
		if(rename(m_filename.c_str(),filename_new)!=0)
			{m_filename=filename_new;}
		}
	r_monitor->filenameChanged(*this,filename_old.c_str());
	return *this;
	}

void Document::contentSave()
	{
	FileHandle f(fopen(m_filename.c_str(),"wb"),file_close);
	auto x=f.get();
	if(x==NULL)
		{throw "Could not open file";}
	auto ptr=begin();
	auto ptr_end=end();
	while(ptr!=ptr_end)
		{
		putc(*ptr,x);
		++ptr;
		}
	m_dirty=0;
	r_monitor->statusChanged(*this);
	}

Document& Document::contentSet(const uint8_t* ptr_data,size_t length)
	{
	m_content.clear();
	m_content.resize(length);
	memcpy(m_content.data(),ptr_data,length);
	m_dirty=1;
	r_monitor->statusChanged(*this);
	return *this;
	}

void Document::contentSaveAs(const char* filename_new)
	{
	Document doc_new(*this);
//	FIXME This will not work if filenameSet failed.
	filenameSet(filename_new);
	contentSave();
	doc_new.contentSave();
	r_monitor->documentCreated(std::move(doc_new));
	}

/**Create a copy of current document, and rename and save the copy.
*/
void Document::contentSaveCopy(const char* filename_new)
	{
	Document doc_new(*this);
	doc_new.filenameSet(filename_new);
	doc_new.contentSave();
	r_monitor->documentCreated(std::move(doc_new));
	}
