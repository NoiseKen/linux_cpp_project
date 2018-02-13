#include "utils_qt.h"
#include "nvme_device.h"
#include "debug.h"
#include "memory.h"
#include "nvme_task.h"
#define NXGN_REF_TAG	0x1A2B3C4D

QString 
NVMeDevice::dsFileName[dsSupported]={
		"identify_controller.ds",
		"identify_namespace.ds",
		"lba_range_type.ds",
		"power_state.ds",
};

class NVMeDevice::_info NVMeDevice::attributes={
	0, 0, 0
};

const char *NVMeDevice::answer[2]={"yes", "no"};

NVMeDevice::NVMeOperation::NVMeOperation(NVMeDevOP op)
{
	Memory<char> *mem;
	mem = new Memory<char>((char *)&cbw.op);
	mem->fill(0, sizeof(struct CBW));	
	cbw.op = op;
	delete mem;
}

NVMeDevice::NVMeOperation::~NVMeOperation(void)
{
}

void 
NVMeDevice::ds_decode(struct nvme_bar *bar, HtmlString &report)
{
	QString d;
	unsigned int i32;
	//report << d.sprintf("BAR address=0x%016lX\n", (uint64_t)bar);
	report << d.sprintf("CAP   = 0x%016lX\n", bar->cap);
	report << d.sprintf("VS    = 0x%08X\n", bar->vs);
	report << d.sprintf("IMTMS = 0x%08X\n", bar->intms);
	report << d.sprintf("IMTMC = 0x%08X\n", bar->intmc);
	report << d.sprintf("CC    = 0x%08X\n", bar->cc);
	report << d.sprintf("CSTS  = 0x%08X\n", bar->csts);
	report << d.sprintf("NSSR  = 0x%08X\n", bar->nssr);
	report << d.sprintf("AQA   = 0x%08X\n", bar->aqa);
	report << d.sprintf("ASQ   = 0x%016lX\n", bar->asq);
	report << d.sprintf("ACQ   = 0x%016lX\n", bar->acq);
	report << d.sprintf("------detail------\n");		
	i32  = bar->cap & 0xFFFF;
	report << d.sprintf("CAP.MQES = 0x%04X => maximum %d enteries\n", i32, i32+1);
	i32  = bar->vs;
	report << d.sprintf("NVMe version = %d.%d\n", (i32>>16), i32&0xFFFF);
	i32  = (bar->cc>>20)&0x0F;
	report << d.sprintf("CC.IOCQES = 0x%02X ==> I/O completion queue entry size = %d\n", i32, 1<<i32);
	i32  = (bar->cc>>16)&0x0F;
	report << d.sprintf("CC.IOSQES = 0x%02X ==> I/O submission queue entry size = %d\n", i32, 1<<i32);		
	i32  = ((bar->cc>>7)&0x0F);
	report << d.sprintf("CC.MPS = 0x%02X ==> memory page size = %d\n", i32, 1<<(i32+12));
	i32  = bar->cc&0x01;
	report << d.sprintf("CC.EN= %d\n", i32);
	i32  = (bar->csts>>0)&0x01;
	report << d.sprintf("CSTS.RDY= %d\n", i32);
}

void 
NVMeDevice::ds_decode(struct fwslot_log_page *fwslot, HtmlString &report)
{
	QString d;
	int i;
	report << d.sprintf("active fw info = 0x%02X\n", fwslot->afi);
#if 0	//if frs is string type		
	char rev[9];
	
	rev[8]=__EOS;
	for(i=1;i<8;i++)
	{		
		memcpy(rev, &fwslot->frss[i][0], 8);
		report << d.sprintf("Slot %d fw rev = %s\n", i, rev);
	}
#else	
	
	for(i=1;i<8;i++)
	{		
		report << d.sprintf("Slot %d fw rev = 0x%016lX\n", i, (unsigned long)fwslot->frsi[i]);
	}
	report << "\n";
#endif		
}

void
NVMeDevice::ds_decode(struct nvme_smart_log *smart, HtmlString &report)
{
	QString d;
	UtilsQT *utils;
	utils = new UtilsQT();
	report << d.sprintf("critical warning = 0x%02X\n", smart->critical_warning);
	report << d.sprintf("temperature = 0x%04X\n", smart->temperature);
	report << d.sprintf("avaliable spare = 0x%02X\n", smart->avail_spare);
	report << d.sprintf("avaliable spare threshold = 0x%02X\n", smart->spare_thresh);
	report << d.sprintf("percentage used = %d%%", smart->percent_used);
	report << d.sprintf("\ndata units read      = 0x");
	report << utils->printf_128((char *)smart->data_units_read);
	report << d.sprintf("\ndata units written   = 0x");
	report << utils->printf_128((char *)smart->data_units_written);
	report << d.sprintf("\nhost read commands   = 0x");
	report << utils->printf_128((char *)smart->host_reads);
	report << d.sprintf("\nhost write commands  = 0x");
	report << utils->printf_128((char *)smart->host_writes);	
	report << d.sprintf("\ncontroller busy time = 0x");
	report << utils->printf_128((char *)smart->ctrl_busy_time);
	report << d.sprintf("\npower cycles         = 0x");
	report << utils->printf_128((char *)smart->power_cycles);
	report << d.sprintf("\npower on hours       = 0x");
	report << utils->printf_128((char *)smart->power_on_hours);
	report << d.sprintf("\nunsafe shoutdowns    = 0x");
	report << utils->printf_128((char *)smart->unsafe_shutdowns);
	report << d.sprintf("\nmedia errors         = 0x");
	report << utils->printf_128((char *)smart->media_errors);
	report << d.sprintf("\n# of err log entries = 0x");
	report << utils->printf_128((char *)smart->num_err_log_entries);	
	
	report << "\n";
	delete utils;
}

