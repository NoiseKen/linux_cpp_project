#ifndef __NVME_IO_H__
#define __NVME_IO_H__
#include <QSet>
#include <QString>
#include "nvme.h"
#include "io_ctrl.h"
#include "nvme_io_type.h"

class NVMeIO
{
public:
	typedef class _CMDSet{
	public:
			QString descriptor;
			unsigned int value;
	}CMDSet;

	enum{
		//sucess=(0<<15),
		openNodeFail=(1<<15),
		notSupportCmd=(2<<15),
	};
	
	static CMDSet adminCmdSet[];
	static const unsigned int adminCmdCount;
	static CMDSet featureIdSet[];
	static const unsigned int featureIdCount;
	int admin_identify(struct nvme_id_ctrl *ctrl);
	int admin_identify(struct nvme_id_ns *ns, unsigned int nsid=1);
	//generic feature function
	int admin_feature(AdminDW10::SetFeature dw10, AdminDW11 dw11);
	int admin_feature(AdminDW10::GetFeature dw10, unsigned int &dw0);
	//specific feature function
	int admin_feature_lrt(bool sv, AdminDW11::LBARangeType dw11, struct nvme_lba_range_type *lrt, unsigned int nsid=1);
	int admin_feature_lrt(unsigned int sel, AdminDW11::LBARangeType dw11, struct nvme_lba_range_type *lrt, unsigned int nsid=1);
	
	int admin_generic(AdminDW10::FirmwareActivate dw10);
	int admin_generic(AdminDW10::FirmwareImageDownload dw10, AdminDW11::FirmwareImageDownload dw11, unsigned int *img);
	int admin_generic(AdminDW10::GetLogPage dw10, void *log, unsigned int nsid=1);
	static QString completion_decode(int sc, NVMeIO *instance=0);
	static void ds_decode(struct nvme_id_ctrl *ctrl, QString &report);
	static void ds_decode(struct nvme_id_ns *ns, QString &report);
	static void ds_decode(struct nvme_lba_range_type *lrt, QString &report, unsigned int cnt=1);
	static void ds_decode(struct err_log_page *err, QString &report, unsigned int cnt=1);
	static void ds_decode(struct nvme_smart_log *smart, QString &report);
	static void ds_decode(struct fwslot_log_page *fwslot, QString &report);
	NVMeIO(QString devNode);
	~NVMeIO(void);
protected:
	QString nodePath;
private:
	static QSet<unsigned int> supportedGenericSetFID;
	static QSet<unsigned int> supportedGenericGetFID;
	static const char *answer[2];
	int nvme_io_pass_through(unsigned int ioctlType, void *cbw);
#define SUPPORTED_SET_FID 	NVME_FEAT_ARBITRATION 	\
			<< NVME_FEAT_POWER_MGMT << NVME_FEAT_TEMP_THRESH << NVME_FEAT_ERR_RECOVERY << NVME_FEAT_VOLATILE_WC \
			<< NVME_FEAT_WRITE_ATOMIC << NVME_FEAT_ASYNC_EVENT
#define SUPPORTED_GET_FID 	NVME_FEAT_ARBITRATION 	\
			<< NVME_FEAT_POWER_MGMT << NVME_FEAT_TEMP_THRESH << NVME_FEAT_ERR_RECOVERY << NVME_FEAT_VOLATILE_WC \
			<< NVME_FEAT_WRITE_ATOMIC << NVME_FEAT_ASYNC_EVENT << NVME_FEAT_NUM_QUEUES <<NVME_FEAT_IRQ_COALESCE << NVME_FEAT_IRQ_CONFIG
};

#endif //ndef __NVME_IO_H__
