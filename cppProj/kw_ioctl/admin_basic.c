#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "nvme_tool.h"
#include "terminal_ctrl.h"

#define REGISTERED_TASK_IDX	TASK_NVME_ADMIN
//
static /*const*/ TStr adminBasicMenu[]=
{
    ITEM_T("Configure admin queue", OP_ADMIN_CONFIG_QUEUE),
    ITEM_T_IS("Delete I/O submission queue", OP_ADMIN_DELETE_IO_SUBMISSION_QUEUE),
    ITEM_T_IS("Create I/O submission queue", OP_ADMIN_CREATE_IO_SUBMISSION_QUEUE),
    ITEM_T_IS("Get log page", OP_ADMIN_GET_LOG_PAGE),
    ITEM_T_IS("Delete I/O completion queue", OP_ADMIN_DELETE_IO_COMPLETION_QUEUE),
    ITEM_T_IS("Create I/O completion queue", OP_ADMIN_CREATE_IO_COMPLETION_QUEUE),
    ITEM_T_IS("Identify", OP_ADMIN_IDENTIFY),
    ITEM_T_IS("Abort", OP_ADMIN_ABORT),	
    ITEM_T_IS("Get feature", OP_ADMIN_GET_FEATURE),
    ITEM_T_IS("Set feature", OP_ADMIN_SET_FEATURE),	
    ITEM_T_IS("Asynchronous event request", OP_ADMIN_ASYNCHRONOUS_EVENT_REQUEST),
    ITEM_T_IS("Firmware active", OP_ADMIN_FIRMWARE_ACTIVATE),
    ITEM_T_IS("Firmware image download", OP_ADMIN_FIRMWARE_IMAGE_DOWNLOAD),	
    __EOM
};

enum{
	NVME_FEAT_ARBITRATION	= 0x01,
	NVME_FEAT_POWER_MGMT	= 0x02,
	NVME_FEAT_LBA_RANGE		= 0x03,			//O
	NVME_FEAT_TEMP_THRESH	= 0x04,
	NVME_FEAT_ERR_RECOVERY	= 0x05,
	NVME_FEAT_VOLATILE_WC	= 0x06,		//O
	NVME_FEAT_NUM_QUEUES	= 0x07,
	NVME_FEAT_IRQ_COALESCE	= 0x08,
	NVME_FEAT_IRQ_CONFIG	= 0x09,
	NVME_FEAT_WRITE_ATOMIC	= 0x0a,
	NVME_FEAT_ASYNC_EVENT	= 0x0b,	
	FEATURE_CNT = 11,
};

#define MAKE_INFO(s)	{s,TO_STR(s)}

struct feature_info{
	int id;
	const char *name;
}featureInfo[FEATURE_CNT]={
	MAKE_INFO(NVME_FEAT_ARBITRATION),
	MAKE_INFO(NVME_FEAT_POWER_MGMT),
	MAKE_INFO(NVME_FEAT_LBA_RANGE),
	MAKE_INFO(NVME_FEAT_TEMP_THRESH),
	MAKE_INFO(NVME_FEAT_ERR_RECOVERY),
	MAKE_INFO(NVME_FEAT_VOLATILE_WC),
	MAKE_INFO(NVME_FEAT_NUM_QUEUES),
	MAKE_INFO(NVME_FEAT_IRQ_COALESCE),
	MAKE_INFO(NVME_FEAT_IRQ_CONFIG),
	MAKE_INFO(NVME_FEAT_WRITE_ATOMIC),
	MAKE_INFO(NVME_FEAT_ASYNC_EVENT),
};	


enum{
	LOG_MSG_NSID=0,
	LOG_MSG_FID,
	//LOG_MSG_DWORD10,
	LOG_MSG_DWORD11,
	LOG_MSG_QID,
	LOG_MSG_CQID,
};

static const char *adminLogMsg[]={
	"nsid (namespace id) = ",
	"feature ID = ",
	//"DWORD10= 0x",
	"DWORD11= 0x",
	"Queue ID = ",
	"Correspond Completion Queue ID = ",	
};

static void
display_error_info(void *ptr)
{	
	struct err_log_page{
		uint64_t errCount;
		uint16_t sqId;
		uint16_t cmdId;
		uint16_t statusField;
		uint16_t errLocation;
		uint64_t lba;
		uint32_t ns;
		uint8_t	vendor;
	}__attribute__((packed)) *err;
	err = (struct err_log_page *)ptr;
	printf("error count = %ld\n", err->errCount);
	printf("submission queue id = %d\n", err->sqId);
	printf("command id = %d\n", err->cmdId);
	printf("status code = 0x%04X\n", err->statusField);
	printf("parameter error location = 0x%04X\n", err->statusField);
	printf("LBA = 0x%016lX\n", err->lba);
	printf("vendor specific info = 0x%02X\n", err->vendor);	
}

