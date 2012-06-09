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
#include "executecode.h"
#include "statusicon.h"

#include <mutex>
#include <cstdlib>
//#include <memory>

#include <gtkmm.h>

#include <string>


//#include <gdkmm-3.0/gdkmm/event.h>

//#include "config.h"



/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/ui/asaus.ui" */
#define UI_FILE "src/asaus.ui"

#ifndef _GUI_H_
#define _GUI_H_

class gui
{
public:
	gui(int argc, char *argv[]);
	
	void settermspace(Gtk::Widget &term);
	std::string getexecuteargs();
	std::string getcompileargs();
	std::string getfilepath();
	bool isobcode();
	bool isdebug();
	std::string optimizegrade();
	
	//turn the background of the fileentry widget red (empty)
	void paintitred();
	//turn it back
	void paintitwhite();

	int fileentrylength();

	void drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context, int, int,
          const Gtk::SelectionData& selection_data, guint, guint time);
	
	int startspin();
	void stopspin();

	bool isvisible();
	void show();
	void hide();

	void exeact();
	void compact();

	//gui unreference();
	
protected:

private:
	Gtk::Main kit;
	
	compilefrontend compilethread;
	executecode executethread;
	statusicon iconthread;

	//variables
	std::string executeargs, compileargs;

	Gtk::Window* main_win=0;
	bool closebutton(GdkEventAny*);
	Gtk::Alignment* terminal;

	//status
	Gtk::Spinner* spinner1=0;
	Gtk::Grid* movecont=0;

	
	Gtk::Alignment* actionchoose=0;
	//execute panel
	Gtk::Grid* exeops=0;
	//exe block
	Gtk::Entry* exeargs=0;

	//compile panel
	Gtk::Grid* compops=0;
	//comp block
	Gtk::CheckButton* debugbutton=0;
	Gtk::CheckButton* obbutton=0;
	Gtk::Entry* opimizelevel=0;
	Gtk::Entry* compargs=0;
	
	Gtk::RadioButton* compilechosebutton=0;
	Gtk::RadioButton* compilechosebutton2=0;

	Gtk::Button* execute=0;
	Gtk::Button* compilebutton=0;

	//filechoose
	Gtk::Button* filechoosebutton=0;
	Gtk::Entry* fileentry=0;
	std::string lastfile;

	//mutexe
	std::timed_mutex blockspin;

	//functions
	void changeaction();

	void compacthelper();
	void exeacthelper();
	
	void fileact();

	//void on_window_iconify(Gtk::Allocation& allocation);
	
	std::vector<Gtk::TargetEntry> Targets;

	
};

#endif // _GUI_H_
