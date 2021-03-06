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

//#include "closingdialog.h"

//#include <mutex>
//#include <cstdlib>
//#include <memory>
#include <gtkmm.h>
#include <string>
#include <vector>


#ifndef _GUI_H_
#define _GUI_H_

class gui
{
public:
	gui(int argc, char *argv[]);
	~gui();
	
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
	void unsetcolor();

	int fileentrylength();

	void drag_insert_as_path(const Glib::RefPtr<Gdk::DragContext>& context, int, int,
          const Gtk::SelectionData& selection_data, guint, guint time);
    void drag_compile(const Glib::RefPtr<Gdk::DragContext>& context, int, int,
          const Gtk::SelectionData& selection_data, guint, guint time);
	void drag_execute(const Glib::RefPtr<Gdk::DragContext>& context, int, int,
          const Gtk::SelectionData& selection_data, guint, guint time);
	
	void startspin();
	void stopspin();

	bool isvisible();
	void show();
	void hide();
	void hideshow();
	void fullscreen();


	void exeact();
	void compact();
	
protected:

private:
	Gtk::Main kit;
	Glib::RefPtr<Gtk::Builder> builder;
	//closingdialog closedia;

	Gdk::RGBA red;
	Gdk::RGBA white;
	Gdk::RGBA black;

	compilefrontend compilethread;
	executecode executethread;
	statusicon iconthread;

	bool on_my_window_state_event(GdkEventWindowState* event);
	//variables
	std::string executeargs, compileargs;

	Glib::RefPtr<Gtk::Window> main_win;
	bool closebutton(GdkEventAny*);
	Glib::RefPtr<Gtk::Alignment> terminal;

	//status
	Glib::RefPtr<Gtk::Spinner> spinner1;
	int spinuser;

	//visiblestate
	int visiblestate;
	
	Glib::RefPtr<Gtk::Alignment> actionchoose;
	//execute panel
	Glib::RefPtr<Gtk::Grid> exeops;
	//exe block
	Glib::RefPtr<Gtk::Entry> exeargs;

	//compile panel
	Glib::RefPtr<Gtk::Grid> compops;
	//comp block
	Glib::RefPtr<Gtk::CheckButton> debugbutton;
	Glib::RefPtr<Gtk::CheckButton> obbutton;
	Glib::RefPtr<Gtk::Entry> optimizelevel;
	Glib::RefPtr<Gtk::Entry> compargs;
	
	Glib::RefPtr<Gtk::RadioButton> compilechosebutton;
	Glib::RefPtr<Gtk::RadioButton> compilechosebutton2;

	Glib::RefPtr<Gtk::Button> execute;
	Glib::RefPtr<Gtk::Button> compilebutton;

	//filechoose
	Glib::RefPtr<Gtk::Button> filechoosebutton;
	Glib::RefPtr<Gtk::Entry> fileentry;
	std::string lastfile;

	//functions
	void changeaction();

	void compacthelper();
	void exeacthelper();
	
	void fileact();
	
	std::vector<Gtk::TargetEntry> Targets;

	
};

#endif // _GUI_H_
