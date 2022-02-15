# Microsoft Developer Studio Project File - Name="covered" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=covered - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "covered.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "covered.mak" CFG="covered - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "covered - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "covered - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "covered - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "covered - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /w /W0 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "covered - Win32 Release"
# Name "covered - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\async.cpp
# End Source File
# Begin Source File

SOURCE=.\Attrib.cpp
# End Source File
# Begin Source File

SOURCE=.\build_node.cpp
# End Source File
# Begin Source File

SOURCE=.\cfg.cpp
# End Source File
# Begin Source File

SOURCE=.\CfgNode.cpp
# End Source File
# Begin Source File

SOURCE=.\DDModel.cpp
# End Source File
# Begin Source File

SOURCE=.\design_dump.cpp
# End Source File
# Begin Source File

SOURCE=.\dup_expr.cpp
# End Source File
# Begin Source File

SOURCE=.\elab_anet.cpp
# End Source File
# Begin Source File

SOURCE=.\elab_expr.cpp
# End Source File
# Begin Source File

SOURCE=.\elab_lval.cpp
# End Source File
# Begin Source File

SOURCE=.\elab_net.cpp
# End Source File
# Begin Source File

SOURCE=.\elab_pexpr.cpp
# End Source File
# Begin Source File

SOURCE=.\elab_scope.cpp
# End Source File
# Begin Source File

SOURCE=.\elab_sig.cpp
# End Source File
# Begin Source File

SOURCE=.\elaborate.cpp
# End Source File
# Begin Source File

SOURCE=.\eval.cpp
# End Source File
# Begin Source File

SOURCE=.\eval_attrib.cpp
# End Source File
# Begin Source File

SOURCE=.\eval_tree.cpp
# End Source File
# Begin Source File

SOURCE=.\expr_dump_clp.cpp
# End Source File
# Begin Source File

SOURCE=.\expr_evaluate.cpp
# End Source File
# Begin Source File

SOURCE=.\functor.cpp
# End Source File
# Begin Source File

SOURCE=.\HName.cpp
# End Source File
# Begin Source File

SOURCE=.\lexor.cpp
# End Source File
# Begin Source File

SOURCE=.\lexor_keyword.cpp
# End Source File
# Begin Source File

SOURCE=.\LineInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\load_module.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Make_Route.cpp
# End Source File
# Begin Source File

SOURCE=.\Module.cpp
# End Source File
# Begin Source File

SOURCE=.\net_assign.cpp
# End Source File
# Begin Source File

SOURCE=.\net_design.cpp
# End Source File
# Begin Source File

SOURCE=.\net_event.cpp
# End Source File
# Begin Source File

SOURCE=.\net_expr.cpp
# End Source File
# Begin Source File

SOURCE=.\net_force.cpp
# End Source File
# Begin Source File

SOURCE=.\net_func.cpp
# End Source File
# Begin Source File

SOURCE=.\net_link.cpp
# End Source File
# Begin Source File

SOURCE=.\net_modulo.cpp
# End Source File
# Begin Source File

SOURCE=.\net_nex_input.cpp
# End Source File
# Begin Source File

SOURCE=.\net_nex_output.cpp
# End Source File
# Begin Source File

SOURCE=.\net_proc.cpp
# End Source File
# Begin Source File

SOURCE=.\net_scope.cpp
# End Source File
# Begin Source File

SOURCE=.\net_udp.cpp
# End Source File
# Begin Source File

SOURCE=.\net_variable.cpp
# End Source File
# Begin Source File

SOURCE=.\netlist.cpp
# End Source File
# Begin Source File

SOURCE=.\netmisc.cpp
# End Source File
# Begin Source File

SOURCE=.\nodangle.cpp
# End Source File
# Begin Source File

SOURCE=.\pad_to_width.cpp
# End Source File
# Begin Source File

SOURCE=.\parse.cpp
# End Source File
# Begin Source File

SOURCE=.\parse_misc.cpp
# End Source File
# Begin Source File

SOURCE=.\PData.cpp
# End Source File
# Begin Source File

SOURCE=.\PDelays.cpp
# End Source File
# Begin Source File

SOURCE=.\PDesign.cpp
# End Source File
# Begin Source File

SOURCE=.\PEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\PExpr.cpp
# End Source File
# Begin Source File

SOURCE=.\pform.cpp
# End Source File
# Begin Source File

SOURCE=.\pform_dump.cpp
# End Source File
# Begin Source File

SOURCE=.\PFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\PGate.cpp
# End Source File
# Begin Source File

