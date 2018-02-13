#ifndef __NVME_DEVICE_H__
#define __NVME_DEVICE_H__
#include "html_string.h"
#include "nvme_ioctrl.h"
//#include "nvme_task.h"
#include "property.h"
#include "debug.h"

class NVMeDevice : public NVMeIOCtrl
{
public:
	typedef enum{
		dsIdentifyController=0,
		dsIdentifyNameSpace,
		dsLBARangeType,
		dsPowerState,
		dsSupported,
	}DSType;

	static QString dsFileName[dsSupported];

	typedef enum{
		opGetDriverVersion=0,
		opScanPCIDeviceList,
		opLockPCIDevice,	
		opLockFirstNVMe,
		opReadCtrlRegister,
		opConfigAdminQueue,
		opAdminCommand,
		opVendorLBARangeType,
		opSubmisCommand,
		opReportIdentifyControllerDS,
		opReportIdentifyNamespaceDS,
		opReportLBARangeTypeDS,
		vaildNVMeDevOP
	}NVMeDevOP;	
	
	class NVMeOperation{
		public:		
		struct CBW{
			NVMeDevOP op;
			//union cbw			
			union {
				struct{
				bool init;
				unsigned int num;
				unsigned int vid;
				unsigned int did;
				}pci;
				struct{
				unsigned char op;
				unsigned int nsid;
				unsigned int dword10;
				unsigned int dword11;				
				}admin;
				struct{
				unsigned char op;
				uint64_t slba;
				uint16_t nblocks;				
				void *prp;	//physical region page entry
				}submis;				
			};
			
			//result back to caller
			int ioStatus;
			void *result;
		}cbw;		
		NVMeOperation(NVMeDevOP op);
		~NVMeOperation(void);			
	};
	
	typedef enum{
		stateNone=0,	//pure clean
		stateLocked, //locked a nvme device
		stateAdmin,	//admin command mode
		stateNVM,	//nvm command mode
	}DeviceState;

	property_ro<NVMeDevice, DeviceState> state;
	property_ro<NVMeDevice, bool> ready;			//true : already identified
	property_ro<NVMeDevice, unsigned int> lbaSize;	//device LBA size
	property_ro<NVMeDevice, uint64_t> maxLBA;	//device maximum LBA
	property_ro<NVMeDevice, bool> bDeviceLocked;
	property_ro<NVMeDevice, bool> bAdminQueueConfigured;
	
	HtmlString operator<<(NVMeOperation &in);
	virtual void on_device_state_change(void){};
	virtual void on_ns_arrtibute_change(void){};	
	virtual int vendor_set_lba_range_type(HtmlString &msg);
	virtual bool store_ds(void){return false;};
	void clear(void);
	
	int admin_feature(unsigned int op, unsigned int nsid, unsigned int dword10, unsigned int dword11, HtmlString &msg);	
	int admin_creat_delete_q(unsigned int op, unsigned int dword10, unsigned int dword11, HtmlString &msg);
	int admin_identify(unsigned int nsid, unsigned int dword10, HtmlString &msg);
	int admin_abort(unsigned int dword10, HtmlString &msg);
	int admin_get_log_page(unsigned int nsid, unsigned int dword10, HtmlString &msg);
	int submis_wrc(unsigned int op, uint64_t slba, uint16_t nblocks, void *prp, HtmlString &msg);
	int submis_flush(HtmlString &msg);
	void debug_nvme(void);
	NVMeDevice(void);
	~NVMeDevice(void);
protected:	
	int get_controller_mps(bool powerOf2, HtmlString &msg);
	
	void ds_decode(struct nvme_bar *bar, HtmlString &report);
	void ds_decode(struct nvme_id_ctrl *ctrl, HtmlString &report);
	void ds_decode(struct nvme_id_ns *ns, HtmlString &report);
	void ds_decode(struct nvme_lba_range_type *lrt, HtmlString &report);
	void ds_decode(struct err_log_page *err, HtmlString &report);
	void ds_decode(struct nvme_smart_log *smart, HtmlString &report);
	void ds_decode(struct fwslot_log_page *fwslot, HtmlString &report);		
private:
	#define FETCH_FLAG(f)	(attributes.flags & (1<<f))
	#define ASSERT_FLAG(f)	(attributes.flags |= (1<<f))
	enum{
		locked=0,
		aq_configured,	//admin queue need configured can issue admin command
		sq_resourced,	//submission queue need resourced can issue nvm command
		identified,		//identified namespace?		
		flagMax
	};

	static class _info{
	public:
		unsigned int flags;				
		unsigned int lbaSize;
		uint64_t maxLBA;		
	}attributes;
	DeviceState _f_get_current_state(void);
	unsigned int _f_get_lba_size(void);
	uint64_t _f_get_max_lba(void);
	bool _f_get_identified(void);
	bool _f_get_lock_flag(void);
	bool _f_get_aq_configured_flag(void);	
	int _issue_admin_cmd(NVMeOperation::CBW &cbw, HtmlString &out);
	int _issue_submis_cmd(NVMeOperation::CBW &cbw, HtmlString &out);
	
	static const char *answer[2];	
};

#endif //ndef __NVME_DEVICE_H__