void
NVMeDevice::ds_decode(struct err_log_page *err, HtmlString &report)
{
	QString d;
	report << d.sprintf("error count = %ld\n", err->errCount);
	report << d.sprintf("submission queue id = %d\n", err->sqId);
	report << d.sprintf("command id = %d\n", err->cmdId);
	report << d.sprintf("status code = 0x%04X\n", err->statusField);
	report << d.sprintf("parameter error location = 0x%04X\n", err->statusField);
	report << d.sprintf("LBA = 0x%016lX\n", err->lba);
	report << d.sprintf("vendor specific info = 0x%02X\n\n", err->vendor);		
}

void 
NVMeDevice::ds_decode(struct nvme_id_ctrl *ctrl, HtmlString &report)
{
	QString d;
	int i;
	char *local;
	local = new char[64];
	report << d.sprintf("PCI Vendor ID (VID) = 0x%04X\n", ctrl->vid);
	report << d.sprintf("PCI Subsystem Vendor ID (SSVID) = 0x%04X\n", ctrl->ssvid);
	memcpy(local, ctrl->sn, sizeof(ctrl->sn)); local[sizeof(ctrl->sn)] =__EOS;
	report << d.sprintf("Serial Number (SN) = %s\n", local);
	memcpy(local, ctrl->mn, sizeof(ctrl->mn)); local[sizeof(ctrl->mn)] =__EOS;
	report << d.sprintf("Model Number (MN) = %s\n", local);
	memcpy(local, ctrl->fr, sizeof(ctrl->fr)); local[sizeof(ctrl->fr)] =__EOS;
	report << d.sprintf("Firmware Revision (FR) = %s\n", local);
	report << d.sprintf("Recommended Arbitration Burst (RAB) = %d\n", ctrl->rab);
	//memcpy(local, ctrl->ieee, sizeof(ctrl->ieee)); local[sizeof(ctrl->ieee)] =__EOS;
	//report << d.sprintf("IEEE OUI Identifier (IEEE) = %s\n", local);
	report << d.sprintf("IEEE OUI Identifier (IEEE) = %02X-%02X-%02X\n", ctrl->ieee[0], ctrl->ieee[1], ctrl->ieee[2]);
	//printf("IEEE OUI Identifier (IEEE) = %02X%02X%02X\n", ctrl->ieee[0],ctrl->ieee[1],ctrl->ieee[2]);
	report << d.sprintf("Controller Multi-Path I/O and Namespace Sharing Capabilities (CMIC) = 0x%02X\n", ctrl->mic);
	report << d.sprintf("Maximum data transfer size = 0x%02X (%d)\n", ctrl->mdts, 1<<ctrl->mdts);
	report << d.sprintf("Controller ID = 0x%04X\n", ctrl->ctrlid);
	report << d.sprintf("Optional Admin Command Support (OACS) = 0x%04X\n", ctrl->oacs);
	report << d.sprintf("Abort Command Limit (ACL) = 0x%02X\n", ctrl->acl);
	report << d.sprintf("Asynchronous Event Request Limit (AERL) = 0x%02X\n", ctrl->aerl);
	report << d.sprintf("Firmware Updates (FRMW) = 0x%02X\n", ctrl->frmw);
	report << d.sprintf("Log Page Attributes (LPA) = 0x%02X\n", ctrl->lpa);
	report << d.sprintf("Error Log Page Entries (ELPE) = 0x%02X\n", ctrl->elpe);
	report << d.sprintf("Number of Power State Support (NPSS) = 0x%02X\n", ctrl->npss);
	report << d.sprintf("  support power state number = %d\n", ctrl->npss+1 );
	report << d.sprintf("Admin Vendor Specific Command Configuration (AVSCC) = 0x%02X\n", ctrl->avscc);
	report << d.sprintf("Autonomous Power State Transition Attributes (APSTA) = 0x%02X\n", ctrl->apsta);
	//512
	report << d.sprintf("Submission Queue Entry Size (SQES) = 0x%02X\n", ctrl->sqes);
	report << d.sprintf("  maximum=%d, required=%d\n", 1<<(ctrl->sqes>>4), 1<<(ctrl->sqes&0xF) );
	report << d.sprintf("Completion Queue Entry Size (CQES) = 0x%02X\n", ctrl->cqes);
	report << d.sprintf("  maximum=%d, required=%d\n", 1<<(ctrl->cqes>>4), 1<<(ctrl->cqes&0xF) );
	report << d.sprintf("Number of Namespaces (NN) = %d\n", ctrl->nn);
	report << d.sprintf("Optional NVM Command Support (ONCS) = 0x%04X\n", ctrl->oncs);
	i = (ctrl->oncs&(1<<0))?0:1;
	report << d.sprintf("  support command Compare ==> %s\n", answer[i]);
	i = (ctrl->oncs&(1<<1))?0:1;
	report << d.sprintf("  support command Write Uncorrectable ==> %s\n", answer[i]);
	i = (ctrl->oncs&(1<<2))?0:1;
	report << d.sprintf("  support command Dataset Management ==> %s\n", answer[i]);
	i = (ctrl->oncs&(1<<3))?0:1;
	report << d.sprintf("  support command Write Zeroes ==> %s\n", answer[i]);
	i = (ctrl->oncs&(1<<4))?0:1;
	report << d.sprintf("  support Save field in Set Feature and Select field in Get Feature ==> %s\n", answer[i]);
	i = (ctrl->oncs&(1<<5))?0:1;
	report << d.sprintf("  support reservation ==> %s\n", answer[i]);					
	report << d.sprintf("Fused Operation Support (FUSES) = 0x%04X\n", ctrl->fuses);
	i = (ctrl->fuses&(1<<0))?0:1;
	report << d.sprintf("  support fused operation ==> %s\n", answer[i]);
	report << d.sprintf("Format NVM Attributes (FNA) = 0x%02X\n", ctrl->fna);
	report << d.sprintf("Volatile Write Cache (VWC) = 0x%02X\n", ctrl->vwc);
	report << d.sprintf("Atomic Write Unit Normal (AWUN) = 0x%04X\n", ctrl->awun);
	report << d.sprintf("Atomic Write Unit Power Fail (AWUPF) = 0x%04X\n", ctrl->awupf);
	report << d.sprintf("NVM Vendor Specific Command Configuration (NVSCC) = 0x%02X\n", ctrl->nvscc);
	report << d.sprintf("Atomic Compare & Write Unit (ACWU) = 0x%04X\n", ctrl->acwu);
	report << d.sprintf("SGL Support (SGLS) = 0x%08X\n", ctrl->sgls);					
	for(i=0;i<ctrl->npss+1;i++)
	{						
		struct nvme_id_power_state *ps;
		unsigned int div;
		ps = &ctrl->psd[i];
		report << d.sprintf("------- power state [%d] description -------------\n", i);
		report << d.sprintf("Maximum Power (MP) = 0x%04X\n", ps->max_power );
		report << d.sprintf("Max Power Scale (MPS) = 0x%02X\n", (ps->flags>>0)&0x01 );
		report << d.sprintf("Non-Operational State (NOPS) = 0x%02X\n", (ps->flags>>1)&0x01 );
		report << d.sprintf("  the controller can processes I/O commands in this power state ==> %s\n",
							((ps->flags>>1)&0x01)?"false":"true" );
		div = ((ps->flags>>0)&0x01)?100:10000;
		report << d.sprintf("  the maximum power consumed in this power state = %2.4fW\n",(float)ps->max_power/div);
		report << d.sprintf("Entry Latency (ENLAT) = 0x%08X = %dms\n", ps->entry_lat, ps->entry_lat );
		report << d.sprintf("Exit Latency (EXLAT) = 0x%08X = %dms\n", ps->exit_lat, ps->exit_lat );
		report << d.sprintf("Relative Read Throughput (RRT) = 0x%02X\n", ps->read_tput);
		report << d.sprintf("Relative Read Latency (RRL) = 0x%02X\n", ps->read_lat);
		report << d.sprintf("Relative Write Throughput (RWT) = 0x%02X\n", ps->write_tput);
		report << d.sprintf("Relative Write Latency (RWL) = 0x%02X\n\n", ps->write_lat);						
	}
	delete local;
	
	if(store_ds())
	{
		int dsIdx = dsIdentifyController;
		Memory<struct nvme_id_ctrl> *mem;
		mem = new Memory<struct nvme_id_ctrl>(ctrl);
		mem->save_to(dsFileName[dsIdx], sizeof(struct nvme_id_ctrl));
		delete mem;
		
		HtmlString::ColorStr cstr;
		cstr.string = QString("(store data structure raw data to file %1)\n\n").arg(dsFileName[dsIdx]);
		cstr.color = HtmlString::colorBlue;
		report << cstr;
	}	
}

