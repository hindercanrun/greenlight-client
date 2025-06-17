#include "..\std_include.hpp"
#include "hook.hpp"

#pragma section(".text")
__declspec(allocate(".text")) BYTE utils::hook::detour::TrampolineBuffer[200 * 20] = {};
SIZE_T utils::hook::detour::TrampolineSize = 0;

namespace utils
{
	namespace hook
	{
		detour::detour()
		{
		}

		detour::detour(void* place, void* target)
		{
			this->create(place, target);
		}

		detour::detour(const size_t place, void* target)
		{
			this->create(place, target);
		}

		detour::~detour()
		{
			this->clear();
		}

		bool detour::create(void* place, void* target)
		{
			if (this->original_length_ != NULL)
			{
				std::runtime_error(symbols::va("Unable to create hook at location: %p", this->place_));
				return false;
			}

			this->place_ = (void*)place;
			this->target_ = target;

			const auto HookSize = detour::write_far_branch(NULL, this->target_, false, false);

			memcpy(this->original_instructions_, this->place_, HookSize);

			this->original_length_ = HookSize;
			this->TrampolineAddress = &detour::TrampolineBuffer[detour::TrampolineSize];

			for (size_t i = 0; i < (HookSize / 4); i++)
			{
				const auto InstructionPtr = (UINT32*)((UINT32)this->place_ + (i * 4));
				detour::TrampolineSize += detour::copy_instruction((UINT32*)&detour::TrampolineBuffer[detour::TrampolineSize], InstructionPtr);
			}

			const auto AfterBranchAddress = (void*)((UINT32)this->place_ + HookSize);
			detour::TrampolineSize += detour::write_far_branch(&detour::TrampolineBuffer[detour::TrampolineSize], AfterBranchAddress, false, true);

			detour::write_far_branch(this->place_, this->target_, false, false);

			return true;
		}

		bool detour::create(size_t place, void* target)
		{
			return this->create(reinterpret_cast<void*>(place), target);
		}

		bool detour::clear()
		{
			if (this->place_ && this->original_length_)
			{
				memcpy(this->place_, this->original_instructions_, this->original_length_);

				this->original_length_ = 0;
				this->place_ = nullptr;

				return true;
			}

			return false;
		}

		size_t detour::copy_instruction(UINT32* dest, const UINT32* src)
		{
			const auto instruction = *src;

			switch (instruction & POWERPC_OPCODE_MASK)
			{
			case POWERPC_OPCODE_B: // B BL BA BLA
			case POWERPC_OPCODE_BC: // BEQ BNE BLT BGE
				return detour::relocate_branch(dest, src);
			default:
				*dest = instruction;
				return 4;
			}
		}

		size_t detour::relocate_branch(UINT32* dest, const UINT32* src)
		{
			const auto instruction = *src;
			const auto instruction_addr = (UINT32)src;

			if (instruction & POWERPC_BRANCH_ABSOLUTE)
			{
				*dest = instruction;
				return 4;
			}

			INT32 offset_bit_size;
			INT32 offset_bit_base;
			UINT32 options;
			BYTE condition_register_bit;

			switch (instruction & POWERPC_OPCODE_MASK)
			{
			case POWERPC_OPCODE_B:
				offset_bit_size = 24;
				offset_bit_base = 2;
				options = POWERPC_BRANCH_OPTIONS_ALWAYS;
				condition_register_bit = 0;
				break;
			case POWERPC_OPCODE_BC:
				offset_bit_size = 14;
				offset_bit_base = 2;
				options = (instruction >> POWERPC_BIT32(10)) & MASK_N_BITS(5);
				condition_register_bit = (instruction >> POWERPC_BIT32(15)) & MASK_N_BITS(5);
				break;
			}

			INT32 offset = instruction & (MASK_N_BITS(offset_bit_size) << offset_bit_base);
			if (offset >> ((offset_bit_size + offset_bit_base) - 1))
			{
				offset |= ~MASK_N_BITS(offset_bit_size + offset_bit_base);
			}

			const auto addr = (void*)(INT32)(instruction_addr + offset);
			return detour::write_far_branch_ex(
				dest,
				addr,
				instruction & POWERPC_BRANCH_LINKED,
				true,
				options,
				condition_register_bit);
		}

		size_t detour::write_far_branch(
			void* dest,
			const void* target,
			bool linked,
			bool preserve_register)
		{
			return detour::write_far_branch_ex(dest, target, linked, preserve_register);
		}

		size_t detour::write_far_branch_ex(
			void* dest,
			const void* target,
			bool linked,
			bool preserve_register,
			UINT32 options,
			BYTE condition_register_bit,
			BYTE index)
		{
			const UINT32 far_asm[] =
			{
				POWERPC_LIS(index, POWERPC_HI((UINT32)target)),
				POWERPC_ORI(index, index, POWERPC_LO( (UINT32)target)),
				POWERPC_MTCTR(index),
				POWERPC_BCCTR(options, condition_register_bit, linked)
			};

			const UINT32 far_asm_preserve[] =
			{
				POWERPC_STD(index, -0x30, 1),
				POWERPC_LIS(index, POWERPC_HI((UINT32)target)),
				POWERPC_ORI(index, index, POWERPC_LO((UINT32)target)),
				POWERPC_MTCTR(index),
				POWERPC_LD(index, -0x30, 1),
				POWERPC_BCCTR(options, condition_register_bit, linked)
			};

			const auto asm_ = preserve_register ? far_asm_preserve : far_asm;
			const auto asm_size = preserve_register ? sizeof(far_asm_preserve) : sizeof(far_asm);

			if (dest)
			{
				memcpy(dest, asm_, asm_size);
			}

			return asm_size;
		}
	}
}