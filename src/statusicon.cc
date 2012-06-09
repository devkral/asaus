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

//For testing use the local icon
#define ICON_FILE "src/asaus.png"


#include "statusicon.h"
#include "gui.h"
#include <iostream>

statusicon::statusicon(gui *refbackt)
{
	refback=refbackt;
	icon=Gtk::StatusIcon::create("Asaus");
	icon->set_from_file(ICON_FILE);

	Targets.push_back( Gtk::TargetEntry("STRING") );
	Targets.push_back( Gtk::TargetEntry("text/plain") );
	icon->signal_activate().connect( sigc::mem_fun(*this,&statusicon::hideshow) );

}

void statusicon::hideshow()
{
	if (refback->isvisible()==true)
		refback->hide();
	else
		refback->show();
}
