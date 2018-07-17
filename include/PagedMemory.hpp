#ifndef __PAGEDMEMORY_HPP
#define __PAGEDMEMORY_HPP


template< typename DATA_TYPE, DATA_TYPE* START, unsigned int PAGES, unsigned int PAGE_SIZE >
struct PagedMemory {

	typedef DATA_TYPE data_type;

	static unsigned int memsize() { return PAGES * PAGE_SIZE; }

	struct DefaultErase {
		void operator()(const unsigned int /*page*/) const {}
	};

	struct DefaultWrite {
		void operator()(data_type dst[], const data_type src[], unsigned int len) const
		{
			if( len == 0 ) { return; }
			if( src == dst ) { return; }
			if( src > dst )
			{
				for( unsigned int i = 0 ; i < len ; ++i ) { dst[i] = src[i]; }
			}
			else
			{
				for( ; len-- > 0 ; ) { dst[len] = src[len]; }
			}
		}
	};

	template< class MEM_WRITE = DefaultWrite, class MEM_ERASE = DefaultErase>
	class Writer {

		static unsigned int min(const unsigned int a, const unsigned int b)
		{
			return ( a < b ) ? a : b;
		}

		static unsigned int do_write_block(const unsigned int seg, const unsigned int seg_offset, const data_type src[], unsigned int seg_len)
		{
			data_type buf[PAGE_SIZE];
			DefaultWrite()(buf, START + seg * PAGE_SIZE, seg_offset);
			DefaultWrite()(buf + seg_offset, src, seg_len);
			DefaultWrite()(buf + seg_offset + seg_len, START + seg * PAGE_SIZE + seg_offset + seg_len, PAGE_SIZE - seg_offset - seg_len);
			MEM_ERASE()(seg);
			MEM_WRITE()(START + seg * PAGE_SIZE, buf, PAGE_SIZE);
			return seg_len;
		}

		static unsigned int do_write(unsigned int offset, const data_type src[], unsigned int len)
		{
			for( unsigned int total = 0 ; ; )
			{
				const unsigned int seg = offset / PAGE_SIZE;
				const unsigned int seg_offset = offset - seg * PAGE_SIZE;
				const unsigned int seg_len = do_write_block(seg, seg_offset, src + total, min(len, PAGE_SIZE - seg_offset));
				total += seg_len;
				len -= seg_len;
				if( len == 0 ) { return total; }
				offset += seg_len;
			}
		}

	public:

		static unsigned int write(const unsigned int dst_offset, const data_type src[], const unsigned int len)
		{
			if( len == 0 ) { return 0; }
			if( dst_offset >= memsize() ) { return 0; }
			const unsigned int wrlen = min(len, memsize() - dst_offset);
			return do_write(dst_offset, src, wrlen);
		}

		static unsigned int write(data_type dst[], const data_type src[], const unsigned int len)
		{
			return ( dst >= START ) ? write(dst - START, src, len) : 0;
		}

	};

	typedef Writer<> DefaultWriter;

};


#endif
