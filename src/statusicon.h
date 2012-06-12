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

#include <vector>
#include <string>
#include <gtkmm.h>

class gui;

#ifndef _STATUSICON_H_
#define _STATUSICON_H_

class statusicon
{
public:
	statusicon(gui *refbackt);
	~statusicon();

protected:
	//Glib::RefPtr<gui> refback 
	gui* refback;
	//std::shared_ptr<gui> refback;

private:
	Glib::RefPtr<Gtk::StatusIcon> icon;
	std::vector<Gtk::TargetEntry> Targets;

};

#endif // _STATUSICON_H_
