//#pragma output CRT_MODEL = 0

// organise sections
#pragma output CRT_ORG_CODE = 0x5DCF
#pragma output CRT_ORG_DATA = 0
#pragma output CRT_ORG_BSS = 0
#pragma output REGISTER_SP = 0xD000

#pragma output CRT_ENABLE_CLOSE = 0          // don't bother closing files on exit
#pragma output CRT_ENABLE_EIDI = 0x1        // disable interrupts at start
#pragma output CRT_ON_EXIT = 0              // jump to 0 on exit
#pragma output CLIB_STDIO_HEAP_SIZE = 0     // no stdio heap (no files)
#pragma output CLIB_EXIT_STACK_SIZE = 0     // no atexit() functions

//#pragma output CLIB_MALLOC_HEAP_SIZE = -0xC000

// create a memory block allocator with one queue
#pragma output CLIB_BALLOC_TABLE_SIZE = 1
#pragma output CRT_STACK_SIZE = 128

// These cause errors...
//#pragma output CLIB_FOPEN_MAX = -1        // no FILE* list
//#pragma output CLIB_OPEN_MAX = -1         // no fd table
