/*
COBS.hpp
Implementation of Consistent Overhead Byte Stuffing coder/decoder
only for a single chunk of data (up to 254 bytes)
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2017-2018
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __COBS_HPP
#define __COBS_HPP

class COBS {

//encode an empty packet
	static int encode_empty(unsigned char out[])
	{
		out[0] = 1;
		out[1] = 0x00;
		return 2;
	}

//encode a single chunk
//len_in is assumed to be a correct value for non-empty data fitting in a single chunk
//ie. len_in > 0 and len_in <= 254
	static int encode_chunk(unsigned char out[], const unsigned char in[], const unsigned int len_in)
	{
		unsigned int j = 0;	//index of pointer to next zero byte
		unsigned int i = 0;
		do
		{
			if( in[i] == 0x00 )
			{
				out[j] = 1 + i - j;	//point to this zero
				j = 1 + i;
			}
			else { out[1 + i] = in[i]; }
		}
		while( i++ < len_in );

		out[i] = 0x00;		//insert packet delimiter
		out[j] = i - j;		//insert pointer to packet delimiter

		return 1 + i;
	}

//decode a single chunk
//it is assumed that in contains a "mostly valid" encoded chunk
//this function will check for stray zeroes not at the edges and if the delimiter is valid
	static int decode_chunk(unsigned char out[], const unsigned char in[], const unsigned int len_in)
	{
		const unsigned int last = len_in - 2;
		if( last == 0 ) { return ( ( in[0] == 1 ) && ( in[1] == 0x00 ) ) ? 0 : -2; }
		unsigned int j = in[0];
		unsigned int i = 1;
		do
		{
			if( i == j )
			{
				if( in[i] == 0x00 ) { return i - 1; }
				out[i - 1] = 0;
				j = i + in[i];
			}
			else
			{
				if( in[i] == 0x00 ) { return -2; }
				out[i - 1] = in[i];
			}
		}
		while( ++i <= last );
		return ( in[i] == 0x00 ) ? last : -2;
	}

public:

//encode a buffer
//data for encoding start from in[0]
//out[0] is assumed to be available and reserved for the overhead byte
//len_in is the length of input data in
//out[len+1] is assumed to be available and reserved for the packet delimiter
//return value: length of the encoded packet (including overhead byte and delimiter) if success,
//              otherwise a negative value
	static int encode(unsigned char out[], const unsigned char in[], const unsigned int len_in)
	{
		if( len_in > 254 ) { return -1; }
		if( len_in == 0 ) { return encode_empty(out); }
		return encode_chunk(out, in, len_in);
	}

//encode in place
//here actual data for encoding start from data[1]
//data[0] is assumed to be available and reserved for the overhead byte
//len is the length of input data, spanning from data[1] to data[len]
//data[len+1] is assumed to be available and reserved for the packet delimiter
	static int encode(unsigned char data[], const unsigned int len) { return encode(data, data + 1, len); }

//decode a buffer
//in contains the encoded data with the overhead byte and packet delimiter
//len is the length of the encoded in data, including the two added bytes
//after decoding, decoded data span from out[0] to out[len-3]
//return value: length of the decoded data if success, negative otherwise
	static int decode(unsigned char out[], const unsigned char in[], const unsigned int len_in)
	{
		if( len_in > 256 ) { return -1; }	//max. length of a single encoded chunk is 256
		if( len_in < 2 ) { return -1; }		//a minimal COBS encoded packet is {1,0}
		if( in[0] == 0x00 ) { return -2; }	//a COBS encoded packet must not start with 0x00
		return decode_chunk(out, in, len_in);
	}

//decode a buffer in place
	static int decode(unsigned char data[], const unsigned int len) { return decode(data, data, len); }

//general templates intended for use with std::vector<unsigned char>
	template < typename T >
	static int encode(T& buf)
	{
		const typename T::size_type len = buf.size();
		buf.insert(buf.begin(), 0);
		buf.push_back(0);
		return encode(buf.data(), len);
	}

	template < typename T >
	static int decode(T& buf)
	{
		const int rv = decode(buf.data(), buf.size());
		buf.pop_back();
		buf.pop_back();
		return rv;
	}

};

#endif
