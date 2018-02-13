#include "extern_shell.h"
#include "log_file.h"
#include "legacy_string.h"
#include "auto_reg_cfg.h"
std::string ExternShell::appRoot;
std::string const ExternShell::runningFile="shpid";
std::string const ExternShell::resultFile="shrst";

void
ExternShell::kill(void)
{
	std::string cmd;
	LogFile *log;
	log = new LogFile(appRoot+"/"+runningFile);
	cmd = "kill -9 " + log->line[log->lineCount-1];	
	delete log;
	system(cmd.c_str());
	cmd = "rm -f " + appRoot+ "/" + runningFile;
	system(cmd.c_str());
	
	log = new LogFile(appRoot+"/"+resultFile);
	*log << "-77777";
	delete log;
	
	//printf("killed : set shrst contain -77777\n");
	//getchar();
}

class ExternShell &
ExternShell::operator <<(std::string in)
{
	parameter += " " + in;
	return *this;
}

int 
ExternShell::exec(void)
{
	int sc;
	LegacyString *ls;
	ls = new LegacyString();
	ls->sprintf("gnome-terminal -e \"%s %s\"", absShell.c_str(), parameter.c_str());	
	//ls->sprintf("%s %s", absShell.c_str(), parameter.c_str());	
	printf("ExternShell : %s\n", ls->str().c_str());
	sc = system(ls->str().c_str());
	delete ls;
	
	while(!is_running());
	
	return sc;
}

std::string 
ExternShell::result_str(void)
{
	std::string lastLine;
	LogFile *log;
	log = new LogFile(appRoot+"/"+resultFile);
	lastLine = log->line[log->lineCount-1];
	delete log;
	return lastLine;	
}

int 
ExternShell::result_num(void)
{
	return atoi(result_str().c_str());
}

bool 
ExternShell::is_running(void)
{
	bool running;
	FileCtrl *fc;
	fc = new FileCtrl(appRoot + "/" + runningFile);	
	running = fc->is_exist();
	delete fc;
	return running;
}

std::string 
ExternShell::morph_result_file(std::string extend)
{
	std::string from, cli;
	from = appRoot+"/"+resultFile;
	//to = from+extend;
	cli = "cp "+from+" "+from+extend;
	system(cli.c_str());
	return std::string(resultFile+extend);
}

ExternShell::ExternShell(std::string shell, std::string stat)
	: absShell(appRoot+"/"+AutoRegCfg().value[CFG_SHELL_FOLDER]+"/"+shell)
	, currentStatus(stat)
{
}

ExternShell::~ExternShell(void)
{
}
