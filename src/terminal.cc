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


#include <vte/vte.h>
#include <iostream>
#include <cstdio>
#include <cassert>

//#include <cstdlib>

#include "terminal.h"

terminal::terminal()
{
#if GCC_VERSION < 40700
	vteterm=0;
	vtetermcc=0;
	//neccessary for vte_terminal_fork_command_full
	err=0;
#endif

	vteterm=vte_terminal_new();
	vtetermcc=Glib::wrap(vteterm);

	//thanks to thestinger for his example
	char *startterm[2]={0,0};
	startterm[0]=vte_get_user_shell();
	if (!startterm[0])
		startterm[0] = (char*)"/bin/sh";
	

	
	bool test=vte_terminal_fork_command_full(VTE_TERMINAL(vteterm),
			                           VTE_PTY_DEFAULT,
			                           NULL,
			                           startterm,
			                           NULL, //Environment
			                           (GSpawnFlags)(G_SPAWN_DO_NOT_REAP_CHILD | G_SPAWN_SEARCH_PATH),  //Spawnflags
			                           NULL,
			                           NULL,
			                           &pidterm,
		                               &err);
	if (!test)
	{
		std::cerr << "Terminal child didn't start.\n";
		assert(true);
	}
	
}

std::string terminal::makecompatible(std::string input)
{
	std::string temp2="";
	for (int i=0;i<input.length();i++)
	{
		temp2+=input[i];
			if (input[i+1]==' ')
				temp2+="\\";
	}
	return temp2;
}

Gtk::Widget *terminal::givevteterm()
{
	return vtetermcc;
}

void terminal::feedexe(std::string input)
{
	vte_terminal_feed_child (VTE_TERMINAL(vteterm),input.c_str(),input.length());
}


void terminal::feedtext(std::string input)
{
	std::string temp2="";
	
	if (input.empty()==false)
	{
		for (int i=0;i<input.length();i++)
		{
			temp2+=input[i];
			if (input[i]=='\n')
				temp2+="\r";
		}
		vte_terminal_feed(VTE_TERMINAL(vteterm),temp2.c_str(),temp2.length());
	}
}

void terminal::reset()
{
	vte_terminal_reset(VTE_TERMINAL(vteterm),true,true);
}


terminal::~terminal()
{
}
