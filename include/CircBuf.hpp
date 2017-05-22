#ifndef __CIRCBUF_HPP
#define __CIRCBUF_HPP

template<
	unsigned int SIZE = 32,
	typename data_typename = unsigned char,
	typename cnt_typename = unsigned int,
	typename error_typename = int
>
struct CircBuf {

	typedef data_typename data_type;
	typedef cnt_typename cnt_type;
	typedef error_typename error_type;

	static const error_type OK = 0;
	static const error_type EMPTY = 1;
	static const error_type FULL = 2;
	static const error_type NULLPTR = -1;

private:

	cnt_type _s, _e;
	data_type data[SIZE];

	static cnt_type __next(const cnt_type i) { return ( i + 1 != SIZE ) ? (i + 1) : 0; }

	bool __empty() const { return _s == _e; }
	bool __full() const { return _s == __next(_e); }

public:

	CircBuf() : _s(0), _e(0) {}

	void clear() { _s = _e = 0; }
	error_type status() const { return __empty() ? EMPTY : (__full() ? FULL : OK); }
	cnt_type count() const { return (( _s > _e ) ? SIZE : 0) + _e - _s; }

//use the fast_* methods with care
//if not necessary, use the regular put and peek methods
//put a value without checking if the buffer is full
	void fast_put(const data_type& d)
	{
		data[_e] = d;
		_e = __next(_e);
	}

//peek a value without checking if the buffer is empty
	data_type fast_peek() const { return data[_s]; }

//put a value, returning an error if the buffer is full
	error_type put(const data_type& d)
	{
		if( __full() ) { return FULL; }
		fast_put(d);
		return OK;
	}

//peek a value, returning an error if the buffer is empty
	error_type peek(data_type& d) const
	{
		if( __empty() ) { return EMPTY; }
		d = fast_peek();
		return OK;
	}

//get a value, returning an error if the buffer is empty
	error_type get(data_type& d)
	{
		const error_type rv = peek(d);
		if( rv != OK ) { return rv; }
		_s = __next(_s);
		return OK;
	}
};

#endif