void 
NVMeDevice::ds_decode(struct nvme_id_ns *ns, HtmlString &report)
{
	const char *rpStr[4]={"Best", "Better", "Good", "Degrade" };			
	QString d;
	int i;
	unsigned int lbaSize;
	lbaSize = 1<<(ns->lbaf[ns->flbas&0x0F].ds);	
	report << d.sprintf("Namespace Size (NSZE) = 0x%016lX\n", ns->nsze);
	report << d.sprintf("  maximum LBA ==> 0x%016lX\n", ns->nsze-1 );
	report << d.sprintf("Namespace Capacity (NCAP) = 0x%016lX (%.2fGB)\n", ns->ncap ,((float)ns->nsze*lbaSize)/(1024*1024*1024));
	report << d.sprintf("Namespace Utilization (NUSE) = 0x%016lX\n", ns->nuse);
	report << d.sprintf("Namespace Features (NSFEAT) = 0x%02X\n", ns->nsfeat);
	report << d.sprintf("Number of LBA Formats (NLBAF) = 0x%02X\n", ns->nlbaf);
	report << d.sprintf("  support LBA formats ==> %d\n", ns->nlbaf+1);
	report << d.sprintf("Formatted LBA Size (FLBAS) = 0x%02X\n", ns->flbas);
	i=(ns->flbas&(1<<4))?0:1;
	report << d.sprintf("  meta data extened in end of LBA data ==> %s\n", answer[i]);
	report << d.sprintf("  which LBA format be used ==> %d\n", ns->flbas&0x0F);
	report << d.sprintf("Metadata Capabilities (MC) = 0x%02X\n", ns->mc);
	i=(ns->mc&(1<<0))?0:1;
	report << d.sprintf("  support meta data append behind LBA data ==> %s\n", answer[i] );
	i=(ns->mc&(1<<1))?0:1;
	report << d.sprintf("  support meta data store in separated buffer ==> %s\n", answer[i] );
	report << d.sprintf("End-to-end Data Protection Capabilities (DPC) = 0x%02X\n", ns->dpc);
	i=(ns->dpc&(1<<4))?0:1;
	report << d.sprintf("  support E2E data locate in last eight bytes of meta data ==> %s\n", answer[i] );
	i=(ns->dpc&(1<<3))?0:1;
	report << d.sprintf("  support E2E data locate in first eight bytes of meta data ==> %s\n", answer[i] );
	i=(ns->dpc&(1<<2))?0:1;
	report << d.sprintf("  support type 3 E2E data protection ==> %s\n", answer[i] );
	i=(ns->dpc&(1<<1))?0:1;
	report << d.sprintf("  support type 2 E2E data protection ==> %s\n", answer[i] );
	i=(ns->dpc&(1<<0))?0:1;
	report << d.sprintf("  support type 1 E2E data protection ==> %s\n", answer[i] );
	report << d.sprintf("End-to-end Data Protection Type Settings (DPS) = 0x%02X\n", ns->dps);
	i=(ns->dps&0x07);
	report << d.sprintf("  current configured E2E type ==> ");
	if(i==0)
	{
		report << d.sprintf("none\n");
	}
	else
	{
		report << d.sprintf("type %d\n", i );
	}
	report << d.sprintf("Namespace Multi-path I/O and Namespace Sharing Capabilities (NMIC) = 0x%02X\n", ns->nmic);
	report << d.sprintf("Reservation Capabilities (RESCAP) = 0x%02X\n", ns->rescap);
	report << d.sprintf("IEEE Extended Unique Identifier = 0x%016lX\n", ns->eui64);					
					
	report << d.sprintf("-------- LBAF description -----------\n");
	for(i=0;i<ns->nlbaf+1;i++)
	{
		struct nvme_lbaf *lbaf;
		lbaf = &ns->lbaf[i];						
		report << d.sprintf("-----> LBAF[%d] = 0x%08X\n", i, *(int*)lbaf);					
		report << d.sprintf("Relative Performace = 0x%02X (%s)\n", lbaf->rp, rpStr[lbaf->rp]);
		report << d.sprintf("LBA data size = %d\n", 1<<lbaf->ds);
		report << d.sprintf("Metadata size = %d\n\n", lbaf->ms);
	}
	
	if(store_ds())
	{
		int dsIdx = dsIdentifyNameSpace;
		Memory<struct nvme_id_ns> *mem;
		mem = new Memory<struct nvme_id_ns>(ns);
		mem->save_to(dsFileName[dsIdentifyNameSpace], sizeof(struct nvme_id_ns));
		delete mem;
		
		HtmlString::ColorStr cstr;
		cstr.string = QString("(store data structure raw data to file %1)\n\n").arg(dsFileName[dsIdx]);
		cstr.color = HtmlString::colorBlue;
		report << cstr;
	}	
}

