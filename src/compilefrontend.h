/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * asaus
 * Copyright (C) alex 2012 <alex@archal>
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


#include "terminal.h"

#include <string>
#include <gtkmm.h>

class gui;

#ifndef _COMPILE_H_
#define _COMPILE_H_


class compilefrontend
{
public:
	compilefrontend(gui *refbackt);
	void compile();
	Gtk::Widget *givevteterm();
protected:
	gui *refback;

private:
	terminal ownterm;


	void tacticgcc();
	void tacticmake(std::string loc);
	void tacticautogen(std::string loc);
	
		
	std::string prepare();
	std::string gflagtrans();
};

#endif // _COMPILE_H_
