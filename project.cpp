//@	{"targets":[{"name":"project.o","type":"object"}]}

#include "project.hpp"

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
		auto ip=m_documents.insert({std::move(key),Document(filename,*this)});
		documentCurrentSet(ip.first->second);
		}
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
		auto i_old=m_documents.find(std::string(filename_old));
		auto doc_deleted=&i_old->second;
		m_documents.erase(i_old);

		auto i=m_documents.find(key);
		if(i==m_documents.end())
			{m_documents.insert({key,document});}
		else
			{i->second=document;}

		if(doc_deleted==r_doc_current)
			{documentCurrentSet(i->second);}
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
