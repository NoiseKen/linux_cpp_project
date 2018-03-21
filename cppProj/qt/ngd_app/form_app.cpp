#include <QFileDialog>
#include <QDir>
#include "utils.h"
#include "utils_qt.h"
#include "form_app.h"
#include "string_list.h"
#include "time_log.h"
#include "memory.h"
#include "debug.h"

using namespace Ui;

void 
FormApp::event_connect(void)
{
	_event = new EventCtrl(this, vcl);
	_event->btn_click_callback = &FormApp::on_btn_click;
	_event->action_trigger_callback = &FormApp::on_action_trigger;
	_event->index_change_callback = &FormApp::on_index_change;
	//_event->text_change_callback = &FormApp::on_text_change;
	//_event->value_change_callback = &FormApp::on_value_change;
	//_event->specific_connect(msgBoxBtnCancel);
}

void 
FormApp::on_index_change(QObject *sender)
{
	QComboBox *cb;	
	int currentIdx;
	unsigned int value;
	bool en=false;
	cb = (QComboBox *)sender;
	currentIdx = cb->currentIndex();
	value = cb->itemData(currentIdx, Qt::UserRole).toUInt();	
	if(cb==cbFID)
	{
		en = (value==NVME_FEAT_LBA_RANGE)?true:false;
		grpGetLRT->setEnabled(en);		
		if(false==en)
		{
			ckbLRTSave->setChecked(en);
		}
	}
}

void 
FormApp::on_action_trigger(QObject *sender)
{
	QAction *ac;
	ac = (QAction *)sender;
	if(0==ac->objectName().indexOf("acDecode"))
	{
		QString fn;
		QString txt;
		fn = QFileDialog::getOpenFileName(0, "Select Data Structure File", QDir::currentPath(), 
					QString("Data Structure (*.ds)"), 0, 0);
		if(!fn.isEmpty())
		{
			txt.clear();
			if(ac==acDecodeIdentCtrl)
			{
				Memory<struct nvme_id_ctrl> *mem;
				mem = new Memory<struct nvme_id_ctrl>(1);
				mem->load_from(fn);				
				NVMeIO::ds_decode(mem->memory, txt);
				delete mem;
			}			
			else
			if(ac==acDecodeIdentNS)
			{
				Memory<struct nvme_id_ns> *mem;
				mem = new Memory<struct nvme_id_ns>(1);
				mem->load_from(fn);
				NVMeIO::ds_decode(mem->memory, txt);
				delete mem;
			}		
			else
			if(ac==acDecodeLRT)
			{
				unsigned int cnt;
				QFile *file;
				file = new QFile(fn);
				cnt = file->size()/sizeof(struct nvme_lba_range_type);
				delete file;
				Memory<struct nvme_lba_range_type> *mem;
				mem = new Memory<struct nvme_lba_range_type>(cnt);
				mem->load_from(fn);
				NVMeIO::ds_decode(mem->memory, txt, cnt);
				delete mem;
			}
			else
			if(ac==acDecodeLogErr)
			{
				unsigned int cnt;
				QFile *file;
				file = new QFile(fn);
				cnt = file->size()/sizeof(struct err_log_page);
				delete file;
				Memory<struct err_log_page> *mem;
				mem = new Memory<struct err_log_page>(cnt);
				mem->load_from(fn, sizeof(struct err_log_page)*cnt);
				NVMeIO::ds_decode(mem->memory, txt, cnt);
				delete mem;
			}
			else
			if(ac==acDecodeLogSmart)
			{
				Memory<struct nvme_smart_log> *mem;
				mem = new Memory<struct nvme_smart_log>(1);
				mem->load_from(fn);
				NVMeIO::ds_decode(mem->memory, txt);
				delete mem;
			}
			else	
			if(ac==acDecodeLogFWSlot)
			{
				Memory<struct fwslot_log_page> *mem;
				mem = new Memory<struct fwslot_log_page>(1);
				mem->load_from(fn);
				NVMeIO::ds_decode(mem->memory, txt);				
				delete mem;
			}
			
			*log << txt << TimeLog::Endl;
		}	
	}
	
			
}

