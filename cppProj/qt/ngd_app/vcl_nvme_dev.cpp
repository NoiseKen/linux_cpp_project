#include "nvme.h"
#include "vcl_nvme_dev.h"
#include "nvme_io.h"
#include "utils.h"
#include "debug.h"

unsigned int
VCLNVMeDev::_f_get_lba_size(void)
{
	QString lbaSize;
	lbaSize = itemList[ITEM_LBA_SIZE]->text();
	return lbaSize.toUInt();
}

uint64_t
VCLNVMeDev::_f_get_max_lba(void)
{
	QString maxLba;
	bool ok;
	maxLba = itemList[ITEM_MAX_LBA]->text();
	maxLba.remove(0,2);	//remove string "0x"
	return maxLba.toUInt(&ok, 16);
}

unsigned char 
VCLNVMeDev::_f_get_elpes(void)
{
	return attributes.elpes;
}

bool 
VCLNVMeDev::_f_get_dev_en(void)
{
	return (Qt::Checked==checkState())?true:false;
}

QString 
VCLNVMeDev::_f_get_dev_node(void)
{
	return text();
}

QString
VCLNVMeDev::_f_get_model_name(void)
{
	return itemList[ITEM_MODEL_NAME]->text();
}

QString
VCLNVMeDev::_f_get_serial_number(void)
{
	return itemList[ITEM_SERIAL_NUMBER]->text();
}

unsigned int
VCLNVMeDev::_f_get_nn(void)
{
	return itemList[ITEM_NS_NUMBER]->text().toUInt();
}

int
VCLNVMeDev::identify(struct nvme_id_ns *ns, unsigned int nsid)
{
	int status;
	class NVMeIO *nvmeIO;
	QString ls, ml;
	
	nvmeIO = new NVMeIO(nodePath);	
	status = nvmeIO->admin_identify(ns, nsid);
	lastIOStatus = nvmeIO->completion_decode(status, nvmeIO);

	if(status==NVME_SC_SUCCESS)
	{
		ls = QString("%1").arg(1<<(ns->lbaf[ns->flbas&0x0F].ds));
		ml = QString().sprintf("0x%016llX", ns->nsze-1);		
	}
	else
	{// NVMe status code
		ls = ml = nvmeIO->completion_decode(status, nvmeIO);
	}
	
	itemList[ITEM_LBA_SIZE]->setText(ls);
	itemList[ITEM_MAX_LBA]->setText(ml);
	
	delete nvmeIO;
	return status;
}


int
VCLNVMeDev::identify(struct nvme_id_ctrl *ctrl)
{
	int status;
	class NVMeIO *nvmeIO;
	QString mn, sn, nn;
	
	nvmeIO = new NVMeIO(nodePath);	
	status = nvmeIO->admin_identify(ctrl);
	lastIOStatus = nvmeIO->completion_decode(status, nvmeIO);

	if(status==NVME_SC_SUCCESS)
	{
		mn = fieldToQString(ctrl->mn, sizeof(ctrl->mn));
		sn = fieldToQString(ctrl->sn, sizeof(ctrl->mn));
		nn = QString().sprintf("%d", ctrl->nn);
	}
	else
	{// NVMe status code
		mn = sn = nvmeIO->completion_decode(status, nvmeIO);
	}
	
	itemList[ITEM_MODEL_NAME]->setText(mn);
	itemList[ITEM_SERIAL_NUMBER]->setText(sn);
	itemList[ITEM_NS_NUMBER]->setText(nn);	
	
	attributes.elpes = ctrl->elpe+1;
	
	delete nvmeIO;
	return status;
}

int VCLNVMeDev::get_feature(unsigned int fid, unsigned int sel, unsigned int &dw0)
{
	int status;
	class NVMeIO *nvmeIO;
	AdminDW10::GetFeature dw10;
	
	dw10.fid = fid;
	dw10.sel = sel;
	nvmeIO = new NVMeIO(nodePath);	
	status = nvmeIO->admin_feature(dw10, dw0);
	lastIOStatus = nvmeIO->completion_decode(status, nvmeIO);
	
	delete nvmeIO;
	return status;	
}

int 
VCLNVMeDev::get_feature(unsigned int num, unsigned int sel, struct nvme_lba_range_type *lrt, unsigned int nsid)
{
	int status;
	class NVMeIO *nvmeIO;	
	AdminDW11::LBARangeType dw11;

	dw11.num = num;
	nvmeIO = new NVMeIO(nodePath);
	status = nvmeIO->admin_feature_lrt(sel, dw11, lrt, nsid);
	lastIOStatus = nvmeIO->completion_decode(status, nvmeIO);	
	delete nvmeIO;
	return status;	
}