void 
NVMeDevice::ds_decode(struct nvme_lba_range_type *lrt, HtmlString &report)
{
	QString d;
	report << d.sprintf("Type = 0x%02X\n", lrt->type);
	report << d.sprintf("Attributes = 0x%02X\n", lrt->attributes);
	report << d.sprintf("Start LBA = 0x%016lX\n", lrt->slba);
	report << d.sprintf("Number of logic block = 0x%016lX\n", lrt->nlb);

	report << d.sprintf("GUID = 0x");
	report << UtilsQT().printf_128((char *)lrt->guid);
	report << d.sprintf("\n");	
}


NVMeDevice::DeviceState
NVMeDevice::_f_get_current_state(void)
{
	DeviceState state;
	if(FETCH_FLAG(sq_resourced))
	{
		state = stateNVM;
	}
	else
	if(FETCH_FLAG(aq_configured))
	{
		state = stateAdmin;
	}
	else
	if(FETCH_FLAG(locked))
	{
		state = stateLocked;
	}
	else
	{
		state = stateNone;
	}
	return state;
}

bool
NVMeDevice::_f_get_identified(void)
{
	return FETCH_FLAG(identified);
}

unsigned int
NVMeDevice::_f_get_lba_size(void)
{
	return attributes.lbaSize;
}

uint64_t
NVMeDevice::_f_get_max_lba(void)
{
	return attributes.maxLBA;
}

