// 第十四章代码

// 选择一个，注释掉其他的
//#define CHAPTER_14_8_2
//#define CHAPTER_14_16_1
//#define CHAPTER_14_16_1_1
#define CHAPTER_14_16_2
//#define CHAPTER_14_16_2_1
// 先加三维观察，再加CHAPTER_14_16_2_3

#ifdef CHAPTER_14_16_1
#define CHAPTER_14_COMMON
#endif

#ifdef CHAPTER_14_16_1_1
#define CHAPTER_14_COMMON
#endif

#ifdef CHAPTER_14_16_2
#define CHAPTER_14_COMMON
#define OPENGL_CAMERA
#endif

#ifdef CHAPTER_14_16_2_1
#define CHAPTER_14_COMMON
#define OPENGL_CAMERA
#endif

//////////////////////////////////////////////////////////////////////////