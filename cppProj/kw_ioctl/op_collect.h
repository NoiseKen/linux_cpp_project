#ifndef _OP_COLLECT_H_
#define _OP_COLLECT_H_
// ********************************************************
// menu_system.c (root)
// ********************************************************
#define ROOT_NVME_MENU_SWAP_NONE		0
#define ROOT_NVME_MENU_SWAP_ADMIN		1
#define ROOT_NVME_MENU_SWAP_SUBIO		2
#define ROOT_NVME_MENU_SWAP_BOTH		3
#define ROOT_NVME_MENU_SWAP_STANDARD	4

extern void root_nvme_menu_swap(int rootSwap);

// ********************************************************
// driver_basic.c
// ********************************************************
typedef enum{
	OP_DRIVER_REPLACE=0,
	OP_DRIVER_VERSION,
	OP_DRIVER_LIST_MODULE,
	OP_DRIVER_LIST_NODE,
}ValidDriverOp;

extern void driver_basic_init(void);

// ********************************************************
// ioctl_basic.c
// ********************************************************
typedef enum{		
	OP_IOCTL_AUTO_LOCK_FIRST=0,
	OP_IOCTL_SCAN_PCI,
	OP_IOCTL_LOCK_PCI,
	OP_IOCTL_GET_NVME_CR,
	OP_IOCTRL_TEST,
}VaildIoctlOp;

extern void lock_function_enable(bool en);
extern void ioctl_basic_init(void);

// ********************************************************
// admin_basic.c
// ********************************************************
#define ADMIN_MENU_SWAP_QUEUE_INIT	0
#define ADMIN_MENU_SWAP_ALL			1
#define ADMIN_MENU_STANDARD			2

typedef enum{
	OP_ADMIN_CONFIG_QUEUE=0,
	OP_ADMIN_DELETE_IO_SUBMISSION_QUEUE,
	OP_ADMIN_CREATE_IO_SUBMISSION_QUEUE,
	OP_ADMIN_GET_LOG_PAGE,
	OP_ADMIN_DELETE_IO_COMPLETION_QUEUE,
	OP_ADMIN_CREATE_IO_COMPLETION_QUEUE,
	OP_ADMIN_IDENTIFY,
	OP_ADMIN_ABORT,	
	OP_ADMIN_GET_FEATURE,
	OP_ADMIN_SET_FEATURE,
	OP_ADMIN_ASYNCHRONOUS_EVENT_REQUEST,
	OP_ADMIN_FIRMWARE_ACTIVATE,
	OP_ADMIN_FIRMWARE_IMAGE_DOWNLOAD,
	OP_ADMIN_MAX_CNT,	
}ValidAdminOp;

extern void admin_cmd_menu_swap(int adminSwap);
extern void admin_basic_init(void);

// ********************************************************
// submis_basic.c
// ********************************************************
typedef enum{	
	OP_SUBMIS_NVME_WRITE=0,
	OP_SUBMIS_NVME_READ,
	OP_SUBMIS_NVME_FLUSH,
	OP_SUBMIS_NVME_COMPARE,
	OP_SUBMIS_MAX_CNT,	
}ValidSubmisOp;

extern void submis_basic_init(void);

// ********************************************************
// test_tools.c
// ********************************************************
typedef enum{	
	OP_TEST_IOMETER=0,
	OP_TEST_COMPLIANCE,
}ValidTestOp;

extern void test_tool_init(void);
#endif //ndef _OP_COLLECT_H_