static void
display_smart_info(void *ptr)
{
	struct nvme_smart_log *smart;	
	smart = (struct nvme_smart_log *)ptr;
	printf("critical warning = 0x%02X\n", smart->critical_warning);
	printf("temperature = 0x%04X\n", smart->temperature);
	printf("avaliable spare = 0x%02X\n", smart->avail_spare);
	printf("avaliable spare threshold = 0x%02X\n", smart->spare_thresh);
	printf("percentage used = %d%%", smart->percent_used);
	printf("\ndata units read      = 0x");
	printf_128((char *)smart->data_units_read);
	printf("\ndata units written   = 0x");
	printf_128((char *)smart->data_units_written);
	printf("\nhost read commands   = 0x");
	printf_128((char *)smart->host_reads);
	printf("\nhost write commands  = 0x");
	printf_128((char *)smart->host_writes);	
	printf("\ncontroller busy time = 0x");
	printf_128((char *)smart->ctrl_busy_time);
	printf("\npower cycles         = 0x");
	printf_128((char *)smart->power_cycles);
	printf("\npower on hours       = 0x");
	printf_128((char *)smart->power_on_hours);
	printf("\nunsafe shoutdowns    = 0x");
	printf_128((char *)smart->unsafe_shutdowns);
	printf("\nmedia errors         = 0x");
	printf_128((char *)smart->media_errors);
	printf("\n# of err log entries = 0x");
	printf_128((char *)smart->num_err_log_entries);	
	
	printf("\n");	
}

static void
display_fw_slot_info(void *ptr)
{	
#if 0	//if frs is string type	
	struct fwslot_log_page{
		u8 afi;
		u8 reserved[7];
		u8 frs[7][8];		
	}__attribute__((packed)) *fwslot;
	int i;
	
	char rev[9];
	
	fwslot = (struct fwslot_log_page *)ptr;
	printf("active fw info = 0x%02X\n", fwslot->afi);
	rev[8]=__EOS;
	for(i=1;i<8;i++)
	{		
		memcpy(rev, &fwslot->frs[i][0], 8);
		printf("Slot %d fw rev = %s\n", i, rev);
	}
#else
	struct fwslot_log_page{
		u8 afi;
		u8 reserved[7];
		u64 frs[7];		
	}__attribute__((packed)) *fwslot;
	int i;
	
	fwslot = (struct fwslot_log_page *)ptr;
	printf("active fw info = 0x%02X\n", fwslot->afi);

	for(i=1;i<8;i++)
	{		
		printf("Slot %d fw rev = 0x%016lX\n", i, (unsigned long)fwslot->frs[i]);
	}
#endif	
}

static void
display_feature(void)
{	
	int i;
	char *msg;
	struct feature_info *info;
	msg = new char[64];
	info = featureInfo;
	for(i=0;i<FEATURE_CNT;i++)
	{
		sprintf(msg, " %s = %d\n", info->name, info->id);
		info++;
		menu_show_report(i, msg);
	}
	
	delete msg;
}

static void
display_lba_range_type(struct nvme_lba_range_type *ptr)
{
	//char buf[17];
	printf("Type = 0x%02X\n", ptr->type);
	printf("Attributes = 0x%02X\n", ptr->attributes);
	printf("Start LBA = 0x%016llX\n", ptr->slba);
	printf("Number of logic block = 0x%016llX\n", ptr->nlb);
	
	//memcpy(buf, ptr->guid, 16);
	//buf[16]=__EOS;
	//printf("GUID = %s\n", buf);	
	printf("GUID = 0x");
	printf_128((char *)ptr->guid);
	printf("\n");
}

void
display_status_code(uint16_t sc)
{
	TerminalCtrl term;
	printf("status code = 0x%04X ==> ", sc);
	
	if(sc & (1<<14))
	{
		printf( "DO_NOT_RETRY, ");
	}

	if(sc & (1<<13))
	{
		printf( "MORE, ");
	}	
	
	sc &= 0x7FF;
	
	switch(sc)
	{
		case NVME_SC_SUCCESS:
			term << COLOR_HI_GREEN;
			printf("%s\n", TO_STR(NVME_SC_SUCCESS));			
			break;
		case NVME_SC_INVALID_FIELD:
			term << COLOR_HI_RED;
			printf("%s\n", TO_STR(NVME_SC_INVALID_FIELD));
			printf("invalid parameter involved!!\n");
			break;			
		case NVME_SC_ABORT_REQ:
			printf("%s\n", TO_STR(NVME_SC_ABORT_REQ));
			break;
		case NVME_SC_INVALID_NS:
			term << COLOR_HI_RED;
			printf("%s\n", TO_STR(NVME_SC_INVALID_NS));
			printf("invalid namespace involved!!\n");
			break;
		case NVME_SC_QID_INVALID:
			term << COLOR_HI_RED;
			printf("%s\n", TO_STR(NVME_SC_QID_INVALID));
			printf("invalid queue id number involved!!\n");
			break;
		case NVME_SC_CQ_INVALID:
			term << COLOR_HI_RED;
			printf("%s\n", TO_STR(NVME_SC_CQ_INVALID));
			printf("invalid completion queue number involved!!\n");
			break;
		case NVME_SC_QUEUE_SIZE:
			term << COLOR_HI_RED;
			printf("%s\n", TO_STR(NVME_SC_QUEUE_SIZE));
			printf("invalid queue size involved!!\n");			
			break;
		case NVME_SC_INVALID_OPCODE:
			term << COLOR_HI_RED;
			printf("%s\n", TO_STR(NVME_SC_INVALID_OPCODE));
			printf("controller not support this command!!\n");
			break;
		case NVME_SC_INVALID_LOG_PAGE:
			term << COLOR_HI_RED;
			printf("%s\n", TO_STR(NVME_SC_INVALID_LOG_PAGE));
			printf("invalid log page id involved!!\n");
			break;
		case NVME_SC_INVALID_CQ_DELETE:
			term << COLOR_HI_RED;
			printf("%s\n", TO_STR(NVME_SC_INVALID_CQ_DELETE));
			printf("invalid completion queue delete!!\n");
			printf("this CQ still used by some SQ(s), please delete that SQ(s) first!!\n");
			break;			
		default:
			printf("please refer spec\n");
			break;			
	}
	
	term << COLOR_NORMAL;
}