void 
FormApp::on_btn_click(QObject *sender)
{
#define IDENTIFY_CTRL	rbIdentifyCtrl->isChecked()
#define IDENTIFY_NS		rbIdentifyNS->isChecked()	
	QAbstractButton *btn;
	QString msg;
	btn = (QAbstractButton *)sender;
	if(btn==btnRefleshDevice)
	{
		enum_nvmd_dev_list();
	}
	else
	if(btn==btnLogClr)
	{
		log->clear();
	}
	else
	if((btn==btnPrev)||(btn==btnNext))
	{
		int idx;
		int cnt;
		cnt = opStackWidget->count();
		idx = opStackWidget->currentIndex();
		idx = (cnt + idx + ((btn==btnPrev)?(-1):(1)))%cnt;
		opStackWidget->setCurrentIndex(idx);
		labOPGrpName->setText(opGrpList[idx]);
	}
	else
	if((btn==rbIdentifyCtrl)||(btn==rbIdentifyNS))
	{
		leNSID->setEnabled(IDENTIFY_NS);
	}
	else
	if(btn==btnIdentifyExecute)
	{//execute nvme identify

		msg = " Identify ";
		msg += (IDENTIFY_CTRL)?"Controller":"Namespace";
		*log << TimeLog::Time << msg << TimeLog::Endl;
		
		
		if(0==nvmeDevCnt)
		{
			*log << " - no any nvme device be emulated!!" << TimeLog::Endl;
			return;
		}
		
		int status;
		QString saveFileName;
		unsigned int devCnt = nvmeDevCnt;
		unsigned int i;
		union{
			struct nvme_id_ctrl *ctrl;
			struct nvme_id_ns *ns;
		};		
		ctrl = new struct nvme_id_ctrl();
		for(i=0;i<devCnt;i++)
		{
			if(!nvmeDev[i]->enabled)
			continue;
			
			msg.sprintf("nvmeDev[%d] (%s) : ",i, QString(nvmeDev[i]->nodePath).toLocal8Bit().data());
			
			*log << msg;
			
			if(IDENTIFY_CTRL)
			{
				status = nvmeDev[i]->identify(ctrl);
				saveFileName.sprintf("Identify_Ctrl_%02d.ds",i);		
			}
			else
			{
				status = nvmeDev[i]->identify(ns, leNSID->text().toUInt());
				saveFileName.sprintf("Identify_NS_%02d.ds",i);
			}
			
			*log << nvmeDev[i]->lastIOStatus << TimeLog::Endl;
			
			if((status==0)&&ckbIdentifySave->isChecked())
			{
				Memory<struct nvme_id_ctrl> *mem;
				mem = new Memory<struct nvme_id_ctrl>(ctrl);
				mem->save_to(saveFileName, sizeof(struct nvme_id_ctrl));
				delete mem;
				*log << " save ds file to " << saveFileName << TimeLog::Endl;
			}
		}
		
		//*log << TimeLog::Endl;
		delete ctrl;				
	}
	else
	if(btn==btnGetFeatureExecute)
	{
		int status;
		unsigned int fid;
		unsigned int sel;
		unsigned int dw0;

		fid = CB_VALUE(cbFID);
		sel = CB_VALUE(cbSEL);
		
		
		msg = " Get Feature - " + CB_TEXT(cbFID);		
		*log << TimeLog::Time << msg << TimeLog::Endl;
		
		
		if(0==nvmeDevCnt)
		{
			*log << " - no any nvme device be emulated!!" << TimeLog::Endl;
			return;
		}
		
		QString saveFileName;
		QString lrtMsg;	
		unsigned int devCnt = nvmeDevCnt;
		unsigned int i;
		lrtMsg.clear();
		for(i=0;i<devCnt;i++)
		{
			if(!nvmeDev[i]->enabled)
			continue;
			
			msg.sprintf("nvmeDev[%d] (%s) : ",i, QString(nvmeDev[i]->nodePath).toLocal8Bit().data());
			
			*log << msg;
			
			if(fid==NVME_FEAT_LBA_RANGE)
			{
				unsigned int num;
				struct nvme_lba_range_type *lrt;
				num = MINIMUM(LE_VALUE(leGetLRTNum), 63);
				lrt = new struct nvme_lba_range_type[num+1];
				status = nvmeDev[i]->get_feature(num, sel, lrt, LE_VALUE(leGetLRTNSID));				
				saveFileName.sprintf("LBA_Range_Type_%02d.ds",i);
				
				if((status==NVME_SC_SUCCESS)&&ckbLRTSave->isChecked())
				{
					Memory<struct nvme_lba_range_type> *mem;
					mem = new Memory<struct nvme_lba_range_type>(lrt);
					mem->save_to(saveFileName, sizeof(struct nvme_lba_range_type)*(num+1));
					delete mem;
					lrtMsg = QString(" save ds file to %1").arg(saveFileName);
					
					//*log << " save ds file to " << saveFileName << TimeLog::Endl;
				}
				
				delete lrt;
			}
			else
			{
				status = nvmeDev[i]->get_feature(fid, sel, dw0);
			}
			
			*log << nvmeDev[i]->lastIOStatus;
						
			if((fid!=NVME_FEAT_LBA_RANGE)&&(status==NVME_SC_SUCCESS))
			{
				*log << " -- DW0=" << QString().sprintf("0x%08X", dw0);
			}
			
			if(!lrtMsg.isEmpty())
			{
				*log << TimeLog::Endl << lrtMsg << TimeLog::Endl;
			}
			
			*log << TimeLog::Endl;			
		}			
	}
	else
	if(btn==btnGetLogPageExecute)
	{
		int status;
		unsigned int lid;

		lid = CB_VALUE(cbLID);
		
		msg = " Get Log Page - " + CB_TEXT(cbLID);		
		*log << TimeLog::Time << msg << TimeLog::Endl;		
		
		if(0==nvmeDevCnt)
		{
			*log << " - no any nvme device be emulated!!" << TimeLog::Endl;
			return;
		}
		
		QString saveFileName;
		union{			
			struct err_log_page *err;
			struct nvme_smart_log *smart;
			struct fwslot_log_page *fwslot;
		};
		unsigned int devCnt = nvmeDevCnt;
		unsigned int i;				

		for(i=0;i<devCnt;i++)
		{
			if(!nvmeDev[i]->enabled)
			continue;
			
			msg.sprintf("nvmeDev[%d] (%s) : ",i, QString(nvmeDev[i]->nodePath).toLocal8Bit().data());
			
			*log << msg;

			switch(lid)		
			{
				case NVME_LOG_ERROR:
					err = new struct err_log_page[nvmeDev[i]->errLogPageEntries];
					status = nvmeDev[i]->get_log_page(err, nvmeDev[i]->errLogPageEntries);
					saveFileName.sprintf("Log_Page_Error_%02d.ds",i);
					break;
				case NVME_LOG_SMART:
					smart = new struct nvme_smart_log();
					status = nvmeDev[i]->get_log_page(smart, LE_VALUE(leGetLogNSID));
					saveFileName.sprintf("Log_Page_Smart_%02d.ds",i);
					break;			
				case NVME_LOG_FW_SLOT:
					fwslot = new struct fwslot_log_page();
					status = nvmeDev[i]->get_log_page(fwslot);
					saveFileName.sprintf("Log_Page_FWSlot_%02d.ds",i);					
					break;						
				default:
					*log << QString().sprintf(" - invalid LID (0x%02X)!!", lid) << TimeLog::Endl;
					return;			
			}
			
			*log << nvmeDev[i]->lastIOStatus << TimeLog::Endl;			
			
			if((status==0)&&ckbLogPageSave->isChecked())
			{				
				if(lid==NVME_LOG_ERROR)
				{
					Memory<struct err_log_page> *mem;
					mem = new Memory<struct err_log_page>(err);
					mem->save_to(saveFileName, sizeof(struct err_log_page)*nvmeDev[i]->errLogPageEntries);
					delete err;
					delete mem;
				}
				else
				if(lid==NVME_LOG_SMART)
				{
					Memory<struct nvme_smart_log> *mem;
					mem = new Memory<struct nvme_smart_log>(smart);
					mem->save_to(saveFileName, sizeof(struct nvme_smart_log));
					delete smart;
					delete mem;
				}
				else
				if(lid==NVME_LOG_FW_SLOT)
				{
					Memory<struct fwslot_log_page> *mem;
					mem = new Memory<struct fwslot_log_page>(fwslot);
					mem->save_to(saveFileName, sizeof(struct fwslot_log_page));
					delete fwslot;
					delete mem;
				}
				
				*log << " save ds file to " << saveFileName << TimeLog::Endl;
			}
		}
	}	
	/*
	else
	if(btn==pushButton_3)
	{
		static bool fl=true;
		nvmeDev[0]->progress_show(fl);
		fl^=true;
	}*/
	
#undef IDENTIFY_CTRL	//rbIdentifyCtrl->isChecked()
#undef IDENTIFY_NS		//rbIdentifyNS->isChecked()
}
#if 0
void
FormApp::on_index_change(QObject *sender)
{
	QRadioButton *rb;
	rb = (QRadioButton *)sender;
	if(rb==rbIdentifyCtrl)
	{
		ledIdentifyFile->setText("IdentifyCtrl.ds");
	}
	else
	if(rb==rbIdentifyNS)
	{
		ledIdentifyFile->setText("IdentifyNS.ds");
	}	
}
#endif
FormApp::FormApp(void) : nvmeDev(this, &FormApp::_f_get_nvme_dev)
					,nvmeDevCnt(this, &FormApp::_f_get_nvme_dev_cnt)
{	
	vcl = new QMainWindow();
	setupUi(vcl);
	vcl->showMaximized();

	event_connect();
	
	opGrpList << "NVMe Basic Command" << "KW Vendor Command";
	gui_init();
}

