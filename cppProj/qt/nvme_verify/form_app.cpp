#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <fstream>
#include <errno.h>
#include <QDir>
#include <stdlib.h>
#include "terminal_ctrl.h"
#include "string_list.h"
#include "utils_qt.h"
#include "memory.h"
#include "form_app.h"
#include "debug.h"
#include "console_cli.h"

#define REPORT_SIZE_LIMIT	(32*1024)
using namespace Ui;

void 
FormApp::event_connect(void)
{
	_event = new EventCtrl(this, _coreWin);	
	_event->btn_click_callback = &FormApp::on_btn_click;
	_event->action_trigger_callback = &FormApp::on_action_trigger;
	_event->index_change_callback = &FormApp::on_index_change;
	_event->text_change_callback = &FormApp::on_text_change;
	_event->value_change_callback = &FormApp::on_value_change;
	//_event->specific_connect(msgBoxBtnCancel);
}

void FormApp::load_report(QString filePath)
{
	txtReport->setText(UtilsQT::load_text_file(filePath));
}

void
FormApp::on_value_change(QObject *sender)
{
	QSlider *slider;
	slider = (QSlider *)sender;
	if(slider==silderFIORW)
	{
		unsigned int val;
		QString str;
		val = slider->value();
		str.sprintf("%d%%\nRead", 100-val);
		labFIORead->setText(str);
		str.sprintf("%d%%\nWrite", val);
		labFIOWrite->setText(str);
	}
}

void
FormApp::on_action_trigger(QObject *sender)
{
	QAction *ac;	
	QString str;
	HtmlString *html;
	ac = (QAction *)sender;	
				
	if(ac == actionCfgOpen)
	{
		load_report("config.dat");		
	}
	else
	if(ac == actionCfgSave)
	{
	}
	else
	if(ac == actionCfgUpdate)
	{
		config_environment();
	}
	else
	if(ac == actionWRCSkipC)
	{
		NVMeTask::wrcSkipCompare = actionWRCSkipC->isChecked();	
	}	
	else
	if(ac == actionExit)
	{
		qApp->quit();
	}
	else
	if(ac == actionReScanPCI)
	{
		
		std::fstream *fs;
		QString str;
		html = new HtmlString("Re-Scan PCI bus");
		str = LibExtract().get_config_value(LibExtract::KEY_PCI_REMOVE_SLOT);
		fs = new std::fstream(str.toStdString().c_str(), std::fstream::out);
	
		if(fs->good())
		{
			*fs << 1;
			*html << "write data to file " << str;	
			delete fs;
		}		
		else
		{
			*html << "cannot open file" << str;
		}
		
		*html << "\n";
		
		str = "/sys/bus/pci/rescan";
		fs = new std::fstream(str.toStdString().c_str(), std::fstream::out);
		if(fs->good())
		{
			*fs << 1;
			*html << "write data to file " << str;
			delete fs;
		}		
		else
		{
			*html << "cannot open file" << str;
		}
		
		*html << "\n";
		
		txtReport->setText(html->xml);
		delete html;
	}
	else
	if(ac==actionIdentifyController)
	{				
		struct nvme_id_ctrl *ctrl;
		ctrl = new struct nvme_id_ctrl();
		Memory<struct nvme_id_ctrl> *mem;		
		mem = new Memory<struct nvme_id_ctrl>(ctrl);		
		html = new HtmlString("Data structure decode - Identify Controller");		
		if(sizeof(struct nvme_id_ctrl) == mem->load_from(dsFileName[dsIdentifyController]))
		{
			chkBoxStoreDS->setChecked(false);
			ds_decode(ctrl, *html);			
		}
		else
		{
			*html << QString("file size not equal as data structure!!");
		}
		txtReport->setText(html->xml);
		delete html;			
		delete mem;
		delete ctrl;		
	}
	else
	if(ac==actionIdentifyNameSpace)
	{				
		struct nvme_id_ns *ns;
		ns = new struct nvme_id_ns();
		Memory<struct nvme_id_ns> *mem;		
		mem = new Memory<struct nvme_id_ns>(ns);		
		html = new HtmlString("Data structure decode - Identify NameSpace");		
		if(sizeof(struct nvme_id_ns) == mem->load_from(dsFileName[dsIdentifyNameSpace]))
		{
			chkBoxStoreDS->setChecked(false);
			ds_decode(ns, *html);			
		}
		else
		{
			*html << QString("file size not equal as data structure!!");
		}
		txtReport->setText(html->xml);
		delete html;			
		delete mem;
		delete ns;		
	}	
	else
	if(ac==actionAbout)
	{		
		QString str;	
#if 1		
		str = 
			//"<img src=\"icon_kw.ico\" alt=\"KW\" width=\"200px\" height=\"300px\">"
			"<img src=\"ngd_transparent_final.gif\" alt=\"KW\" width=\"240\" height=\"60\"><br>"
			"<strong><em>proprietary & confidential, All right reserved</em></strong>"
			"<p><strong><em>2013-2014 Ken Wang</em></strong></p>"
			"<p><a href=\"http://www.kwdata.com\">http://www.kwdata.com</a></p><br>"
			;		  
		//str += QString().sprintf( "<p align=\"right\">Build Date : %s, %s</p>", __DATE__, __TIME__ );
#endif		
		str += QString().sprintf( "<p align=\"right\">Ver : %02d.%02d.%04d</p>", MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION );
		QMessageBox *about;
		about = new QMessageBox(QMessageBox::NoIcon, "About Application", str);
		about->exec();
		delete about;
		//debug_nvme();
	}
	else
	if(ac==actionWRCIncInt)
	{
		unsigned int inc;
		if(actionWRCIncInt->isChecked())
		{
			inc = 0x01;
		}
		else
		{
			inc = 0x01010101;
		}
		NVMeTask::increaseVal = inc;
	}
	else
	if(ac==actionEnTestTools)
	{
		HtmlString *html;
		NVMeOperation nvmeOP(opAdminCommand);
		html = new HtmlString(NVMeIOCtrl::admin_op_to_string(adminCmdOp));
		nvmeOP.cbw.admin.op = nvme_admin_identify;
		nvmeOP.cbw.admin.nsid = 1;
		nvmeOP.cbw.admin.dword10 = 0;
		nvmeOP.cbw.admin.dword11 = 0;
		*html = *this << nvmeOP;
		
		delete html;
		
		toolsTab->setEnabled(true);		
		nvmeCmdTab->setEnabled(true);
		adminCmdTab->setEnabled(true);
	}
}