int
VCLNVMeDev::get_log_page(struct err_log_page *err, unsigned int logNum)
{
	int status;
	class NVMeIO *nvmeIO;	
	AdminDW10::GetLogPage dw10;
	dw10.lid = NVME_LOG_ERROR;
	dw10.numd = (sizeof(struct err_log_page)*logNum)/sizeof(int)-1;
	nvmeIO = new NVMeIO(nodePath);
	status = nvmeIO->admin_generic(dw10, err);
	lastIOStatus = nvmeIO->completion_decode(status, nvmeIO);
	delete nvmeIO;
	return status;	
}

int 
VCLNVMeDev::get_log_page(struct nvme_smart_log *smart, unsigned int nsid)
{
	int status;
	class NVMeIO *nvmeIO;	
	AdminDW10::GetLogPage dw10;
	dw10.lid = NVME_LOG_SMART;
	dw10.numd = sizeof(struct nvme_smart_log)/sizeof(int)-1;
	nvmeIO = new NVMeIO(nodePath);
	status = nvmeIO->admin_generic(dw10, smart, nsid);
	lastIOStatus = nvmeIO->completion_decode(status, nvmeIO);
	delete nvmeIO;
	return status;		
}

int 
VCLNVMeDev::get_log_page(struct fwslot_log_page *fwslot)
{
	int status;
	class NVMeIO *nvmeIO;	
	AdminDW10::GetLogPage dw10;
	dw10.lid = NVME_LOG_FW_SLOT;
	dw10.numd = sizeof(struct fwslot_log_page)/sizeof(int)-1;
	nvmeIO = new NVMeIO(nodePath);
	status = nvmeIO->admin_generic(dw10, fwslot);
	lastIOStatus = nvmeIO->completion_decode(status, nvmeIO);
	delete nvmeIO;
	return status;	
}

QString
VCLNVMeDev::fieldToQString(char *field, unsigned int size)
{
	QString str;
	char *tmpBuf;
	tmpBuf = new char[size+1];
	tmpBuf[size]=__EOS;
	memcpy(tmpBuf, field, size);
	str.sprintf("%s", tmpBuf);
	delete tmpBuf;
	return str;
}

void
VCLNVMeDev::parent_table_init(void)
{
	QTableWidgetItem *item;
	QTableWidget *parent;
	int devRow;
	unsigned int i;
	parent = tableWidget();
	devRow = row();
	for(i=0;i<ITEM_MAX_CNT;i++)
	{
		item = new QTableWidgetItem();
		item->setFlags(Qt::ItemIsEnabled);
		itemList << item;
		//item put from column 1 physicall, so i plus 1
		parent->setItem(devRow, COLUMN(i), item);		
	}
}

VCLNVMeDev::VCLNVMeDev(QString node) : QTableWidgetItem()
		, nodePath(this, &VCLNVMeDev::_f_get_dev_node)
		, enabled(this, &VCLNVMeDev::_f_get_dev_en)
		, lbaSize(this, &VCLNVMeDev::_f_get_lba_size)
		, maxLBA(this, &VCLNVMeDev::_f_get_max_lba)
		, modelName(this, &VCLNVMeDev::_f_get_model_name)
		, serialNumber(this, &VCLNVMeDev::_f_get_serial_number)
		, namespaceNumber(this, &VCLNVMeDev::_f_get_nn)
		, errLogPageEntries(this, &VCLNVMeDev::_f_get_elpes)
{
	setText(node);
	setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
	setCheckState(Qt::Checked);
	gui_init();
}

VCLNVMeDev::~VCLNVMeDev(void)
{
	//Debug()<<"VCL NVME DEV DESTRUCT!!\n";
	unsigned int i;
	for(i=0;i<ITEM_MAX_CNT;i++)
	{
		delete itemList[i];
	}
}

void VCLNVMeDev::progress_show(bool show)
{
	if(show)
	{
		progress = new QProgressBar();
		progress->setTextVisible(true);
		progress->setRange(0, 100);
		progress->setValue(0);
		tableWidget()->setCellWidget(row(), COLUMN(ITEM_PROGRESS), progress);
	}
	else
	{
		tableWidget()->removeCellWidget(row(), ITEM_PROGRESS+1);
	}
}

void
VCLNVMeDev::gui_init(void)
{
	memset(&attributes, 0, sizeof(struct _DevAttribute));
}
