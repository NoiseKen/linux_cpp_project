#include "nvme_io.h"
#include "utils.h"
#include "utils_qt.h"
#include "debug.h"

const char *NVMeIO::answer[2]={"yes", "no"};

NVMeIO::CMDSet NVMeIO::adminCmdSet[]={
	{"Delete I/O submission queue (0x00)", nvme_admin_delete_sq},
	{"Create I/O submission queue (0x01)", nvme_admin_create_sq},
	{"Get log page (0x02)", nvme_admin_get_log_page},
	{"Delete I/O completion queue (0x04)", nvme_admin_delete_cq},
	{"Create I/O completion queue (0x05)", nvme_admin_create_cq},
	{"Identify (0x06)", nvme_admin_identify},
	{"Abort (0x08)", nvme_admin_abort_cmd},
	{"Set feature (0x09)", nvme_admin_set_features},
	{"Get feature (0x0A)", nvme_admin_get_features},
	{"Asynchronous event request (0x0C)", nvme_admin_async_event},
	{"Firmware active (0x10)", nvme_admin_activate_fw},
	{"Firmware image download (0x11)", nvme_admin_download_fw},
	{"Format NVM (0x80)", nvme_admin_format_nvm},
	{"Security Send (0x81)", nvme_admin_security_send},
	{"Security Receive (0x82)", nvme_admin_security_recv},
	//{"Invalid command for test (0x31)", nvme_admin_invalid_test},
};

unsigned int const NVMeIO::adminCmdCount = sizeof(adminCmdSet)/sizeof(CMDSet);


NVMeIO::CMDSet NVMeIO::featureIdSet[]={
	{"Arbitration (0x01)", NVME_FEAT_ARBITRATION},
	{"Power Management (0x02)", NVME_FEAT_POWER_MGMT},
	{"LBA Range Type (0x03)", NVME_FEAT_LBA_RANGE},
	{"Temperature Threshold (0x04)", NVME_FEAT_TEMP_THRESH},
	{"Error Recovery (0x05)", NVME_FEAT_ERR_RECOVERY},
	{"Volatile Write Cache (0x06)", NVME_FEAT_VOLATILE_WC},
	{"Number of Queues (0x07)", NVME_FEAT_NUM_QUEUES},
	{"Interrupt Coalescing (0x08)", NVME_FEAT_IRQ_COALESCE},
	{"Interrupt Vector Configuration (0x09)", NVME_FEAT_IRQ_CONFIG},
	{"Write Atomicity (0x0A)", NVME_FEAT_WRITE_ATOMIC},
	{"Asynchronous Event Configuration (0x0B)", NVME_FEAT_ASYNC_EVENT},
};

unsigned int const NVMeIO::featureIdCount = sizeof(featureIdSet)/sizeof(CMDSet);

//static variable
QSet<unsigned int> NVMeIO::supportedGenericSetFID = QSet<unsigned int>() << SUPPORTED_SET_FID;
QSet<unsigned int> NVMeIO::supportedGenericGetFID = QSet<unsigned int>() << SUPPORTED_GET_FID; 

int 
NVMeIO::admin_identify(struct nvme_id_ctrl *ctrl)
{
	struct nvme_admin_cmd *adm;
	int ioctlStatus;
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));
	adm->opcode = nvme_admin_identify;
	adm->cdw10 = 0x01;	//for identify controller
	//adm->cdw14 = KW_REF_TAG;
	adm->data_len = sizeof(struct nvme_id_ctrl);
	adm->addr = (uint64_t)ctrl;
	ioctlStatus = nvme_io_pass_through(NVME_IOCTL_ADMIN_CMD, adm);
	if(0==ioctlStatus)
	{
		ioctlStatus = adm->result;
	}	
	delete adm;
	return ioctlStatus;									
}

int 
NVMeIO::admin_identify(struct nvme_id_ns *ns, unsigned int nsid)
{
	struct nvme_admin_cmd *adm;
	int ioctlStatus;	
	
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));
	adm->opcode = nvme_admin_identify;
	adm->nsid = nsid;
	adm->cdw10 = 0x00;	//for identify namespace
	//adm->cdw14 = KW_REF_TAG;
	adm->data_len = sizeof(struct nvme_id_ns);
	adm->addr = (uint64_t)ns;
	ioctlStatus = nvme_io_pass_through(NVME_IOCTL_ADMIN_CMD, adm);
	delete adm;
	return ioctlStatus;									
}

