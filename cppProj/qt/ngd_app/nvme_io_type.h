#ifndef __NVME_IO_TYPE_H__
#define __NVME_IO_TYPE_H__

	typedef union {
		unsigned int d32;
		union GetFeature{
			unsigned int d32;
			struct{
				unsigned int fid:8;
				unsigned int sel:3;
				unsigned int res:21;
			};
		}getFeature;
		union SetFeature{
			unsigned int d32;
			struct{
				unsigned int fid:8;
				unsigned int res:23;
				unsigned int sv:1;
			};
		}setFeature;
		union FirmwareActivate{
			unsigned int d32;
			struct{
				unsigned int fs:3;
				unsigned int aa:2;
				unsigned int res:27;
			};
		}firmwareActivate;
		union FirmwareImageDownload{
			unsigned int d32;
			struct{
				unsigned int numd;
			};
		}firmwareImageDownload;
		union GetLogPage{
			unsigned int d32;
			struct{
				unsigned int lid:8;
				unsigned int res0:8;
				unsigned int numd:12;
				unsigned int res1:4;
			};
		}getLogPage;
	}AdminDW10;
	
	typedef union{
		unsigned int d32;
		union Arbitration{
			unsigned int d32;
			struct{
				unsigned int ab:3;
				unsigned int res:5;
				unsigned int lpw:8;
				unsigned int mpw:8;
				unsigned int hpw:8;
			};
		}arbitration;
		union PowerManagement{
			unsigned int d32;
			struct{
				unsigned int ps:5;
				unsigned int res:27;
			};
		}powerManagement;
		union LBARangeType{
			unsigned int d32;
			struct{
				unsigned int num:6;
				unsigned int res:26;
			};
		}lbaRangeType;
		union TemperatureThreshold{
			unsigned int d32;
			struct{
				unsigned int tmpth:16;
				unsigned int res:16;
			};
		}temperatureThreshold;
		union ErrorRecovery{
			unsigned int d32;
			struct{
				unsigned int tler:16;
				unsigned int res:16;
			};
		}errorRecovery;
		union VolatileWriteCache{
			unsigned int d32;
			struct{
				unsigned int wce:1;
				unsigned int res:31;
			};
		}volatileWriteCache;
		union NumberOfQueues{
			unsigned int d32;
			struct{
				unsigned int nsqa:16;
				unsigned int ncqa:16;
			};
		}numberOfQueues;
		union InterruptCoalescing{
			unsigned int d32;
			struct{
				unsigned int thr:8;
				unsigned int time:8;
				unsigned int res:16;
			};
		}interruptCoalescing;
		union InterruptVectorConfiguration{
			unsigned int d32;
			struct{
				unsigned int iv:16;
				unsigned int cd:1;
				unsigned int res:15;
			};
		}interruptVectorConfiguration;
		union WriteAtomicity{
			unsigned int d32;
			struct{
				unsigned int dn:1;
				unsigned int res:31;
			};
		}writeAtomicity;
		union AsynchronousEventConfiguration{
			unsigned int d32;
			struct{
				unsigned int smart:8;
				unsigned int res:24;
			};
		}asynchronousEventConfiguration;
		union AutonomousPowerStateTransition{
			unsigned int d32;
			struct{
				unsigned int apste:1;
				unsigned int res:30;
			};
		}autonomousPowerStateTransition;
		union FirmwareImageDownload{
			unsigned int d32;
			struct{
				unsigned int ofst;
			};
		}firmwareImageDownload;			
	}AdminDW11;

struct err_log_page{
	uint64_t errCount;
	uint16_t sqId;
	uint16_t cmdId;
	uint16_t statusField;
	uint16_t errLocation;
	uint64_t lba;
	uint32_t ns;
	uint8_t	vendor;
	uint8_t res[63-29+1];
}__attribute__((packed));

struct fwslot_log_page{
	uint8_t afi;
	uint8_t reserved[7];
	union{
		uint8_t frss[7][8];
		uint64_t frsi[7];
	};
	uint8_t res[511-64+1];
}__attribute__((packed));
#endif //ndef __NVME_IO_TYPE_H__
