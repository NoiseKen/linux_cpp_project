#include "tnvme_script.h"
#include "auto_reg_cfg.h"
#include "log_file.h"
#include "legacy_string.h"
#include "string_list.h"
#include "extension_string.h"

std::string const TNVMeScript::summaryFile="summary.log";
std::string const TNVMeScript::grpXYZCfgFile="cfg/tnvme_sh.cfg";

void 
TNVMeScript::summary_log_prepare(bool start)
{
	LogFile *log;
	log = new LogFile(summaryFolder+"/"+summaryFile);
	std::string dateTime;
	dateTime = log->get_date_time();		
	if(start)
	{
		log->destory();
		log->create();
		dateTime = "New Log Create @ " + dateTime;
	}
	else
	{
		dateTime = "End Log @ " + dateTime;
	}
	
	*log << dateTime;
	delete log;	
}

std::string 
TNVMeScript::group_sub_test(int grp, int n)
{
	LogFile *lf;
	std::string test="";
	int start;
	int cnt=group_test_count(grp, &start);
	lf = new LogFile(ExternShell::appRoot+"/"+grpXYZCfgFile);
	if(n<cnt)
	{
		test = lf->line[start+n];
	}	
	
	delete lf;
	return test;	
}

int 
TNVMeScript::group_test_count(int grp, int *start)
{
	LegacyString *ls;
	LogFile *lf;
	lf = new LogFile(ExternShell::appRoot+"/"+grpXYZCfgFile);
	ls = new LegacyString();	
	ls->sprintf("[Grp%d]", grp);
	int cnt=lf->lineCount;
	int n=0;
	if(cnt>0)
	{
		//search Grp start line
		std::string str;
		int i=0;
		do{
			str = lf->line[i++];
		}while((str!=ls->str())&&(i<cnt));
		//search item below Grp
		
		if(NULL!=start)
		{
			*start=i;
		}
		
		if(i<cnt)
		{			
			do{
				str = lf->line[i++];
				if(str!="")
				{
					n++;
				}
			}while((str!=""));
		}				
	}
	delete ls;
	delete lf;
	return n;	
}

int 
TNVMeScript::group_count(void)
{
	LogFile *lf;
	lf = new LogFile(ExternShell::appRoot+"/"+grpXYZCfgFile);
	int cnt=lf->lineCount;	
	int grpCnt=0;
	for(int i=0;i<cnt;i++)
	{
		std::string str;
		str = lf->line[i];
		if(str.find("[Grp")!=std::string::npos)
		{
			grpCnt++;
		}
	}
	delete lf;
	return grpCnt;	
}

void 
TNVMeScript::shell_monitor(int sc)
{
	if(sc==SHELL_FINISH)
	{
		summary_log_prepare(false);
	}
}

void 
TNVMeScript::final_summary(void)
{
	StringList *sl;
	ExtensionString *es;
	sl = new StringList();
	es = new ExtensionString("=");
	sl->load_from(summaryFolder+"/"+summaryFile);
	std::string key, val;
	int lineCnt = sl->size();
	int passCnt, failCnt, skipCnt, totalCnt;
	passCnt=failCnt=skipCnt=totalCnt=0;
	for(int i=0;i<lineCnt;i++)
	{		
		es->string = (*sl)[i];
		key = (*es)[0]; val=(*es)[1]; 
		if(key=="pass  item")
		{
			passCnt += atoi(val.c_str());
		}
		else
		if(key=="fail  item")
		{
			val=std::string(val.c_str(), 2);
			failCnt += atoi(val.c_str());
		}
		else
		if(key=="skip  item")
		{
			skipCnt += atoi(val.c_str());			
		}
		else
		if(key=="total item")
		{
			totalCnt += atoi(val.c_str());			
		}				
	}
	
	*sl << "\n----------- REGRESSION TOTAL SUMMARY -----------";
	
	key = "total test = " + std::to_string(totalCnt);
	*sl << key;

	key = "pass test  = " + std::to_string(passCnt);
	*sl << key;
	
	key = "fail test  = " + std::to_string(failCnt);
	*sl << key;
	
	key = "skip test  = " + std::to_string(skipCnt);
	*sl << key;
	
	sl->save_to(summaryFolder+"/"+summaryFile);
	
	delete sl;
	delete es;
}

TNVMeShell *
TNVMeScript::_new_shell(std::string test)
{
	AutoRegCfg *cfg;
	cfg = new AutoRegCfg();
	
	TNVMeShell *shell;
	shell = new TNVMeShell(  cfg->value[CFG_TNVME_ROOT]
							,test
							,ExternShell::appRoot+"/"+cfg->value[CFG_AUTO_LOG_FOLDER]
							,summaryFile
							);
	delete cfg;
	return shell;
}

TNVMeScript::TNVMeScript(std::string logFolder, std::string grpXYZ) 
	: ShellScript(ShellScript::SCRIPT_INFINITE, _new_shell(grpXYZ))
	, summaryFolder(logFolder)
{

}

TNVMeScript::~TNVMeScript(void)
{
}
