//@	{"targets":[{"name":"document.o","type":"object"}]}

#include "document.hpp"
#include "utf8decoder.hpp"
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
,m_filename(filename)
	{
	reload();
	}

void Document::filenameSet(const char* filename_new)
	{
	auto filename_old=m_filename;
	if(m_filename!=filename_new)
		{
		if(m_filename.size()==0)
			{
			m_filename=std::string(filename_new);
			contentSave();
			r_monitor->filenameChanged(*this,"");
			return;
			}
		contentSave();
		if(rename(m_filename.c_str(),filename_new)==0)
			{m_filename=std::string(filename_new);}
		}
	r_monitor->filenameChanged(*this,filename_old.c_str());
	}

void Document::contentSave()
	{
	if(m_filename.size()==0)
		{return;}
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

Document& Document::contentSet(const uint8_t* ptr_data,size_t n)
	{
	if(n==length())
		{
		if(memcmp(ptr_data,begin(),length())==0)
			{return *this;}
		}
	m_content.clear();
	m_content.resize(n);
	memcpy(m_content.data(),ptr_data,n);
	m_dirty=1;
	r_monitor->statusChanged(*this);
	return *this;
	}

void Document::contentSaveAs(const char* filename_new)
	{
	Document doc_new(*this);
	doc_new.m_filename=filename_new;
	doc_new.contentSave();
	contentSave();
	r_monitor->documentCreated(std::move(doc_new));
	}

void Document::contentSaveCopy(const char* filename_new)
	{
	Document doc_new(*this);
	doc_new.contentSave();
	r_monitor->documentCreated(std::move(doc_new));
	m_filename=filename_new;
	contentSave();
	}

void Document::remove()
	{
	if(m_filename.size()!=0)
		{
		if(::remove(m_filename.c_str())==0)
			{r_monitor->documentRemoved(*this);}
		}
	}

void Document::reload()
	{
	m_content=fileLoad(m_filename.c_str());
	m_sel={0,0,0,0};
	m_dirty=0;
	if(length()==0)
		{contentSave();}
	}


bool Document::textfileIs() const noexcept
	{
	UTF8Decoder decoder(begin(),end());
	uint32_t codepoint=0;
	while(!decoder.done())
		{
		auto status=decoder.codepointGet(codepoint);
		if(!status || codepoint==0)
			{return 0;}
		}
	return 1;
	}
