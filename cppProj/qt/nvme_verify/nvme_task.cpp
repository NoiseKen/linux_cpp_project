#include "nvme_task.h"
#include "debug.h"

unsigned int
NVMeTask::increaseVal = 0x01010101;
bool 
NVMeTask::wrcSkipCompare=false;

void
NVMeTask::run(void)
{
	unsigned short nlba;
	while(currentLBA<cfg.elba)
	{				
		if((currentLBA+cfg.nblocksPerStep+1)>cfg.elba)
		{
			nlba = cfg.elba - currentLBA - 1;
		}
		else
		{
			nlba = cfg.nblocksPerStep;
		}
		
		repCh << QString().sprintf("Strat process LBA 0x%016lX, nBlocks = %d\n", currentLBA, nlba);
		
		setTerminationEnabled(false);
		if(0==exec(nlba))
		{
			currentLBA+=nlba+1;
		}
		else
		{
			stop_task();
		}
		setTerminationEnabled(true);
						
		if((currentLBA == cfg.elba)&&(testFail==false))
		{
			switch(cfg.tt)
			{
				case march:
					if(marchFilled==false)
					{
						marchFilled=true;
						currentLBA = cfg.slba;
					}
					break;
				case sweepNLB:
					sweep_nlb_next();
					break;
				default:
					break;
			}
		}
	}	
}

void NVMeTask::pattern_fill(Memory<testUnit> *mem, unsigned int size)
{
	if(wrcSkipCompare)
	return;
	
	switch(cfg.dt)
	{
		case patternFix:
			mem->fill(cfg.basePattern, size);
			//cfg.basePattern = mem->memory[size/sizeof(testUnit)-1];
			break;
		case patternIncrease:
			mem->increase(cfg.basePattern, increaseVal, size);
			cfg.basePattern = mem->memory[size/sizeof(testUnit)-1]+increaseVal;
			break;
		case patternDecrease:
			mem->decrease(cfg.basePattern, increaseVal, size);
			cfg.basePattern = mem->memory[size/sizeof(testUnit)-1]-increaseVal;
			break;
		case patternToggle:					
			mem->toggle(cfg.basePattern, size);
			cfg.basePattern = mem->memory[size/sizeof(testUnit)-1]^(testUnit)-1;
			break;
	}	
}


int 
NVMeTask::nvme_io_split(unsigned int op, unsigned short nblocks, testUnit *prp)
{
	int ret = -1;
	uint64_t lba=currentLBA;
	if(cfg.nthreads==0)
	{
		HtmlString *html;
		html = new HtmlString();
		ret = dev->submis_wrc(op, lba, nblocks, prp, *html);
		delete html;
	}
	else
	{//multi threads
		QString localLog;
		localLog.clear();
		//localLog ="\n";
		int nthreads;	
		//get actual needed thread number
		nthreads = ((cfg.nthreads>nblocks)?nblocks:cfg.nthreads) + 1;
		//unsigned char blks[nthreads];
		unsigned short *blks;
		blks = new unsigned short[nthreads];
		//assign blocks for each thread
		int rep = nblocks+1;
		int i=0;
		memset(blks, 0, nthreads*sizeof(unsigned short));	
		#if 0
		while(rep--)
		{
			blks[i]++;
			i=(i+1)%(nthreads);
		}
		#else
		int div=rep/nthreads;
		blks[0]=div + rep%nthreads;
		for(i=1;i<nthreads;i++)
		{
			blks[i]=div;
		}
		#endif

		//create thread group & collect to a list
		
		QList<NVMeIOTask *> *list;
		NVMeIOTask *io;
		unsigned int offset=0;
		list = new QList<NVMeIOTask *>();
		for(i=0;i<nthreads;i++)
		{
			io = new NVMeIOTask(this);
			io->op = op;
			io->slba = lba;
			io->nblocks = blks[i]-1;	//0 base
			io->memory = &prp[offset];
			io->tag = i;
			//io->ioStatus = 8888;
			*list << io;
			//next thread update
			lba+=blks[i];
			offset += blks[i]*dev->lbaSize/sizeof(testUnit);			
		}
		
		delete blks;
		
		//start all thread
		for(i=0;i<nthreads;i++)
		{
			(*list)[i]->start();
		}

		//wait all thread finish
#if 1
		bool bFinished;
		do{
			bFinished=true;	
			for(i=0;i<nthreads;i++)
			{
				bFinished &= (*list)[i]->isFinished();
			}
		}while(!bFinished);
#else
		do{			
			for(i=0;i<nthreads;i++)
			{
				if(false==(*list)[i]->isFinished())
				continue;
			}
		}while(0);
#endif
		
		ret = 0;
		//free all thread and collect result
		for(i=0;i<list->size();i++)
		{			
			io = (*list)[i];
			ret |= io->ioStatus;			
			localLog += io->log;
			
			delete io;
		}	
		list->clear();
		delete list;
				
		//localLog +="\n";
		
		repCh << localLog;		
	}	
			
	return ret;
}

