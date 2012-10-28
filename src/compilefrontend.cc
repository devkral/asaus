/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * asaus
 * Copyright (C) alex 2012 <devkral@web.de>
 * 
 * asaus is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * asaus is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "compilefrontend.h"
#include "gui.h"
#include "terminal.h"

#include <giomm.h>
#include <iostream>
//#include <magic.h>
//#include <unistd.h>
//#include <gdk/gdkkeysyms.h>


//makecompatible is in terminal.h

compilefrontend::compilefrontend(gui *refbackt): ownterm(){
	refback=refbackt;
}

std::string compilefrontend::prepare()
{
	std::string patho=refback->getfilepath();
	unsigned int pathlength=patho.length();
	if (refback->getfilepath().rfind(".")>=pathlength-4 && refback->getfilepath().rfind(".") <=pathlength)
		patho.erase(patho.rfind("."),pathlength);
	else
	{
		patho+=".out";
	}

	if (refback->isobcode())
		return "-o "+ownterm.makecompatible(patho)+".o "+gflagtrans();
	else
		return "-o "+ownterm.makecompatible(patho)+" "+gflagtrans();
}

std::string compilefrontend::gflagtrans()
{
	std::string temp="";

	if (refback->isdebug()==true)
	{
		temp+="-g ";
	}
	
	if (refback->isobcode()==true)
	{
		temp+="-c ";
	}
	
	if (refback->optimizegrade()!="")
	{
		temp+="-O"+refback->optimizegrade()+" ";
	}

	std::string tempcompargs=refback->getcompileargs();
	if (!tempcompargs.empty())
		temp+=tempcompargs+" ";
	temp+=ownterm.makecompatible(refback->getfilepath());
	return temp;
}

std::string compilefrontend::filesuffix(std::string inputstr)
{
	unsigned int pathlength=inputstr.length();
	return inputstr.substr(inputstr.rfind("."));
}


Gtk::Widget *compilefrontend::givevteterm()
{
	return ownterm.givevteterm ();
}

void compilefrontend::tacticgcc()
{
	std::string summaryc="/usr/bin/gcc -Wall "+prepare()+"\n";
	ownterm.feedexe(summaryc);
}
void compilefrontend::tacticgxx()
{
	std::string summaryc="/usr/bin/g++ -Wall "+prepare()+"\n";
	ownterm.feedexe(summaryc);
}

void compilefrontend::tacticmake(std::string loc)
{
	std::string summaryc="cd "+ownterm.makecompatible(loc)+";make "+refback->getcompileargs()+"\n";
	ownterm.feedexe(summaryc);
}

void compilefrontend::tacticautogen(std::string loc)
{
	refback->unsetcolor();
	std::string summaryc="cd "+ownterm.makecompatible(loc)+";autogen "+refback->getcompileargs()+"\n";
	ownterm.feedexe(summaryc);
}


void compilefrontend::compile()
{
	if(refback->fileentrylength()>0)
	{
		Glib::RefPtr<Gio::File> temp = Gio::File::create_for_path (refback->getfilepath());
		if (temp->query_exists () | temp->query_file_type()  == Gio::FILE_TYPE_SYMBOLIC_LINK | temp->query_file_type()  == Gio::FILE_TYPE_REGULAR)
		{
			refback->unsetcolor();
			std::string testmakeautogen= temp->get_basename ();
			std::string filesuftemp =filesuffix(testmakeautogen);
			Glib::RefPtr<Gio::File> parent=temp->get_parent();
			if (testmakeautogen=="Makefile")
				tacticmake(parent->get_path());
			else if (testmakeautogen=="autogen.sh")
				tacticautogen(parent->get_path());
			else if (filesuftemp==".cc" | filesuftemp==".cp" | filesuftemp==".cxx" \
			         | filesuftemp==".cpp" | filesuftemp==".cPP"| filesuftemp==".c++" | filesuftemp==".C")
				tacticgxx();
			else
				tacticgcc();
			//else
			//	ownterm.feedtext("Unknown file type\n");
		}
		else
		{
			refback->paintitred();
			ownterm.feedtext("Path doesn't exist\n");
		}
	}
	else
	{
		refback->paintitred();
		ownterm.feedtext("Path entry is empty\n");
	}
}

compilefrontend::~compilefrontend()
{
	refback=0;
}