int
FormApp::system_call(QString cmd, QStringList arg, HtmlString &msg)
{
	return UtilsQT().system_call(cmd, arg, msg);		
}

int
FormApp::system_call(QString cmd1, QStringList arg1, QString cmd2, QStringList arg2, HtmlString &msg)
{
	return UtilsQT().system_call(cmd1, arg1, cmd2, arg2, msg);
}

void
FormApp::remove_driver(HtmlString &msg)
{
	bool keepStdOut;
	HtmlString dummy;	
	keepStdOut = radioBtnRemoveAll->isChecked();
	if(keepStdOut)
	{
		msg << "Remove nvme.ko & dnvme.ko ... ";
	}
	
	system_call("rmmod", QStringList() << "nvme.ko" << "dnvme.ko", 
				keepStdOut?msg:dummy);

	//clear in NVMeDevice
	clear();
	
	toolsTab->setEnabled(false);
}

void
FormApp::on_text_change(QObject *sender)
{	
	QLineEdit *le;
	QString name;
	le = (QLineEdit *)sender;
	name = le->objectName();
	if(name.indexOf("DW10Field")>0)	
	{		
		dword10->update();				
		if((adminCmdOp==nvme_admin_set_features)||(adminCmdOp==nvme_admin_get_features))
		{
			unsigned int currentFID;
			currentFID = dword10->dword & 0xFF;
			if(lastFID != currentFID)
			{
				dword11->clear();
				if(adminCmdOp==nvme_admin_set_features)
				{
					QString schemeTag;				
					switch(currentFID)
					{
						case NVME_FEAT_ARBITRATION:
							schemeTag = TO_STR(NVME_FEAT_ARBITRATION);
							break;
						case NVME_FEAT_POWER_MGMT:
							schemeTag = TO_STR(NVME_FEAT_POWER_MGMT);
							break;					
						case NVME_FEAT_LBA_RANGE:
							schemeTag = TO_STR(NVME_FEAT_LBA_RANGE);
							break;					
						case NVME_FEAT_TEMP_THRESH:
							schemeTag = TO_STR(NVME_FEAT_TEMP_THRESH);
							break;					
						case NVME_FEAT_ERR_RECOVERY:
							schemeTag = TO_STR(NVME_FEAT_ERR_RECOVERY);
							break;					
						case NVME_FEAT_VOLATILE_WC:
							schemeTag = TO_STR(NVME_FEAT_VOLATILE_WC);
							break;					
						case NVME_FEAT_NUM_QUEUES:
							schemeTag = TO_STR(NVME_FEAT_NUM_QUEUES);
							break;					
						case NVME_FEAT_IRQ_COALESCE:
							schemeTag = TO_STR(NVME_FEAT_IRQ_COALESCE);
							break;					
						case NVME_FEAT_IRQ_CONFIG:
							schemeTag = TO_STR(NVME_FEAT_IRQ_CONFIG);
							break;					
						case NVME_FEAT_WRITE_ATOMIC:
							schemeTag = TO_STR(NVME_FEAT_WRITE_ATOMIC);
							break;					
						case NVME_FEAT_ASYNC_EVENT:
							schemeTag = TO_STR(NVME_FEAT_ASYNC_EVENT);
							break;					
						case NVME_FEAT_PS_TRANSITION:
							schemeTag = TO_STR(NVME_FEAT_PS_TRANSITION);
							break;
					}
					
					dword11->scheme(schemeTag);
				}
				else
				if(currentFID==NVME_FEAT_LBA_RANGE)
				{
					dword11->scheme(TO_STR(NVME_FEAT_LBA_RANGE));
				}
				
				lastFID = currentFID;
			}
		}
	}
	else
	if(name.indexOf("DW11Field")>0)	
	{
		dword11->update();
	}
}

int
FormApp::console_system_call(QString cli)
{
	int i;
	_coreWin->hide();
	i=ConsoleCLI().run(cli);
	_coreWin->show();
	return i;
}

