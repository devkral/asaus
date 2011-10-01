/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.cc
 * Copyright (C) alex 2011 <devkral@web.de>
 * 
 * 
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name ``alex'' nor the name of any other
 *    contributor may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 
 * asaus IS PROVIDED BY alex ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL alex OR ANY OTHER CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * 
 * 
 */

#include <gtkmm.h>
#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;
using namespace Gtk;

class asaus{
	
	
	public:
	string flags;		//user compiler flags
	string path;		//path with ending
	string arguments;	//arguments, which are added to the executable
	string optimized;	//optgrade
	bool obcode;		//objectcode true, false
	bool debug;			//include debug code
	private:
	bool is_execute;

	string prepare(bool compile)
	{
		unsigned int pathlength=path.length();
		string patho=path;
		if (path.rfind(".o")==pathlength-2)
		{
			patho.erase(patho.rfind(".o"),patho.find_last_of(".o"));
		};
		if (path.rfind(".cc")==pathlength-3)
		{
			patho.erase(patho.rfind(".cc"),patho.find_last_of(".cc"));
		};
		if (path.rfind(".cxx")==pathlength-4)
		{
			patho.erase(patho.rfind(".cxx"),patho.find_last_of(".cxx"));
		};
		if (path.rfind(".cpp")==pathlength-4)
		{
			patho.erase(patho.rfind(".cpp"),patho.find_last_of(".cpp"));
		};
		
		if (path.rfind(".o") > pathlength-2 && path.rfind(".cc") > pathlength-3 && path.rfind(".cxx") > pathlength-4 && path.rfind(".cpp") > pathlength-4 && compile==false)
		{
			patho=path;
		}
		
		if (compile==false)
		{
			return patho;
		}
		else
		{
			if (obcode==true)
			return "-o "+patho+".o"+" "+flags;
			else
			return "-o "+patho+" "+flags;
		}
	}

	string gflagtrans()
	{
		string temp="";
		if (obcode==true)
		{
			temp+=" -c";
		}

		if (debug==true)
		{
			temp+=" -g";
		}
		
		if (optimized!="")
		{
			temp+=" -O"+optimized[0];
			
		}
		
		return temp;
	}
	

public:	
	asaus()
	{
		debug=false;
		obcode=false;
		is_execute=false;
		optimized="";
		
	}
	
	
	void compile()
	{
			string summaryc="/usr/bin/g++ -Wall "+prepare(true)+gflagtrans()+" "+path;
			system(summaryc.c_str());
	}	
	

	static void *execute(void *as)
	{
		
		if (((asaus *)as)->prepare(false)!="")
		{
			string summarye="/usr/bin/xterm -hold -e "+((asaus *)as)->prepare(false)+" "+((asaus *)as)->arguments;
			((asaus *)as)->is_execute=true;
			system(summarye.c_str());
			((asaus *)as)->is_execute=false;
		}
		return 0;
	};
	
	void work(void *as)
	{
		((asaus *)as)->compile();
	}

};





bool fin(GdkEventAny*)
{
	cout << "Goodbye" << endl;
	return false;
};


class graphic
{
//Declarations

pthread_t spini;
asaus compas;
Main graphicmain;
Window window;
Entry path, flags, pathe, optim, arguments;
Grid gri;
Label pathl, flagsl, optimal, argumentsl;
Button send, filechose, execute;
CheckButton object_code, debugb;
Spinner spin;
vector<TargetEntry> Targets;

	void chose()
	{
		
		FileChooserDialog select("Please choose a file", FILE_CHOOSER_ACTION_OPEN);
		select.add_button(Gtk::Stock::CANCEL, RESPONSE_CANCEL);
		select.add_button("Select", RESPONSE_OK);
		switch
		(
			select.run()
		)
		{
			case(RESPONSE_OK): path.set_text(select.get_filename());
			compas.path=select.get_filename();
			break;
			case(RESPONSE_CANCEL):
			break;
			default: cerr << "Invalid button" << endl;
			break;
		}
	}
	
	void set_path()
	{
		compas.path=path.get_text();
	}
	