static void
display_completion(struct nvme_completion *nvmeCQ)
{
	short sc;		
	printf("result=0x%08X, Head=%d, Qid=%d, CmdId=%d, status=0x%04X\n",
		nvmeCQ->result, nvmeCQ->sq_head, nvmeCQ->sq_id, nvmeCQ->command_id, nvmeCQ->status);
	sc = (nvmeCQ->status>>1);//&0x7FF;	
	
	display_status_code(sc);
}

/*
 * function	: admin_cmd_menu_swap
 * 			  menu table swap
 * parameter: [adminSwap]=swap mode define in menu_system.h
 * return	: none
 */
void
admin_cmd_menu_swap(int adminSwap)
{
	unsigned char attr;
	int i;
	
	if(adminSwap==ADMIN_MENU_SWAP_QUEUE_INIT)
	{//swap to admin queue init
		ChangeItemAttribute(adminBasicMenu, OP_ADMIN_CONFIG_QUEUE, ATTR_TASK);
		attr = ATTR_STRING;
	}
	else
	{//all without admin queue init
		ChangeItemAttribute(adminBasicMenu, OP_ADMIN_CONFIG_QUEUE, ATTR_STRING);
		attr = ATTR_TASK;
	}	

	for(i=OP_ADMIN_DELETE_IO_SUBMISSION_QUEUE;i<OP_ADMIN_MAX_CNT;i++)
	{
		ChangeItemAttribute(adminBasicMenu, i, attr);
	}

	if(adminSwap==ADMIN_MENU_STANDARD)
	{
		ChangeItemAttribute(adminBasicMenu, OP_ADMIN_DELETE_IO_SUBMISSION_QUEUE, ATTR_STRING);
		ChangeItemAttribute(adminBasicMenu, OP_ADMIN_CREATE_IO_SUBMISSION_QUEUE, ATTR_STRING);
		ChangeItemAttribute(adminBasicMenu, OP_ADMIN_DELETE_IO_COMPLETION_QUEUE, ATTR_STRING);
		ChangeItemAttribute(adminBasicMenu, OP_ADMIN_CREATE_IO_COMPLETION_QUEUE, ATTR_STRING);		
	}
	
	//exception
	ChangeItemAttribute(adminBasicMenu, OP_ADMIN_ASYNCHRONOUS_EVENT_REQUEST, ATTR_STRING);
}

static Progress
admin_get_log_page(MenuSystemExport *par)
{
	Progress flag;
	struct nvme_admin_cmd *adm;
	char *msg;
	void *ptr;
	int ioctlStatus;
	menu_set_log_mode();

	msg = new char[128];	
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));

	adm->opcode = nvme_admin_get_log_page;
	adm->nsid = par->parameter[1];	
	adm->data_len = 512;
	ptr = new char[adm->data_len];	
	adm->addr = (uint64_t)ptr;
	//DWORD10 = number of DWORD
	adm->cdw10 = ((adm->data_len/4-1)<<16)|par->parameter[0];
	adm->timeout_ms = 1000;
	ioctlStatus = ioctl_nvme_pass_through(PASS_THROUGH_ADMIN_CMD, adm, msg);
	if(ioctlStatus < 0)
	{
		printf("%s\n", msg);
		flag = TEST_FAIL;
	}
	else
	{
		if( ioctlStatus == 0 )
		{
			switch(par->parameter[0])
			{
				case 0x01:	//Error Information
					display_error_info(ptr);
					break;
				case 0x02:	//Smart Information
					display_smart_info(ptr);
					break;
				case 0x03:	//FW slot Information	
					display_fw_slot_info(ptr);
					break;
			}
		
			printf("\n");
		}
		display_completion(&adm->acq);			
		flag = TEST_PASS;						
	}
	
	//MEMORY_SLOT_RELEASE(ptr);
	delete (char *)ptr;
	delete adm;
	delete msg;
	return flag;
#undef FW_IMAGE_FILE_SIZE
}

static Progress
admin_firmware_activate(MenuSystemExport *par)
{
	Progress flag;
	struct nvme_admin_cmd *adm;
	char *msg;
	//int dword10;
	int ioctlStatus;
	menu_set_log_mode();

	msg = new char[128];
	
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));	

	adm->opcode = nvme_admin_activate_fw;
	//DWORD10 = number of DWORD	
	adm->cdw10 = ((par->parameter[1]&0x03)<<3)|(par->parameter[0]&0x07);
	adm->timeout_ms = 1000;
	ioctlStatus = ioctl_nvme_pass_through(PASS_THROUGH_ADMIN_CMD, adm, msg);
	if(ioctlStatus < 0)
	{
		printf("%s\n", msg);
		flag = TEST_FAIL;
	}
	else
	{
		display_completion(&adm->acq);			
		flag = TEST_PASS;						
	}

	delete adm;
	delete msg;
	return flag;
#undef FW_IMAGE_FILE_SIZE
}