int 
NVMeIO::admin_feature(AdminDW10::SetFeature dw10, AdminDW11 dw11)
{
	struct nvme_admin_cmd *adm;
	int ioctlStatus=notSupportCmd;	
	if(supportedGenericSetFID.contains(dw10.fid))
	{	
		adm = new struct nvme_admin_cmd();
		memset(adm, 0, sizeof(struct nvme_admin_cmd));
		adm->opcode = nvme_admin_set_features;
		adm->cdw10 = dw10.d32;
		adm->cdw11 = dw11.d32;

		ioctlStatus = nvme_io_pass_through(NVME_IOCTL_ADMIN_CMD, adm);
		delete adm;
	}
	return ioctlStatus;		
}

int 
NVMeIO::admin_feature(AdminDW10::GetFeature dw10, unsigned int &dw0)
{
	struct nvme_admin_cmd *adm;
	int ioctlStatus=notSupportCmd;	
	if(supportedGenericGetFID.contains(dw10.fid))
	{	
		adm = new struct nvme_admin_cmd();
		memset(adm, 0, sizeof(struct nvme_admin_cmd));
		adm->opcode = nvme_admin_get_features;
		adm->cdw10 = dw10.d32;

		ioctlStatus = nvme_io_pass_through(NVME_IOCTL_ADMIN_CMD, adm);
		if(ioctlStatus==NVME_SC_SUCCESS)
		{
			dw0 = adm->result;
		}
		delete adm;
	}
	return ioctlStatus;		
}

//set lrt
int
NVMeIO::admin_feature_lrt(bool sv, AdminDW11::LBARangeType dw11, struct nvme_lba_range_type *lrt, unsigned int nsid)
{
	struct nvme_admin_cmd *adm;
	int ioctlStatus;	
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));
	adm->opcode = nvme_admin_set_features;
	adm->nsid = nsid;
	adm->cdw10 = NVME_FEAT_LBA_RANGE | ((sv)?(1<<31):0);
	adm->cdw11 = dw11.d32;
	adm->addr = (uint64_t)lrt;
	adm->data_len = sizeof(struct nvme_lba_range_type)*(dw11.num+1);
	ioctlStatus = nvme_io_pass_through(NVME_IOCTL_ADMIN_CMD, adm);
	delete adm;
	return ioctlStatus;	
}

//get lrt
int
NVMeIO::admin_feature_lrt(unsigned int sel, AdminDW11::LBARangeType dw11, struct nvme_lba_range_type *lrt, unsigned int nsid)
{
	struct nvme_admin_cmd *adm;
	int ioctlStatus;
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));
	adm->opcode = nvme_admin_get_features;
	adm->nsid = nsid;
	adm->cdw10 = NVME_FEAT_LBA_RANGE | (((sel&0x07))<<8);
	adm->cdw11 = dw11.d32;
	adm->addr = (uint64_t)lrt;
	adm->data_len = sizeof(struct nvme_lba_range_type)*(dw11.num+1);
	ioctlStatus = nvme_io_pass_through(NVME_IOCTL_ADMIN_CMD, adm);
	delete adm;
	return ioctlStatus;	
}

int
NVMeIO::admin_generic(AdminDW10::FirmwareActivate dw10)
{
	struct nvme_admin_cmd *adm;
	int ioctlStatus;	
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));
	adm->opcode = nvme_admin_activate_fw;
	adm->cdw10 = dw10.d32;

	ioctlStatus = nvme_io_pass_through(NVME_IOCTL_ADMIN_CMD, adm);
	delete adm;
	return ioctlStatus;	
}

int
NVMeIO::admin_generic(AdminDW10::FirmwareImageDownload dw10, AdminDW11::FirmwareImageDownload dw11, unsigned int *img)
{
	struct nvme_admin_cmd *adm;
	int ioctlStatus;	
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));
	adm->opcode = nvme_admin_download_fw;
	adm->cdw10 = dw10.d32;
	adm->cdw11 = dw11.d32;
	adm->addr = (uint64_t)img;
	adm->data_len = (dw10.numd+1) * 4;

	ioctlStatus = nvme_io_pass_through(NVME_IOCTL_ADMIN_CMD, adm);
	delete adm;
	return ioctlStatus;	
}

