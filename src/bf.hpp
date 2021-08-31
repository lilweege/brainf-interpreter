#ifndef BF_H
#define BF_H

#include <array>
#include <cassert>
#include <stdexcept>
#include <vector>

namespace bf {

	using cell_t = unsigned char;
	enum class OP : char {
		INC_PTR, // >
		DEC_PTR, // <
		INC_VAL, // +
		DEC_VAL, // -
		PUT_CHR, // .
		GET_CHR, // ,
		JMP_FWD, // [
		JMP_BCK, // ]
	};

	struct instruction {
		OP op;
		size_t jmp;
	};


	template <size_t STACK_SZ=1024>
	class stack {
		std::array<size_t, STACK_SZ> _st;
		size_t _sp = 0;
	public:
		auto push(size_t x) { _st[_sp++] = x; }
		auto pop() -> size_t { return _st[--_sp]; }
		[[nodiscard]] auto empty() const -> bool { return _sp == 0; }
		[[nodiscard]] auto full() const -> bool { return _sp >= STACK_SZ; }
	};


	template <size_t PROGRAM_SZ=65535>
	auto compile(std::string_view source) -> std::vector<instruction> {
		std::vector<instruction> program;

		if (size(source) >= PROGRAM_SZ)
			throw std::logic_error("Compile Error: Program size exceeded max program size");
		
		program.reserve(PROGRAM_SZ);
		stack call_st;

		for (char c : source) switch (c) {
			case '>': program.emplace_back(OP::INC_PTR); break;
			case '<': program.emplace_back(OP::DEC_PTR); break;
			case '+': program.emplace_back(OP::INC_VAL); break;
			case '-': program.emplace_back(OP::DEC_VAL); break;
			case '.': program.emplace_back(OP::PUT_CHR); break;
			case ',': program.emplace_back(OP::GET_CHR); break;
			case '[': {
				if (call_st.full())
					throw std::logic_error("Compile Error: Max stack size would be exceeded");
				call_st.push(size(program));
				program.emplace_back(OP::JMP_FWD);
			} break;
			case ']': {
				if (call_st.empty())
					throw std::logic_error("Compile Error: Unmatched ']'");
				size_t jmp = call_st.pop();
				program[jmp].jmp = size(program);
				program.emplace_back(OP::JMP_BCK, jmp);
			} break;
			default: break;
		}

		if (!call_st.empty())
			throw std::logic_error("Compile Error: Unmatched '['");

		return program;
	}


	template <size_t TAPE_SZ=30000>
	auto execute(const auto& program, FILE* in=stdin, FILE* out=stdout) {
		std::array<cell_t, TAPE_SZ> tape{};

		for (size_t ptr{}, pc{}, N{size(program)}; pc < N; ++pc) {
			switch (program[pc].op) {
				case OP::INC_PTR: ++ptr; break;
				case OP::DEC_PTR: --ptr; break;
				case OP::INC_VAL: ++tape[ptr]; break;
				case OP::DEC_VAL: --tape[ptr]; break;
				case OP::PUT_CHR: fputc(tape[ptr], out); break;
				case OP::GET_CHR: tape[ptr] = cell_t(fgetc(in)); break;
				case OP::JMP_FWD: if (!tape[ptr]) pc = program[pc].jmp; break;
				case OP::JMP_BCK: if ( tape[ptr]) pc = program[pc].jmp; break;
				[[unlikely]] default: assert(false);
			}

			if (ptr >= TAPE_SZ)
				throw std::runtime_error(ptr == TAPE_SZ ?
					"Runtime Error: ptr fell off right end of tape" :
					"Runtime Error: ptr fell off left end of tape");
		}
	}
} // namespace bf

#endif // BF_H