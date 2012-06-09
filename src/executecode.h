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

#include "terminal.h"
#include <string>
#include <gtkmm.h>

class gui;

#ifndef _EXECUTECODE_H_
#define _EXECUTECODE_H_

class executecode
{
public:
	executecode(gui *refbackt);
	void execute();
	Gtk::Widget *givevteterm();
protected:
	gui* refback=0;
	
private:
	terminal ownterm;
	std::string prepexecute();
};

#endif // _EXECUTECODE_H_