int 
NVMeDevice::vendor_set_lba_range_type(HtmlString &msg)
{
	struct nvme_lba_range_type *lrt;
	struct nvme_admin_cmd *adm;
	Memory<char> *mem;
	int ioctlStatus;	

	lrt = new struct nvme_lba_range_type();
	mem = new Memory<char>((char *)lrt);
	mem->fill(0, sizeof(struct nvme_lba_range_type));
	delete mem;

	lrt->type=0x00;
	lrt->attributes=0x01;	
	lrt->slba=0x00;
	lrt->nlb=0x14640000;
	//lrt->guid[16];

	adm = new struct nvme_admin_cmd();
	mem = new Memory<char>((char *)adm);
	mem->fill(0, sizeof(struct nvme_admin_cmd));
	delete mem;
	
	adm->data_len = sizeof(struct nvme_lba_range_type);
	adm->addr = (uint64_t)lrt;
	
	adm->opcode = nvme_admin_set_features;
	adm->nsid = 1;
	adm->cdw10 = NVME_FEAT_LBA_RANGE;
	adm->cdw11 = 0x00;
	adm->cdw14 = NXGN_REF_TAG;
	adm->timeout_ms = 1000;
	ioctlStatus = nvme_pass_through(PASS_THROUGH_ADMIN_CMD, adm, msg);
	
	delete adm;		
	delete lrt;
	
	return ioctlStatus;
}

HtmlString 
NVMeDevice::operator<<(NVMeOperation &in)
{
	HtmlString out("NVMeDevice operator <<");	
	bool bStatusChange=false;
	int ioStatus;
	switch(in.cbw.op)
	{
		case opGetDriverVersion:
			ioStatus = get_driver_version((int *)in.cbw.result, out);
			break;
		case opReadCtrlRegister:
			ioStatus = get_controller_register((struct nvme_bar *)in.cbw.result, out);
			if(ioStatus==0)
			{
				ds_decode((struct nvme_bar *)in.cbw.result, out);
			}			
			break;
		case opScanPCIDeviceList:
			ioStatus = get_pci_dev_list(in.cbw.pci.vid, in.cbw.pci.did, out, (AutoScanPci *)in.cbw.result);
			break;
		case opLockPCIDevice:
		case opLockFirstNVMe:
			if(in.cbw.op == opLockPCIDevice)
			{
				ioStatus = lock_pci_device(in.cbw.pci.vid, in.cbw.pci.did, 
								in.cbw.pci.num, in.cbw.pci.init, (int *)in.cbw.result, out);
			}
			else
			{
				ioStatus = auto_lock_first(in.cbw.pci.vid, in.cbw.pci.did, in.cbw.pci.init, out);				
			}
			
			if(ioStatus==0)
			{
				ASSERT_FLAG(locked);
				if(in.cbw.pci.init)
				{
					ASSERT_FLAG(aq_configured);
					ASSERT_FLAG(sq_resourced);
				}
				
				bStatusChange = true;
			}
			break;
		case opConfigAdminQueue:
			ioStatus = config_admin_queue(out);
			if(ioStatus==0)
			{
				ASSERT_FLAG(aq_configured);
				bStatusChange = true;
			}
			break;
		case opAdminCommand:
			ioStatus = _issue_admin_cmd(in.cbw, out);
			break;
		case opVendorLBARangeType:
			ioStatus = vendor_set_lba_range_type(out);
			break;
		case opSubmisCommand:
			ioStatus = _issue_submis_cmd(in.cbw, out);			
			break;			
		default:
			ioStatus = -1;
			out << "not support yet\n";
			break;
	}
	
	in.cbw.ioStatus = ioStatus;
	
	if(bStatusChange)
	{
		on_device_state_change();
	}
	
	return out;
}

int
NVMeDevice::_issue_admin_cmd(NVMeOperation::CBW &cbw, HtmlString &out)
{	
	int ioStatus=0;
	
	switch(cbw.admin.op)
	{
		case nvme_admin_delete_sq:				
		case nvme_admin_create_sq:
		case nvme_admin_delete_cq:
		case nvme_admin_create_cq:
			if(state==stateAdmin)
			{
				ioStatus = admin_creat_delete_q(cbw.admin.op, cbw.admin.dword10, cbw.admin.dword11, out);
			}
			else
			{
				out << "can not send SQ related command when device in NVM state!";
			}
			break;
		case nvme_admin_get_features:
		case nvme_admin_set_features:
			ioStatus = admin_feature(cbw.admin.op, cbw.admin.nsid, cbw.admin.dword10, cbw.admin.dword11, out);									
			break;			
		case nvme_admin_identify:
			ioStatus = admin_identify(cbw.admin.nsid, cbw.admin.dword10, out);						
			break;
		case nvme_admin_abort_cmd:
			ioStatus = admin_abort(cbw.admin.dword10, out);
			break;
		case nvme_admin_get_log_page:
			ioStatus = admin_get_log_page(cbw.admin.nsid, cbw.admin.dword10, out);
			break;

		default:
			out << "not support yet command";
			break;		
	}

	return ioStatus;
}

int
NVMeDevice::_issue_submis_cmd(NVMeOperation::CBW &cbw, HtmlString &out)
{	
	int ioStatus=0;
	
	switch(cbw.submis.op)
	{		
		//nvme_cmd_write_uncor	= 0x04,
		//nvme_cmd_dsm		= 0x09,
		case nvme_cmd_write:
		case nvme_cmd_read:
		case nvme_cmd_compare:
			ioStatus = submis_wrc(cbw.submis.op, cbw.submis.slba, cbw.submis.nblocks, cbw.submis.prp, out);
			break;
		case nvme_cmd_flush:
			ioStatus = submis_flush(out);
			break;		
		default:
			out << "not support yet command";
			break;		
	}

	return ioStatus;
}


