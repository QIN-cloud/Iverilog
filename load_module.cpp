#include "config.h"
#include "util.h"
#include "parse_api.h"
#include "compiler.h"
#include <iostream>
#include <map>
#include <string>
#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>


/*
 * The module library items are maps of key names to file name within
 * the directory.
 */
struct module_library {
      char*dir;
      bool key_case_sensitive;
      map<string,const char*>name_map;
      struct module_library*next;
};

static struct module_library*library_list = 0;
static struct module_library*library_last = 0;

const char dir_character = '/';
extern FILE *depend_file;

/*
 * Use the type name as a key, and search the module library for a
 * file name that has that key.
 */
bool load_module(const char*type)
{
      char path[4096];
      char*ltype = strdup(type);

      for (char*tmp = ltype ; *tmp ;  tmp += 1)
	    *tmp = tolower(*tmp);

      for (struct module_library*lcur = library_list
		 ; lcur != 0 ;  lcur = lcur->next) {

	    const char*key = lcur->key_case_sensitive? type : ltype;
	    map<string,const char*>::const_iterator cur;
	    cur = lcur->name_map.find(key);
	    if (cur == lcur->name_map.end())
		  continue;

	    sprintf(path, "%s%c%s", lcur->dir, dir_character, (*cur).second);

	    if(depend_file) {
		  fprintf(depend_file, "%s\n", path);
	    }

	    if (ivlpp_string) {
		  char*cmdline = (char*)malloc(strlen(ivlpp_string) +
					       strlen(path) + 2);
		  strcpy(cmdline, ivlpp_string);
		  strcat(cmdline, " ");
		  strcat(cmdline, path);
		  FILE*file = _popen(cmdline, "r");

		  if (verbose_flag)
			cerr << "Executing: " << cmdline << endl;

		  pform_parse(path, file);
		  fclose(file);
		  free(cmdline);

	    } else {
		  if (verbose_flag)
			cerr << "Loading library file "
			     << path << "." << endl;

		  FILE*file = fopen(path, "r");
		  assert(file);
		  pform_parse(path, file);
		  fclose(file);
	    }

	    return true;
      }


      return false;
}
