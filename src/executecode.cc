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

#include "executecode.h"
#include "gui.h"

#include <iostream>
#include <giomm.h>

executecode::executecode(gui *refbackt): ownterm()
{
	refback=refbackt;
}

//makecompatible is in terminal.h

std::string executecode::prepexecute()
{
	std::string patho=""+refback->getfilepath();
	unsigned int pathlength=patho.length();
	if (refback->getfilepath().rfind(".")>=pathlength-4 && refback->getfilepath().rfind(".") <=pathlength)
		patho.erase(patho.rfind("."),pathlength);
	else
	{
		patho+=".out";
	}
	return patho;
}

void executecode::execute()
{
	if (refback->fileentrylength()>0)
	{
		Glib::RefPtr<Gio::File> test1 = Gio::File::create_for_path (prepexecute());
		if (test1->query_exists())
			std::string summaryc=prepexecute();
		else
		{
			Glib::RefPtr<Gio::File> test2 = Gio::File::create_for_path (refback->getfilepath());
			if (test2->query_exists())
				std::string summaryc=refback->getfilepath();
			else
			{
				ownterm.feedtext("Application not found\n");
			}
		}
		std::string summaryc=ownterm.makecompatible(prepexecute())+" "+refback->getexecuteargs()+"\n";
		// 2>&1
		ownterm.feedexe(summaryc);
	}
	else
	{
		refback->paintitred();
		ownterm.feedtext("Path entry is empty\n");
	}

}


Gtk::Widget *executecode::givevteterm()
{
	return ownterm.givevteterm();
}

executecode::~executecode()
{
	refback=0;
}