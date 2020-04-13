#ifndef _LOGSTREAM_H_
#define _LOGSTREAM_H_
#include <cstring>
#include <cassert>
#include <functional>
#include <string>
#include <algorithm>
#include <cinttypes>	// for decimal notation
#include <cstdlib>
#include <cstdio>		// for snprintf
#include "../include/MyGlobal.h"

using std::string;
using std::function;

MUDUO_COPY_BEGIN
template<int SIZE>
class FixedBuffer
{
	NONCOPYABLE (FixedBuffer)
public:
	FixedBuffer ()
		:cur_{ data_ }
	{
		setCookie (cookieStart);
	}

	~FixedBuffer ()
	{
		setCookie (cookieEnd);
	}
	void append (const char* buf, size_t len)
	{
		if (avail () > len) {
			memcpy (cur_, buf, len);
			cur_ += len;
		}
	}
	const char* data () const { return data_ };
	size_t length () const { return static_cast<size_t> (cur_ - data_ };
	char* current () { return cur_; }
	// available space in this buffer
	int avail ()const { return static_cast<int>(end () - cur_); }
	void add (size_t len) { cur_ += len; }

	void reset () { cur_ = data_; }
	void bzero () { memset (data_, 0, sizeof data_); }

	// for GDB
	const char* debugString ()
	{
		*cur_ = '\0';
		return data_;
	}
	void setCookie (std::function<void (void)>cookie) { cookie_ = cookie; }

	// for unit test
	string toString () const { return string (data_, cur_); }

private:
	const char* end ()const { return data_ + sizeof (data_); }
	std::function<void (void)> cookie_;		//??
	static void cookieStart (){ }			//?
	static void cookieEnd () { }			//?
	char data_[SIZE];
	char* cur_;
};

class LogStream
{
	using self = LogStream;
public:
	using Buffer = FixedBuffer<4000 * 1000>;
	self& operator<<(bool v)
	{
		buffer_.append (v ? "1" : "0", 1);
		return *this;
	}
	self& operator<<(const short);
	self& operator<<(const unsigned short);
	self& operator<<(const int);
	self& operator<<(const unsigned int);
	self& operator<<(const long);
	self& operator<<(const unsigned long);
	self& operator<<(const long long);
	self& operator<<(const unsigned long long);
	self& operator<<(const void*);
	self& operator<<(const float v);
	self& operator<<(const double);
	self& operator<<(const char v)
	{
		buffer_.append (&v, 1);
		return *this;
	}
	self& operator<<(const char* str)
	{
		if (str) {
			buffer_.append (str, strlen (str));
		}
		else {
			buffer_.append ("(null)", 6);
		}
	}
	self& operator<<(const unsigned char* str)
	{
		// reinterpret_cast: 用来对相同大小的空间做不同类型解读，可以用作指针的转换
		return operator<<(reinterpret_cast<const char*>(str));
	}
	self& operator<<(const string& v)
	{
		buffer_.append (v.data (), v.size ());
		return *this;
	}
	self& operator<<(const Buffer& v)
	{
		*this << v.toString ();
		return *this;
	}

	void append (const char* data, size_t len)
	{
		buffer_.append (data, len);
	}
	const Buffer& buffer () const { return buffer_; }
	void resetBuffer () { buffer_.reset (); }

private:
	// 编译时检测最大数值位数是否可以覆盖所有的10进制数值类型
	void staticCheck ();					
	template<typename T>
	void formatInteger (T);

	Buffer buffer_;
	static const int kMaxNumericSize = 32;	// 最大的数值位数
};

class Format
{
public:
	template<typename T>
	Format (const char* format, T val);

	const char* data () const { return buf_; }
	int length () const { return length_; }

private:
	char buf_[32];
	size_t length_;
};

inline LogStream& operator<<(LogStream& s, const Format& fmt)
{
	s.append (fmt.data (), fmt.length ());
	return s;
}



MUDUO_COPY_END

#endif // !_LOGSTREAM_H_

