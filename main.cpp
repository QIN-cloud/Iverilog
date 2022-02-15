#include "compiler.h"
#include "netlist.h"
#include "pform.h"
#include "parse_api.h"
#include "PDesign.h"
#include "cfg.h"
#include "testpath.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <fstream>
/*
* Verbose messages enabled.
*/
bool verbose_flag = false;
bool warn_portbinding = false;
bool error_implicit = false;
bool warn_implicit  = false;

extern int getopt(int argc, char*argv[], char*fmt);
extern int optind;
extern char*optarg;

extern Design* elaborate(list <const char*>root);

/*
* Keep a heap of identifier strings that I encounter. This is a more
* efficient way to allocate those strings.
*/
StringHeapLex lex_strings;

char*ivlpp_string = 0;

FILE *depend_file = NULL;

std::map<std::string,unsigned> missing_modules;

std::list<const char*> library_suff;

int yydebug;

int main(int argc, char** argv)
{
    min_typ_max_flag = TYP;
    min_typ_max_warn = 10;
    yydebug = 0;
	list<const char*> roots;
	
	bool help_flag = false;
	unsigned flag_errors = 0;
	bool toggle = false; //true for slice, false for gentest;
	bool fsm = false;
	bool combine = false;
	bool path = false;
	bool statement = false;
	bool all = false;
	
	const char* fsm_vars = 0;
	const char* target_file = 0; //target output file for slice or testgen;
	const char* cover_module = 0;
	const char* vcd_file = 0;
	
	int opt;
	
	char *tmp;
	unsigned i;
	
	while ((opt = getopt(argc, argv, "A:M:T:F:P:S:V:h:s:o:v:")) != EOF) switch (opt) {
	  case 'A':
		  all = true;
		  break;
	  case 'M':
		  cover_module = optarg;
		  break;
	  case 'T':
		  toggle = true;
		  break;
	  case 'F':
		  fsm = true;
		  break;
	  /*
	  case 'C':
		  combine = true;
		  break;
	  */

	  case 'v':
		  tmp = strdup(optarg);
		  tmp = new char[strlen(optarg)-2];
		  for(i = 1; i < strlen(optarg)-1; ++i)
			  tmp[i-1] = optarg[i];
		  tmp[i-1] = '\0';
		  fsm_vars = tmp;
		  break;
	  case 'h':
		  help_flag = true;
		  break;
	  case 'P':
		  path = true;
		  break;
	  case 'o':
		  target_file = optarg;
		  break;
	  case 's':
		  roots.push_back(optarg);
		  break;
	  case 'S':
		  statement = true;
		  break;
	  case 'V':
		  vcd_file = optarg;
		  break;
	  default:
		  flag_errors += 1;
		  break;
	}
	
	if(all || fsm)
	{
		if(!fsm_vars)
		{
			cerr << "When using the -All or -Fsm option, you must specify the state register variable with -v option" << endl;
			cerr << "Please use -Help option for usage" << endl;
			return 1;
		}
	}

	if (flag_errors || help_flag) {
		cout << "Simulation Coverage Analysis 1.0 " << endl <<
			"usage: covered <options> <file>\n"
			"options:\n"
			"\t-M<modname name>	Indicate which module to be analysized.\n"
			"\t-Toggle			Do Toggle coverage report for module.\n"
			//"\t-Combine		Do Combination coverage report for module.\n"
			"\t-Fsm			Do FSM coverage report for module, must with -v option.\n"
			"\t-Statement		Do Statement coverage report for module.\n"
			"\t-Path			Do Path coverage report for module.\n"
			"\t-All			Do All coverage report for module.\n"
			"\t-help			Print usage information, and exit.\n"
			"\t-v{v1,...vn}		Variables of FSM to be analysized\n"
			"\t-o<file>		Write coverage analysis results output to <file>.\n"
			"\t-s<module name>		Select the top-level module.\n"
			"\t-V<file>		Read VCD file information from <file>.\n"
			;
		return 1;
	}
	
	if (optind == argc) {
		cerr << "No input files. Please use -help option to display usage." << endl;
		return 1;
	}
	
    int rc = pform_parse(argv[optind]);
	
	if(rc)
		return rc;
	
	map<string,Module*>::iterator mod;
	for (mod = pform_modules.begin();
	mod != pform_modules.end();
	mod ++ )
	{
		(*mod).second->traversal_proc();
	}
	
	PDesign design;
	design.set_modules(pform_modules);
	design.set_udps(pform_primitives);
	
	if (roots.empty()) 
	{
		map<string,bool> mentioned_p;
		if (verbose_flag)
			cout << "LOCATING TOP-LEVEL MODULES" << endl << "  ";
		for (mod = pform_modules.begin(); mod != pform_modules.end(); mod++) 
		{
			list<PGate*> gates = (*mod).second->get_gates();
			list<PGate*>::const_iterator gate;
			for (gate = gates.begin(); gate != gates.end(); gate++) 
			{
				PGModule *mod = dynamic_cast<PGModule*>(*gate);
				if (mod) 
				{
					// Note that this module has been instantiated
					mentioned_p[mod->get_type()] = true;
				}
			}
		}
		
		for (mod = pform_modules.begin(); mod != pform_modules.end(); mod++)
		{
			if (mentioned_p[(*mod).second->mod_name()] == false) 
			{
				roots.push_back((*mod).second->mod_name());
			}
		}
	}
	
	if (roots.empty()) 
	{
		cerr << "No top level modules, and no -s option." << endl;
		return 1;
	}
		  
	Design*des = elaborate(roots);
	
	if ((des == 0) || (des->errors > 0)) {
		if (des != 0) {
			cerr << des->errors
				<< " error(s) during elaboration." << endl;
		} else {
			cerr << "Elaboration failed" << endl;
		}
		
		if (missing_modules.size() > 0) {
			cerr << "*** These modules were missing:" << endl;
			
			map<string,unsigned>::const_iterator idx;
			for (idx = missing_modules.begin()
				; idx != missing_modules.end()
				; idx ++)
				cerr << "        " << (*idx).first
				<< " referenced " << (*idx).second
				<< " times."<< endl;
			
			cerr << "***" << endl;
		}
		return des->errors;
	}
		  
	//do coverage report
	if(cover_module && vcd_file)
	{
		design.build_nodes();
		if(!target_file)
			target_file = "coverage.rep";

		ofstream report(target_file);

		if(all)
		{
			design.togglecover( cover_module, vcd_file, report);
			//design.combiniecov(des, cover_module, vcd_file, report);
			design.fsmcov(cover_module, vcd_file, fsm_vars, report);
			design.pathcover(des, cover_module, vcd_file, report, 1);
			design.pathcover(des, cover_module, vcd_file, report, 0);
		}

		if(toggle)
			design.togglecover(cover_module, vcd_file, report);
		
		//if(combine)
			//design.combiniecov(des, cover_module, vcd_file, target_file);

		if(fsm)
			design.fsmcov(cover_module, vcd_file, fsm_vars, report);
		
		if(path)
			design.pathcover(des, cover_module, vcd_file, report, 0);
		
		if(statement)
			design.pathcover(des, cover_module, vcd_file, report, 1);
	}
	
	exit(0);
}