int
NVMeDevice::admin_get_log_page(unsigned int nsid, unsigned int dword10, HtmlString &msg)
{
	struct nvme_admin_cmd *adm;
	void *ptr;
	int ioctlStatus;
	
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));

	adm->opcode = nvme_admin_get_log_page;
	adm->nsid = nsid;	
	adm->data_len = 512;
	ptr = new char[adm->data_len];	
	adm->addr = (uint64_t)ptr;
	//DWORD10 = number of DWORD
	//adm->cdw10 = ((adm->data_len/4-1)<<16)|par->parameter[0];
	adm->cdw10 = dword10;
	adm->cdw14 = NXGN_REF_TAG;
	adm->timeout_ms = 1000;
	ioctlStatus = nvme_pass_through(PASS_THROUGH_ADMIN_CMD, adm, msg);
	if( ioctlStatus >= 0 )
	{
		if( ioctlStatus == 0 )
		{
			HtmlString::ColorStr cstr;
			cstr.color = HtmlString::colorBlue;			
			switch(dword10&0xFF)
			{
				case 0x01:	//Error Information
					cstr.string = "Error Information\n";
					ds_decode((struct err_log_page*)ptr, msg<<cstr);
					break;
				case 0x02:	//Smart Information
					cstr.string = "SMART/ Health Information\n";
					ds_decode((struct nvme_smart_log*)ptr, msg<<cstr);
					break;
				case 0x03:	//FW slot Information
					cstr.string = "FW slot Information\n";
					ds_decode((struct fwslot_log_page*)ptr, msg<<cstr);
					break;
			}
		
			msg << "\n";
		}

		report_completion(adm->result, msg);

	}
	
	delete (char *)ptr;
	delete adm;
	return ioctlStatus;
}

int 
NVMeDevice::admin_abort(unsigned int dword10, HtmlString &msg)
{
	struct nvme_admin_cmd *adm;
	int ioctlStatus;	

	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));
	adm->opcode = nvme_admin_abort_cmd;
	adm->cdw10 = dword10;
	adm->cdw14 = NXGN_REF_TAG;
	adm->timeout_ms = 1000;
	ioctlStatus = nvme_pass_through(PASS_THROUGH_ADMIN_CMD, adm, msg);
	if(ioctlStatus >= 0)
	{
		msg << "\n";
		report_completion(adm->result, msg);			
	}
	delete adm;
	return ioctlStatus;
}

void 
NVMeDevice::debug_nvme(void)
{
	attributes.lbaSize = 4096;
	attributes.maxLBA = (16<<20)/4096-1;
	attributes.flags = (1<<flagMax)-1;
	on_ns_arrtibute_change();	
}

int 
NVMeDevice::admin_identify(unsigned int nsid, unsigned int dword10, HtmlString &msg)
{
	struct nvme_admin_cmd *adm;
	int cns;
	int ioctlStatus;	
	union{
		void *ptr;
		struct nvme_id_ns *ns;
		struct nvme_id_ctrl *ctrl;
	}id;
	
	HtmlString::ColorStr cstr;
	
	adm = new struct nvme_admin_cmd();
	cns = dword10&0x03;
	ioctlStatus = (cns==0)?sizeof(struct nvme_id_ns):sizeof(struct nvme_id_ctrl);
	id.ptr = new char[ioctlStatus];
		
	memset(adm, 0, sizeof(struct nvme_admin_cmd));
	memset(id.ptr, 0xAA, ioctlStatus);
	adm->opcode = nvme_admin_identify;
	adm->nsid = nsid;
	adm->cdw10 = dword10;
	adm->cdw14 = NXGN_REF_TAG;
	adm->data_len = ioctlStatus;
	adm->addr = (uint64_t)id.ptr;
	ioctlStatus = nvme_pass_through(PASS_THROUGH_ADMIN_CMD, adm, msg);
	cstr.color = HtmlString::colorBlue;
	if(ioctlStatus>=0)
	{//NVME status code success
		if(ioctlStatus==0)
		{
			if(cns==0)
			{//namespace
				//msg << QString().sprintf("Namespace %d identify info\n", nsid);
				cstr.string = QString().sprintf("Namespace %d identify info\n", nsid);
				msg << cstr;
				ds_decode(id.ns, msg);
				
				ASSERT_FLAG(identified);
				attributes.lbaSize = 1<<(id.ns->lbaf[id.ns->flbas&0x0F].ds); 
				attributes.maxLBA = id.ns->nsze-1;
				on_ns_arrtibute_change();
			}
			else
			{//controller
				//msg << QString().sprintf("Controller identify info\n");
				cstr.string = QString().sprintf("Controller identify info\n");
				msg << cstr;
				ds_decode(id.ctrl, msg);		
			}
		}
									
		report_completion(adm->result, msg);			
	}
	
	delete adm;
	delete (char *)id.ptr;
	return ioctlStatus;								
}