static Progress
admin_firmware_download(MenuSystemExport *par)
{
#define FW_IMAGE_FILE_SIZE	(4*1024)
	Progress flag;
	struct nvme_admin_cmd *adm;
	char *msg;
	void *ptr;
	int ioctlStatus;
	menu_set_log_mode();
	
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));
	
	msg = new char[128];	
	//here should open image file and get file size
	adm->data_len = FW_IMAGE_FILE_SIZE;	
	ptr = new char[adm->data_len];
	adm->addr = (uint64_t)ptr;
	//here should read image file to ptr
	memset(ptr, 0xCC, adm->data_len);		

	adm->opcode = nvme_admin_download_fw;
	//DWORD10 = number of DWORD
	adm->cdw10 = adm->data_len/4 - 1;
	//DWORD11 = fw image offset
	adm->cdw11 = 0;
	adm->timeout_ms = 1000;
	ioctlStatus = ioctl_nvme_pass_through(PASS_THROUGH_ADMIN_CMD, adm, msg);
	if(ioctlStatus < 0)
	{
		printf("%s\n", msg);
		flag = TEST_FAIL;
	}
	else
	{
		display_completion(&adm->acq);			
		flag = TEST_PASS;						
	}

	delete (char *)ptr;
	delete adm;
	delete msg;
	return flag;
#undef FW_IMAGE_FILE_SIZE
}

static Progress
admin_creat_delete_q(MenuSystemExport *par)
{
	Progress flag;
	struct nvme_admin_cmd *adm;
	char *msg;
	void *ptr;
	int mps;
	int ioctlStatus;
	menu_set_log_mode();
	mps = ioctl_get_controller_mps(true);
	
	if(mps<0)
	{
		printf("get controller mps fail!!");
		return TEST_FAIL;
	}
	
	msg = new char[128];
	ptr = NULL;
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));	
	switch(par->testOP)
	{		
		case OP_ADMIN_CREATE_IO_SUBMISSION_QUEUE:			
		case OP_ADMIN_CREATE_IO_COMPLETION_QUEUE:						
			adm->data_len = 128;
			ptr = aligned_malloc(adm->data_len, mps);
			adm->addr = (uint64_t)ptr;
			adm->cdw10 = (0x03FF<<16)|par->parameter[0];
			if(par->testOP==OP_ADMIN_CREATE_IO_SUBMISSION_QUEUE)
			{
				adm->opcode = nvme_admin_create_sq;
				adm->cdw11 = (par->parameter[1]<<16)|0x05;
			}
			else
			{
				adm->opcode = nvme_admin_create_cq;
				adm->cdw11 = 0x03;				
			}
			break;			
		case OP_ADMIN_DELETE_IO_COMPLETION_QUEUE:
		case OP_ADMIN_DELETE_IO_SUBMISSION_QUEUE:
			adm->cdw10 = par->parameter[0];
			adm->opcode = (par->testOP==OP_ADMIN_DELETE_IO_SUBMISSION_QUEUE)?	\
					nvme_admin_delete_sq:nvme_admin_delete_cq;
			break;
	}
		
	adm->timeout_ms = 1000;
	ioctlStatus = ioctl_nvme_pass_through(PASS_THROUGH_ADMIN_CMD, adm, msg);
	if(ioctlStatus < 0)
	{
		printf("%s\n", msg);
		flag = TEST_FAIL;
	}
	else
	{
		display_completion(&adm->acq);			
		flag = TEST_PASS;						
	}

	if(ptr!=NULL)
	{
		aligned_free(ptr);
	}
	delete adm;
	delete msg;
	return flag;
}

static Progress
admin_abort(MenuSystemExport *par)
{
	Progress flag;
	struct nvme_admin_cmd *adm;
	char *msg;
	int ioctlStatus;	

	msg = new char[128];
	menu_set_log_mode();
	
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));
	adm->opcode = nvme_admin_abort_cmd;
	//[31:16]command id ; [15:0]queue id
	adm->cdw10 = (par->parameter[1]<<16)|(par->parameter[0]<<0);
	adm->timeout_ms = 1000;
	ioctlStatus = ioctl_nvme_pass_through(PASS_THROUGH_ADMIN_CMD, adm, msg);
	if(ioctlStatus < 0)
	{
		printf("%s\n", msg);
		flag = TEST_FAIL;
	}
	else
	{
		display_completion(&adm->acq);			
		flag = TEST_PASS;						
	}
		
	delete adm;
	delete msg;
	return flag;
}

