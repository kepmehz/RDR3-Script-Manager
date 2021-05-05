#pragma once

namespace rage
{
	enum eThreadState
	{
		idle,
		running,
		killed,
		unk3,
		unk4,
	};

	class scrThreadContext
	{
	public:
		uint32_t m_threadId;                  // 0x0000
		uint32_t m_scriptHash;                // 0x0004
		eThreadState m_threadState;           // 0x0008
		uint32_t m_instructionPtr;            // 0x000C
		uint32_t m_framePtr;                  // 0x0010
		uint32_t m_stackPointer;              // 0x0014
		uint32_t m_timerA;                    // 0x0018
		uint32_t m_timerB;                    // 0x001C
		uint32_t m_timerC;                    // 0x0020
		uint32_t m_unk1;                      // 0x0024
		uint32_t m_unk2;                      // 0x0028 
		char pad_002C[0x24];                  // 0x002C  
		uint32_t m_stackSize;                 // 0x0050
		char pad_0054[0x65C];                 // 0x0054
	};

	class scrThread
	{
	public:
		virtual ~scrThread() = default;
		virtual eThreadState Reset(uint32_t programId, void const* args, int argsCount) = 0;
		virtual eThreadState Run(int opsCount) = 0;
		virtual eThreadState Update(int opsCount) = 0;
		virtual void Kill() = 0;

		scrThreadContext m_context;                // 0x0008
		uintptr_t m_stack;                         // 0x06B8
		char pad_06C0[0x4];                        // 0x06C0
		uint32_t m_opsCount;                       // 0x06C4
		char pad_06C8[0x8];                        // 0x06C8
		char* m_exitMessage;                       // 0x06D0
		uint32_t m_scriptHash;                     // 0x06D8
		char pad_06DC[0x4];                        // 0x06DC
	};
	static_assert(sizeof(scrThread) == 0x6E0, "rage::scrThread is not properly sized");
}