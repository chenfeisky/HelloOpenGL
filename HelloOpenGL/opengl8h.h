// 第八章代码

// 选择一个，注释掉其他的
//#define CHAPTER_8_4_16 // 二维观察
//#define CHAPTER_8_4_16_test // 多视口，多窗口，子窗口等示例 二维观察
//#define CHAPTER_8_7_1 // Cohen-Sutherland线段裁剪算法
//#define CHAPTER_8_7_2 // 梁友栋-Barsky线段裁剪算法
//#define CHAPTER_8_8_1 // Sutherland-Hodgman多边形裁剪算法
//#define CHAPTER_8_EXERCISE_1
//#define CHAPTER_8_EXERCISE_2
//#define CHAPTER_8_EXERCISE_3
//#define CHAPTER_8_EXERCISE_4
//#define CHAPTER_8_EXERCISE_4_1 // OpenGL矩阵实现上一习题(其中2,3,4步使用不同的方法)
//#define CHAPTER_8_EXERCISE_5
//#define CHAPTER_8_EXERCISE_6
//#define CHAPTER_8_EXERCISE_7
//#define CHAPTER_8_EXERCISE_8
//#define CHAPTER_8_EXERCISE_9
//#define CHAPTER_8_EXERCISE_10
//#define CHAPTER_8_EXERCISE_11
//#define CHAPTER_8_EXERCISE_12
//#define CHAPTER_8_EXERCISE_13
//#define CHAPTER_8_EXERCISE_14
//#define CHAPTER_8_EXERCISE_15
//#define CHAPTER_8_EXERCISE_16 // 简单的Weiler-Atherton算法，没有考虑多边相交于一点等复杂情况
//#define CHAPTER_8_EXERCISE_16_1 // 考虑了上述复杂情况的完整Weiler-Atherton算法，应该是能取代上者
//#define CHAPTER_8_EXERCISE_16_2 // 检测每个像素点内外的方法求两多边形交，可满足复杂交点情况（计算每个像素的内外，性能低）
//#define CHAPTER_8_EXERCISE_17
//#define CHAPTER_8_EXERCISE_18
//#define CHAPTER_8_EXERCISE_19
//#define CHAPTER_8_EXERCISE_20
//#define CHAPTER_8_EXERCISE_21 // 全部保留或全部舍弃字符串 全部保留或全部舍弃字符
//#define CHAPTER_8_EXERCISE_22
//#define CHAPTER_8_EXERCISE_23
//#define CHAPTER_8_EXERCISE_24
//#define CHAPTER_8_EXERCISE_ADD_1 // Sutherlan-Hodgman算法和Weiler-Atherton算法两者实现
//#define CHAPTER_8_EXERCISE_ADD_1_1 // Cohen-Sutherland凸多边形裁剪算法
//#define CHAPTER_8_EXERCISE_ADD_1_2 // Liang-Barsky凸多边形裁剪算法
//#define CHAPTER_8_EXERCISE_ADD_1_3 // NLN凸多边形裁剪算法
//#define CHAPTER_8_EXERCISE_ADD_1_4 // 凸多边形裁剪算法（直接计算各交点，然后根据进出关系组合）
#define CHAPTER_8_EXERCISE_ADD_2 // Sutherlan-Hodgman算法和Weiler-Atherton算法两者实现
//#define CHAPTER_8_EXERCISE_ADD_2_1 // Weiler-Atherton裁剪


#ifdef CHAPTER_8_EXERCISE_5
#define CHAPTER_8_7_1
#endif

#ifdef CHAPTER_8_7_1
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_7
#define CHAPTER_8_7_2
#endif

#ifdef CHAPTER_8_EXERCISE_9
#define CHAPTER_8_7_2
#endif

#ifdef CHAPTER_8_7_2
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_8_1
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_1
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_2
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_3
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_4
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_4_1
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_6
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_8
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_10
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_11
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_12
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_13
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_14
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_15
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_16
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_17
#define CHAPTER_8_EXERCISE_16_1
#endif

#ifdef CHAPTER_8_EXERCISE_18
#define CHAPTER_8_EXERCISE_16_1
#endif

#ifdef CHAPTER_8_EXERCISE_19
#define CHAPTER_8_EXERCISE_16_1
#endif

#ifdef CHAPTER_8_EXERCISE_16_1
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_16_2
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_20
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_21
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_22
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_23
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_24
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1_1
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1_2
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1_3
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_1_4
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_2
#define CHAPTER_8_COMMON
#endif

#ifdef CHAPTER_8_EXERCISE_ADD_2_1
#define CHAPTER_8_COMMON
#endif