int 
NVMeTask::exec(unsigned short nblocks)
{		
	//return (cfg.nthreads==0)?singal_thread(nblocks):multi_thread(nblocks);
	testUnit *write;
	testUnit *read;
	unsigned int size;
	QString localLog;
	int ret=-1;
	//localLog.clear();
	size = dev->lbaSize*(nblocks+1);
	write = new testUnit[size/sizeof(testUnit)];
	read = new testUnit[size/sizeof(testUnit)];
	//read buffer zero init		
	Memory<testUnit> *mem;	
	mem = new Memory<testUnit>(read);
	if(!wrcSkipCompare)
	{
		mem->fill(0xdeadc0de, size/sizeof(testUnit));
	}
	delete mem;
	
	mem = new Memory<testUnit>(write);	
	switch(cfg.tt)
	{
#if 1		
		case w1r1c1:
		case sweepNLB:
			localLog = QString().sprintf("fill pattern (0x%08X)... ", cfg.basePattern);
			pattern_fill(mem, size);			
			localLog += "write ... ";
			ret = nvme_io_split(nvme_cmd_write, nblocks, write);
			if(ret==0)
			{
				localLog += "read ... ";
				ret = nvme_io_split(nvme_cmd_read, nblocks, read);
				if(ret==0)
				{
					//write[10]=0xAAAAAAAA;
					localLog += "compare ... ";
					if(!wrcSkipCompare)
					{
						ret = mem->compare(read, size, cmpLog);
					}
					else
					{
						localLog += "skip ... ";
					}
				}
			}

			if(ret==0)
			{
				localLog += "pass!!\n\n";
			}
			else
			{
				localLog += "fail\n\n";
				localLog += cmpLog;
				//stop_task();
				
				mem->save_to("write.wrc", size);
				delete mem;
				mem = new Memory<testUnit>(read);
				mem->save_to("read.wrc", size);				
			}	
			
			break;
		case march:
			if(marchFilled)
			{
				mem->fill(cfg.fillPattern, size);
				localLog = "Read init ... ";
				ret = nvme_io_split(nvme_cmd_read, nblocks, read);
				if(ret==0)
				{					
					localLog += "cmpare init ... ";
					ret = mem->compare(read, size, cmpLog);
					if(ret==0)
					{
						localLog += QString().sprintf("Write pattern (0x%08X)... ", cfg.basePattern);
						pattern_fill(mem, size);						
						ret = nvme_io_split(nvme_cmd_write, nblocks, write);
						if(ret==0)
						{
							localLog += "read ... ";
							ret = nvme_io_split(nvme_cmd_read, nblocks, read);
							if(ret==0)
							{
								localLog += "compare ... ";
								//ret = mem->compare(read, size, cmpLog);	
								if(!wrcSkipCompare)
								{
									ret = mem->compare(read, size, cmpLog);
								}
								else
								{
									localLog += "skip ... ";
								}
							}
						}
					}					
				}
				
				if(ret==0)
				{
					localLog += "pass!!\n\n";
				}
				else
				{
					localLog += "fail\n\n";
					localLog += cmpLog;
					//stop_task();
				
					mem->save_to("write.wrc", size);
					delete mem;
					mem = new Memory<testUnit>(read);
					mem->save_to("read.wrc", size);				
				}				
			}
			else
			{				
				localLog = QString().sprintf("fill init pattern (0x%08X) @ LBA 0x%016lX ... ", cfg.fillPattern, currentLBA);
				mem->fill(cfg.fillPattern, size);
				
				ret = nvme_io_split(nvme_cmd_write, nblocks, write);
				if(ret==0)
				{
					localLog += "pass!!\n\n";
				}
				else
				{
					localLog += "fail\n\n";
					//marchFilled = true;					
					//stop_task();
				}
			}
			break;					
#endif
		default:
			ret = 0;
			nblocks^=1;
			//msleep(50);
			break;
	}

	repCh << localLog;
	
	delete mem;
	delete write;
	delete read;
	return ret;		
}