	void drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context, int, int,
          const Gtk::SelectionData& selection_data, guint, guint time)
	{
		string temp=selection_data.get_data_as_string();
		if (temp.find("file://")==0)
		{
			temp.erase(0,7);
			temp.erase(temp.end()-2,temp.end());
		}
		else
		cout << temp.find("file://");
		path.set_text(temp);
		context->drag_finish(false, true, time);	
	}
	
	
	void set_flags()
	{
		compas.flags=flags.get_text();
	}
	
	void set_args()
	{
		compas.arguments=arguments.get_text();
	}
	static void *spinan(void *graphico)
	{
		((graphic *)graphico)->spin.show_now();
		((graphic *)graphico)->spin.start();
		((graphic *)graphico)->compas.work(&((graphic *)graphico)->compas);
		((graphic *)graphico)->spin.stop();
		((graphic *)graphico)->spin.hide();
		return 0;
	}
	
	void invoke()
	{
		if (spini!=0)
		pthread_join(spini, NULL);
		pthread_create(&spini,NULL,&graphic::spinan,this);
		
	}
	
	void exe()
	{
		pthread_t exet;
		pthread_create(&exet,NULL,&asaus::execute,&compas);
	
	}
	
	void set_obcode()
	{
		compas.obcode=object_code.get_active();	
		
	}
	
	void set_debug()
	{
		compas.debug=debugb.get_active();	
		
	}
	
	void set_optim()
	{
		compas.optimized=optim.get_text();
	}

public:
	
	
	graphic(int argc, char *argv[]) : compas(), graphicmain(argc,argv), window(), path(), flags(), send("Compile", false), filechose("Datei auswählen", false),
	execute("Execute program", false), object_code("Objectcode?", false), debugb("Debug mode?", false)
	{
		//Declarations
		spini=0;
		Targets.push_back( Gtk::TargetEntry("STRING") );
		Targets.push_back( Gtk::TargetEntry("text/plain") );
		
		
		//Label 
		pathl.set_text("Path: ");
		flagsl.set_text("Additional compiler arguments: ");
		optimal.set_text("Optimization level:\nempty=no optimization");
		argumentsl.set_text("Arguments for executatable");
		
		//window
		window.set_title( "Asaus Compiler");
		window.add(gri);
		window.set_resizable(false);
		//window.drag_source_set_icon( )	
		window.drag_dest_set(Targets);
		window.signal_drag_data_received().connect(sigc::mem_fun(*this, &graphic::drag_data_get));
		
		window.signal_delete_event().connect(sigc::ptr_fun(fin));
		
		//Buttons
		send.signal_clicked().connect(sigc::mem_fun(this, &graphic::invoke));
		filechose.signal_clicked().connect(sigc::mem_fun(this, &graphic::chose));
		execute.signal_clicked().connect(sigc::mem_fun(this, &graphic::exe));
		
		object_code.signal_toggled().connect(sigc::mem_fun(this, &graphic::set_obcode));
		debugb.signal_toggled().connect(sigc::mem_fun(this, &graphic::set_debug));

		//entry
		path.drag_dest_unset();
		path.drag_dest_set(Targets);
		path.signal_drag_data_received().connect(sigc::mem_fun(*this, &graphic::drag_data_get));
		path.signal_changed().connect(sigc::mem_fun(this, &graphic::set_path));
		flags.signal_changed().connect(sigc::mem_fun(this, &graphic::set_flags));
		optim.signal_changed().connect(sigc::mem_fun(this, &graphic::set_optim));
		arguments.signal_changed().connect(sigc::mem_fun(this, &graphic::set_args));
		
		//grid
		gri.set_row_homogeneous(true);
		gri.attach(pathl,1,1,2,1);
		gri.attach(path,3,1,2,1);
		gri.attach(filechose,5,1,1,1);
		
		gri.attach(object_code,1,2,1,1);
		gri.attach(debugb,2,2,1,1);
		gri.attach(optimal,3,2,1,1);
		gri.attach(optim,4,2,1,1);
		
		gri.attach(flagsl,1,3,2,1);
		gri.attach(flags,3,3,1,1);
		gri.attach(argumentsl,4,3,1,1);
		gri.attach(arguments,5,3,1,1);
		
		gri.attach(spin,1,4,1,1);
		gri.attach(send,3,4,1,1);
		gri.attach(execute,4,4,1,1);
		
		window.show_all_children();
		spin.hide();
		Main::run(window);

	}
	
};

int main(int argc, char *argv[])
{
  graphic(argc, argv);
  return 0;
}