FormApp::~FormApp(void)
{
	clr_nvme_dev_list();
	delete vcl;
	delete sheetStyle;
	delete _event;
}

class VCLNVMeDev * 
FormApp::_f_get_nvme_dev(unsigned int idx)
{
	unsigned int exist;
	class VCLNVMeDev *result=NULL;
	exist = nvmeDevCnt;
	if(idx<exist)
	{
		result = (class VCLNVMeDev *)(nvmeDevTable->item(idx, 0));
	}
	return result;
}

unsigned int
FormApp::_f_get_nvme_dev_cnt(void)
{
	return nvmeDevTable->rowCount();
}


void
FormApp::clr_nvme_dev_list(void)
{
	unsigned int cnt;
	class VCLNVMeDev *dev;
	cnt = nvmeDevCnt;
		
	for(unsigned int i=0;i<cnt;i++)
	{
		dev = nvmeDev[i];
		delete dev;
	}
	
	nvmeDevTable->setRowCount(0);
}

void
FormApp::enum_nvmd_dev_list(void)
{	
	StringList list;
	unsigned int i, cnt;
	
	clr_nvme_dev_list();
	*log << TimeLog::Time << " Enumerate NVMe Device : ";
	UtilsQT().system_call("ls /dev/nvme?", list);
	cnt = list.size();
	for(i=0;i<cnt;i++)
	{		
		class VCLNVMeDev *dev;
		nvmeDevTable->insertRow(i);		
		dev = new VCLNVMeDev(QString().fromStdString(list[i]));
		nvmeDevTable->setItem(i,0,dev);
		dev->parent_table_init();
		
		struct nvme_id_ctrl *ctrl;
		//struct nvme_id_ns *ns;
		ctrl = new struct nvme_id_ctrl();
		//ns = new struct nvme_id_ns();
		dev->identify(ctrl);
		//dev->identify(ns);
		delete ctrl;
		//delete ns;
	}
	
	*log << QString("%1 device(s) found").arg(cnt) << TimeLog::Endl;	
}