int 
NVMeDevice::admin_creat_delete_q(unsigned int op, unsigned int dword10, unsigned int dword11, HtmlString &msg)
{
	struct nvme_admin_cmd *adm;
	void *ptr;
	int mps;
	int ioctlStatus;

	mps = get_controller_mps(true, msg);
	
	if(mps<0)
	{
		msg << "get controller mps fail!!";
		return -1;
	}
		
	ptr = NULL;
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));	
	
	if(op&0x01)
	{
		adm->data_len = 128;
		ptr = UtilsQT().aligned_malloc(adm->data_len, mps);
		adm->addr = (uint64_t)ptr;
	}

	adm->opcode = op;
	adm->cdw10 = dword10;
	adm->cdw11 = dword11;
	adm->cdw14 = NXGN_REF_TAG;
	adm->timeout_ms = 1000;
	ioctlStatus = nvme_pass_through(PASS_THROUGH_ADMIN_CMD, adm, msg);
	if(ioctlStatus >= 0)
	{
		report_completion(adm->result, msg);		
	}

	if(ptr!=NULL)
	{
		UtilsQT().aligned_free(ptr);
	}

	delete adm;
	return ioctlStatus;	
}


void
NVMeDevice::clear(void)
{
	Memory<char> *mem;
	mem = new Memory<char>((char*)&attributes);
	mem->fill(0, sizeof(attributes));
	delete mem;
	
	on_device_state_change();
	on_ns_arrtibute_change();
}

bool
NVMeDevice::_f_get_lock_flag(void)
{
	return (FETCH_FLAG(locked))?true:false;
}

bool 
NVMeDevice::_f_get_aq_configured_flag(void)
{
	return (FETCH_FLAG(aq_configured))?true:false;
}


int
NVMeDevice::admin_feature(unsigned int op, unsigned int nsid, unsigned int dword10, unsigned int dword11, HtmlString &msg)
{
	struct nvme_admin_cmd *adm;
	QString d;
	void *ds;
	int ioctlStatus;
	unsigned int d32;
	bool issue=true;

	ds = NULL;
	adm = new struct nvme_admin_cmd();
	memset(adm, 0, sizeof(struct nvme_admin_cmd));
	
	if(op == nvme_admin_get_features)
	{		
		adm->data_len = 128;
		ds = new char[adm->data_len];
		adm->addr = (uint64_t)ds;
	}
	else
	{//set feature
		unsigned int fid;
		fid = dword10 & 0xFF;
		switch(fid)
		{
			case NVME_FEAT_ARBITRATION:
			case NVME_FEAT_POWER_MGMT:
			//case NVME_FEAT_LBA_RANGE:
			case NVME_FEAT_TEMP_THRESH:
			case NVME_FEAT_ERR_RECOVERY:
			case NVME_FEAT_VOLATILE_WC:
			case NVME_FEAT_NUM_QUEUES:
			case NVME_FEAT_IRQ_COALESCE:
			case NVME_FEAT_IRQ_CONFIG:
			case NVME_FEAT_WRITE_ATOMIC:
			case NVME_FEAT_ASYNC_EVENT:
			//case NVME_FEAT_PS_TRANSITION:
				break;
			default:
				issue = false;
				ioctlStatus = 0;
				msg << d.sprintf("set feature FID 0x%02X, not support in this App,sorry", fid);
				break;
				
		}		
	}

	if(issue)
	{
		adm->opcode = op;
		adm->nsid = nsid;
		adm->cdw10 = dword10;
		adm->cdw11 = dword11;
		adm->cdw14 = NXGN_REF_TAG;
		adm->timeout_ms = 1000;
		ioctlStatus = nvme_pass_through(PASS_THROUGH_ADMIN_CMD, adm, msg);
		if(ioctlStatus >= 0)
		{//here should only for get feature to show PRP data structure
		//short data will in completion queue 'result' value
			if(ioctlStatus==0)
			{
				if(op == nvme_admin_get_features)
				{
					unsigned int fid;
					
					fid = dword10 & 0xFF;
					switch(fid)
					{
						case NVME_FEAT_ARBITRATION:
							d32 = (adm->result>>24)&0xFF;
							msg << d.sprintf("High Priority Weight = 0x%02X (HPW = %d)\n", d32, d32+1);
							d32 = (adm->result>>16)&0xFF;
							msg << d.sprintf("Medium Priority Weight = 0x%02X (MPW = %d)\n", d32, d32+1);
							d32 = (adm->result>>8)&0xFF;
							msg << d.sprintf("Low Priority Weight = 0x%02X (LPW = %d)\n", d32, d32+1);
							d32 = (adm->result>>0)&0x07;
							msg << d.sprintf("Arbitration Burst = 0x%02X (AB = %d)\n", d32, 1<<d32);
							msg << d.sprintf("ps : AB=128 mean no limit\n");
							break;
						case NVME_FEAT_POWER_MGMT:
							d32 = (adm->result>>0)&0x1F;
							msg << d.sprintf("Power State = 0x%02X (PS = %d)\n", d32, d32);
							break;
						case NVME_FEAT_LBA_RANGE:
							ds_decode((struct nvme_lba_range_type *)ds, msg);
							msg << d.sprintf("\n");
							break;
						case NVME_FEAT_TEMP_THRESH:
							d32 = (adm->result>>0)&0xFFFF;
							msg << d.sprintf("Temperature Threshold = 0x%04X (TMPTH = %d K)\n", d32, d32);
							break;
						case NVME_FEAT_ERR_RECOVERY:
							d32 = (adm->result>>0)&0xFFFF;
							msg << d.sprintf("Time Limited Error Recovery = 0x%04X (TLER = %d ms)\n", d32, d32*100);
							msg << d.sprintf("ps : TLER=0 mean no limit\n");
							break;
						case NVME_FEAT_VOLATILE_WC:
							d32 = (adm->result>>0)&0x01;
							msg << d.sprintf("Volatile Write Cache Enable = 0x%02X (WCE = %s)\n", d32, (d32)?"enable":"disable");
							break;					
						case NVME_FEAT_NUM_QUEUES:
							d32 = (adm->result>>16)&0xFFFF;
							msg << d.sprintf("Number of I/O Completion Queues Allocated = 0x%04X (NCQA = %d)\n",
												d32, d32+1);
							d32 = (adm->result>>0)&0xFFFF;
							msg << d.sprintf("Number of I/O Submission Queues Allocated = 0x%04X (NSQA = %d)\n",
												d32, d32+1);						
							break;
						case NVME_FEAT_IRQ_COALESCE:
							d32 = (adm->result>>8)&0xFF;
							msg << d.sprintf("Aggregation Time = 0x%02X (TIME = %d)\n", d32, d32);
							d32 = (adm->result>>0)&0xFF;
							msg << d.sprintf("Aggregation Threshold = 0x%04X (THR = %d)\n", d32, d32+1);
							msg << d.sprintf("ps : TIME=0 mean no delay or disabled\n");
							break;
						case NVME_FEAT_IRQ_CONFIG:
							d32 = (adm->result>>16)&0xFF;
							msg << d.sprintf("Coalescing Disable = 0x%02X (CD = %s)\n", d32, (d32)?"true":"false");
							d32 = (adm->result>>0)&0xFFFF;
							msg << d.sprintf("Interrupt Vector = 0x%04X (IV = %d)\n", d32, d32);
							break;
						case NVME_FEAT_WRITE_ATOMIC:
							d32 = (adm->result>>0)&0xFF;
							msg << d.sprintf("Disable Normal = 0x%02X (DN = %s)\n", d32, (d32)?"true":"false");
							break;						
						//NVME_FEAT_ASYNC_EVENT	= 0x0b,												
						default:
							break;
					}
				}
			}
		
			msg << "\n";
		
			report_completion(adm->result, msg);

		}	
	}
	
	if(ds!=NULL)
	{
		delete (char *)ds;
	}
	
	delete adm;

	return ioctlStatus;
}