static Progress
admin_feature(MenuSystemExport *par)
{
	Progress flag;
	struct nvme_admin_cmd *adm;
	void *ds;
	char * msg;
	int ioctlStatus;
	unsigned int d32;

	msg = new char[64];
	menu_set_log_mode();
	ds = NULL;
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));
	
	if(par->testOP==OP_ADMIN_GET_FEATURE)
	{
		adm->opcode = nvme_admin_get_features;
		adm->nsid = par->parameter[1];
		adm->data_len = 128;
	}
	else
	{
		adm->opcode = nvme_admin_set_features;
		adm->cdw11 = par->parameter[1];
	}

	adm->cdw10 = par->parameter[0]; //feature id

	if(adm->data_len)
	{
		ds = new char[adm->data_len];
		adm->addr = (uint64_t)ds;
	}

	adm->timeout_ms = 1000;
	ioctlStatus = ioctl_nvme_pass_through(PASS_THROUGH_ADMIN_CMD, adm, msg);
	if(ioctlStatus < 0)
	{
		printf("%s\n", msg);
		flag = TEST_FAIL;
	}
	else
	{//here should only for get feature to show PRP data structure
	 //short data will in completion queue 'result' value
		if(ioctlStatus==0)
		{
			if(par->testOP == OP_ADMIN_GET_FEATURE)
			{
				switch(par->parameter[0])
				{
					case NVME_FEAT_ARBITRATION:
						d32 = (adm->acq.result>>24)&0xFF;
						printf("High Priority Weight = 0x%02X (HPW = %d)\n", d32, d32+1);
						d32 = (adm->acq.result>>16)&0xFF;
						printf("Medium Priority Weight = 0x%02X (MPW = %d)\n", d32, d32+1);
						d32 = (adm->acq.result>>8)&0xFF;
						printf("Low Priority Weight = 0x%02X (LPW = %d)\n", d32, d32+1);
						d32 = (adm->acq.result>>0)&0x07;
						printf("Arbitration Burst = 0x%02X (AB = %d)\n", d32, 1<<d32);
						printf("ps : AB=128 mean no limit\n\n");
						break;
					case NVME_FEAT_POWER_MGMT:
						d32 = (adm->acq.result>>0)&0x1F;
						printf("Power State = 0x%02X (PS = %d)\n\n", d32, d32);
						break;
					case NVME_FEAT_LBA_RANGE:
						display_lba_range_type((struct nvme_lba_range_type *)ds);
						printf("\n");
						break;
					case NVME_FEAT_TEMP_THRESH:
						d32 = (adm->acq.result>>0)&0xFFFF;
						printf("Temperature Threshold = 0x%04X (TMPTH = %d K)\n\n", d32, d32);
						break;
					case NVME_FEAT_ERR_RECOVERY:
						d32 = (adm->acq.result>>0)&0xFFFF;
						printf("Time Limited Error Recovery = 0x%04X (TLER = %d ms)\n", d32, d32*100);
						printf("ps : TLER=0 mean no limit\n\n");
						break;
					case NVME_FEAT_VOLATILE_WC:
						d32 = (adm->acq.result>>0)&0x01;
						printf("Volatile Write Cache Enable = 0x%02X (WCE = %s)\n\n", d32, (d32)?"enable":"disable");
						break;					
					case NVME_FEAT_NUM_QUEUES:
						d32 = (adm->acq.result>>16)&0xFFFF;
						printf("Number of I/O Completion Queues Requested = 0x%04X (NCQR = %d)\n",
							d32, d32+1);
						d32 = (adm->acq.result>>0)&0xFFFF;
						printf("Number of I/O Submission Queues Requested = 0x%04X (NSQR = %d)\n\n",
							d32, d32+1);						
						break;
					case NVME_FEAT_IRQ_COALESCE:
						d32 = (adm->acq.result>>8)&0xFF;
						printf("Aggregation Time = 0x%02X (TIME = %d)\n", d32, d32);
						d32 = (adm->acq.result>>0)&0xFF;
						printf("Aggregation Threshold = 0x%04X (THR = %d)\n", d32, d32+1);
						printf("ps : TIME=0 mean no delay or disabled\n\n");
						break;
					case NVME_FEAT_IRQ_CONFIG:
						d32 = (adm->acq.result>>16)&0xFF;
						printf("Coalescing Disable = 0x%02X (CD = %s)\n", d32, (d32)?"true":"false");
						d32 = (adm->acq.result>>0)&0xFFFF;
						printf("Interrupt Vector = 0x%04X (IV = %d)\n\n", d32, d32);
						break;
					case NVME_FEAT_WRITE_ATOMIC:
						d32 = (adm->acq.result>>0)&0xFF;
						printf("Disable Normal = 0x%02X (DN = %s)\n\n", d32, (d32)?"true":"false");
						break;						
	//NVME_FEAT_ASYNC_EVENT	= 0x0b,												
					default:
						break;
				}
			}
		}		
		display_completion(&adm->acq);
		flag = TEST_PASS;
	}	
	
	if(ds!=NULL)
	delete (char *)ds;
	
	delete adm;
	delete msg;

	return flag;
}