void
FormApp::gui_init(void)
{		
	//set style
	sheetStyle = new QPlastiqueStyle();	
	QObjectList *objList;
	objList = new QObjectList();
	UtilsQT().extend_all_object(vcl, *objList);
	QObjectList::iterator item;
	for(item = objList->begin();item < objList->end();item++)
	{				
		QObject *obj;
		obj = *item;
		if(!obj->objectName().isEmpty())
		{
			if(obj->inherits("QGroupBox"))
			{
				((QGroupBox*)obj)->setStyle(sheetStyle);
			}
			/*
			else
			if(obj->inherits("QFrame"))
			{
				((QFrame*)obj)->setStyle(sheetStyle);
			}*/
		}
	}
	delete objList;

	opStackWidget->setCurrentIndex(0);
	rbIdentifyCtrl->click();
	//nvmeOperation->layout()->setAlignment(Qt::AlignTop);

	//FID combo box
	int cnt;
	NVMeIO::CMDSet *set;
	set = NVMeIO::featureIdSet;
	cnt = NVMeIO::featureIdCount;
	cbFID->clear();
	cbFID->setMaxCount(cnt);	
	for(int i=0; i<cnt; i++)
	{			
		cbFID->addItem(set->descriptor, set->value );
		set++;
	}
	
	cbFID->setCurrentIndex(0);
	//SEL combo box
	cbSEL->addItem("Current (0x00)", 0);
	cbSEL->addItem("Default (0x01)", 1);
	cbSEL->addItem("Saved (0x02)", 2);
	
	//LID combo box
	cbLID->addItem("Error Information (0x01)", 1);
	cbLID->addItem("SMART / Health Information (0x02)", 2);
	cbLID->addItem("Firmware Slot Information (0x03)", 3);
	
}
