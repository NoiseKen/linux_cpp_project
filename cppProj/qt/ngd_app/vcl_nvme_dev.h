#ifndef __VCL_NVME_DEV_H__
#define __VCL_NVME_DEV_H__
#include <QTableWidgetItem>
#include <QProgressBar>
#include "nvme_io.h"
#include "property.h"

class VCLNVMeDev : public QTableWidgetItem
{
	//Q_OBJECT
public:
	property_ro<VCLNVMeDev, QString> nodePath;
	property_ro<VCLNVMeDev, bool> enabled;
	property_ro<VCLNVMeDev, unsigned int> lbaSize;
	property_ro<VCLNVMeDev, uint64_t> maxLBA;
	property_ro<VCLNVMeDev, QString> modelName;	
	property_ro<VCLNVMeDev, QString> serialNumber;
	property_ro<VCLNVMeDev, unsigned int> namespaceNumber;
	property_ro<VCLNVMeDev, unsigned char> errLogPageEntries;
	QString lastIOStatus;
	int identify(struct nvme_id_ctrl *ctrl);
	int identify(struct nvme_id_ns *ns, unsigned int nsid=1);
	int get_feature(unsigned int fid, unsigned int sel, unsigned int &dw0);
	int get_feature(unsigned int num, unsigned int sel, struct nvme_lba_range_type *lrt, unsigned int nsid=1);
	int get_log_page(struct err_log_page *err, unsigned int logNum=1);
	int get_log_page(struct nvme_smart_log *smart, unsigned int nsid=1);
	int get_log_page(struct fwslot_log_page *fwslot);
	
	void parent_table_init(void);
	VCLNVMeDev(QString node);
	~VCLNVMeDev(void);
protected:
	virtual QString _f_get_dev_node(void);
private:
	#define COLUMN(item)	(item+1)
	enum{
		ITEM_MODEL_NAME=0,
		ITEM_SERIAL_NUMBER,
		ITEM_NS_NUMBER,
		ITEM_LBA_SIZE,
		ITEM_MAX_LBA,
		ITEM_PROGRESS,
		ITEM_MAX_CNT
	};
	
	struct _DevAttribute{
		unsigned char elpes;
	}attributes;
	
	QList<QTableWidgetItem *> itemList;
	QProgressBar *progress;
	void progress_show(bool show);
	unsigned int _f_get_lba_size(void);
	uint64_t _f_get_max_lba(void);
	bool _f_get_dev_en(void);
	QString _f_get_model_name(void);
	QString _f_get_serial_number(void);
	unsigned int _f_get_nn(void);
	unsigned char _f_get_elpes(void);
	QString fieldToQString(char *field, unsigned int size);		
	void gui_init(void);
};

#endif //ndef __VCL_NVME_DEV_H__