SOURCE=.\PTask.cpp
# End Source File
# Begin Source File

SOURCE=.\pUdp.cpp
# End Source File
# Begin Source File

SOURCE=.\PWire.cpp
# End Source File
# Begin Source File

SOURCE=.\set_width.cpp
# End Source File
# Begin Source File

SOURCE=.\slice.cpp
# End Source File
# Begin Source File

SOURCE=.\slice_dump.cpp
# End Source File
# Begin Source File

SOURCE=.\stat_dump_clp.cpp
# End Source File
# Begin Source File

SOURCE=.\Statement.cpp
# End Source File
# Begin Source File

SOURCE=.\StringHeap.cpp
# End Source File
# Begin Source File

SOURCE=.\symbol_search.cpp
# End Source File
# Begin Source File

SOURCE=.\sync.cpp
# End Source File
# Begin Source File

SOURCE=.\testpath.cpp
# End Source File
# Begin Source File

SOURCE=.\vcdvar.cpp
# End Source File
# Begin Source File

SOURCE=.\verinum.cpp
# End Source File
# Begin Source File

SOURCE=.\verireal.cpp
# End Source File
# Begin Source File

SOURCE=.\XGetopt.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Attrib.h
# End Source File
# Begin Source File

SOURCE=.\cfg.h
# End Source File
# Begin Source File

SOURCE=.\CfgNode.h
# End Source File
# Begin Source File

SOURCE=.\compiler.h
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\DDModel.h
# End Source File
# Begin Source File

SOURCE=.\functor.h
# End Source File
# Begin Source File

SOURCE=.\HName.h
# End Source File
# Begin Source File

SOURCE=.\lexor_keyword.h
# End Source File
# Begin Source File

SOURCE=.\LineInfo.h
# End Source File
# Begin Source File

SOURCE=.\Make_Route.h
# End Source File
# Begin Source File

SOURCE=.\Module.h
# End Source File
# Begin Source File

SOURCE=.\named.h
# End Source File
# Begin Source File

SOURCE=.\netlist.h
# End Source File
# Begin Source File

SOURCE=.\netmisc.h
# End Source File
# Begin Source File

SOURCE=.\parse.h
# End Source File
# Begin Source File

SOURCE=.\parse_api.h
# End Source File
# Begin Source File

SOURCE=.\parse_misc.h
# End Source File
# Begin Source File

SOURCE=.\PData.h
# End Source File
# Begin Source File

SOURCE=.\PDelays.h
# End Source File
# Begin Source File

SOURCE=.\PDesign.h
# End Source File
# Begin Source File

SOURCE=.\PEvent.h
# End Source File
# Begin Source File

SOURCE=.\PExpr.h
# End Source File
# Begin Source File

SOURCE=.\pform.h
# End Source File
# Begin Source File

SOURCE=.\PGate.h
# End Source File
# Begin Source File

SOURCE=.\PTask.h
# End Source File
# Begin Source File

SOURCE=.\PUdp.h
# End Source File
# Begin Source File

SOURCE=.\PWire.h
# End Source File
# Begin Source File

SOURCE=.\slice.h
# End Source File
# Begin Source File

SOURCE=.\Statement.h
# End Source File
# Begin Source File

SOURCE=.\StringHeap.h
# End Source File
# Begin Source File

SOURCE=.\svector.h
# End Source File
# Begin Source File

SOURCE=.\testpath.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# Begin Source File

SOURCE=.\vcdvar.h
# End Source File
# Begin Source File

SOURCE=.\verinum.h
# End Source File
# Begin Source File

SOURCE=.\verireal.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Lex Files"

# PROP Default_Filter "lex"
# Begin Source File

SOURCE=.\lexor.lex

!IF  "$(CFG)" == "covered - Win32 Release"

!ELSEIF  "$(CFG)" == "covered - Win32 Debug"

# Begin Custom Build - Compiling lex file...
InputPath=.\lexor.lex

"lexor.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -PVL -s -olexor.cpp lexor.lex

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "YACC Files"

# PROP Default_Filter "y"
# Begin Source File

SOURCE=.\parse.y

!IF  "$(CFG)" == "covered - Win32 Release"

!ELSEIF  "$(CFG)" == "covered - Win32 Debug"

# Begin Custom Build - Compiling YACC file...
InputPath=.\parse.y

BuildCmds= \
	del parse.h \
	bison --verbose -t -p VL -d -o parse.cpp parse.y \
	rename parse.hpp parse.h \
	

"parse.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"parse.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