int NVMeIO::admin_generic(AdminDW10::GetLogPage dw10, void *log, unsigned int nsid)
{
	struct nvme_admin_cmd *adm;
	int ioctlStatus;	
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));
	adm->opcode = nvme_admin_get_log_page;
	adm->nsid = nsid;
	adm->cdw10 = dw10.d32;
	adm->addr = (uint64_t)log;
	adm->data_len = (dw10.numd+1) * sizeof(int);

	ioctlStatus = nvme_io_pass_through(NVME_IOCTL_ADMIN_CMD, adm);
	delete adm;
	return ioctlStatus;	
}

void
NVMeIO::ds_decode(struct fwslot_log_page *fwslot, QString &report)
{
	QString d;
	int i;
	report += d.sprintf("active fw info = 0x%02X\n", fwslot->afi);
#if 1	//if frs is string type		
	char rev[9];
	
	rev[8]=__EOS;
	for(i=1;i<8;i++)
	{		
		memcpy(rev, &fwslot->frss[i-1][0], 8);
		report += d.sprintf("Slot %d fw rev = %s\n", i, rev);
	}
#else
	for(i=1;i<8;i++)
	{
		report += d.sprintf("Slot %d fw rev = 0x%016lX\n", i, fwslot->frsi[i-1]);
	}
#endif
}

void 
NVMeIO::ds_decode(struct nvme_smart_log *smart, QString &report)
{
	QString d;
	unsigned short temp;
	UtilsQT *utils;
	utils = new UtilsQT();
	report += d.sprintf(" -- Smart Log Page --\n");
	report += d.sprintf("critical warning = 0x%02X\n", smart->critical_warning);
	temp = (smart->temperature[1]<<8)+smart->temperature[0];
	report += d.sprintf("temperature = 0x%04X\n", temp);
	report += d.sprintf("avaliable spare = 0x%02X\n", smart->avail_spare);
	report += d.sprintf("avaliable spare threshold = 0x%02X\n", smart->spare_thresh);
	report += d.sprintf("percentage used = %d%%", smart->percent_used);
	report += d.sprintf("\ndata units read      = 0x");
	report += utils->printf_128((char *)smart->data_units_read);
	report += d.sprintf("\ndata units written   = 0x");
	report += utils->printf_128((char *)smart->data_units_written);
	report += d.sprintf("\nhost read commands   = 0x");
	report += utils->printf_128((char *)smart->host_reads);
	report += d.sprintf("\nhost write commands  = 0x");
	report += utils->printf_128((char *)smart->host_writes);	
	report += d.sprintf("\ncontroller busy time = 0x");
	report += utils->printf_128((char *)smart->ctrl_busy_time);
	report += d.sprintf("\npower cycles         = 0x");
	report += utils->printf_128((char *)smart->power_cycles);
	report += d.sprintf("\npower on hours       = 0x");
	report += utils->printf_128((char *)smart->power_on_hours);
	report += d.sprintf("\nunsafe shoutdowns    = 0x");
	report += utils->printf_128((char *)smart->unsafe_shutdowns);
	report += d.sprintf("\nmedia errors         = 0x");
	report += utils->printf_128((char *)smart->media_errors);
	report += d.sprintf("\n# of err log entries = 0x");
	report += utils->printf_128((char *)smart->num_err_log_entries);	
	
	report += "\n";
	delete utils;	
}

void 
NVMeIO::ds_decode(struct err_log_page *err, QString &report, unsigned int cnt)
{
	unsigned int i;
	QString d;
	for(i=0;i<cnt;i++)
	{
		report += d.sprintf(" -- Error Log Page[0x%02X] --\n", i);
		report += d.sprintf("error count = %ld\n", err->errCount);
		report += d.sprintf("submission queue id = %d\n", err->sqId);
		report += d.sprintf("command id = %d\n", err->cmdId);
		report += d.sprintf("status code = 0x%04X\n", err->statusField);
		report += d.sprintf("parameter error location = 0x%04X\n", err->statusField);
		report += d.sprintf("LBA = 0x%016lX\n", err->lba);
		report += d.sprintf("vendor specific info = 0x%02X\n\n", err->vendor);
		err++;
	}
}