static Progress
admin_identify(MenuSystemExport *par)
{
	Progress flag;
	struct nvme_admin_cmd *adm;
	char *msg;
	int ioctlStatus;	
	union{
		void *ptr;
		struct nvme_id_ns *ns;
		struct nvme_id_ctrl *ctrl;
	}id;
	const char *answer[2]={"yes", "no"};
	int i;

	msg = new char[128];
	menu_set_log_mode();
	
	if(par->parameter[1]>1)
	{
		printf("cns only can be 0 for namespace or 1 for controller\n");
		flag = TEST_ILLEGAL_PARAMETER;
	}
	else
	{
		adm = new struct nvme_admin_cmd();
		ioctlStatus = (par->parameter[1]==0)?sizeof(struct nvme_id_ns):sizeof(struct nvme_id_ctrl);
		id.ptr = new char[ioctlStatus];
		
		memset(adm, 0, sizeof(struct nvme_admin_cmd));
		adm->opcode = nvme_admin_identify;
		adm->nsid = par->parameter[0];
		adm->cdw10 = par->parameter[1];
		adm->data_len = ioctlStatus;
		adm->addr = (uint64_t)id.ptr;
		ioctlStatus = ioctl_nvme_pass_through(PASS_THROUGH_ADMIN_CMD, adm, msg);
		if(ioctlStatus < 0)
		{
			printf("%s\n", msg);
			flag = TEST_FAIL;
		}
		else
		{
			if(ioctlStatus==0)
			{//NVME status code success
				if(par->parameter[1]==0)
				{//namespace
					const std::string rpStr[4]={"Best", "Better", "Good", "Degrade" };					
					int idx;								
					printf("Namespace %d identify info\n", par->parameter[0]);
					printf("Namespace Size (NSZE) = 0x%016llX\n", id.ns->nsze);
					printf("  maximum LBA ==> 0x%016llX\n", id.ns->nsze-1 );
					printf("Namespace Capacity (NCAP) = 0x%016llX (%.2fGB)\n", id.ns->ncap ,((float)id.ns->nsze*LBA_SIZE)/(1024*1024*1024));
					printf("Namespace Utilization (NUSE) = 0x%016llX\n", id.ns->nuse);
					printf("Namespace Features (NSFEAT) = 0x%02X\n", id.ns->nsfeat);
					printf("Number of LBA Formats (NLBAF) = 0x%02X\n", id.ns->nlbaf);
					printf("  support LBA formats ==> %d\n", id.ns->nlbaf+1);
					printf("Formatted LBA Size (FLBAS) = 0x%02X\n", id.ns->flbas);
					i=(id.ns->flbas&(1<<4))?0:1;
					printf("  meta data extened in end of LBA data ==> %s\n", answer[i]);
					printf("  which LBA format be used ==> %d\n", id.ns->flbas&0x0F);
					printf("Metadata Capabilities (MC) = 0x%02X\n", id.ns->mc);
					i=(id.ns->mc&(1<<0))?0:1;
					printf("  support meta data append behind LBA data ==> %s\n", answer[i] );
					i=(id.ns->mc&(1<<1))?0:1;
					printf("  support meta data store in separated buffer ==> %s\n", answer[i] );
					printf("End-to-end Data Protection Capabilities (DPC) = 0x%02X\n", id.ns->dpc);
					i=(id.ns->dpc&(1<<4))?0:1;
					printf("  support T10 locate in last eight bytes of meta data ==> %s\n", answer[i] );
					i=(id.ns->dpc&(1<<3))?0:1;
					printf("  support T10 locate in first eight bytes of meta data ==> %s\n", answer[i] );
					i=(id.ns->dpc&(1<<2))?0:1;
					printf("  support type 3 T10 protection ==> %s\n", answer[i] );
					i=(id.ns->dpc&(1<<1))?0:1;
					printf("  support type 2 T10 protection ==> %s\n", answer[i] );
					i=(id.ns->dpc&(1<<0))?0:1;
					printf("  support type 1 T10 protection ==> %s\n", answer[i] );
					printf("End-to-end Data Protection Type Settings (DPS) = 0x%02X\n", id.ns->dps);
					i=(id.ns->dpc&0x07);
					printf("  current configured T10 type ==> ");
					if(i==0)
					{
						printf("none\n");
					}
					else
					{
						printf("type %d\n", i );
					}
					printf("Namespace Multi-path I/O and Namespace Sharing Capabilities (NMIC) = 0x%02X\n", id.ns->nmic);
					printf("Reservation Capabilities (RESCAP) = 0x%02X\n", id.ns->rescap);
					printf("IEEE Extended Unique Identifier = 0x%016llX\n", id.ns->eui64);					
					
					printf("-------- LBAF description -----------\n");
					for(idx=0;idx<id.ns->nlbaf+1;idx++)
					{
						struct nvme_lbaf *lbaf;
						lbaf = &id.ns->lbaf[idx];						
						printf("-----> LBAF[%d] = 0x%08X\n", idx, *(int*)lbaf);					
						printf("Relative Performace = 0x%02X (%s)\n", lbaf->rp, rpStr[lbaf->rp].c_str());
						printf("LBA data size = %d\n", 1<<lbaf->ds);
						printf("Metadata size = %d\n", lbaf->ms);
					}
				}
				else
				{//controller										
					printf("Controller identify info\n");
					printf("PCI Vendor ID (VID) = 0x%04X\n", id.ctrl->vid);
					printf("PCI Subsystem Vendor ID (SSVID) = 0x%04X\n", id.ctrl->ssvid);
					memcpy(msg, id.ctrl->sn, sizeof(id.ctrl->sn)); msg[sizeof(id.ctrl->sn)] =__EOS;
					printf("Serial Number (SN) = %s\n", msg);
					memcpy(msg, id.ctrl->mn, sizeof(id.ctrl->mn)); msg[sizeof(id.ctrl->mn)] =__EOS;
					printf("Model Number (MN) = %s\n", msg);
					memcpy(msg, id.ctrl->fr, sizeof(id.ctrl->fr)); msg[sizeof(id.ctrl->fr)] =__EOS;
					printf("Firmware Revision (FR) = %s\n", msg);
					printf("Recommended Arbitration Burst (RAB) = %d\n", id.ctrl->rab);
					memcpy(msg, id.ctrl->ieee, sizeof(id.ctrl->ieee)); msg[sizeof(id.ctrl->ieee)] =__EOS;
					printf("IEEE OUI Identifier (IEEE) = %s\n", msg);
					//printf("IEEE OUI Identifier (IEEE) = %02X%02X%02X\n", id.ctrl->ieee[0],id.ctrl->ieee[1],id.ctrl->ieee[2]);
					printf("Controller Multi-Path I/O and Namespace Sharing Capabilities (CMIC) = 0x%02X\n", id.ctrl->mic);										
					printf("Maximum data transfer size = 0x%02X (%d)\n", id.ctrl->mdts, 1<<id.ctrl->mdts);
					printf("Controller ID = 0x%04X\n", id.ctrl->ctrlid);
					printf("Optional Admin Command Support (OACS) = 0x%04X\n", id.ctrl->oacs);
					printf("Abort Command Limit (ACL) = 0x%02X\n", id.ctrl->acl);
					printf("Asynchronous Event Request Limit (AERL) = 0x%02X\n", id.ctrl->aerl);
					printf("Firmware Updates (FRMW) = 0x%02X\n", id.ctrl->frmw);
					printf("Log Page Attributes (LPA) = 0x%02X\n", id.ctrl->lpa);
					printf("Error Log Page Entries (ELPE) = 0x%02X\n", id.ctrl->elpe);
					printf("Number of Power State Support (NPSS) = 0x%02X\n", id.ctrl->npss);
					printf("  support power state number = %d)\n", id.ctrl->npss+1 );
					printf("Admin Vendor Specific Command Configuration (AVSCC) = 0x%02X\n", id.ctrl->avscc);
					printf("Autonomous Power State Transition Attributes (APSTA) = 0x%02X\n", id.ctrl->apsta);
					//512
					printf("Submission Queue Entry Size (SQES) = 0x%02X\n", id.ctrl->sqes);
					printf("  maximum=%d, required=%d)\n", 1<<(id.ctrl->sqes>>4), 1<<(id.ctrl->sqes&0xF) );
					printf("Completion Queue Entry Size (CQES) = 0x%02X\n", id.ctrl->cqes);
					printf("  maximum=%d, required=%d)\n", 1<<(id.ctrl->cqes>>4), 1<<(id.ctrl->cqes&0xF) );
					printf("Number of Namespaces (NN) = %d\n", id.ctrl->nn);
					printf("Optional NVM Command Support (ONCS) = 0x%04X\n", id.ctrl->oncs);
					i = (id.ctrl->oncs&(1<<0))?0:1;
					printf("  support command Compare ==> %s\n", answer[i]);
					i = (id.ctrl->oncs&(1<<1))?0:1;
					printf("  support command Write Uncorrectable ==> %s\n", answer[i]);
					i = (id.ctrl->oncs&(1<<2))?0:1;
					printf("  support command Dataset Management ==> %s\n", answer[i]);
					i = (id.ctrl->oncs&(1<<3))?0:1;
					printf("  support command Write Zeroes ==> %s\n", answer[i]);
					i = (id.ctrl->oncs&(1<<4))?0:1;
					printf("  support Save field in Set Feature and Select field in Get Feature ==> %s\n", answer[i]);
					i = (id.ctrl->oncs&(1<<5))?0:1;
					printf("  support reservation ==> %s\n", answer[i]);					
					printf("Fused Operation Support (FUSES) = 0x%04X\n", id.ctrl->fuses);
					i = (id.ctrl->fuses&(1<<0))?0:1;
					printf("  support fused operation ==> %s\n", answer[i]);
					printf("Format NVM Attributes (FNA) = 0x%02X\n", id.ctrl->fna);
					printf("Volatile Write Cache (VWC) = 0x%02X\n", id.ctrl->vwc);
					printf("Atomic Write Unit Normal (AWUN) = 0x%04X\n", id.ctrl->awun);
					printf("Atomic Write Unit Power Fail (AWUPF) = 0x%04X\n", id.ctrl->awupf);
					printf("NVM Vendor Specific Command Configuration (NVSCC) = 0x%02X\n", id.ctrl->nvscc);
					printf("Atomic Compare & Write Unit (ACWU) = 0x%04X\n", id.ctrl->acwu);
					printf("SGL Support (SGLS) = 0x%08X\n", id.ctrl->sgls);					
					for(i=0;i<id.ctrl->npss+1;i++)
					{						
						struct nvme_id_power_state *ps;
						unsigned int div;
						ps = &id.ctrl->psd[i];
						printf("------- power state [%d] description -------------\n", i);						
						printf("Maximum Power (MP) = 0x%04X\n", ps->max_power );
						printf("Max Power Scale (MPS) = 0x%02X\n", (ps->flags>>0)&0x01 );
						printf("Non-Operational State (NOPS) = 0x%02X\n", (ps->flags>>1)&0x01 );
						printf("  the controller can processes I/O commands in this power state ==> %s\n",
									((ps->flags>>1)&0x01)?"false":"true" );
						div = ((ps->flags>>0)&0x01)?100:10000;
						printf("  the maximum power consumed in this power state = %2.4fW\n",(float)ps->max_power/div);
						printf("Entry Latency (ENLAT) = 0x%08X = %dms\n", ps->entry_lat, ps->entry_lat );
						printf("Exit Latency (EXLAT) = 0x%08X = %dms\n", ps->exit_lat, ps->exit_lat );
						printf("Relative Read Throughput (RRT) = 0x%02X\n", ps->read_tput);
						printf("Relative Read Latency (RRL) = 0x%02X\n", ps->read_lat);
						printf("Relative Write Throughput (RWT) = 0x%02X\n", ps->write_tput);
						printf("Relative Write Latency (RWL) = 0x%02X\n", ps->write_lat);						
					}
				}				
				printf("\n");
			}
			
			display_completion(&adm->acq);
			
			flag = TEST_PASS;						
		}
		
		delete adm;
		delete (char *)id.ptr;
	}
		
	delete msg;
	return flag;
}

