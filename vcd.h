#ifndef __VCD_H
#define __VCD_H

#include "HName.h"
#include <map>
#include <string>
#include <stdio.h>

typedef struct symbol_entry
{
	hname_t name;
	int lsb, msb;

}

extern map<string,hname_t> vcd_sym_table;

void vcd_parse( char* vcd_file );
void vcd_parse_def( FILE* vcd );
void vcd_parse_def_ignore( FILE* vcd );
void vcd_parse_def_var( FILE* vcd );
void vcd_parse_def_scope( FILE* vcd );


#endif