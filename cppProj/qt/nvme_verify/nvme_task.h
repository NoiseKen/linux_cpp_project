#ifndef __NVME_TASK_H__
#define __NVME_TASK_H__
#include <QThread>
#include "property.h"
#include "html_string.h"
#include "nvme_device.h"
#include "memory.h"
#include "report_channel.h"
typedef unsigned int testUnit;

class NVMeTask : public QThread
{
public:
	static bool wrcSkipCompare;
	static unsigned int increaseVal;
	typedef enum{
		patternFix=0,
		patternIncrease,
		patternDecrease,
		patternToggle,
	}PatternType;
	
	typedef enum{
		w1r1c1=0,	//wrc 
		march,
		sweepNLB,
	}TestType;
	
	typedef struct{
		uint64_t slba;						//start LBA
		uint64_t elba;						//end LBA
		uint32_t basePattern;				//base data pattern
		uint32_t fillPattern;				//march test used fill pattern
		uint32_t nblocksPerStep;			//number of blocks per step (0 base)
		uint16_t nthreads;					//number of threads,
		PatternType dt;		//data pattern type
		TestType tt;
	}Config;		
		
	void run(void);
	void stop_task(void);
	property_ro<NVMeTask, unsigned int> progress;
	property_ro<NVMeTask, QString> report;
	property_ro<NVMeTask, bool> fail;
	NVMeTask(NVMeDevice *target, Config &para);
	~NVMeTask(void);	
protected:
	virtual int exec(unsigned short nblocks);
private:
	int nvme_io_split(unsigned int op, unsigned short nblocks, testUnit *prp);	
	bool testFail;
	bool marchFilled;
	//QString log;	
	ReportChannel<QString> repCh;
	QString cmpLog;
	//HtmlString *html;
	void pattern_fill(Memory<testUnit> *mem, unsigned int size);
	//Memory<unsigned int> *menWrite;
	//Memory<unsigned int> *menRead;	
	Config cfg;
	NVMeDevice *dev;
	void sweep_nlb_next(void);	
	bool _f_get_fail(void);
	unsigned int _f_get_progress(void);
	QString _f_get_report(void);
	//unsigned int nSteps;
	uint64_t currentLBA;

	class NVMeIOTask : public QThread
	{
	public:
		unsigned int op;
		unsigned int tag;
		uint64_t slba;
		uint16_t nblocks;
		testUnit *memory;
		void run(void);
		int ioStatus;		
		QString log;		
		NVMeIOTask(class NVMeTask *own);
		~NVMeIOTask(void);
	protected:
	private:
		class NVMeTask *owner;
	};
	
};


#endif //ndef __NVME_TASK_H__
