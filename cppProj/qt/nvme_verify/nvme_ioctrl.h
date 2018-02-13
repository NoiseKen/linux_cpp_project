#ifndef __NVME_IOCTRL_H__
#define __NVME_IOCTRL_H__
#include <QString>
#include <QComboBox>
#include "lib_extract.h"
#include "html_string.h"
#include "nvme_tool.h"
#include "io_ctrl.h"

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
	NVME_FEAT_PS_TRANSITION	= 0x0c,

		
	NVME_LOG_PAGE_RESERVED	= 0x00,
	NVME_LOG_PAGE_ERR_INFO	= 0x01,
	NVME_LOG_SMART_HEALTH_INFO	= 0x02,
	NVME_LOG_PAGE_FW_SLOT_INFO	= 0x03,
	NVME_LOG_PAGE_RES_NOTIFY	= 0x80,		
};

class NVMeIOCtrl
{
public:
	//gobal define	
	typedef enum{
		cmdSetAdminiCmd=0,
		cmdSetMax
	}ValidCmdSetIdx;
	
	typedef class _CMDSet{
	public:
			QString descriptor;
			unsigned int value;
	}CMDSet;

	static QString passThroughDeviceNode;
	static QString blockDeviceNode;
	int get_driver_version(int *version, HtmlString &msg);
	int get_pci_dev_list(unsigned int vid, unsigned int did, HtmlString &msg, AutoScanPci *list);
	int lock_pci_device(unsigned int vid, unsigned int did, unsigned int targetNumber
												, bool init, int *classCode, HtmlString &msg);
	int auto_lock_first(unsigned int vid, unsigned int did, bool init, HtmlString &msg);
	int get_controller_register(struct nvme_bar *bar, HtmlString &msg);	
	int config_admin_queue(HtmlString &msg);			
	void supported_admin_cmd_set(QComboBox *cbBox);
	static QString admin_op_to_string(unsigned int adminop);
	static QString submis_op_to_string(unsigned int submisop);

	NVMeIOCtrl(void);
	~NVMeIOCtrl(void);
protected:	
	static CMDSet adminCmdSet[];
	typedef enum{
		PASS_THROUGH_DEV_VENDOR=0,
		PASS_THROUGH_ADMIN_CMD,
		PASS_THROUGH_SUBMIS_CMD,
		PASS_THROUGH_TYPE_MAX
	}ValidPassThroughType;
	static unsigned int ioCmdID[PASS_THROUGH_TYPE_MAX];	
	int lastErrno;
	int nvme_pass_through(ValidPassThroughType type, void *cbw, HtmlString &msg);
	void report_completion(int status, HtmlString &report);
	void report_status_code(uint16_t sc, HtmlString &report);	
private:
/*
	static class PassThroughType{
		public:
		unsigned int t2c;
		LibExtract::ValidConfigKey t2k;
	}ptTable[PASS_THROUGH_TYPE_MAX];	*/
};

#endif //ndef __NVME_IOCTRL_H__