static Progress
admin_queue_config(MenuSystemExport *par)
{
	Progress flag;
	NvmeVendorCmd vendor;
	char *msg;
	msg = new char[64];
	memset(&vendor, 0, sizeof(struct nvme_vendor_cmd));				
	vendor.cmd = IOCTL_VENDOR_CONFIG_ADMQ;
	if(0 == ioctl_nvme_pass_through(PASS_THROUGH_DEV_VENDOR, &vendor, msg))
	{
		admin_cmd_menu_swap(ADMIN_MENU_SWAP_ALL);
		sprintf(msg, "controller admin queue init success!!");
		flag = TEST_PASS;					
	}
	else
	{
		flag = TEST_FAIL;
	}
	
	menu_show_report(0, msg);
	delete msg;
	
	return flag;
}

static void
task_admin_basic_init(unsigned char op)
{
	int iReport=0;
	menu_clr_report(0);
	switch(op)
	{
		case OP_ADMIN_IDENTIFY:			
			menu_question_push((TStr)adminLogMsg[LOG_MSG_NSID], modeDEC, 1);
			menu_question_push((TStr)"cns (0=namespace, 1=controller) = ", modeDEC, 1);
			break;
		case OP_ADMIN_GET_FEATURE:
		case OP_ADMIN_SET_FEATURE:			
			display_feature();			
			menu_question_push((TStr)adminLogMsg[LOG_MSG_FID], modeDEC, 2);
			if(op==OP_ADMIN_GET_FEATURE)
			{
				menu_question_push((TStr)adminLogMsg[LOG_MSG_NSID], modeDEC, 1);
			}
			else
			{
				menu_question_push((TStr)adminLogMsg[LOG_MSG_DWORD11], modeHEX, 8);
			}
			break;
		case OP_ADMIN_ABORT:
			menu_question_push((TStr)adminLogMsg[LOG_MSG_QID], modeHEX, 8);
			menu_question_push((TStr)"Command ID = ", modeDEC, 2);
			break;
		case OP_ADMIN_CREATE_IO_COMPLETION_QUEUE:
		case OP_ADMIN_DELETE_IO_COMPLETION_QUEUE:
		case OP_ADMIN_DELETE_IO_SUBMISSION_QUEUE:
			menu_question_push((TStr)adminLogMsg[LOG_MSG_QID], modeHEX, 8);
			break;		
		case OP_ADMIN_CREATE_IO_SUBMISSION_QUEUE:
			menu_question_push((TStr)adminLogMsg[LOG_MSG_QID], modeHEX, 8);
			menu_question_push((TStr)adminLogMsg[LOG_MSG_CQID], modeHEX, 8);
			break;
		case OP_ADMIN_FIRMWARE_ACTIVATE:
			menu_show_report(iReport++, (TStr)"Slot number 0 mean choice by controller");
			menu_show_report(iReport++, (TStr)"Activate action code (2 bits):\n");
			menu_show_report(iReport++, (TStr)" 0 = only replace fw slot\n");
			menu_show_report(iReport++, (TStr)" 1 = replace fw slot and activate in next reset\n");
			menu_show_report(iReport++, (TStr)" 2 = activate fw slot in next reset\n");
			menu_show_report(iReport++, (TStr)" 3 = reserved\n");
			menu_question_push((TStr)"Slot number = ", modeDEC, 1);
			menu_question_push((TStr)"Activate action = ", modeDEC, 1);
			break;
		case OP_ADMIN_GET_LOG_PAGE:
			menu_show_report(iReport++, (TStr)"Log page identifier id:\n");
			menu_show_report(iReport++, (TStr)"  00   = reserved\n");
			menu_show_report(iReport++, (TStr)"  01   = Error Information\n");
			menu_show_report(iReport++, (TStr)"  02   = SMART / Health Information\n");
			menu_show_report(iReport++, (TStr)"  03   = Firmware Slot Information\n");
			menu_show_report(iReport++, (TStr)" 04~7F = reserved\n");
			menu_show_report(iReport++, (TStr)" 80~BF = I/O Command Set Specific\n");
			menu_show_report(iReport++, (TStr)" C0~FF = Vendor Specific\n");			
			menu_question_push((TStr)"Log page id = 0x", modeHEX, 2);
			menu_question_push((TStr)adminLogMsg[LOG_MSG_NSID], modeDEC, 1);
			break;
	}
}


