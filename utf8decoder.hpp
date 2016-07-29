//@	{
//@	 "targets":[{"name":"utf8decoder.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"utf8decoder.o","rel":"implementation"}]
//@	}

#ifndef IDEA_UTF8DECODER_HPP
#define IDEA_UTF8DECODER_HPP

#include <cstdint>

namespace Idea
	{
	class UTF8Decoder
		{
		public:
			UTF8Decoder(const uint8_t* begin,const uint8_t* end):
				r_begin(begin),r_end(end)
				{}

			bool codepointGet(uint32_t& codepoint);
			bool done() const noexcept
				{return r_begin==r_end;}

		private:
			const uint8_t* r_begin;
			const uint8_t* r_end;

			uint32_t m_state;
			uint32_t m_codep;
		};
	}


#endif