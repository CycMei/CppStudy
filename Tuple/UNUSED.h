

// __attribute__((unused)) 没有使用变量消除警告
//-Wno-unused-but-set-variable
#ifdef UNUSED
	#elif defined(__GNUC__)
		#define UNUSED(x) UNUSED_##x __attribute__((unused))
	#elif defined(__LCLINT__)
		#define UNUSED(x) x;
#else
	#define UNUSED(x) x
#endif