static Progress 
task_admin_basic(MenuSystemExport *par)
{	
	Progress flag;
	ValidateTask task=NULL;	
	switch(par->testOP)
	{
		case OP_ADMIN_CONFIG_QUEUE:
			task = admin_queue_config;
			break;
		case OP_ADMIN_IDENTIFY:
			task = admin_identify;
			break;
		case OP_ADMIN_GET_FEATURE:
		case OP_ADMIN_SET_FEATURE:
			task = admin_feature;
			break;
		case OP_ADMIN_ABORT:
			task = admin_abort;
			break;
		case OP_ADMIN_DELETE_IO_SUBMISSION_QUEUE:
		case OP_ADMIN_CREATE_IO_SUBMISSION_QUEUE:
		case OP_ADMIN_DELETE_IO_COMPLETION_QUEUE:
		case OP_ADMIN_CREATE_IO_COMPLETION_QUEUE:
			task = admin_creat_delete_q;
			break;
		case OP_ADMIN_FIRMWARE_ACTIVATE:
			task = admin_firmware_activate;
			break;
		case OP_ADMIN_FIRMWARE_IMAGE_DOWNLOAD:
			task = admin_firmware_download;
			break;
		case OP_ADMIN_GET_LOG_PAGE:
			task = admin_get_log_page;
			break;
		default:
			flag = TEST_ILLEGAL_PARAMETER;
			break;
	}
	
	if(task != NULL)
	{
		flag = task(par);
	}
	
	return flag;
}

void
admin_basic_init(void)
{	
	menu_system_task_register( REGISTERED_TASK_IDX, adminBasicMenu, task_admin_basic_init, task_admin_basic );
}
