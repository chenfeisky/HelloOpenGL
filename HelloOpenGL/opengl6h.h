// 第六章代码

// 选择一个，注释掉其他的
//#define CHAPTER_6_Test_1  // Bresenham中点画线算法
//#define CHAPTER_6_1_2
//#define CHAPTER_6_1_3
//#define CHAPTER_6_4_2
//#define CHAPTER_6_5_2
//#define CHAPTER_6_EXERCISE_1
//#define CHAPTER_6_EXERCISE_2
//#define CHAPTER_6_EXERCISE_3
//#define CHAPTER_6_EXERCISE_4
//#define CHAPTER_6_EXERCISE_5
//#define CHAPTER_6_EXERCISE_6
//#define CHAPTER_6_EXERCISE_7
//#define CHAPTER_6_EXERCISE_7_Test_1 // 并行画线：垂直距离算法
//#define CHAPTER_6_EXERCISE_7_Test_2 // 并行画线：扫描线算法
//#define CHAPTER_6_EXERCISE_8
//#define CHAPTER_6_EXERCISE_9
//#define CHAPTER_6_EXERCISE_10
//#define CHAPTER_6_EXERCISE_11
//#define CHAPTER_6_EXERCISE_12
//#define CHAPTER_6_EXERCISE_13
//#define CHAPTER_6_EXERCISE_14
//#define CHAPTER_6_EXERCISE_15
//#define CHAPTER_6_EXERCISE_16   // 三阶泰勒展开式中点算法(决策参数简化为有限次加法)
//#define CHAPTER_6_EXERCISE_16_Test1   // n阶泰勒展开式中点算法(决策参数需要加法和乘法)
//#define CHAPTER_6_EXERCISE_17
//#define CHAPTER_6_EXERCISE_18
//#define CHAPTER_6_EXERCISE_19
//#define CHAPTER_6_EXERCISE_20
//#define CHAPTER_6_EXERCISE_21
//#define CHAPTER_6_EXERCISE_22
//#define CHAPTER_6_EXERCISE_23
//#define CHAPTER_6_EXERCISE_24
//#define CHAPTER_6_EXERCISE_25
//#define CHAPTER_6_EXERCISE_26
//#define CHAPTER_6_EXERCISE_27
//#define CHAPTER_6_EXERCISE_28
//#define CHAPTER_6_EXERCISE_29	// 简单线宽函数，不含端点处理
//#define CHAPTER_6_EXERCISE_30
//#define CHAPTER_6_EXERCISE_31
//#define CHAPTER_6_EXERCISE_32
//#define CHAPTER_6_EXERCISE_33
//#define CHAPTER_6_EXERCISE_34
//#define CHAPTER_6_EXERCISE_35
//#define CHAPTER_6_EXERCISE_36
//#define CHAPTER_6_EXERCISE_37 // Wu反走样算法
//#define CHAPTER_6_EXERCISE_38
//#define CHAPTER_6_EXERCISE_39
//#define CHAPTER_6_EXERCISE_40
#define CHAPTER_6_EXERCISE_41
//#define CHAPTER_6_EXERCISE_42
//#define CHAPTER_6_EXERCISE_43
//#define CHAPTER_6_EXERCISE_44
//#define CHAPTER_6_EXERCISE_45
//#define CHAPTER_6_EXERCISE_46
//#define CHAPTER_6_EXERCISE_47
//#define CHAPTER_6_EXERCISE_48	// 可指定图案填充起始点的 扫描线/边界/泛滥 填充算法 
//#define CHAPTER_6_EXERCISE_49
//#define CHAPTER_6_EXERCISE_50 // 字符图案填充
//#define CHAPTER_6_EXERCISE_51
//#define CHAPTER_6_EXERCISE_52
//#define CHAPTER_6_EXERCISE_53
//#define CHAPTER_6_EXERCISE_54 // Bresenham 零宽
//#define CHAPTER_6_EXERCISE_54_Test1 // 过取样（即超采样）
//#define CHAPTER_6_EXERCISE_54_Test2 // 过取样（多重采样,即边缘采样）
//#define CHAPTER_6_EXERCISE_54_Test3 // 过取样（加权,超采样,多重采样）
//#define CHAPTER_6_EXERCISE_55 // 中点线 零宽
//#define CHAPTER_6_EXERCISE_56 // 零宽椭圆线 / 过采样椭圆线 / 多重采样椭圆填充
//#define CHAPTER_6_EXERCISE_57 // 超采样/多重采样填充,同CHAPTER_6_EXERCISE_54_Test1和CHAPTER_6_EXERCISE_54_Test2
//#define CHAPTER_6_EXERCISE_58 // 区域取样
//#define CHAPTER_6_EXERCISE_ADD_1
//#define CHAPTER_6_EXERCISE_ADD_2

#ifdef CHAPTER_6_1_2
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_1_3
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_4_2
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_Test_1
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_5_2
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_1
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_2
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_3
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_4
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_5
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_6
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_7
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_7_Test_1
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_7_Test_2
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_8
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_9
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_10
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_11
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_12
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_13
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_14
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_15
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_16
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_16_Test1
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_17
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_18
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_19
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_20
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_21
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_22
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_23
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_24
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_25
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_26
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_27
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_28
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_29
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_30
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_31
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_32
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_33
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_34
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_35
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_36
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_37
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_38
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_39
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_40
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_41
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_42
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_43
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_44
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_45
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_46
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_47
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_48
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_49
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_50
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_51
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_52
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_53
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_54
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_54_Test1
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_54_Test2
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_54_Test3
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_55
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_56
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_57
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_58
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_ADD_1
#define CHAPTER_6_COMMON
#endif

#ifdef CHAPTER_6_EXERCISE_ADD_2
#define CHAPTER_6_COMMON
#endif