void
FormApp::on_btn_click(QObject *sender)
{
	QPushButton *btn;
	HtmlString *html;
	QString str;
	btn = (QPushButton *)sender;

	if(btn == btnReplaceDriver)
	{
		html = new HtmlString("Replace Driver");
		
		remove_driver(*html);

		if(radioBtnVerifyDriver->isChecked())
		{								
			*html << "Install verify nvmd.ko ... ";			
			//system_call("insmod", QStringList() << QString("%1/work/cppProj/nvme/myNVMe/drivers/block/nvme.ko").arg(QDir::homePath()), *html);
			system_call("insmod", QStringList() << LibExtract().get_config_value(LibExtract::KEY_VERIFY_DRIVER), *html);
			//system_task_management(true, QStringList() << "insmod" << LibExtract().get_config_value(LibExtract::KEY_KW_DRIVER));
			//while(system!=NULL);
		}
		else
		if(radioBtnComplianceDriver->isChecked())
		{			
			*html << "Install module dnvmd.ko ... ";
			//system_call("insmod", QStringList() << QString("%1/work/compliance/dnvme/dnvme.ko").arg(QDir::homePath()), *html);
			system_call("insmod", QStringList() << LibExtract().get_config_value(LibExtract::KEY_CMPL_DRIVER), *html);
			//system_task_management(true, QStringList() << "insmod" << LibExtract().get_config_value(LibExtract::KEY_CMPL_DRIVER));
			//while(system!=NULL);
		}
		else
		if(radioBtnStandardDriver->isChecked())
		{			
			int ok;
			*html << "Install standard nvmd.ko ... ";
			
			ok = system_call("modprobe", QStringList() << "nvme", *html);
			//system_task_management(true, QStringList() << "modprobe" << "nvme");
			//while(system!=NULL);			
			
			//toolsTab->setEnabled((0==systemTaskLastExitCode)?true:false);
			toolsTab->setEnabled((0==ok)?true:false);
		}
		
		txtReport->setText(html->xml);
		delete html;

	}
	else
	if(btn == btnAutoCfgScript)
	{
		radioBtnVerifyDriver->setChecked(true);
		btnReplaceDriver->click();
		btnCheckDriverVer->click();
		if(driverVer != -1)
		{
			bool cfgNVMeTest;
			cfgNVMeTest = radioBtnNVMeTest->isChecked();
			txtReport->append("Driver version check ok");

			chkBoxLockInit->setChecked(cfgNVMeTest);
			btnLock1stNVMe->click();
		
			if(bDeviceLocked)
			{
				QWidget *target;
				btnConfigAdminQueue->click();
				
				if(bAdminQueueConfigured)
				{
					target = NULL;
					if(cfgNVMeTest)
					{						
						comboBoxAdminCmd->setCurrentIndex(5);	//identify
						btnAdminCmdExecute->click();
					
						if(ready)
						{
							target = nvmeCmdTab;
						}
					}
					else
					{
						target = adminCmdTab;
					}
					
					if(target != NULL)
					{
						tabWidget->setCurrentWidget(target);
					}				
				}
			}

		}
		else
		{
			QColor color;
			color = txtReport->textColor();
			txtReport->setTextColor(QCOLOR_HI_RED);
			txtReport->append("can not get driver version");
			txtReport->setTextColor(color);
		}
	}
	else
	if(btn == btnListDriver)
	{
		html = new HtmlString("List nvme module");		
		system_call("lsmod", QStringList(), "grep", QStringList() << "nvme", *html);
		txtReport->setText(html->xml);
		delete html;
	}
	else
	if(btn == btnListNode)
	{
		html = new HtmlString("list nvme dev node");
		system_call("ls", QStringList() << "-al" << "/dev", "grep", QStringList() << "nvme", *html);
		txtReport->setText(html->xml);
		delete html;
	}	
	else
	if(btn == btnCheckDriverVer)
	{		
		int version;
		NVMeOperation nvmeOP(opGetDriverVersion);				
		html = new HtmlString("Check Driver Version");
		nvmeOP.cbw.result = &version;				
		*html = *this << nvmeOP;
		if(nvmeOP.cbw.ioStatus<0)
		{//nvme io pass through fail			
			//HtmlString::ColorStr cstr(HtmlString::colorRed, 
			//			"\nplease make sure used correct KW nvme driver, or user have root permission!!");			
			//html << cstr;
		}
		else
		{//nvme io pass through success			
			*html << QString("Driver Version = %1.%2.%3")
								.arg((version>>24)&0xFF)
								.arg((version>>16)&0xFF)
								.arg((version>>0)&0xFFFF);
								
			driverVer = version;
			/*
			if(version != IOCTL_APP_VER)
			{
				term << COLOR_HI_RED;
				term << "Driver version not match Application!!\n";
				term << COLOR_NORMAL;
			}*/
		}
		txtReport->setText(html->xml);
		delete html;		
	}
	else
	if(btn == btnScanPciDev)
	{		
		bool ok;
		AutoScanPci *list;
		NVMeOperation nvmeOP(opScanPCIDeviceList);
		html = new HtmlString("Scan PCI Device");
		list = (AutoScanPci *)new char[8192];
		nvmeOP.cbw.pci.vid = editVID->text().toUInt(&ok,16);
		nvmeOP.cbw.pci.did = editDID->text().toUInt(&ok,16);
		nvmeOP.cbw.result = (void *)list;
		*html = *this << nvmeOP;
		if(nvmeOP.cbw.ioStatus < 0)
		{			
			*html << (HtmlString::ColorStr){HtmlString::colorRed,
			"\nplease make sure used correct verify driver, or user have root permission!!"};	
		}
		else
		{			
			unsigned int i;
			HtmlString::ColorStr cstr;
			
			for(i=0;i<list->cnt;i++)
			{				
				if(list->info[i].classCode==PCI_CLASS_STORAGE_EXPRESS)
				{					
					cstr.color = HtmlString::colorGreen;
				}
				else
				{
					cstr.color = HtmlString::colorDefault;
				}				
				
				cstr.string.sprintf("%03d: VID=0x%04X, DID=0x%04X, class code = 0x%08X\n", i,  
							list->info[i].vid, list->info[i].did, list->info[i].classCode);		
													
				*html << cstr;
			}
					
			cstr.color = HtmlString::colorBlue;			
			cstr.string = QString("total %1 pci device(s) found in this platform\n").arg(list->cnt);
			*html << cstr;			
		}			

		txtReport->setText(html->xml);		
		delete (char *)list;
		delete html;
	}
	else
	if(btn == btnLockNVMe)
	{
		bool init;
		int classCode;
		NVMeOperation nvmeOP(opLockPCIDevice);
		html = new HtmlString("Lock PCI Device");
		nvmeOP.cbw.pci.vid = editVID->text().toUInt(&init,16);
		nvmeOP.cbw.pci.did = editDID->text().toUInt(&init,16);
		nvmeOP.cbw.pci.num = spinBoxTargetNumber->value();
		init = (Qt::Checked == chkBoxLockInit->checkState())?true:false;
		nvmeOP.cbw.pci.init = init;
		nvmeOP.cbw.result = &classCode;
		*html = *this << nvmeOP;
		if(nvmeOP.cbw.ioStatus < 0)
		{			
			if(lastErrno==EPERM)
			{
				HtmlString::ColorStr cstr;
				cstr.color = HtmlString::colorRed;
				cstr.string = str.sprintf("Class code 0x%08X not match NVMe class(0x%08X)!!"
									,classCode, PCI_CLASS_STORAGE_EXPRESS);
				*html << cstr;
			}			
		}
		else
		if(nvmeOP.cbw.ioStatus == 0)
		{			
			*html << "NVMe Device locked!!";
		}
				
		txtReport->setText(html->xml);	
		delete html;
	}
	else
	if(btn == btnLock1stNVMe)
	{
		bool init;
		NVMeOperation nvmeOP(opLockFirstNVMe);
		html = new HtmlString("Lock 1st NVMe Device");					
		nvmeOP.cbw.pci.vid = editVID->text().toUInt(&init,16);
		nvmeOP.cbw.pci.did = editDID->text().toUInt(&init,16);
		init = (Qt::Checked == chkBoxLockInit->checkState())?true:false;
		nvmeOP.cbw.pci.init = init;
		*html = *this << nvmeOP;
		
		txtReport->setText(html->xml);
		delete html;

	}
	if(btn == btnReadRegister)
	{		
		NVMeOperation nvmeOP(opReadCtrlRegister);
		struct nvme_bar *bar;
		bar = new struct nvme_bar();
		Memory<char> mem((char *)bar);
		mem.fill(0, sizeof(struct nvme_bar));
		html = new HtmlString("NVMe Controller Register Information");
		nvmeOP.cbw.result = bar;
		*html = *this << nvmeOP;
		if(nvmeOP.cbw.ioStatus < 0)
		{
			*html << (HtmlString::ColorStr){HtmlString::colorRed,
			"\nplease make sure used correct verify driver, or user have root permission!!"};
		}

		txtReport->setText(html->xml);
		delete bar;
		delete html;
	}
	else
	if(btn==btnConfigAdminQueue)
	{		
		NVMeOperation nvmeOP(opConfigAdminQueue);
		html = new HtmlString("Configure Admin Queue");
		*html = *this << nvmeOP;
		txtReport->setText(html->xml);
		delete html;
	}
	else
	if(btn==btnAdminCmdExecute)
	{		
		NVMeOperation nvmeOP(opAdminCommand);
		html = new HtmlString(NVMeIOCtrl::admin_op_to_string(adminCmdOp));
		nvmeOP.cbw.admin.op = adminCmdOp;
		nvmeOP.cbw.admin.nsid = editNSID->text().toUInt();
		nvmeOP.cbw.admin.dword10 = dword10->dword;
		nvmeOP.cbw.admin.dword11 = dword11->dword;
		*html = *this << nvmeOP;
		txtReport->setText(html->xml);
		delete html;
	}
	else
	if(btn==btnNVMCmdExecute)
	{
		bool issue=true;		
		//int mps;
		NVMeOperation nvmeOP(opSubmisCommand);		
		unsigned int *prp=NULL;
		unsigned int nblocks;
		html = new HtmlString(NVMeIOCtrl::submis_op_to_string(submisCmdOp));
		nblocks = editNBlocks->text().toUInt();
		#if 0
		mps = get_controller_mps(true, *html);
		if(mps<0)
		{
			*html << "get controller mps fail!!";
			issue = false;
		}
		else
		{	
		#endif
			unsigned int size;
			unsigned int basicValue;
			size = (nblocks+1)*lbaSize;
			basicValue = editBasicDataPattern->text().toUInt(&issue, 16);
			//prp = (unsigned int*)Utils().aligned_malloc(size, mps);
			prp = new unsigned int[size/sizeof(int)];
			Memory<unsigned int> *mem;
			mem = new Memory<unsigned int>(prp);
			if(submisCmdOp==nvme_cmd_write)
			{			
				//fill memory
				switch(patternType)
				{
					case NVMeTask::patternFix:
						mem->fill(basicValue, size);
						break;
					case NVMeTask::patternIncrease:
						mem->increase(basicValue, NVMeTask::increaseVal, size);
						break;
					case NVMeTask::patternDecrease:
						mem->decrease(basicValue, NVMeTask::increaseVal, size);
						break;
					case NVMeTask::patternToggle:
						mem->toggle(basicValue, size);
						break;
					default:
						issue = false;
						break;
				}
			}
			else
			if(submisCmdOp==nvme_cmd_read)
			{
				mem->fill(0, size);
			}
			
			delete mem;
		//}
		
		if(issue)
		{
			HtmlString::ColorStr cstr;
			uint64_t slba=editSLBA->text().toUInt(&issue, 16);
			nvmeOP.cbw.submis.op = submisCmdOp;
			nvmeOP.cbw.submis.slba = slba;
			nvmeOP.cbw.submis.nblocks = nblocks;
			nvmeOP.cbw.submis.prp = prp;
			
			if((submisCmdOp==nvme_cmd_read))//||(submisCmdOp==nvme_cmd_read))
			{
				cstr.color = HtmlString::colorBlue;
				cstr.string = str.sprintf("LBA = 0x%016lX, block number = %d\n", slba, nblocks+1 );
				*html << cstr;
			}
			//nvme device command issue
			*html << (QString)(*this << nvmeOP);
		}
		else
		{
			*html << "not expected result, on_btn_click btnNVMCmdExecute";
		}
		
		txtReport->setText(html->xml);
		
		if(prp!=NULL)
		{
			//Utils().aligned_free(prp);
			delete prp;
		}
		
		delete html;
		
	}
	else
	if(btn==btnReportLoad)
	{
		QFileDialog *open;
		QStringList	selected;
		QStringList filters;
		filters << "html files (*.htm *.html *.xml)"
				<< "Text files (*.txt)"
				<< "Any files (*)";		
		open = new QFileDialog();//(QWidget*)this, "Open Report File", QDir::currentPath(), QDir::AllEntries)
		open->setDirectory(QDir::current());
		open->setViewMode(QFileDialog::Detail);
		open->setAcceptMode(QFileDialog::AcceptOpen);
		open->setFileMode(QFileDialog::ExistingFile);
		open->setNameFilters(filters);
		open->exec();
		selected = open->selectedFiles();
		if(selected.size())
		{
			load_report(selected[0]);
		}
		delete open;		
	}
	else
	if(btn==btnReportSave)
	{
		QFileDialog *save;
		QStringList	selected;
		QStringList filters;
		filters << "html files (*.htm *.html *.xml)"
				//<< "Text files (*.txt)"
				//<< "Any files (*)"
				;		
		save = new QFileDialog();//(QWidget*)this, "Open Report File", QDir::currentPath(), QDir::AllEntries)		
		save->setDirectory(QDir::current());
		//save->setOption(QFileDialog::DontConfirmOverwrite, false);
		save->setViewMode(QFileDialog::Detail);
		save->setAcceptMode(QFileDialog::AcceptSave);		
		save->setFileMode(QFileDialog::AnyFile);
		save->setNameFilters(filters);
		save->setDefaultSuffix("html");
		save->exec();
		selected = save->selectedFiles();		
		if(selected.size())
		{
			QFileInfo *finfo;
			//Debug() << save->selectedNameFilter();
			finfo = new QFileInfo(selected[0]);
			if(!finfo->isDir())
			{
				StringList *slist;
				slist = new StringList();
				slist->string = txtReport->toHtml().toStdString();
				slist->save_to(selected[0].toStdString());
				delete slist;
			}
			delete finfo;			
		}

		delete save;			
	}
	else
	if(btn==btnWRCWholeDisk)
	{		
		uint64_t mlba;
		mlba = maxLBA;
		editELBA->setText(QString().sprintf("%016lX", mlba));
	}
	else
	if(btn==btnWRCExecute)
	{
		NVMeTask::Config cfg;		
		unsigned int sizeoflba;
		sizeoflba=lbaSize;
		if(sizeoflba&((1<<9)-1))
		{
			str.sprintf("Invalid NVMe device lba size(%d), please check again!!", sizeoflba);
		}
		else		
		{
			wrc_cfg_grab(cfg);
			editWRCLoopCnt->setText("0");
			wrc_task_management(true, cfg);
		}
	}
	else
	if(btn==btnIOMeterTest)
	{				
		QString ip4;
		QString iometer;		
		UtilsQT().get_eth0_ip4(ip4);
		iometer = LibExtract().get_config_value(LibExtract::KEY_TEST_IOMETER);
		iometer.sprintf("%s -i %s -m %s"
						,iometer.toLocal8Bit().data()
						,editClientIP->text().toLocal8Bit().data()
						,ip4.toLocal8Bit().data());
		console_system_call(iometer);
		//system_task_management(true, QStringList()<<iometer<<"-i"<<editClientIP->text()<<"-m"<<ip4);		
	}
	else
	if(btn==btnFIOExecute)
	{
		QString fio;
		int ok;
//fio --filename=/dev/sdb --direct=1 --rw=rw --refill_buffers --norandommap --randrepeat=0 --ioengine=libaio --bs=4k --rwmixread=100 --iodepth=16 --numjobs=16make --runtime=60 --group_reporting --name=4ktest	
		//fio.sprintf("fio --direct=1 --rw=rw --refill_buffers --norandommap --randrepeat=0 --ioengine=libaio --group_reporting
		//			--filename=/dev/sdb --bs=4k --rwmixread=100 --iodepth=16 --numjobs=16make --runtime=60  --name=4ktest");
		fio.sprintf("fio --filename=%s --direct=1 --rw=%s --refill_buffers --norandommap --randrepeat=0 --ioengine=libaio"
					" --bs=%s --rwmixread=%d --iodepth=%d --numjobs=%d --runtime=%d --size=%s --group_reporting --name=%s"
					,NVMeIOCtrl::blockDeviceNode.toLocal8Bit().data()
					//,"/dev/sdb"
					, (radioBtnFIORWSeq->isChecked())?"rw":"randrw"
					, editFIOBS->text().toLocal8Bit().data()				//block size
					, 100-silderFIORW->value()				//read percentage
					, editFIOQDepth->text().toUInt()					//io depth
					, editFIONJOB->text().toUInt()					//jobs
					, editFIORunTime->text().toUInt()				//runtime
					, editFIOSize->text().toLocal8Bit().data()		//size
					, editFIOTestName->text().toLocal8Bit().data());		//name
					
		if(chkBoxFIOSaveLog->isChecked())
		{
			fio += QString(" --output=%1.log").arg(editFIOTestName->text());
		}
				
		ok = console_system_call(fio);
		
		fio = "Execute FIO test ";
		if(ok==0)		
		{
			fio += "success";
		}
		else
		{
			fio += "fail";
		}
		
		txtReport->append(fio);
	}
	else
	if(btn==btnReportClr)
	{
		txtReport->clear();
		txtReport->setTextColor(Qt::black);
	}
	else
	if(btn==msgBoxBtnCancel)
	{			
		NVMeTask::Config *cfg;
		cfg = new NVMeTask::Config();
		wrc_task_management(false, *cfg);
		delete cfg;
	}	
}

void FormApp::wrc_cfg_grab(NVMeTask::Config &cfg)
{
	bool ok;
	cfg.slba=editSLBA->text().toUInt(&ok,16);
	cfg.elba=editELBA->text().toUInt(&ok,16);
	cfg.basePattern=editBasicDataPattern->text().toUInt(&ok,16);
	cfg.fillPattern=editMarchFill->text().toUInt(&ok,16);
	cfg.nblocksPerStep = editNBlocks->text().toUInt();
	cfg.nthreads = editNThread->text().toUInt();
	cfg.dt = patternType;
	cfg.tt = testType;
}

bool FormApp::store_ds(void)
{
	bool checked;
	checked = chkBoxStoreDS->isChecked();
	chkBoxStoreDS->setChecked(false);
	return checked;
}

void FormApp::on_device_state_change(void)
{
	QString str;
	QString style;
	
	switch(state)
	{
		case stateNone:
			str = "none";
			style = "QLabel { background-color : ; color : #777777; }";
			break;
		case stateLocked:
			str = "locded";
			style = "QLabel { background-color : ; color : #0000FF; }";
			break;		
		case stateAdmin:
			str = "admin test mode";
			style = "QLabel { background-color : ; color : #00FF00; }";
			break;		
		case stateNVM:
			str = "nvm test mode";
			style = "QLabel { background-color : ; color : #FF00FF; }";
			break;
	}
	
	stateStatus->setStyleSheet(style);
	stateStatus->setText(str);
	
	adminCmdTab->setEnabled(bAdminQueueConfigured);
	btnConfigAdminQueue->setEnabled(!bAdminQueueConfigured);		
}

void FormApp::on_ns_arrtibute_change(void)
{
	QString str;
	QString style;
	bool unlock=false;
	if(ready)
	{//identified can read attribute
		if(state==stateNVM)
		{
			unlock = true;
		}
		str.sprintf("lba size = %u, maximum lba = 0x%016lX", (unsigned int)lbaSize, (uint64_t)maxLBA );
		style = "QLabel { background-color : ; color : #FF00FF; }";
	}
	else
	{//not be identified
		str = "none";
		style = "QLabel { background-color : ; color : #777777; }";	
	}

	nsAttributeStatus->setStyleSheet(style);
	nsAttributeStatus->setText(str);
	
	nvmeCmdTab->setEnabled(unlock);
	//btnConfigAdminQueue->setEnabled(!bAdminQueueConfigured);		
}

void 
FormApp::on_index_change(QObject *sender)
{	
	QComboBox *cmbBox;
	int currentIdx;
	unsigned int value;
	
	cmbBox = (QComboBox *)sender;
	currentIdx = cmbBox->currentIndex();
	value = cmbBox->itemData(currentIdx, Qt::UserRole).toUInt();
	
	if(cmbBox == comboBoxAdminCmd)
	{
		bool bSchemeDW11 = true;
		QString schemeTag;
		grpBoxAdminOperation->setTitle(cmbBox->itemText(currentIdx));		
		//dword10->clear();
		//dword11->clear();
		schemeTag = NVMeIOCtrl::admin_op_to_string(value);
		switch(value)
		{
			case nvme_admin_delete_sq:
				//schemeTag = TO_STR(nvme_admin_delete_sq);
				break;			
			case nvme_admin_create_sq:
				//schemeTag = TO_STR(nvme_admin_create_sq);
				break;			
			case nvme_admin_get_log_page:
				//schemeTag = TO_STR(nvme_admin_get_log_page);
				break;			
			case nvme_admin_delete_cq:
				//schemeTag = TO_STR(nvme_admin_delete_cq);
				break;
			case nvme_admin_create_cq:
				//schemeTag = TO_STR(nvme_admin_create_cq);
				break;
			case nvme_admin_identify:								
				//schemeTag = TO_STR(nvme_admin_identify);
				break;
			case nvme_admin_abort_cmd:								
				//schemeTag = TO_STR(nvme_admin_abort_cmd);
				break;				
			case nvme_admin_set_features:
				//schemeTag = TO_STR(nvme_admin_set_features);
				bSchemeDW11 = false;
				break;
			case nvme_admin_get_features:
				//schemeTag = TO_STR(nvme_admin_get_features);
				dword11->clear();				
				bSchemeDW11 = false;
				break;
			case nvme_admin_async_event:
				//schemeTag = TO_STR(nvme_admin_async_event);
				break;
			case nvme_admin_activate_fw:
				//schemeTag = TO_STR(nvme_admin_activate_fw);
				break;
			case nvme_admin_download_fw:
				//schemeTag = TO_STR(nvme_admin_download_fw);
				break;
			case nvme_admin_format_nvm:
				//schemeTag = TO_STR(nvme_admin_format_nvm);
				break;
			case nvme_admin_security_send:
				//schemeTag = TO_STR(nvme_admin_security_send);
				break;
			case nvme_admin_security_recv:
				//schemeTag = TO_STR(nvme_admin_security_recv);
				break;
			case nvme_admin_invalid_test:
				//schemeTag = TO_STR(nvme_admin_invalid_test);
				break;
			default:
				schemeTag.clear();
				break;
		}
		
		if(!schemeTag.isEmpty())
		{
			dword10->scheme(schemeTag);
			if(bSchemeDW11)
			{
				dword11->scheme(schemeTag);
			}
		}
	}	
}

unsigned int
FormApp::_f_get_admin_cmd_op(void)
{
	unsigned int currentIdx;
	unsigned int value;
	currentIdx = comboBoxAdminCmd->currentIndex();
	value = comboBoxAdminCmd->itemData(currentIdx, Qt::UserRole).toUInt();
	return value;
}

unsigned int
FormApp::_f_get_submis_cmd_op(void)
{
	unsigned int op;
	if(radioBtnNVMRead->isChecked())
	{
		op = nvme_cmd_read;
	}
	else
	if(radioBtnNVMWrite->isChecked())
	{
		op = nvme_cmd_write;
	}
	else
	if(radioBtnNVMFlush->isChecked())
	{
		op = nvme_cmd_flush;
	}
	else
	{	
		op = -1;
	}
	
	return op;
}

NVMeTask::PatternType
FormApp::_f_get_pattern_type(void)
{
	NVMeTask::PatternType type;
	if(radioBtnFixPattern->isChecked())
	{
		type = NVMeTask::patternFix;
	}
	else
	if(radioBtnIncreasePattern->isChecked())
	{
		type = NVMeTask::patternIncrease;
	}
	else
	if(radioBtnDecreasePattern->isChecked())
	{
		type = NVMeTask::patternDecrease;
	}
	else
	if(radioBtnTogglePattern->isChecked())
	{
		type = NVMeTask::patternToggle;
	}	
	else
	{	
		type = (NVMeTask::PatternType)-1;
	}
	
	return type;
}

NVMeTask::TestType
FormApp::_f_get_test_type(void)
{
	NVMeTask::TestType type;
	if(radioBtnW1R1C1->isChecked())
	{
		type = NVMeTask::w1r1c1;
	}
	else
	if(radioBtnMarch->isChecked())
	{
		type = NVMeTask::march;
	}
	else
	if(radioBtnSWNLB->isChecked())
	{
		type = NVMeTask::sweepNLB;
	}
	else
	{	
		type = (NVMeTask::TestType)-1;
	}
	
	return type;
}


void
FormApp::wrc_task_management(bool en, NVMeTask::Config &cfg)
{
	if(en)
	{
		if(wrc==NULL)
		{	
			wrc = new NVMeTask(this,cfg);
			//wrc->cfg.nSteps = 100;			
			wrcProgress->setVisible(true);
			wrc->start();
			tabWidget->setEnabled(false);

			//setInterval(100);
			if(!isIntimerEvent)
			{
				setSingleShot(true);
				start(TIMER_INTERVAL_WRC_TASK);
			}			
			wrcLog.clear();
			wrc_cancel_msg_box(true);
			//msgBox->show();	
		}
		else
		{
			Debug()<<"previous wrc task not completion, please wait!!";
		}
	}	
	else
	{
		if(wrc!=NULL)
		{
			wrc->terminate();
			while(wrc->isRunning());
			wrcProgress->setVisible(false);
			wrcLog += wrc->report;
			txtReport->setPlainText(wrcLog);
			txtReport->moveCursor(QTextCursor::End);			
			delete wrc;
			wrc = NULL;
			wrc_cancel_msg_box(false);
			//msgBox->hide();
		}		
		stop();
		tabWidget->setEnabled(true);	
	}			
}

void
FormApp::wrc_cancel_msg_box(bool en)
{
	static QMessageBox *msgBox;
	//static QPoint lastPos=QMessageBox().pos();
	if(en)
	{
		msgBox = new QMessageBox();
		msgBox->setWindowTitle("WRC Task");
		msgBox->setText("Want to cancel WRC test ?");
		msgBoxBtnCancel = new QPushButton();
		msgBoxBtnCancel->setText("Cancel");
		msgBox->addButton(msgBoxBtnCancel, QMessageBox::RejectRole);
		_event->specific_connect(msgBoxBtnCancel);
		//msgBox->move(lastPos);
		msgBox->show();	
	}
	else
	{
		//lastPos = msgBox->pos();
		delete msgBoxBtnCancel;
		delete msgBox;
	}	
}

FormApp::FormApp(void) : adminCmdOp(this, &FormApp::_f_get_admin_cmd_op)
				, submisCmdOp(this, &FormApp::_f_get_submis_cmd_op)
				, patternType(this, &FormApp::_f_get_pattern_type)
				, testType(this, &FormApp::_f_get_test_type)
{	
	lastFID = -1;
	driverVer = -1;
	_coreWin = new QMainWindow();
	setupUi(_coreWin);
	
	//_coreWin->show();
	_coreWin->showMaximized();
	dword10 = new SpecDWord(grpBoxDW10, "DWORD10");
	dword11 = new SpecDWord(grpBoxDW11, "DWORD11");
	
	gui_init();	
	isIntimerEvent = false;
	wrc = NULL;
	event_connect();
	config_environment();
	TerminalCtrl() << TerminalCtrl::TC_CONFIG_NO_RESPONSE;
}

FormApp::~FormApp(void)
{
#if 0
	QObjectList *objList;	
	objList = new QObjectList();
	UtilsQT().extend_all_object(_coreWin, *objList);
	QObjectList::iterator item;
	for(item = objList->begin();item < objList->end();item++)
	{				
		QString name;
		QObject *obj;
		obj = *item;				
		//if(!obj->objectName().isEmpty())
		//if(obj->inherits("QLineEdit"))
		name = obj->objectName();
		if(   (obj!=statusBar)
			&&(obj!=menubar)
			&&(obj!=centralwidget)
			&&(obj!=splitter)
			&&(obj!=tabWidget)
		)  
		{	
			Debug()<<QString("delete Object %1").arg(name);
			
			delete obj;			
		}		
	}
	
	delete objList;		
#endif
	delete stateTitle;
	delete stateStatus;	
	delete wrcProgress;
	delete _coreWin;
	delete _event;
	delete dword10;
	delete dword11;
	delete sheetStyle;

	if(wrc!=NULL)
	{
		delete wrc;
	}	
	
	UtilsQT::flush_stdin(false);
	TerminalCtrl() << "\n" << TerminalCtrl::TC_CONFIG_STANDARD;
}

void
FormApp::gui_init(void)
{		
	//status bar insert
	stateTitle = new QLabel();
	stateStatus = new QLabel();		
	nsAttributeTitle = new QLabel();
	nsAttributeStatus = new QLabel();
	wrcProgress = new QProgressBar(statusBar);
	stateTitle->setStyleSheet("QLabel { background-color : ; color : #000000; font-weight: bold }");
	nsAttributeTitle->setStyleSheet("QLabel { background-color : ; color : #000000; font-weight: bold }");
	stateTitle->setText("NVMe Device State :");	
	nsAttributeTitle->setText("Namespace[1] Attributes :");	
	on_device_state_change();
	on_ns_arrtibute_change();
	//qApp->setWindowIcon(QIcon("icon_kw.ico"));
	
	
	statusBar->addWidget(stateTitle);
	statusBar->addWidget(stateStatus);
	statusBar->addWidget(nsAttributeTitle);
	statusBar->addWidget(nsAttributeStatus);
	statusBar->addWidget(wrcProgress);
	wrcProgress->setVisible(false);

	//Admin command Create/ Delete queue
	//NVMeIOCtrl().supported_admin_cmd_set(comboBoxAdminCmd);
	supported_admin_cmd_set(comboBoxAdminCmd);
	
	//set style
	sheetStyle = new QPlastiqueStyle();	
	QObjectList *objList;
	objList = new QObjectList();
	UtilsQT().extend_all_object(_coreWin, *objList);
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
	tabWidget->setCurrentIndex(0);
	
	toolsTab->setEnabled(false);
}

void
FormApp::config_environment(void)
{
	NVMeIOCtrl::passThroughDeviceNode = LibExtract().get_config_value(LibExtract::KEY_NODE_DEV);
	NVMeIOCtrl::blockDeviceNode = LibExtract().get_config_value(LibExtract::KEY_NODE_BLK);
}

void
FormApp::timerEvent(QTimerEvent * e)
{
	if(QEvent::Timer==e->type())
	{		
//static unsigned int i=0;
//Debug()<< QString("TimerEvent = %1").arg(i++);
		isIntimerEvent=true;
		//stop();
		_coreWin->setUpdatesEnabled(false);
		//Debug()<<QString("Event type = %1").arg(e->type());
		if(wrc!=NULL)
		{//wrc task running	
			QString report;
			int progress=wrc->progress;
			report = wrc->report;
			if(!report.isEmpty())
			{								
#if 1
				while(wrcLog.size() > REPORT_SIZE_LIMIT)
				{
					int breakpos;
					breakpos = wrcLog.indexOf("\n", 0);
					if(breakpos!=-1)
					{
						wrcLog.remove(0, breakpos+1);
					}
					else
					{
						break;
					}
				}				
#else
				if(wrcLog.size() > REPORT_SIZE_LIMIT)
				{
					wrcLog.clear();
				}			
#endif
				wrcLog += report;
				txtReport->setPlainText(wrcLog);
				txtReport->moveCursor(QTextCursor::End);
			}
			wrcProgress->setValue(progress);

			if(progress>=100)
			{
				if(progress > 100)
				{
					Debug()<<QString().sprintf("WRC Progress error, P=%d", progress);
				}
								
				NVMeTask::Config *cfg;
				unsigned int loop=0;
				bool keepGo=false;
				cfg = new NVMeTask::Config();

				if(!wrc->fail)
				{					
					if(ckbInfiniteLoop->isChecked())
					{						
						if(actionWRCLoopIncrease->isChecked())
						{
							bool ok;
							unsigned int pattern = editBasicDataPattern->text().toUInt(&ok,16);
							pattern+=NVMeTask::increaseVal;
							editBasicDataPattern->setText(QString().sprintf("%X",pattern));
						}
						loop = editWRCLoopCnt->text().toUInt()+1;
						editWRCLoopCnt->setText(QString("%1").arg(loop));
						//close last task
						wrc_task_management(false, *cfg);
						wrc_cfg_grab(*cfg);
						keepGo = true;						
						//txtReport->append(QString("\nrestart test, ----- loop %1 -------\n").arg(loop));						
					}
				}
				wrc_task_management(keepGo, *cfg);
				if(keepGo)
				{
					wrcLog += QString("\nrestart test, ----- loop %1 -------\n").arg(loop);
				}
				delete cfg;
			}			
		}

		if(wrc!=NULL)
		{
			start(TIMER_INTERVAL_WRC_TASK);
		}
		
		_coreWin->setUpdatesEnabled(true);
		isIntimerEvent=false;		
	}
	else
	{
		Debug() << QString("%1:not expected event!").arg(__func__);
	}	
}