void
NVMeTask::stop_task(void)
{	
	currentLBA = cfg.elba;
	testFail = true;	
}

void
NVMeTask::sweep_nlb_next(void)
{
	if(++cfg.nblocksPerStep<cfg.fillPattern)
	{
		currentLBA = 0;
		cfg.elba = cfg.nblocksPerStep+1;
	}
}

bool
NVMeTask::_f_get_fail(void)
{
	return testFail;
}

QString 
NVMeTask::_f_get_report(void)
{
	QString rep;
	repCh >> rep;
	return rep;
}

unsigned int 
NVMeTask::_f_get_progress(void)
{
	unsigned int total;
	uint64_t processed;	
	
	if(testFail==true)
	{
		return 100;
	}	
	
	total = cfg.elba-cfg.slba+1;
	processed = currentLBA-cfg.slba+1;
	
	switch(cfg.tt)
	{
		case march:
			//processed = ((marchFilled)?50:0)+(processed>>1);
			
			if(marchFilled==false)
			{
				processed = 0;
			}
			break;
		case sweepNLB:
			total = cfg.fillPattern;
			//processed = cfg.nblocksPerStep+1;
			//plus 1 alreay in sweep_nlb_next
			processed = cfg.nblocksPerStep;
			break;
		default:
			break;
	}
	
	return (processed*100)/total;
	//return (currentStep*100)/nSteps;
}

NVMeTask::NVMeTask(NVMeDevice *target, Config &para) : progress(this, &NVMeTask::_f_get_progress)
					, report(this, &NVMeTask::_f_get_report)
					, fail(this, &NVMeTask::_f_get_fail)
{
	uint64_t maxlba;	
	memcpy(&cfg, &para, sizeof(Config));
	//number bloks per step is 0 base;
	//nSteps = (cfg.elba - cfg.slba + 1 + cfg.nblocksPerStep)/(cfg.nblocksPerStep+1);	
	
	dev = target;
	testFail = false;
	maxlba = dev->maxLBA;
	
	if(cfg.tt==sweepNLB)
	{//slba = start NLB, elba = end NLB
		cfg.elba &= 0xFFFF;
		//last test nlbks store to cfg.fillPattern		
		cfg.fillPattern = ((maxlba>cfg.elba)?cfg.elba:maxlba)+1;		
		cfg.nthreads = 0;
		cfg.nblocksPerStep = cfg.slba-1;	//start from slba; sweep_nlb_next will increase 1
		cfg.slba = 0;
		sweep_nlb_next();
	}
	else
	{	
		cfg.elba++;	//end lba need be test
		currentLBA = cfg.slba;		
		marchFilled = false;
		if(cfg.elba<cfg.slba)
		{
			repCh << QString().sprintf("Invalid parameter, Start LBA(0x%016lX) greater than End LBA(0x%016lX)\n", cfg.slba, cfg.elba);
			cfg.elba=cfg.slba;
			//cfg.slba = cfg.elba;
			stop_task();
		}
			
		if(cfg.elba>(maxlba+1))
		{		
			repCh << QString().sprintf("Invalid parameter, End LBA(0x%016lX) greater than Device max LBA(0x%016lX)\n", cfg.elba-1, maxlba);
			//cfg.slba = cfg.elba;
			stop_task();
		}	
		//repCh = new ReportChannel();
	}
	
}

NVMeTask::~NVMeTask(void)
{
	//delete repCh;
}
//--------------------------------------------------NVMeIOTask----------------------
void NVMeTask::NVMeIOTask::run(void)
{
	if((op==nvme_cmd_write)||(op==nvme_cmd_read))
	{
		HtmlString *html;
		html = new HtmlString();		
		
		ioStatus = owner->dev->submis_wrc(op, slba, nblocks, memory, *html);			
		
		if(ioStatus!=0)
		{
			log.sprintf("T[%d], slba = 0x%016lX, nlb = %d, ioStatus = %d\n", tag, slba, nblocks, ioStatus);
		}
		
		delete html;
	}
	else
	{
		ioStatus = -1;
	}
	//setTerminationEnabled(true);
}

NVMeTask::NVMeIOTask::NVMeIOTask(NVMeTask *own)
{
	owner = own;
	log.clear();
}

NVMeTask::NVMeIOTask::~NVMeIOTask(void)
{	
}