void 
NVMeIO::ds_decode(struct nvme_lba_range_type *lrt, QString &report, unsigned int cnt)
{
	unsigned int i;
	QString d;
	for(i=0;i<cnt;i++)
	{
		report += d.sprintf(" -- LBA Range Type[0x%02X] --\n", i);
		report += d.sprintf("Type = 0x%02X\n", lrt->type);
		report += d.sprintf("Attributes = 0x%02X\n", lrt->attributes);
		report += d.sprintf("Start LBA = 0x%016llX\n", lrt->slba);
		report += d.sprintf("Number of logic block = 0x%016llX\n", lrt->nlb);

		report += d.sprintf("GUID = 0x");
		report += UtilsQT().printf_128((char *)lrt->guid);
		report += d.sprintf("\n\n");
		lrt++;
	}
}

void 
NVMeIO::ds_decode(struct nvme_id_ctrl *ctrl, QString &report)
{
	QString d;
	int i;
	char *local;
	local = new char[64];
	report += d.sprintf(" -- Identify Controller --\n");
	report += d.sprintf("PCI Vendor ID (VID) = 0x%04X\n", ctrl->vid);
	report += d.sprintf("PCI Subsystem Vendor ID (SSVID) = 0x%04X\n", ctrl->ssvid);
	memcpy(local, ctrl->sn, sizeof(ctrl->sn)); local[sizeof(ctrl->sn)] =__EOS;
	report += d.sprintf("Serial Number (SN) = %s\n", local);
	memcpy(local, ctrl->mn, sizeof(ctrl->mn)); local[sizeof(ctrl->mn)] =__EOS;
	report += d.sprintf("Model Number (MN) = %s\n", local);
	memcpy(local, ctrl->fr, sizeof(ctrl->fr)); local[sizeof(ctrl->fr)] =__EOS;
	report += d.sprintf("Firmware Revision (FR) = %s\n", local);
	report += d.sprintf("Recommended Arbitration Burst (RAB) = %d\n", ctrl->rab);
	//memcpy(local, ctrl->ieee, sizeof(ctrl->ieee)); local[sizeof(ctrl->ieee)] =__EOS;
	//report += d.sprintf("IEEE OUI Identifier (IEEE) = %s\n", local);
	report += d.sprintf("IEEE OUI Identifier (IEEE) = %02X-%02X-%02X\n", ctrl->ieee[0], ctrl->ieee[1], ctrl->ieee[2]);
	//printf("IEEE OUI Identifier (IEEE) = %02X%02X%02X\n", ctrl->ieee[0],ctrl->ieee[1],ctrl->ieee[2]);
	report += d.sprintf("Controller Multi-Path I/O and Namespace Sharing Capabilities (CMIC) = 0x%02X\n", ctrl->mic);
	report += d.sprintf("Maximum data transfer size = 0x%02X (%d)\n", ctrl->mdts, 1<<ctrl->mdts);
	report += d.sprintf("Controller ID = 0x%04X\n", ctrl->cntlid);
	report += d.sprintf("Optional Admin Command Support (OACS) = 0x%04X\n", ctrl->oacs);
	report += d.sprintf("Abort Command Limit (ACL) = 0x%02X\n", ctrl->acl);
	report += d.sprintf("Asynchronous Event Request Limit (AERL) = 0x%02X\n", ctrl->aerl);
	report += d.sprintf("Firmware Updates (FRMW) = 0x%02X\n", ctrl->frmw);
	report += d.sprintf("Log Page Attributes (LPA) = 0x%02X\n", ctrl->lpa);
	report += d.sprintf("Error Log Page Entries (ELPE) = 0x%02X\n", ctrl->elpe);
	report += d.sprintf("Number of Power State Support (NPSS) = 0x%02X\n", ctrl->npss);
	report += d.sprintf("  support power state number = %d\n", ctrl->npss+1 );
	report += d.sprintf("Admin Vendor Specific Command Configuration (AVSCC) = 0x%02X\n", ctrl->avscc);
	report += d.sprintf("Autonomous Power State Transition Attributes (APSTA) = 0x%02X\n", ctrl->apsta);
	//512
	report += d.sprintf("Submission Queue Entry Size (SQES) = 0x%02X\n", ctrl->sqes);
	report += d.sprintf("  maximum=%d, required=%d\n", 1<<(ctrl->sqes>>4), 1<<(ctrl->sqes&0xF) );
	report += d.sprintf("Completion Queue Entry Size (CQES) = 0x%02X\n", ctrl->cqes);
	report += d.sprintf("  maximum=%d, required=%d\n", 1<<(ctrl->cqes>>4), 1<<(ctrl->cqes&0xF) );
	report += d.sprintf("Number of Namespaces (NN) = %d\n", ctrl->nn);
	report += d.sprintf("Optional NVM Command Support (ONCS) = 0x%04X\n", ctrl->oncs);
	i = (ctrl->oncs&(1<<0))?0:1;
	report += d.sprintf("  support command Compare ==> %s\n", answer[i]);
	i = (ctrl->oncs&(1<<1))?0:1;
	report += d.sprintf("  support command Write Uncorrectable ==> %s\n", answer[i]);
	i = (ctrl->oncs&(1<<2))?0:1;
	report += d.sprintf("  support command Dataset Management ==> %s\n", answer[i]);
	i = (ctrl->oncs&(1<<3))?0:1;
	report += d.sprintf("  support command Write Zeroes ==> %s\n", answer[i]);
	i = (ctrl->oncs&(1<<4))?0:1;
	report += d.sprintf("  support Save field in Set Feature and Select field in Get Feature ==> %s\n", answer[i]);
	i = (ctrl->oncs&(1<<5))?0:1;
	report += d.sprintf("  support reservation ==> %s\n", answer[i]);					
	report += d.sprintf("Fused Operation Support (FUSES) = 0x%04X\n", ctrl->fuses);
	i = (ctrl->fuses&(1<<0))?0:1;
	report += d.sprintf("  support fused operation ==> %s\n", answer[i]);
	report += d.sprintf("Format NVM Attributes (FNA) = 0x%02X\n", ctrl->fna);
	report += d.sprintf("Volatile Write Cache (VWC) = 0x%02X\n", ctrl->vwc);
	report += d.sprintf("Atomic Write Unit Normal (AWUN) = 0x%04X\n", ctrl->awun);
	report += d.sprintf("Atomic Write Unit Power Fail (AWUPF) = 0x%04X\n", ctrl->awupf);
	report += d.sprintf("NVM Vendor Specific Command Configuration (NVSCC) = 0x%02X\n", ctrl->nvscc);
	report += d.sprintf("Atomic Compare & Write Unit (ACWU) = 0x%04X\n", ctrl->acwu);
	report += d.sprintf("SGL Support (SGLS) = 0x%08X\n", ctrl->sgls);					
	for(i=0;i<ctrl->npss+1;i++)
	{						
		struct nvme_id_power_state *ps;
		unsigned int div;
		ps = &ctrl->psd[i];
		report += d.sprintf("------- power state [%d] description -------------\n", i);
		report += d.sprintf("Maximum Power (MP) = 0x%04X\n", ps->max_power );
		report += d.sprintf("Max Power Scale (MPS) = 0x%02X\n", (ps->flags>>0)&0x01 );
		report += d.sprintf("Non-Operational State (NOPS) = 0x%02X\n", (ps->flags>>1)&0x01 );
		report += d.sprintf("  the controller can processes I/O commands in this power state ==> %s\n",
							((ps->flags>>1)&0x01)?"false":"true" );
		div = ((ps->flags>>0)&0x01)?100:10000;
		report += d.sprintf("  the maximum power consumed in this power state = %2.4fW\n",(float)ps->max_power/div);
		report += d.sprintf("Entry Latency (ENLAT) = 0x%08X = %dms\n", ps->entry_lat, ps->entry_lat );
		report += d.sprintf("Exit Latency (EXLAT) = 0x%08X = %dms\n", ps->exit_lat, ps->exit_lat );
		report += d.sprintf("Relative Read Throughput (RRT) = 0x%02X\n", ps->read_tput);
		report += d.sprintf("Relative Read Latency (RRL) = 0x%02X\n", ps->read_lat);
		report += d.sprintf("Relative Write Throughput (RWT) = 0x%02X\n", ps->write_tput);
		report += d.sprintf("Relative Write Latency (RWL) = 0x%02X\n\n", ps->write_lat);						
	}
	delete local;
}

