
#define MUDUO_COPY_BEGIN namespace muduo_copy{
#define MUDUO_COPY_END	}
#define FILEUTIL_BEGIN	namespace FileUtil{
#define FILEUTIL_END	}

#define NONCOPYABLE(x) \
public:\
	x(const x&) = delete;\
	void operator=(const x&) = delete;