int NVMeDevice::submis_flush(HtmlString &msg)
{
	struct nvme_user_io *io;
	int ioctlStatus;
	io = new struct nvme_user_io();
	Memory<char> *mem;
	mem = new Memory<char>((char *)io);
	mem->fill(0, sizeof(struct nvme_user_io));
	delete mem;
	
	io->opcode = nvme_cmd_flush;
	io->control = 1<<15;	//limited retry
	io->reftag = NXGN_REF_TAG;
	ioctlStatus = nvme_pass_through(PASS_THROUGH_SUBMIS_CMD, io, msg);
	if(ioctlStatus >= 0)
	{	
		report_status_code(ioctlStatus, msg);
	}

	delete io;	
	return ioctlStatus;		
}

int 
NVMeDevice::submis_wrc(unsigned int op, uint64_t slba, uint16_t nblocks, void *prp, HtmlString &msg)
{
	struct nvme_user_io *io;
	int ioctlStatus;
	io = new struct nvme_user_io();
	Memory<char> *mem;
	mem = new Memory<char>((char *)io);
	mem->fill(0, sizeof(struct nvme_user_io));
	delete mem;
	
	io->opcode = op;
	io->control = 1<<15;	//limited retry
	io->nblocks = nblocks;
	io->addr = (uint64_t)prp;
	io->slba = slba;
	io->reftag = NXGN_REF_TAG;
	ioctlStatus = nvme_pass_through(PASS_THROUGH_SUBMIS_CMD, io, msg);
	if(ioctlStatus >= 0)
	{
		if(!msg.isEmptyTitle)
		{
			if((op==nvme_cmd_read)&&(!NVMeTask::wrcSkipCompare))
			{
				unsigned int size;
				size = (nblocks+1) * lbaSize;
				Memory<unsigned int> *mem;
				mem = new Memory<unsigned int>((unsigned int*)prp);
				mem->dump(0, size, msg);
			
				msg << "\n";
			}		
		
			report_status_code(ioctlStatus, msg);
		}
	}

	delete io;	
	return ioctlStatus;		
}

int
NVMeDevice::get_controller_mps(bool powerOf2, HtmlString &msg)
{
	struct nvme_bar* bar;
	int result;
	bar = new struct nvme_bar();
	memset(bar, 0x00, sizeof(struct nvme_bar));
	result = get_controller_register(bar, msg);
	
	if(result==0)
	{
		result = (((bar->cc>>7)&0x0F)+12);
		if(powerOf2==false)
		{
			result <<= 1;
		}
	}
	delete bar;
	return result;	
}


NVMeDevice::NVMeDevice(void) : NVMeIOCtrl()
				,state(this, &NVMeDevice::_f_get_current_state)
				,ready(this, &NVMeDevice::_f_get_identified)
				,lbaSize(this, &NVMeDevice::_f_get_lba_size)
				,maxLBA(this, &NVMeDevice::_f_get_max_lba)
				,bDeviceLocked(this, &NVMeDevice::_f_get_lock_flag)
				,bAdminQueueConfigured(this, &NVMeDevice::_f_get_aq_configured_flag)
{
	clear();
}

NVMeDevice::~NVMeDevice(void)
{
}