void 
NVMeIO::ds_decode(struct nvme_id_ns *ns, QString &report)
{
	const char *rpStr[4]={"Best", "Better", "Good", "Degrade" };			
	QString d;
	int i;
	unsigned int lbaSize;
	lbaSize = 1<<(ns->lbaf[ns->flbas&0x0F].ds);
	
	report += d.sprintf(" -- Identify Namespace --\n");
	report += d.sprintf("Namespace Size (NSZE) = 0x%016llX\n", ns->nsze);
	report += d.sprintf("  maximum LBA ==> 0x%016llX\n", ns->nsze-1 );
	report += d.sprintf("Namespace Capacity (NCAP) = 0x%016llX (%.2fGB)\n", ns->ncap ,((float)ns->nsze*lbaSize)/(1024*1024*1024));
	report += d.sprintf("Namespace Utilization (NUSE) = 0x%016llX\n", ns->nuse);
	report += d.sprintf("Namespace Features (NSFEAT) = 0x%02X\n", ns->nsfeat);
	report += d.sprintf("Number of LBA Formats (NLBAF) = 0x%02X\n", ns->nlbaf);
	report += d.sprintf("  support LBA formats ==> %d\n", ns->nlbaf+1);
	report += d.sprintf("Formatted LBA Size (FLBAS) = 0x%02X\n", ns->flbas);
	i=(ns->flbas&(1<<4))?0:1;
	report += d.sprintf("  meta data extened in end of LBA data ==> %s\n", answer[i]);
	report += d.sprintf("  which LBA format be used ==> %d\n", ns->flbas&0x0F);
	report += d.sprintf("Metadata Capabilities (MC) = 0x%02X\n", ns->mc);
	i=(ns->mc&(1<<0))?0:1;
	report += d.sprintf("  support meta data append behind LBA data ==> %s\n", answer[i] );
	i=(ns->mc&(1<<1))?0:1;
	report += d.sprintf("  support meta data store in separated buffer ==> %s\n", answer[i] );
	report += d.sprintf("End-to-end Data Protection Capabilities (DPC) = 0x%02X\n", ns->dpc);
	i=(ns->dpc&(1<<4))?0:1;
	report += d.sprintf("  support E2E data locate in last eight bytes of meta data ==> %s\n", answer[i] );
	i=(ns->dpc&(1<<3))?0:1;
	report += d.sprintf("  support E2E data locate in first eight bytes of meta data ==> %s\n", answer[i] );
	i=(ns->dpc&(1<<2))?0:1;
	report += d.sprintf("  support type 3 E2E data protection ==> %s\n", answer[i] );
	i=(ns->dpc&(1<<1))?0:1;
	report += d.sprintf("  support type 2 E2E data protection ==> %s\n", answer[i] );
	i=(ns->dpc&(1<<0))?0:1;
	report += d.sprintf("  support type 1 E2E data protection ==> %s\n", answer[i] );
	report += d.sprintf("End-to-end Data Protection Type Settings (DPS) = 0x%02X\n", ns->dps);
	i=(ns->dps&0x07);
	report += d.sprintf("  current configured E2E type ==> ");
	if(i==0)
	{
		report += d.sprintf("none\n");
	}
	else
	{
		report += d.sprintf("type %d\n", i );
	}
	report += d.sprintf("Namespace Multi-path I/O and Namespace Sharing Capabilities (NMIC) = 0x%02X\n", ns->nmic);
	report += d.sprintf("Reservation Capabilities (RESCAP) = 0x%02X\n", ns->rescap);
	//report += d.sprintf("IEEE Extended Unique Identifier = 0x%016llX\n", ns->eui64);
	report += "IEEE Extended Unique Identifier = ";
	for(i=0;i<8;i++)
	{
		report += d.sprintf("%c-", ns->eui64[i]);
	}
	report.remove(report.length(),1);
	report += "\n";
	
				
	report += d.sprintf("-------- LBAF description -----------\n");
	for(i=0;i<ns->nlbaf+1;i++)
	{
		struct nvme_lbaf *lbaf;
		lbaf = &ns->lbaf[i];						
		report += d.sprintf("-----> LBAF[%d] = 0x%08X\n", i, *(int*)lbaf);					
		report += d.sprintf("Relative Performace = 0x%02X (%s)\n", lbaf->rp, rpStr[lbaf->rp]);
		report += d.sprintf("LBA data size = %d\n", 1<<lbaf->ds);
		report += d.sprintf("Metadata size = %d\n\n", lbaf->ms);
	}
}

