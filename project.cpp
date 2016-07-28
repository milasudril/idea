//@	{"targets":[{"name":"project.o","type":"object"}]}

#include "project.hpp"
#include <cassert>
#include <sys/stat.h>
#include <unistd.h>

using namespace Idea;

Project::Project(const char* project_root,View& view):m_root(project_root)
	,r_doc_current(nullptr)
	{
	viewAttach(view);
	documentCreate("");
	}

Project& Project::documentCreate(const char* filename)
	{
	auto key=std::string(filename);
	auto i=m_documents.find(key);
	if(i==m_documents.end())
		{
		i=m_documents.insert({std::move(key),Document(filename,*this)}).first;
		}
	documentCurrentSet(i->second);
	return *this;
	}

Project& Project::documentCurrentSet(Document& document)
	{
	r_doc_current=&document;
	r_view->documentCurrentSet(*this);
	return *this;
	}

Project& Project::documentCurrentSet(const char* filename)
	{
	auto key=std::string(filename);
	auto i=m_documents.find(key);
	if(i==m_documents.end())
		{
		auto ip=m_documents.insert({std::move(key),Document(filename,*this)});
		return documentCurrentSet(ip.first->second);
		}
	return documentCurrentSet(i->second);
	}


Project& Project::viewAttach(View& view)
	{
	r_view=&view;
	r_view->projectSet(*this);
	return *this;
	}

void Project::statusChanged(const Document& document)
	{
//	r_view->statusUpdate(document);
	}

void Project::filenameChanged(const Document& document,const char* filename_old)
	{
	auto key=std::string(document.filenameGet());
	if(key!=filename_old)
		{
		auto i=m_documents.find(key);
		if(i==m_documents.end())
			{i=m_documents.insert({key,document}).first;}
		else
			{i->second=document;}

		auto i_old=m_documents.find(std::string(filename_old));
		auto doc_deleted=&i_old->second;
		if(doc_deleted==r_doc_current)
			{documentCurrentSet(i->second);}
		m_documents.erase(i_old);
		}
	}

void Project::documentCreated(Document&& document_new)
	{
	auto key=std::string(document_new.filenameGet());
	auto i=m_documents.find(key);
	if(i==m_documents.end())
		{
		documentCurrentSet(m_documents.insert({std::move(key),std::move(document_new)}).first->second);
		}
	else
		{
		i->second=std::move(document_new);
		documentCurrentSet(i->second);
		}
	}

void Project::documentRemoved(Document& document)
	{
	auto key=std::string(document.filenameGet());
		{
		auto i=m_documents.find(std::string(""));
		assert(i!=m_documents.end());
		documentCurrentSet(i->second);
		}

		{
		auto i=m_documents.find(key);
		m_documents.erase(i);
		}
	}

Project& Project::directoryCreate(const char* name)
	{
	mkdir(name,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
	return *this;
	}

void Project::directoryRemove(const char* name)
	{
	rmdir(name);
	}

void Project::fileRemove(const char* name)
	{
	auto key=std::string(name);
	auto i=m_documents.find(key);
	if(i!=m_documents.end())
		{i->second.remove();}
	}

void Project::documentRemoved(const char* name)
	{
	auto key=std::string(name);
	auto i=m_documents.find(key);
	if(i!=m_documents.end())
		{
		if(&i->second==r_doc_current)
			{
			auto i_empty=m_documents.find(std::string(""));
			assert(i_empty!=m_documents.end());
			documentCurrentSet(i_empty->second);
			}
		m_documents.erase(i);
		}
	}

void Project::documentsSave()
	{
	auto i=m_documents.begin();
	auto i_end=m_documents.end();
	while(i!=i_end)
		{
		if(i->second.dirtyIs())
			{i->second.contentSave();}
		++i;
		}
	}