QString
NVMeIO::completion_decode(int sc, NVMeIO *instance)
{
#define CASE_TO_STR(n)	case n:desc = TO_STR(n);break
	QString desc;
	if(sc<0)
	{
		desc.sprintf("io err, error code = %d", sc);
	}
	else
	{
		switch(sc)
		{
			CASE_TO_STR(NVME_SC_SUCCESS);
			CASE_TO_STR(NVME_SC_INVALID_OPCODE);
			CASE_TO_STR(NVME_SC_INVALID_FIELD);
			CASE_TO_STR(NVME_SC_CMDID_CONFLICT);
			CASE_TO_STR(NVME_SC_DATA_XFER_ERROR);
			CASE_TO_STR(NVME_SC_POWER_LOSS);
			CASE_TO_STR(NVME_SC_INTERNAL);
			CASE_TO_STR(NVME_SC_ABORT_REQ);
			CASE_TO_STR(NVME_SC_ABORT_QUEUE);
			CASE_TO_STR(NVME_SC_FUSED_FAIL);
			CASE_TO_STR(NVME_SC_FUSED_MISSING);
			CASE_TO_STR(NVME_SC_INVALID_NS);
			CASE_TO_STR(NVME_SC_CMD_SEQ_ERROR);
			CASE_TO_STR(NVME_SC_LBA_RANGE);
			CASE_TO_STR(NVME_SC_CAP_EXCEEDED);
			CASE_TO_STR(NVME_SC_NS_NOT_READY);
			CASE_TO_STR(NVME_SC_CQ_INVALID);
			CASE_TO_STR(NVME_SC_QID_INVALID);
			CASE_TO_STR(NVME_SC_QUEUE_SIZE);
			CASE_TO_STR(NVME_SC_ABORT_LIMIT);
			CASE_TO_STR(NVME_SC_ABORT_MISSING);
			CASE_TO_STR(NVME_SC_ASYNC_LIMIT);
			CASE_TO_STR(NVME_SC_FIRMWARE_SLOT);
			CASE_TO_STR(NVME_SC_FIRMWARE_IMAGE);
			CASE_TO_STR(NVME_SC_INVALID_VECTOR);
			CASE_TO_STR(NVME_SC_INVALID_LOG_PAGE);
			CASE_TO_STR(NVME_SC_INVALID_FORMAT);
			CASE_TO_STR(NVME_SC_BAD_ATTRIBUTES);
			CASE_TO_STR(NVME_SC_WRITE_FAULT);
			CASE_TO_STR(NVME_SC_READ_ERROR);
			CASE_TO_STR(NVME_SC_GUARD_CHECK);
			CASE_TO_STR(NVME_SC_APPTAG_CHECK);
			CASE_TO_STR(NVME_SC_REFTAG_CHECK);
			CASE_TO_STR(NVME_SC_COMPARE_FAILED);
			CASE_TO_STR(NVME_SC_ACCESS_DENIED);
			CASE_TO_STR(NVME_SC_DNR);
			case openNodeFail:
				desc = "open node fail";	
				if(instance)
				{
					desc += QString( "(%1)").arg(instance->nodePath);
				}
				break;
			case notSupportCmd:
				desc = "class NVMeIO not support this command!!";
				break;
			default:
				desc.sprintf("0x%04X (please refer NVMe spec)", sc);
				break;
		}
	}
	return desc;
#undef CASE_TO_STR	
}

int 
NVMeIO::nvme_io_pass_through(unsigned int ioctlType, void *cbw)
{
	int status;
	IOCtrl *ioCtrl;
	status = 0;

	ioCtrl = new IOCtrl(nodePath.toStdString());
	if (!ioCtrl->bConnected)
	{
		status = openNodeFail;
	} 
	else
	{
		status = ioCtrl->pass_through(ioctlType, cbw);
    }
    delete ioCtrl;
    return status;			
}

NVMeIO::NVMeIO(QString devNode)
{
	nodePath = devNode;
}

NVMeIO::~NVMeIO(void)
{
}
