#include "common.h"

#if CHAPTER==4
#include "opengl4h.h"

#ifdef CHAPTER_4_COMMON
GLsizei winWidth = 800, winHeight = 600;

// 判断浮点数相等
bool equal(float f1, float f2)
{
	return std::abs(f1 - f2) < 0.0001;
}

#endif


#ifdef CHAPTER_4_Test
enum Order
{
	AntiClockwise,  // 逆时针
	Clockwise,		// 顺时针
};


void drawPoint(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	int point1[] = { 50, 100 };
	int point2[] = { 75, 100 };
	int point3[] = { 100, 100 };
	glBegin(GL_POINTS);
	glVertex2iv(point1);
	glVertex2iv(point2);
	glVertex2iv(point3);
	glEnd();

	glFlush();
}

struct Vec
{
	float x;
	float y;
};

// 计算两直线的交点
// 直线公式：AX+BY+C=0
bool linesPoint(float A1, float B1, float C1, float A2, float B2, float C2, Vec& point)
{
	if (equal(A1 * B2, A2 * B1))
		return false;

	point.x = (B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1);
	point.y = (A2 * C1 - A1 * C2) / (A1 * B2 - A2 * B1);
	return true;
}

// 检查点是否与射线方向相同
// 射线由端点rayPoint和角度rayRadius构成
bool testSameDir(Vec rayPoint, float rayRadius, Vec testPoint)
{
	if (equal(PI / 2, rayRadius))
		return testPoint.x == rayPoint.x && testPoint.y > rayPoint.y;
	if (equal(3 * PI / 2, rayRadius))
		return testPoint.x == rayPoint.x && testPoint.y < rayPoint.y;
	if (0 <= rayRadius && rayRadius < PI / 2)
		return testPoint.x > rayPoint.x && testPoint.y >= rayPoint.y;
	if (PI / 2 < rayRadius && rayRadius <= PI)
		return testPoint.x < rayPoint.x && testPoint.y >= rayPoint.y;
	if (PI < rayRadius && rayRadius < PI * 3 / 2)
		return testPoint.x < rayPoint.x && testPoint.y < rayPoint.y;
	if (PI * 3 / 2 < rayRadius && rayRadius <= PI * 2)
		return testPoint.x > rayPoint.x && testPoint.y <= rayPoint.y;
	return false;
};


vector<vector<Vec>> drawPolygons 
= { 
		{
			{ 375, 500 },
			{ 500, 300 },
			{ 200, 260 },
			{ 138, 430 },
		}, 
		{
			{ 150, 550 },
			{ 50, 300 },
			{ 250, 300 },
		},
};

Vec __P;
Vec rayPoint;

void drowPolygon(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	int j = 0;
	for (auto& polygon : drawPolygons)
	{
		for (int i = 0; i < polygon.size(); ++i)
		{
			j = i + 1 >= polygon.size() ? 0 : i + 1;
			glBegin(GL_LINES);
			glVertex2i(polygon[i].x, polygon[i].y);
			glVertex2i(polygon[j].x, polygon[j].y);
			glEnd();
		}
	}

	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex2i(__P.x, __P.y);
	glVertex2i(rayPoint.x, rayPoint.y);
	glEnd();

	glColor3f(0.0, 0.4, 0.2);

	glFlush();
}

// 计算两平面向量的叉积（向量积）的Z轴坐标
int crossProduct(const Vec& vector1, const Vec& vector2)
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}

// 根据顶点数组得到边向量
void getSideVec(const vector<Vec>& vertex, vector<Vec>& sideVec)
{
	sideVec.clear();
	sideVec.resize(0);
	int j = 0;
	for (int i = 0; i < vertex.size(); ++i)
	{
		j = i + 1 >= vertex.size() ? 0 : i + 1;
		sideVec.push_back(Vec());
		sideVec.back().x = vertex[j].x - vertex[i].x;
		sideVec.back().y = vertex[j].y - vertex[i].y;
	}
}

// 按时针顺序排序顶点数组：
// 依次相连边向量叉积当正值多于负值则为逆时针，当负值多于正值则为顺时针
void sortPoints(Order order, vector<Vec>& vertex)
{
	vector<Vec> sideVec;
	getSideVec(vertex, sideVec);

	int crossUp = 0;
	int crossDown = 0;
	int j = 0;
	for (int i = 0; i < sideVec.size(); ++i)
	{
		j = i + 1 >= sideVec.size() ? 0 : i + 1;
		int value = crossProduct(sideVec[i], sideVec[j]);
		if (equal(value, 0))
			continue;

		if (value > 0)
			crossUp++;
		else
			crossDown++;
	}

	if ((order == AntiClockwise && crossUp < crossDown) ||
		(order == Clockwise && crossUp > crossDown))
	{
		std::reverse(vertex.begin(), vertex.end());
	}
}

// 是否是凸多边形:
// 逆时针排序后，如果有边向量叉积为负，则为凹多边形
bool isConvexPolygon(const vector<Vec>& vertex)
{
	vector<Vec> sortVertex = vertex;
	sortPoints(AntiClockwise, sortVertex);

	vector<Vec> sideVec;
	getSideVec(sortVertex, sideVec);

	int j = 0;
	for (int i = 0; i < sideVec.size(); ++i)
	{
		j = i + 1 >= sideVec.size() ? 0 : i + 1;
		float xp = crossProduct(sideVec[i], sideVec[j]);
		if (!equal(xp, 0) && xp < 0)
			return false;
	}
	return true;
}


// 分割多边形：
// 第一条凹边的延长线与最近的边作切割
void cutPolygon(const vector<Vec>& vertex, vector<vector<Vec>>& polygons, bool isSort = false)
{
	vector<Vec> sortVertex = vertex;

	if (!isSort)
		sortPoints(AntiClockwise, sortVertex);
	
	if (isConvexPolygon(sortVertex))
	{
		return polygons.push_back(sortVertex);
	}
	else
	{
		vector<Vec> sideVec;
		getSideVec(sortVertex, sideVec);

		int findIndex = 0;
		int j = 0;
		for (int i = 0; i < sideVec.size(); ++i)
		{
			j = i + 1 >= sideVec.size() ? 0 : i + 1;
			float xp = crossProduct(sideVec[i], sideVec[j]);
			if (!equal(xp, 0) && xp < 0)
			{
				findIndex = i;
				break;
			}
		}

		auto getRealIndex = [&sortVertex](int index)
		{
			if (index >= 0)
				return index % sortVertex.size();
			else
				return sortVertex.size() + index;
		};
		Vec firstSide1 = sortVertex[getRealIndex(findIndex)];
		Vec firstSide2 = sortVertex[getRealIndex(findIndex + 1)];

		Vec cutPoint = {0, 0};
		int cutStartIndex = 0;
		int cutEndIndex = 0;
		bool first = true;
		for (int i = 0; i < sortVertex.size(); ++i)
		{
			if (i != getRealIndex(findIndex - 1) &&
				i != getRealIndex(findIndex) &&
				i != getRealIndex(findIndex + 1))
			{
				Vec cutStart = sortVertex[getRealIndex(i)];
				Vec cutEnd = sortVertex[getRealIndex(i + 1)];
			
				float A1 = firstSide1.y - firstSide2.y;
				float B1 = firstSide2.x - firstSide1.x;
				float C1 = firstSide2.x * (firstSide2.y - firstSide1.y) + firstSide2.y * (firstSide1.x - firstSide2.x);

				float A2 = cutStart.y - cutEnd.y;
				float B2 = cutEnd.x - cutStart.x;
				float C2 = cutEnd.x * (cutEnd.y - cutStart.y) + cutEnd.y * (cutStart.x - cutEnd.x);

				Vec Point;
				if(!linesPoint(A1, B1, C1, A2, B2, C2, Point))
					continue;
				
				float vecFirstX = firstSide2.x - firstSide1.x;
				float vecFirstY = firstSide2.y - firstSide1.y;
				float vecCutX = Point.x - firstSide2.x;
				float vecCutY = Point.y - firstSide2.y;

				bool sameDir = false;
				if (equal(vecFirstX, 0))
					sameDir = equal(vecCutX, 0);
				else
					sameDir = vecCutX * vecFirstX > 0;
				if (!sameDir) continue;
				if (equal(vecFirstY, 0))
					sameDir = equal(vecCutY, 0);
				else
					sameDir = vecCutY * vecFirstY > 0;
				if (!sameDir) continue;

				float curDis = vecCutX * vecCutX + vecCutY * vecCutY;
				float lastDis = (cutPoint.x - firstSide2.x) * (cutPoint.x - firstSide2.x)
					+ (cutPoint.y - firstSide2.y) * (cutPoint.y - firstSide2.y);
				if (first || curDis < lastDis)
				{
					cutPoint.x = Point.x;
					cutPoint.y = Point.y;
					cutStartIndex = i;
					cutEndIndex = i + 1;
					first = false;
				}
			}
		}

		vector<Vec> vertex1;
		vector<Vec> vertex2;
		vertex1.push_back(sortVertex[getRealIndex(findIndex + 1)]);
		vertex1.push_back({ cutPoint.x, cutPoint.y });
		for (int i = getRealIndex(cutEndIndex);; ++i)
		{
			if (getRealIndex(i) != getRealIndex(findIndex + 1))
				vertex1.push_back(sortVertex[getRealIndex(i)]);
			else
				break;
		}

		for (int i = getRealIndex(findIndex + 1);; ++i)
		{
			if (getRealIndex(i) != getRealIndex(cutEndIndex))
				vertex2.push_back(sortVertex[getRealIndex(i)]);
			else
				break;
		}
		vertex2.push_back({ cutPoint.x, cutPoint.y });

		cutPolygon(vertex1, polygons, true);
		cutPolygon(vertex2, polygons, true);
	}
}

// 分割多边形成三角形集:
void cutPolygonT(const vector<Vec>& vertex, vector<vector<Vec>>& polygons, bool isSort = false)
{
	if (!isConvexPolygon(vertex))
		return;

	vector<Vec> sortVertex = vertex;

	if (!isSort)
		sortPoints(AntiClockwise, sortVertex);

	if (sortVertex.size() < 3)
		return;

	polygons.push_back({ sortVertex[0], sortVertex[1], sortVertex[2] });
	sortVertex.erase(sortVertex.begin() + 1);
	cutPolygonT(sortVertex, polygons, true);
}

float radius = 0.0f;

// 内外测试：奇偶规则
bool isInnerPoint(const vector<Vec>& vertex, Vec point)
{
	radius = std::rand() % 360;
	radius = radius / 180 * PI;
	for (int i = 0; i < vertex.size(); i++)
	{
		if (vertex[i].x != point.x)
		{
			if (equal(PI / 2, radius) || equal(PI * 3 / 2, radius))
				continue;

			if (equal((vertex[i].y - point.y) / (vertex[i].x - point.x), std::tan(radius)) && testSameDir(point, radius, vertex[i]))
				return isInnerPoint(vertex, point);
		}
		else
		{
			if ((equal(PI / 2, radius) || equal(PI * 3 / 2, radius)) && testSameDir(point, radius, vertex[i]))
				return isInnerPoint(vertex, point);
		}
	}

	int count = 0;
	int j = 0;
	for (int i = 0; i < vertex.size(); ++i)
	{
		j = i + 1 >= vertex.size() ? 0 : i + 1;

		float A1 = vertex[i].y - vertex[j].y;
		float B1 = vertex[j].x - vertex[i].x;
		float C1 = vertex[j].x * (vertex[j].y - vertex[i].y) + vertex[j].y * (vertex[i].x - vertex[j].x);

		float A2;
		float B2;
		float C2;
		if (equal(PI / 2, radius) || equal(PI * 3 / 2, radius))
		{
			A2 = 1;
			B2 = 0;
			C2 = -point.x;
		}
		else
		{ 
			A2 = -std::tan(radius);
			B2 = 1;
			C2 = std::tan(radius) * point.x - point.y;
		}		

		Vec p;
		if (linesPoint(A1, B1, C1, A2, B2, C2, p))
		{
			if (testSameDir(point, radius, p))
			{
				float lineX1 = vertex[i].x < vertex[j].x ? vertex[i].x : vertex[j].x;
				float lineX2 = vertex[i].x < vertex[j].x ? vertex[j].x : vertex[i].x;
				float lineY1 = vertex[i].y < vertex[j].y ? vertex[i].y : vertex[j].y;
				float lineY2 = vertex[i].y < vertex[j].y ? vertex[j].y : vertex[i].y;
				if (lineX1 <= p.x && p.x <= lineX2 && lineY1 <= p.y && p.y <= lineY2)
					count++;
			}
		}
	}
	return count % 2 == 1;
}

// 统计向量与多边形叉积的数量
// 叉积为正+1，为负-1，多边形必须为已排序
int calculateCrossPcount(const vector<Vec>& vertex, Vec point, float radius )
{
	vector<Vec> sideVec;
	getSideVec(vertex, sideVec);

	int ret = 0;
	int j = 0;
	for (int i = 0; i < vertex.size(); ++i)
	{
		j = i + 1 >= vertex.size() ? 0 : i + 1;

		float A1 = vertex[i].y - vertex[j].y;
		float B1 = vertex[j].x - vertex[i].x;
		float C1 = vertex[j].x * (vertex[j].y - vertex[i].y) + vertex[j].y * (vertex[i].x - vertex[j].x);

		float A2;
		float B2;
		float C2;
		if (equal(PI / 2, radius) || equal(PI * 3 / 2, radius))
		{
			A2 = 1;
			B2 = 0;
			C2 = -point.x;
		}
		else
		{
			A2 = -std::tan(radius);
			B2 = 1;
			C2 = std::tan(radius) * point.x - point.y;
		}

		Vec p;
		if (linesPoint(A1, B1, C1, A2, B2, C2, p))
		{
			if (testSameDir(point, radius, p))
			{
				float lineX1 = vertex[i].x < vertex[j].x ? vertex[i].x : vertex[j].x;
				float lineX2 = vertex[i].x < vertex[j].x ? vertex[j].x : vertex[i].x;
				float lineY1 = vertex[i].y < vertex[j].y ? vertex[i].y : vertex[j].y;
				float lineY2 = vertex[i].y < vertex[j].y ? vertex[j].y : vertex[i].y;
				if (lineX1 <= p.x && p.x <= lineX2 && lineY1 <= p.y && p.y <= lineY2)
				{
					Vec testp;
					int sign = 0;
					if (equal(PI / 2, radius) || equal(PI * 3 / 2, radius))
						sign = 0;
					else if ((0 <= radius && radius < PI / 2) || (PI * 3 / 2 < radius && radius <= 2 * PI))
						sign = 1;
					else
						sign = -1;

					float dx = 100000;

					testp.x = __P.x + dx * sign;
					if (equal(PI / 2, radius))
					{
						testp.y = __P.y + dx;
					}
					else if (equal(PI * 3 / 2, radius))
					{
						testp.y = __P.y - dx;
					}
					else
					{
						testp.y = std::tan(radius) * dx * sign + __P.y;
					}
					int cp = crossProduct({ testp.x - __P.x, testp.y - __P.y }, sideVec[i]);
					if (cp > 0)
						ret++;
					else
						ret--;
				}
			}
		}
	}
	return ret;
}


// 内外测试：非零环绕数
bool isInnerPointR(const vector<Vec>& vertex, Vec point)
{
	radius = std::rand() % 360;
	radius = radius / 180 * PI;
	for (int i = 0; i < vertex.size(); i++)
	{
		if (vertex[i].x != point.x)
		{
			if (equal(PI / 2, radius) || equal(PI * 3 / 2, radius))
				continue;

			if (equal((vertex[i].y - point.y) / (vertex[i].x - point.x), std::tan(radius)) && testSameDir(point, radius, vertex[i]))
				return isInnerPointR(vertex, point);
		}
		else
		{
			if ((equal(PI / 2, radius) || equal(PI * 3 / 2, radius)) && testSameDir(point, radius, vertex[i]))
				return isInnerPointR(vertex, point);
		}
	}
	
	return calculateCrossPcount(vertex, point, radius) != 0;
}


// 内外测试：两区域的并集
bool isInnerPointU(const vector<Vec>& vertexA, const vector<Vec>& vertexB, Vec point, bool isSort = false)
{
	vector<Vec> sortVertexA = vertexA;
	vector<Vec> sortVertexB = vertexB;

	if (!isSort)
	{
		sortPoints(AntiClockwise, sortVertexA);
		sortPoints(AntiClockwise, sortVertexB);
	}

	radius = std::rand() % 360;
	radius = radius / 180 * PI;
	int count = 0;

	for (int i = 0; i < sortVertexA.size(); i++)
	{
		if (sortVertexA[i].x != point.x)
		{
			if (equal(PI / 2, radius) || equal(PI * 3 / 2, radius))
				continue;

			if (equal((sortVertexA[i].y - point.y) / (sortVertexA[i].x - point.x), std::tan(radius)) && testSameDir(point, radius, sortVertexA[i]))
				return isInnerPointU(sortVertexA, sortVertexB, point, true);
		}
		else
		{
			if ((equal(PI / 2, radius) || equal(PI * 3 / 2, radius)) && testSameDir(point, radius, sortVertexA[i]))
				return isInnerPointU(sortVertexA, sortVertexB, point, true);
		}
	}

	for (int i = 0; i < sortVertexB.size(); i++)
	{
		if (sortVertexB[i].x != point.x)
		{
			if (equal(PI / 2, radius) || equal(PI * 3 / 2, radius))
				continue;

			if (equal((sortVertexB[i].y - point.y) / (sortVertexB[i].x - point.x), std::tan(radius)) && testSameDir(point, radius, sortVertexB[i]))
				return isInnerPointU(sortVertexA, sortVertexB, point, true);
		}
		else
		{
			if ((equal(PI / 2, radius) || equal(PI * 3 / 2, radius)) && testSameDir(point, radius, sortVertexB[i]))
				return isInnerPointU(sortVertexA, sortVertexB, point, true);
		}
	}

	count += calculateCrossPcount(sortVertexA, point, radius);
	count += calculateCrossPcount(sortVertexB, point, radius);

	return count > 0;
}

// 内外测试：两区域的交集
bool isInnerPointI(const vector<Vec>& vertexA, const vector<Vec>& vertexB, Vec point, bool isSort = false)
{
	vector<Vec> sortVertexA = vertexA;
	vector<Vec> sortVertexB = vertexB;

	if (!isSort)
	{
		sortPoints(AntiClockwise, sortVertexA);
		sortPoints(AntiClockwise, sortVertexB);
	}

	radius = std::rand() % 360;
	radius = radius / 180 * PI;
	int count = 0;

	for (int i = 0; i < sortVertexA.size(); i++)
	{
		if (sortVertexA[i].x != point.x)
		{
			if (equal(PI / 2, radius) || equal(PI * 3 / 2, radius))
				continue;

			if (equal((sortVertexA[i].y - point.y) / (sortVertexA[i].x - point.x), std::tan(radius)) && testSameDir(point, radius, sortVertexA[i]))
				return isInnerPointI(sortVertexA, sortVertexB, point, true);
		}
		else
		{
			if ((equal(PI / 2, radius) || equal(PI * 3 / 2, radius)) && testSameDir(point, radius, sortVertexA[i]))
				return isInnerPointI(sortVertexA, sortVertexB, point, true);
		}
	}

	for (int i = 0; i < sortVertexB.size(); i++)
	{
		if (sortVertexB[i].x != point.x)
		{
			if (equal(PI / 2, radius) || equal(PI * 3 / 2, radius))
				continue;

			if (equal((sortVertexB[i].y - point.y) / (sortVertexB[i].x - point.x), std::tan(radius)) && testSameDir(point, radius, sortVertexB[i]))
				return isInnerPointI(sortVertexA, sortVertexB, point, true);
		}
		else
		{
			if ((equal(PI / 2, radius) || equal(PI * 3 / 2, radius)) && testSameDir(point, radius, sortVertexB[i]))
				return isInnerPointI(sortVertexA, sortVertexB, point, true);
		}
	}

	count += calculateCrossPcount(sortVertexA, point, radius);
	count += calculateCrossPcount(sortVertexB, point, radius);

	return count > 1;
}

// 内外测试：两区域的差集
bool isInnerPointS(const vector<Vec>& vertexA, const vector<Vec>& vertexB, Vec point, bool isSort = false)
{
	vector<Vec> sortVertexA = vertexA;
	vector<Vec> sortVertexB = vertexB;

	if (!isSort)
	{
		sortPoints(AntiClockwise, sortVertexA);
		sortPoints(Clockwise, sortVertexB);
	}

	radius = std::rand() % 360;
	radius = radius / 180 * PI;
	int count = 0;

	for (int i = 0; i < sortVertexA.size(); i++)
	{
		if (sortVertexA[i].x != point.x)
		{
			if (equal(PI / 2, radius) || equal(PI * 3 / 2, radius))
				continue;

			if (equal((sortVertexA[i].y - point.y) / (sortVertexA[i].x - point.x), std::tan(radius)) && testSameDir(point, radius, sortVertexA[i]))
				return isInnerPointS(sortVertexA, sortVertexB, point, true);
		}
		else
		{
			if ((equal(PI / 2, radius) || equal(PI * 3 / 2, radius)) && testSameDir(point, radius, sortVertexA[i]))
				return isInnerPointS(sortVertexA, sortVertexB, point, true);
		}
	}

	for (int i = 0; i < sortVertexB.size(); i++)
	{
		if (sortVertexB[i].x != point.x)
		{
			if (equal(PI / 2, radius) || equal(PI * 3 / 2, radius))
				continue;

			if (equal((sortVertexB[i].y - point.y) / (sortVertexB[i].x - point.x), std::tan(radius)) && testSameDir(point, radius, sortVertexB[i]))
				return isInnerPointS(sortVertexA, sortVertexB, point, true);
		}
		else
		{
			if ((equal(PI / 2, radius) || equal(PI * 3 / 2, radius)) && testSameDir(point, radius, sortVertexB[i]))
				return isInnerPointS(sortVertexA, sortVertexB, point, true);
		}
	}

	count += calculateCrossPcount(sortVertexA, point, radius);
	count += calculateCrossPcount(sortVertexB, point, radius);

	return count > 0;
}


void getrayPoint()
{
	int sign = 0;
	if (equal(PI / 2, radius) || equal(PI * 3 / 2, radius))
		sign = 0;
	else if ((0 <= radius && radius < PI / 2) || (PI * 3 / 2 < radius && radius <= 2 * PI))
		sign = 1;
	else
		sign = -1;
	float dx = 100000;
	rayPoint.x = __P.x + dx * sign;
	if (equal(PI / 2, radius))
	{
		rayPoint.y = __P.y + dx;
	}
	else if (equal(PI * 3 / 2, radius))
	{
		rayPoint.y = __P.y - dx;
	}
	else
	{
		rayPoint.y = std::tan(radius) * dx * sign + __P.y;
	}
}

void polygonTest(void)
{

	// 判断多边形是否为凸多边形
	//printf("%d", isConvexPolygon(vertexs));


	// 切割凹多边形
	//vector<Vec> vertexs = { { 550, 300 },
	//{ 660, 550 },
	//{ 700, 50 },
	//{ 556, 190 },
	//{ 50, 150 },
	//{ 75, 450 },
	//{ 250, 300 },
	//{ 450, 500 }, };
	//cutPolygon(vertexs, drawPolygons);


	// 切割多边形为三角形
	//vector<Vec> vertexs = { { 300, 550 },
	//{ 200, 300 },
	//{ 400, 100 },
	//{ 600, 200 },
	//{ 600, 400 },
	//};
	//cutPolygonT(vertexs, drawPolygons);


	// 内外测试:奇偶规则
	//vector<Vec> vertexs =
	//{
	//	{ 400, 550 },
	//	{ 300, 50 },
	//	{ 100, 350 },
	//	{ 500, 470 },
	//	{ 600, 200 },
	//	{ 50, 150 },
	//	{ 475, 350 },
	//};
	//__P = { 412, 385 };
	////__P = { 276, 207 };
	////__P = { 240, 255 };
	////__P = { 186, 182 };
	//printf("%d", isInnerPoint(vertexs, __P));
	//getrayPoint();


	//// 内外测试:非零环绕数
	//vector<Vec> vertexs =
	//{
	//	{ 400, 550 },
	//	{ 300, 50 },
	//	{ 100, 350 },
	//	{ 500, 470 },
	//	{ 600, 200 },
	//	{ 50, 150 },
	//	{ 475, 350 },
	//};
	//__P = { 412, 385 };
	////__P = { 276, 207 };
	////__P = { 240, 255 };
	////__P = { 186, 182 };
	//printf("%d", isInnerPointR(vertexs, __P));
	//getrayPoint();


	//// 内外测试:两区域的并集
	//vector<Vec> vertexsA =
	//{
	//	{ 375, 500 },
	//	{ 500, 300 },
	//	{ 200, 260 },
	//	{ 138, 430 },
	//};
	//vector<Vec> vertexsB =
	//{
	//	{ 150, 550 },
	//	{ 50, 300 },
	//	{ 250, 300 },
	//};
	//__P = { 200, 340 };
	////__P = { 172, 423 };
	////__P = { 286, 385 };
	////__P = { 97, 327 };
	////__P = { 223, 494 };
	//printf("%d", isInnerPointU(vertexsA, vertexsB, __P));
	//getrayPoint();

	//// 内外测试:两区域的交集
	//vector<Vec> vertexsA =
	//{
	//	{ 375, 500 },
	//	{ 500, 300 },
	//	{ 200, 260 },
	//	{ 138, 430 },
	//};
	//vector<Vec> vertexsB =
	//{
	//	{ 150, 550 },
	//	{ 50, 300 },
	//	{ 250, 300 },
	//};
	//__P = { 200, 340 };
	////__P = { 172, 423 };
	////__P = { 286, 385 };
	////__P = { 97, 327 };
	////__P = { 223, 494 };
	//printf("%d", isInnerPointI(vertexsA, vertexsB, __P));
	//getrayPoint();

	// 内外测试:两区域的差集
	vector<Vec> vertexsA =
	{
		{ 375, 500 },
		{ 500, 300 },
		{ 200, 260 },
		{ 138, 430 },
	};
	vector<Vec> vertexsB =
	{
		{ 150, 550 },
		{ 50, 300 },
		{ 250, 300 },
	};
	//__P = { 200, 340 };
	//__P = { 172, 423 };
	__P = { 286, 385 };
	//__P = { 97, 327 };
	//__P = { 223, 494 };
	printf("%d", isInnerPointS(vertexsA, vertexsB, __P));
	getrayPoint();

	glutDisplayFunc(drowPolygon);
}
#endif

#ifdef CHAPTER_4_15_2
GLuint regHex;
void code_4_15_2_draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	glCallList(regHex);

	glFlush();
}
void code_4_15_2()
{
	const double TWO_PI = 6.2831853;
	GLdouble theta;
	GLint x, y, k;
	regHex = glGenLists(1);
	glNewList(regHex, GL_COMPILE);
	glBegin(GL_POLYGON);
	for (k = 0; k < 6; k++)
	{
		theta = TWO_PI *k / 6.0;
		x = 200 + 150 * cos(theta);
		y = 200 + 150 * sin(theta);
		glVertex2i(x, y);
	}
	glEnd();
	glEndList();

	glutDisplayFunc(code_4_15_2_draw);
}
#endif

#ifdef CHAPTER_4_16
const double TWO_PI = 6.2831853;
GLsizei winWidth = 400, winHeight = 400;
GLuint regHex;
class screenPt
{
private:
	GLint x, y;
public:
	screenPt()
	{
		x = y = 0;
	}
	void setCoords(GLint xCoord, GLint yCoord)
	{
		x = xCoord;
		y = yCoord;
	}
	GLint getx() const
	{
		return x;
	}
	GLint gety() const
	{
		return y;
	}
};
static void init(void)
{
	screenPt hexVertex, circCtr;
	GLdouble theta;
	GLint k;
	circCtr.setCoords(winWidth / 2, winHeight / 2);
	glClearColor(1.0, 1.0, 1.0, 0.0);

	regHex = glGenLists(1);
	glNewList(regHex, GL_COMPILE);
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
			for (k = 0; k < 6; k++)
			{
				theta = TWO_PI *k / 6.0;
				hexVertex.setCoords(circCtr.getx() + 150 * cos(theta), circCtr.gety() + 150 * sin(theta));
				glVertex2i(hexVertex.getx(), hexVertex.gety());
			}
		glEnd();
	glEndList();
}
void regHexagon(void)
{
	printf("regHexagon\n");
	glClear(GL_COLOR_BUFFER_BIT);
	glCallList(regHex);
	glFlush();
}
void winReshapeFcn(int newWidth, int newHeight)
{
	printf("winReshapeFcn\n");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//将（newWidth， newHeight）映射到（winWidth， winHeight），再按（newWidth， newHeight）裁切
	//gluOrtho2D(0.0, (GLdouble)newWidth, 0.0, (GLdouble)newHeight);

	// 不改变形状，只按（newWidth， newHeight）裁切
	//gluOrtho2D(0.0, (GLdouble)winWidth, 0.0, (GLdouble)winHeight);

	// 保持图形与窗口的尺寸比例(只能缩小,放大的话因为映射区域的限制，将不能显示完全，参见书上注释)
	gluOrtho2D(0.0, (GLdouble)winWidth * ((GLdouble)winWidth / (GLdouble)newWidth), 0.0, (GLdouble)winHeight * ((GLdouble)winHeight / (GLdouble)newHeight));
	
	glClear(GL_COLOR_BUFFER_BIT);
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Reshape-Function & Display-List Example");
	init();
	glutDisplayFunc(regHexagon);
	glutReshapeFunc(winReshapeFcn);
	glutMainLoop();
}
#endif

#ifdef CHAPTER_4_EXAMPLE_1
GLsizei winWidth = 600, winHeight = 500;
GLint xRaster = 25, yRaster = 150;
GLubyte label[36] = { 'J', 'a', 'n', 'F', 'e', 'b', 'M', 'a', 'r',
					  'A', 'p', 'r', 'M', 'a', 'y', 'J', 'u', 'n', 
					  'J', 'u', 'l', 'A', 'u', 'g', 'S', 'e', 'p',
					  'O', 'c', 't', 'N', 'o', 'v', 'D', 'e', 'c'};
GLint dataValue[12] = { 420, 342, 324, 310, 262, 185,
						190, 196, 217, 240, 312, 438 };
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 600.0, 0.0, 500.0);
}
void lineGraph(void)
{
	GLint month, k;
	GLint x = 30;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_STRIP);
	for (k = 0; k < 12; k++)
		glVertex2i(x + k * 50, dataValue[k]);
	glEnd();
	glColor3f(1.0, 0.0, 0.0);
	xRaster = 25;
	for (k = 0; k < 12; k++)
	{
		glRasterPos2i(xRaster + k * 50, dataValue[k] - 4);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '*');
	}
	glColor3f(0.0, 0.0, 0.0);
	xRaster = 20;
	for (month = 0; month < 12; month++)
	{
		glRasterPos2i(xRaster, yRaster);
		for (k = 3 * month; k < 3 * month + 3; k++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, label[k]);
		xRaster += 50;
	}
	glFlush();
}
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));
	glClear(GL_COLOR_BUFFER_BIT);
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Line Chart Data Plot");
	init();
	glutDisplayFunc(lineGraph);
	glutReshapeFunc(winReshapeFcn);
	glutMainLoop();
}
#endif


#ifdef CHAPTER_4_EXAMPLE_2
GLsizei winWidth = 600, winHeight = 500;
GLint xRaster = 25, yRaster = 150;
GLubyte label[36] = { 'J', 'a', 'n', 'F', 'e', 'b', 'M', 'a', 'r',
'A', 'p', 'r', 'M', 'a', 'y', 'J', 'u', 'n',
'J', 'u', 'l', 'A', 'u', 'g', 'S', 'e', 'p',
'O', 'c', 't', 'N', 'o', 'v', 'D', 'e', 'c' };
GLint dataValue[12] = { 420, 342, 324, 310, 262, 185,
190, 196, 217, 240, 312, 438 };
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 600.0, 0.0, 500.0);
}
void barChart(void)
{
	GLint month, k;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	for (k = 0; k < 12; k++)
		glRecti(20 + k * 50, 165, 40 + k * 50, dataValue[k]);
	glColor3f(0.0, 0.0, 0.0);
	xRaster = 20;
	for (month = 0; month < 12; month++)
	{
		glRasterPos2i(xRaster, yRaster);
		for (k = 3 * month; k < 3 * month + 3; k++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, label[k]);
		xRaster += 50;
	}
	glFlush();
}
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));
	glClear(GL_COLOR_BUFFER_BIT);
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Line Chart Data Plot");
	init();
	glutDisplayFunc(barChart);
	glutReshapeFunc(winReshapeFcn);
	glutMainLoop();
}
#endif

#ifdef CHAPTER_4_EXAMPLE_3
const GLdouble twoPi = 6.283185;
class scrPt
{
public:
	GLint x, y;
};
GLsizei winWidth = 400, winHeight = 300;
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}
void circleMidpoint(scrPt center, GLint radius)
{
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 1000; i++)
	{
		double angle = i * twoPi / 1000;
		glVertex2d(center.x + radius * cos(angle), center.y + radius * sin(angle));
	}
	glEnd();
}
void pieChart(void)
{
	scrPt circCtr, piePt;
	GLint radius = winWidth / 4;
	GLdouble sliceAngle, previousSliceAngle = 0.0;
	GLint k, nSlices = 12;
	GLfloat dataValues[12] = { 10.0, 7.0, 13.0, 5.0, 13.0, 14.0, 3.0, 16.0, 5.0, 3.0, 17.0, 8.0 };
	GLfloat dataSum = 0.0;
	circCtr.x = winWidth / 2;
	circCtr.y = winHeight / 2;
	circleMidpoint(circCtr, radius); // 自己实现
	for (k = 0; k < nSlices; k++)
		dataSum += dataValues[k];
	for (k = 0; k < nSlices; k++)
	{
		sliceAngle = twoPi * dataValues[k] / dataSum + previousSliceAngle;
		piePt.x = circCtr.x + radius * cos(sliceAngle);
		piePt.y = circCtr.y + radius * sin(sliceAngle);
		glBegin(GL_LINES);
		glVertex2i(circCtr.x, circCtr.y);
		glVertex2i(piePt.x, piePt.y);
		glEnd();
		previousSliceAngle = sliceAngle;
	}
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	pieChart();
	glFlush();
}
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));
	glClear(GL_COLOR_BUFFER_BIT);
	winWidth = newWidth;
	winHeight = newHeight;
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Pie Chart");
	init();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	glutMainLoop();
}
#endif

#ifdef CHAPTER_4_EXAMPLE_4
struct screenPt
{
	GLint x;
	GLint y;
};
typedef enum {limacon = 1, cardioid, threeLeaf, fourLeaf, spiral } curveName;
GLsizei winWidth = 600, winHeight = 500;
void init(void)
{
	glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}
void lineSegment(screenPt pt1, screenPt pt2)
{
	glBegin(GL_LINES);
	glVertex2i(pt1.x, pt1.y);
	glVertex2i(pt2.x, pt2.y);
	glEnd();
}
void drawCurve(GLint curveNum)
{
	const GLdouble twoPi = 6.283185;
	const GLint a = 175, b = 60;
	GLfloat r, theta, dtheta = 1.0 / float(a);
	GLint x0 = 200, y0 = 250;
	screenPt curvePt[2];
	glColor3f(0.0, 0.0, 0.0);
	curvePt[0].x = x0;
	curvePt[0].y = y0;
	switch (curveNum)
	{
	case limacon: curvePt[0].x += a + b; break;
	case cardioid: curvePt[0].x += a + a; break;
	case threeLeaf: curvePt[0].x += a; break;
	case fourLeaf: curvePt[0].x += a; break;
	case spiral: break;
	default: break;
	}
	theta = dtheta;
	while (theta < twoPi)
	{
		switch (curveNum)
		{
		case limacon:
			r = a * cos(theta) + b; break;
		case cardioid:
			r = a * (1 + cos(theta)); break;
		case threeLeaf:
			r = a * cos(3 * theta); break;
		case fourLeaf:
			r = a * cos(2 * theta); break;
		case spiral:
			r = (a / 4.0) * theta; break;
		default:	break;
		}
		curvePt[1].x = x0 + r * cos(theta);
		curvePt[1].y = y0 + r * sin(theta);
		lineSegment(curvePt[0], curvePt[1]);
		curvePt[0].x = curvePt[1].x;
		curvePt[0].y = curvePt[1].y;
		theta += dtheta;
	}
}
void displayFcn(void)
{
	GLint curveNum;
	glClear(GL_COLOR_BUFFER_BIT);
	cout << "\nEnter the integer value corresponding to \n";
	cout << "one of the following curve names.\n";
	cout << "Press any other key to exit. \n";
	cout << "\n1-limacon, 2-cardioid, 3-threeLeaf, 4-fourLeaf, 5-spiral:	";
	cin >> curveNum;
	if (curveNum == 1 || curveNum == 2 || curveNum == 3 || curveNum == 4 || curveNum == 5)
		drawCurve(curveNum);
	else
		exit(0);
	glFlush();
}
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));
	glClear(GL_COLOR_BUFFER_BIT);
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Draw Curves");
	init();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	glutMainLoop();
}
#endif

#ifdef CHAPTER_4_EXERCISE_1
struct Point
{
	GLint x, y, z;
};
struct Edge
{
	Point point1, point2;
};
struct Surface
{
	vector<Edge> edges;
};
vector<Point> vertexTable;
vector<Edge> edgeTable;
vector<Surface> surfaceTable;
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	for (auto& edge : edgeTable)
	{
		glBegin(GL_LINES);
		glVertex3i(edge.point1.x, edge.point1.y, edge.point1.z);
		glVertex3i(edge.point2.x, edge.point2.y, edge.point2.z);
		glEnd();
	}
	glFlush();
}
void code_4_exercise_1()
{
	// 顶点表 
	Point v1 = { 0, 0, 0 }, v2 = { 0, 0, 1 }, v3 = { 1, 0, 1 }, v4 = { 1, 0, 0 },\
		  v5 = { 0, 1, 0 }, v6 = { 0, 1, 1 }, v7 = { 1, 1, 1 }, v8 = { 1, 1, 0 };
	vertexTable = { v1, v2, v3, v4, v5, v6, v7, v8 };

	// 边表
	Edge e1 = { v1, v2 }, e2 = { v2, v3 }, e3 = { v3, v4 }, e4 = { v4, v1 }, \
		 e5 = { v5, v1 }, e6 = { v6, v2 }, e7 = { v7, v3 }, e8 = { v8, v4 }, \
		 e9 = { v5, v6 }, e10 = { v6, v7 }, e11 = { v7, v8 }, e12 = { v8, v5 };
	edgeTable = {e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12};

	// 面表
	Surface s1 = { { e1, e5, e9, e6 } }, s2 = { { e2, e7, e10, e6 } }, s3 = { { e3, e8, e11, e7 } }, s4 = { { e4, e8, e12, e5 } }, \
			s5 = { { e9, e10, e11, e12 } }, s6 = { { e1, e2, e3, e4 } };
	surfaceTable = { s1, s2, s3, s4, s5, s6 };

	glLoadIdentity();
	gluOrtho2D(0, 8, 0, 6);

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_4_EXERCISE_2
struct Point
{
	GLint x, y, z;
};
struct Edge
{
	Point point1, point2;
};
struct Surface
{
	vector<Edge> edges;
};
struct Surface_2
{
	vector<Point> points;
};
struct Surface_3
{
	vector<vector<GLint>> points;
};
void code_4_exercise_2()
{
	int size1, size2, size3;
	//////////////////////////////////////////////////////////////////////////
	// 第一种：顶点表，边表，面表
	{
		// 顶点表
		Point v1 = { 0, 0, 0 }, v2 = { 0, 0, 1 }, v3 = { 1, 0, 1 }, v4 = { 1, 0, 0 }, \
			v5 = { 0, 1, 0 }, v6 = { 0, 1, 1 }, v7 = { 1, 1, 1 }, v8 = { 1, 1, 0 };
		vector<Point> vertexTable = { v1, v2, v3, v4, v5, v6, v7, v8 };

		// 边表
		Edge e1 = { v1, v2 }, e2 = { v2, v3 }, e3 = { v3, v4 }, e4 = { v4, v1 }, \
			e5 = { v5, v1 }, e6 = { v6, v2 }, e7 = { v7, v3 }, e8 = { v8, v4 }, \
			e9 = { v5, v6 }, e10 = { v6, v7 }, e11 = { v7, v8 }, e12 = { v8, v5 };
		vector<Edge> edgeTable = { e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12 };

		// 面表
		Surface s1 = { { e1, e5, e9, e6 } }, s2 = { { e2, e7, e10, e6 } }, s3 = { { e3, e8, e11, e7 } }, s4 = { { e4, e8, e12, e5 } }, \
			s5 = { { e9, e10, e11, e12 } }, s6 = { { e1, e2, e3, e4 } };
		vector<Surface> surfaceTable = { s1, s2, s3, s4, s5, s6 };

		size1 = sizeof(vertexTable) + sizeof(edgeTable) + sizeof(surfaceTable);
	}

	//////////////////////////////////////////////////////////////////////////
	// 第二种：顶点表，面表
	{
		// 顶点表 
		Point v1 = { 0, 0, 0 }, v2 = { 0, 0, 1 }, v3 = { 1, 0, 1 }, v4 = { 1, 0, 0 }, \
			v5 = { 0, 1, 0 }, v6 = { 0, 1, 1 }, v7 = { 1, 1, 1 }, v8 = { 1, 1, 0 };
		vector<Point> vertexTable = { v1, v2, v3, v4, v5, v6, v7, v8 };

		// 面表
		Surface_2 s1 = { { v2, v1, v5, v6 } }, s2 = { { v2, v3, v7, v6 } }, s3 = { { v3, v4, v8, v7 } }, s4 = { { v1, v4, v8, v5 } }, \
			s5 = { { v6, v7, v8, v5 } }, s6 = { { v2, v3, v4, v1 } };
		vector<Surface_2> surfaceTable = { s1, s2, s3, s4, s5, s6 };

		size2 = sizeof(vertexTable) + sizeof(surfaceTable);
	}

	//////////////////////////////////////////////////////////////////////////
	// 第三种：面表
	{
		// 面表
		Surface_3 s1 = { { { 0, 0, 1 }, { 0, 0, 0 }, { 0, 1, 0 }, { 0, 1, 1 } } },\
			s2 = { { { 0, 0, 1 }, { 1, 0, 1 }, { 1, 1, 1 }, { 0, 1, 1 } } },\
			s3 = { { { 1, 0, 1 }, { 1, 0, 0 }, { 1, 1, 0 }, { 1, 1, 1 } } },\
			s4 = { { { 0, 0, 0 }, { 1, 0, 0 }, { 1, 1, 0 }, { 0, 1, 0 } } },\
			s5 = { { { 0, 1, 1 }, { 1, 1, 1 }, { 1, 1, 0 }, { 0, 1, 0 } } },\
			s6 = { { { 0, 0, 1 }, { 1, 0, 1 }, { 1, 0, 0 }, { 0, 0, 0 } } };
		vector<Surface_3> surfaceTable = { s1, s2, s3, s4, s5, s6 };
		size3 = sizeof(surfaceTable);
	}
	printf("size1=%d\n", size1);
	printf("size2=%d\n", size2);
	printf("size3=%d\n", size3);
}
#endif

#ifdef CHAPTER_4_EXERCISE_3
struct Point
{
	GLint x, y, z;
};
vector<vector<Point>> polygon = 
{
	{ { 0, 0, 1 }, { 0, 0, 0 }, { 0, 1, 0 }, { 0, 1, 1 } },
	{ { 0, 0, 1 }, { 1, 0, 1 }, { 1, 1, 1 }, { 0, 1, 1 } },
	{ { 1, 0, 1 }, { 1, 0, 0 }, { 1, 1, 0 }, { 1, 1, 1 } },
	{ { 0, 0, 0 }, { 1, 0, 0 }, { 1, 1, 0 }, { 0, 1, 0 } },
	{ { 0, 1, 1 }, { 1, 1, 1 }, { 1, 1, 0 }, { 0, 1, 0 } },
	{ { 0, 0, 1 }, { 1, 0, 1 }, { 1, 0, 0 }, { 0, 0, 0 } }
};
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	for (auto& surface : polygon)
	{
		int i = 0;
		for (int i = 0; i < surface.size(); i++)
		{
			int j = i + 1 >= surface.size() ? 0 : i + 1;
			glBegin(GL_LINES);
			glVertex3i(surface[i].x, surface[i].y, surface[i].z);
			glVertex3i(surface[j].x, surface[j].y, surface[j].z);
			glEnd();
		}
	}
	glFlush();
}
void code_4_exercise_3()
{
	glLoadIdentity();
	gluOrtho2D(0, 8, 0, 6);

	glutDisplayFunc(displayFcn);
}
#endif

#ifdef CHAPTER_4_EXERCISE_4
struct Point
{
	GLint x, y, z;
};
struct Surface;
struct Edge
{
	Point point1, point2;
	vector<Surface*> pSurface;
};
struct Surface
{
	vector<Edge> edges;
	void setEdges(const vector<Edge>& eds){ edges = eds; }
};
// 顶点表
Point v1 = { 0, 0, 0 }, v2 = { 0, 0, 1 }, v3 = { 1, 0, 1 }, v4 = { 1, 0, 0 }, \
v5 = { 0, 1, 0 }, v6 = { 0, 1, 1 }, v7 = { 1, 1, 1 }, v8 = { 1, 1, 0 };
vector<Point> vertexTable = { v1, v2, v3, v4, v5, v6, v7, v8 };

// 边表
Surface s1, s2, s3, s4, s5, s6;
Edge e1 = { v1, v2, { &s1, &s6 } }, e2 = { v2, v3, { &s2, &s6 } }, e3 = { v3, v4, { &s3, &s6 } }, e4 = { v4, v1, {&s4, &s6} }, \
e5 = { v5, v1, { &s1, &s4 } }, e6 = { v6, v2, { &s1, &s2 } }, e7 = { v7, v3, { &s2, &s3 } }, e8 = { v8, v4, {&s3, &s4} }, \
e9 = { v5, v6, { &s1, &s5 } }, e10 = { v6, v7, { &s2, &s5 } }, e11 = { v7, v8, { &s3, &s5 } }, e12 = { v8, v5, {&s4,&s5} };
vector<Edge> edgeTable = { e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12 };

// 面表
vector<Surface> surfaceTable;
void initialize()
{
	s1.setEdges({ e1, e5, e9, e6 });
	s2.setEdges({ e2, e7, e10, e6 });
	s3.setEdges({ e3, e8, e11, e7 });
	s4.setEdges({ e4, e8, e12, e5 });
	s5.setEdges({ e9, e10, e11, e12 });
	s6.setEdges({ e1, e2, e3, e4 });
	surfaceTable = { s1, s2, s3, s4, s5, s6 };
}
inline bool operator==(const Point& p1, const Point& p2)
{
	return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}
inline bool operator<(const Point& p1, const Point& p2)
{
	return p1.x < p2.x && p1.y < p2.y && p1.z < p2.z;
}
inline bool operator!=(const Point& p1, const Point& p2)
{
	return !(p1 == p2);
}
inline bool operator==(const Edge& e1, const Edge& e2)
{
	return e1.point1 == e2.point1 && e1.point2 == e2.point2;
}
inline bool operator!=(const Edge& e1, const Edge& e2)
{
	return !(e1 == e2);
}
inline bool operator==(const Surface& s1, const Surface& s2)
{
	return s1.edges == s2.edges;
}
inline bool operator!=(const Surface& s1, const Surface& s2)
{
	return !(s1 == s2);
}
bool checkVertex()
{
	map<Point, int> pointCount;
	for (auto point : vertexTable)
	{
		pointCount[point] = 0;
		for (auto&edge : edgeTable)
		{
			if (point == edge.point1)
				pointCount[point]++;
			if (point== edge.point2)
				pointCount[point]++;
		}
	}
	for (auto point : vertexTable)
	{
		if (pointCount[point] < 2)
			return false;
	}
	return true;
}
bool checkEdge()
{
	for (auto edge : edgeTable)
	{
		bool inSurface = false;
		for (auto surface : surfaceTable)
		{
			for (auto e : surface.edges)
			{
				if (e == edge)
					inSurface = true;
			}
		}
		if (!inSurface)
			return false;
	}
	return true;
}
bool checkSurfaceClose()
{
	for (auto surface : surfaceTable)
	{
		vector<Point> alonePoints ;
		for (int i = 0; i < surface.edges.size(); i++)
		{
			bool find = false;
			for (vector<Point>::iterator it = alonePoints.begin(); it != alonePoints.end(); it++)
			{
				if (surface.edges[i].point1 == *it)
				{
					alonePoints.erase(it);
					find = true;
					break;
				}
			}
			if (!find)
				alonePoints.push_back(surface.edges[i].point1);

			find = false;
			for (vector<Point>::iterator it = alonePoints.begin(); it != alonePoints.end(); it++)
			{
				if (surface.edges[i].point2 == *it)
				{
					alonePoints.erase(it);
					find = true;
					break;
				}
			}
			if (!find)
				alonePoints.push_back(surface.edges[i].point2);
		}
		if (!alonePoints.empty())
			return false;
	}
	return true;
}
bool checkShareEdge()
{
	for (auto& checkSurface : surfaceTable)
	{
		bool share = false;
		for (auto& s : surfaceTable)
		{
			if (s != checkSurface)
			{
				for (auto& checkEdge : checkSurface.edges)
				{
					for (auto& e : s.edges)
					{
						share = true;
						goto check;
					}
				}
			}
		}
	check:
		if (!share)
			return false;
	}
	return true;
}
bool checkEdgeSurfaceConsistency()
{
	for (auto& checkSurface : surfaceTable)
	{
		for (auto& edge : checkSurface.edges)
		{
			bool consistency = false;
			for (auto& ps : edge.pSurface)
			{
				if (*ps == checkSurface)
				{
					consistency = true;
					break;
				}
			}
			if (!consistency)
				return false;
		}
	}
	return true;
}
void code_4_exercise_4()
{
	initialize();
	printf("每一顶点至少有两条边以其作为端点：%s\n", checkVertex() ? "是":"否");
	printf("每条边至少是一个多边形的组成部分：%s\n", checkEdge() ? "是": "否");
	printf("每一个多边形都是封闭的：%s\n", checkSurfaceClose() ? "是": "否");
	printf("每个多边形至少有一条共享边：%s\n", checkShareEdge() ? "是":"否");
	printf("如果边表包含指向多边形的指针，那么由多边形指针引用的每条边都有一个反向指针指回该多边形：%s\n", checkEdgeSurfaceConsistency() ? "是": "否");
}
#endif
#ifdef CHAPTER_4_EXERCISE_5
void code_4_exercise_5()
{
	// 左：A = -1, B = 0, C = 0, D = 0
	// 前：A = 0, B = 0, C = 1, D = -1
	// 右：A = 1, B = 0, C = 0, D = -1
	// 后：A = 0 B = 0, C = -1, D = 0
	// 上：A = 0, B = 1, C = 0, D = -1
	// 下：A = 0, B = -1, C = 0, D = 0
}
#endif

#ifdef CHAPTER_4_EXERCISE_6
struct Point
{
	GLint x, y, z;
};
struct Surface
{
	vector<Point> points;
};
void calc(Surface s)
{
	int x1 = s.points[0].x, y1 = s.points[0].y, z1 = s.points[0].z;
	int x2 = s.points[1].x, y2 = s.points[1].y, z2 = s.points[1].z;
	int x3 = s.points[2].x, y3 = s.points[2].y, z3 = s.points[2].z;
	int A = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
	int B = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
	int C = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
	int D = -x1*(y2*z3 - y3*z2) - x2*(y3*z1 - y1*z3) - x3*(y1*z2 - y2*z1);
	printf("A=%d, B=%d, C=%d, D=%d\n", A, B, C, D);
}
void code_4_exercise_6()
{
	Point v1 = { 0, 0, 0 }, v2 = { 0, 0, 1 }, v3 = { 1, 0, 1 }, v4 = { 1, 0, 0 }, \
		v5 = { 0, 1, 0 }, v6 = { 0, 1, 1 }, v7 = { 1, 1, 1 }, v8 = { 1, 1, 0 };
	Surface s1 = { { v1, v2, v6, v5 } }, s2 = { { v2, v3, v7, v6 } }, s3 = { { v3, v4, v8, v7 } }, s4 = { { v1, v5, v8, v4 } }, \
		s5 = { { v5, v6, v7, v8 } }, s6 = { { v1, v4, v3, v2 } };
	vector<Surface> surfaceTable = { s1, s2, s3, s4, s5, s6 };
	calc(s1);
	calc(s2);
	calc(s3);
	calc(s4);
	calc(s5);
	calc(s6);
}
#endif

#ifdef CHAPTER_4_EXERCISE_7
struct Point
{
	GLint x, y, z;
};
void checkPoint(Point p)
{
	int A = -1, B = 0, C = 0, D = 0;
	int ret = A * p.x + B * p.y + C * p.z + D;
	if (ret > 0)
		printf("点（%d, %d, %d）在平面前方\n", p.x, p.y, p.z);
	else if (ret == 0)
		printf("点（%d, %d, %d）在平面上\n", p.x, p.y, p.z);
	else
		printf("点（%d, %d, %d）在平面后方\n", p.x, p.y, p.z);
}
void code_4_exercise_7()
{
	checkPoint({-1, 0, -1});
	checkPoint({ 0, 0, 1 });
	checkPoint({ 1, 1, 1 });
}
#endif

#ifdef CHAPTER_4_EXERCISE_8
struct Point
{
	GLfloat x, y, z;
};
struct Surface
{
	vector<Point> points;
};
Point v1 = { 0, 0, 0 }, v2 = { 0, 0, 1 }, v3 = { 1, 0, 1 }, v4 = { 1, 0, 0 }, \
v5 = { 0, 1, 0 }, v6 = { 0, 1, 1 }, v7 = { 1, 1, 1 }, v8 = { 1, 1, 0 };
Surface s1 = { { v1, v2, v6, v5 } }, s2 = { { v2, v3, v7, v6 } }, s3 = { { v3, v4, v8, v7 } }, s4 = { { v1, v5, v8, v4 } }, \
s5 = { { v5, v6, v7, v8 } }, s6 = { { v1, v4, v3, v2 } };
vector<Surface> surfaceTable = { s1, s2, s3, s4, s5, s6 };
void checkPoint(Point p)
{
	bool teston = false;
	for (auto& s : surfaceTable)
	{
		int x1 = s.points[0].x, y1 = s.points[0].y, z1 = s.points[0].z;
		int x2 = s.points[1].x, y2 = s.points[1].y, z2 = s.points[1].z;
		int x3 = s.points[2].x, y3 = s.points[2].y, z3 = s.points[2].z;
		int A = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
		int B = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
		int C = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
		int D = -x1*(y2*z3 - y3*z2) - x2*(y3*z1 - y1*z3) - x3*(y1*z2 - y2*z1);
		if (A * p.x + B * p.y + C * p.z + D > 0)
		{
			printf("点（%f, %f, %f）在立方体外部\n", p.x, p.y, p.z);
			return;
		}
		else if (A * p.x + B * p.y + C * p.z + D == 0)
		{
			teston = true;
		}
	}
	if (teston)
	{
		printf("点（%f, %f, %f）在立方体表面\n", p.x, p.y, p.z);
	}
	else
	{
		printf("点（%f, %f, %f）在立方体内部\n", p.x, p.y, p.z);
	}
}
void code_4_exercise_8()
{
	checkPoint({ 2, 1, 1 });
	checkPoint({ 0.5, 0.5, 0.5 });
	checkPoint({ 1, 0.5, 0.5 });
	checkPoint({ 1, 1, 1 });
}
#endif

#ifdef CHAPTER_4_EXERCISE_9
void code_4_exercise_9()
{
	// 如果变为左手坐标系，则Ax+By+Cz+D>0表示内部，Ax+By+Cz+D<0表示外部.因为从外部看向内部，逆时针正方向就是坐标轴正方向，
	//而左右手坐标系坐标轴正方向正好相反，即在右手系中的外部,正好是左手系的内部（比如对于立方体的最外面,左手Z>0表示外部，右手Z>0表示内部）
}
#endif

#ifdef CHAPTER_4_EXERCISE_10
struct Point
{
	GLfloat x, y, z;
};
bool check(Point p)
{
	return 15 * p.x + 21 * p.y + 9 * p.z + 0 == 0;
}
void code_4_exercise_10()
{
	if (check({ 2, -1, -1 }) && check({ 1, -2, 2 }))
		printf("五点共面\n");
	else
		printf("五点不共面\n");
}
#endif

#ifdef CHAPTER_4_EXERCISE_11
struct Point
{
	GLfloat x, y, z;
};
void check(const vector<Point>& vertexTable)
{
	assert (vertexTable.size() == 4);
	
	int x1 = vertexTable[0].x, y1 = vertexTable[0].y, z1 = vertexTable[0].z;
	int x2 = vertexTable[1].x, y2 = vertexTable[1].y, z2 = vertexTable[1].z;
	int x3 = vertexTable[2].x, y3 = vertexTable[2].y, z3 = vertexTable[2].z;
	int A = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
	int B = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
	int C = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
	int D = -x1*(y2*z3 - y3*z2) - x2*(y3*z1 - y1*z3) - x3*(y1*z2 - y2*z1);
	int ret = A * vertexTable[3].x + B * vertexTable[3].y + C * vertexTable[3].z + D;
	if (ret == 0)
		printf("四边形共面\n");
	else
		printf("四边形不共面\n");

}
void code_4_exercise_11()
{
	check({ { 0, 0, 0 }, { 0, 0, 1 }, { 1, 0, 1 }, { 1, 0, 0 } });
	check({ { 0, 0, 0 }, { 0, 0, 1 }, { 1, 0, 1 }, { 1, 1, 1 } });
}
#endif

#ifdef CHAPTER_4_EXERCISE_12
struct Point
{
	GLfloat x, y, z;
};
void check(const vector<Point>& vertexTable)
{
	assert(vertexTable.size() >= 4);

	int x1 = vertexTable[0].x, y1 = vertexTable[0].y, z1 = vertexTable[0].z;
	int x2 = vertexTable[1].x, y2 = vertexTable[1].y, z2 = vertexTable[1].z;
	int x3 = vertexTable[2].x, y3 = vertexTable[2].y, z3 = vertexTable[2].z;
	int A = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
	int B = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
	int C = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
	int D = -x1*(y2*z3 - y3*z2) - x2*(y3*z1 - y1*z3) - x3*(y1*z2 - y2*z1);
	for (int i = 3; i < vertexTable.size(); i++)
	{
		if (A * vertexTable[i].x + B * vertexTable[i].y + C * vertexTable[i].z + D != 0)
		{
			printf("多边形不共面\n");
			return;
		}
	}
	printf("多边形共面\n");
}
void code_4_exercise_12()
{
	check({ { 0, 0, 0 }, { 340, 30, 0 }, { 154, 0, 0 }, { 1, 45, 0 }, { 432, 4, 1 }, { 90, 45, 0 } });
	check({ { 0, 2, 0 }, { 340, 2, 0 }, { 154, 2, 0 }, { 1, 2, 0 }, { 432, 2, 1 }, { 90, 2, 0 } });
}
#endif

#ifdef CHAPTER_4_EXERCISE_13
struct Point
{
	GLfloat x, y, z;
};
vector<vector<Point>> ploygons;
void drowPolygon(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	int j = 0;
	for (auto& polygon : ploygons)
	{
		for (int i = 0; i < polygon.size(); ++i)
		{
			j = i + 1 >= polygon.size() ? 0 : i + 1;
			glBegin(GL_LINES);
			glVertex2i(polygon[i].x, polygon[i].y);
			glVertex2i(polygon[j].x, polygon[j].y);
			glEnd();
		}
	}
	glFlush();
}
void cut(const vector<Point>& vertex, vector<vector<Point>>& triangles)
{
	vector<Point> points = vertex;

	if (points.size() < 3)
		return;

	triangles.push_back({ points[0], points[1], points[2] });
	points.erase(points.begin() + 1);
	cut(points, triangles);
}

void code_4_exercise_13()
{
	cut({ { 0, 0, 0 }, { 100, 0, 0 }, { 100, 100, 0 }, { 0, 100, 0 } }, ploygons);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drowPolygon);
}
#endif

#ifdef CHAPTER_4_EXERCISE_14
struct Point
{
	GLfloat x, y, z;
};
vector<vector<Point>> ploygons;
void drowPolygon(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	int j = 0;
	for (auto& polygon : ploygons)
	{
		for (int i = 0; i < polygon.size(); ++i)
		{
			j = i + 1 >= polygon.size() ? 0 : i + 1;
			glBegin(GL_LINES);
			glVertex2i(polygon[i].x, polygon[i].y);
			glVertex2i(polygon[j].x, polygon[j].y);
			glEnd();
		}
	}
	glFlush();
}
void cut(const vector<Point>& vertex, vector<vector<Point>>& triangles)
{
	vector<Point> points = vertex;

	if (points.size() < 3)
		return;

	triangles.push_back({ points[0], points[1], points[2] });
	points.erase(points.begin() + 1);
	cut(points, triangles);
}

void code_4_exercise_14()
{
	cut({ { 416, 287, 0 }, { 367, 247, 0 }, { 361, 201, 0 }, { 388, 176, 0 }, \
	{ 483, 165, 0 }, { 530, 220, 0 }, { 516, 280, 0 }, { 480, 292, 0 } }, ploygons);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drowPolygon);
}
#endif

#ifdef CHAPTER_4_EXERCISE_15
struct Point
{
	GLfloat x, y, z;
};
vector<vector<Point>> ploygons;
void drowPolygon(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	int j = 0;
	for (auto& polygon : ploygons)
	{
		for (int i = 0; i < polygon.size(); ++i)
		{
			j = i + 1 >= polygon.size() ? 0 : i + 1;
			glBegin(GL_LINES);
			glVertex2i(polygon[i].x, polygon[i].y);
			glVertex2i(polygon[j].x, polygon[j].y);
			glEnd();
		}
	}
	glFlush();
}
void cut(const vector<Point>& vertex, vector<vector<Point>>& triangles)
{
	vector<Point> points = vertex;

	if (points.size() < 3)
		return;

	triangles.push_back({ points[0], points[1], points[2] });
	points.erase(points.begin() + 1);
	cut(points, triangles);
}

void code_4_exercise_15()
{
	cut({ { 504, 427, 0 }, { 553, 358, 0 }, { 666, 415, 0 }, { 626, 483, 0 }, { 551, 506, 0 } }, ploygons);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drowPolygon);
}
#endif

#ifdef CHAPTER_4_EXERCISE_16
struct Point
{
	GLfloat x, y, z;
};
inline bool operator==(const Point& p1, const Point& p2)
{
	return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}
inline bool operator<(const Point& p1, const Point& p2)
{
	return p1.x < p2.x && p1.y < p2.y && p1.z < p2.z;
}
void check(const vector<Point>& ploygon)
{
	for (int i = 0; i < ploygon.size(); i++)
	{
		for (int j = i + 1; j < ploygon.size(); j++)
		{
			if (ploygon[i] == ploygon[j])
			{
				printf("多边形是有重复顶点或共线顶点的退化多边形\n");
				return;
			}
		}
	}
	printf("多边形是非退化多边形\n");
}
void code_4_exercise_16()
{
	check({ { 0, 0, 0 }, { 100, 0, 0 }, { 100, 100, 0 }, { 0, 100, 0 } });
	check({ { 0, 0, 0 }, { 100, 0, 0 }, { 100, 100, 0 }, { 0, 100, 0 }, { 0, 0, 0 } });
}
#endif

#ifdef CHAPTER_4_EXERCISE_17
struct Point
{
	GLfloat x, y, z;
};
struct Edge
{
	Point point1, point2;
};
vector<Point> ploygon1 = { { 262, 160, 0 }, { 128, 257, 0 }, { 243, 376, 0 }, { 344, 293, 0 }, {100,346,0} };
vector<Point> ploygon2 = { { 557, 187, 0 }, { 463, 303, 0 }, { 632, 504, 0 }, {697,195,0} };
void drowPolygon(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	int j = 0;
	for (int i = 0; i < ploygon1.size(); ++i)
	{
		j = i + 1 >= ploygon1.size() ? 0 : i + 1;
		glBegin(GL_LINES);
		glVertex2i(ploygon1[i].x, ploygon1[i].y);
		glVertex2i(ploygon1[j].x, ploygon1[j].y);
		glEnd();
	}
	for (int i = 0; i < ploygon2.size(); ++i)
	{
		j = i + 1 >= ploygon2.size() ? 0 : i + 1;
		glBegin(GL_LINES);
		glVertex2i(ploygon2[i].x, ploygon2[i].y);
		glVertex2i(ploygon2[j].x, ploygon2[j].y);
		glEnd();
	}
	glFlush();
}
// 计算两条直线的交点（直线公式：Ax+By+C=0）
bool linesPoint(float A1, float B1, float C1, float A2, float B2, float C2, Point& point)
{
	if (equal(A1 * B2, A2 * B1))
		return false;

	point.x = (B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1);
	point.y = (A2 * C1 - A1 * C2) / (A1 * B2 - A2 * B1);
	return true;
}
void check(const vector<Point>& ploygon)
{
	vector<Edge> edges;
	for (int i = 0; i < ploygon.size(); i++)
	{
		int j = i + 1 < ploygon.size() ? i + 1 : 0;
		edges.push_back({ ploygon[i], ploygon[j]});
	}
	for (int i = 0; i < edges.size(); i++)
	{
		for (int j = i + 1; j < edges.size(); j++)
		{
			float A1 = edges[i].point2.y - edges[i].point1.y;
			float B1 = edges[i].point1.x - edges[i].point2.x;
			float C1 = edges[i].point2.x * edges[i].point1.y - edges[i].point1.x * edges[i].point2.y;
			float A2 = edges[j].point2.y - edges[j].point1.y;
			float B2 = edges[j].point1.x - edges[j].point2.x;
			float C2 = edges[j].point2.x * edges[j].point1.y - edges[j].point1.x * edges[j].point2.y;
			Point point;
			if (linesPoint(A1, B1, C1, A2, B2, C2, point))
			{
				float E1Xmax = edges[i].point1.x > edges[i].point2.x ? edges[i].point1.x : edges[i].point2.x;
				float E1Xmin = edges[i].point1.x > edges[i].point2.x ? edges[i].point2.x : edges[i].point1.x;
				float E1Ymax = edges[i].point1.y > edges[i].point2.y ? edges[i].point1.y : edges[i].point2.y;
				float E1Ymin = edges[i].point1.y > edges[i].point2.y ? edges[i].point2.y : edges[i].point1.y;
				
				float E2Xmax = edges[j].point1.x > edges[j].point2.x ? edges[j].point1.x : edges[j].point2.x;
				float E2Xmin = edges[j].point1.x > edges[j].point2.x ? edges[j].point2.x : edges[j].point1.x;
				float E2Ymax = edges[j].point1.y > edges[j].point2.y ? edges[j].point1.y : edges[j].point2.y;
				float E2Ymin = edges[j].point1.y > edges[j].point2.y ? edges[j].point2.y : edges[j].point1.y;

				if (point.x > E1Xmin && point.x < E1Xmax && point.x > E2Xmin && point.x < E2Xmax &&
					point.y > E1Ymin && point.y < E1Ymax && point.y > E2Ymin && point.y < E2Ymax)
				{
					printf("多边形是自相交多边形\n");
					return;
				}
			}
		}
	}
	printf("多边形不是自相交多边形\n");
}
void code_4_exercise_17()
{
	check(ploygon1);
	check(ploygon2);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drowPolygon);
}
#endif

#ifdef CHAPTER_4_EXERCISE_18
struct Vector
{
	float x, y;
};
struct Point
{
	GLfloat x, y, z;
};
struct Edge
{
	Point point1, point2;
};
vector<Point> ploygon1 = { { 63, 272, 0 }, { 126, 479, 0 }, { 245, 267, 0 }, { 150, 327, 0 }};
vector<Point> ploygon2 = { { 425, 189, 0 }, { 449, 473, 0 }, { 621, 292, 0 }};
void drowPolygon(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	int j = 0;
	for (int i = 0; i < ploygon1.size(); ++i)
	{
		j = i + 1 >= ploygon1.size() ? 0 : i + 1;
		glBegin(GL_LINES);
		glVertex2i(ploygon1[i].x, ploygon1[i].y);
		glVertex2i(ploygon1[j].x, ploygon1[j].y);
		glEnd();
	}
	for (int i = 0; i < ploygon2.size(); ++i)
	{
		j = i + 1 >= ploygon2.size() ? 0 : i + 1;
		glBegin(GL_LINES);
		glVertex2i(ploygon2[i].x, ploygon2[i].y);
		glVertex2i(ploygon2[j].x, ploygon2[j].y);
		glEnd();
	}
	glFlush();
}
int crossProduct(const Vector& vector1, const Vector& vector2)
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}
bool isConvexPolygon(const vector<Point>& ploygon)
{
	vector<Edge> edges;
	for (int i = 0; i < ploygon.size(); i++)
	{
		int j = i + 1 < ploygon.size() ? i + 1 : 0;
		edges.push_back({ ploygon[i], ploygon[j] });
	}
	int j = 0;
	float last = 0;
	for (int i = 0; i < edges.size(); ++i)
	{
		j = i + 1 >= edges.size() ? 0 : i + 1;
		float xp = crossProduct({ edges[i].point2.x - edges[i].point1.x, edges[i].point2.y - edges[i].point1.y }, 
		{ edges[j].point2.x - edges[j].point1.x, edges[j].point2.y - edges[j].point1.y });
		if (!equal(xp, 0))
		{
			if (last == 0)
			{
				last = xp;
			}
			else
			{
				if (last * xp < 0)
				{
					printf("多边形是凹多边形\n");
					return false;
				}
			}
		}
	}
	printf("多边形是凸多边形\n");
	return true;
}

void code_4_exercise_18()
{
	isConvexPolygon(ploygon1);
	isConvexPolygon(ploygon2);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drowPolygon);
}
#endif

#ifdef CHAPTER_4_EXERCISE_19
struct Vector
{
	float x, y;
};
struct Point
{
	GLfloat x, y, z;
};
inline bool operator==(const Point& p1, const Point& p2)
{
	return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}
inline bool operator<(const Point& p1, const Point& p2)
{
	return p1.x < p2.x && p1.y < p2.y && p1.z < p2.z;
}
vector<Point> ploygon = { { 387, 139, 0 }, { 586, 458, 0 }, { 388, 394, 0 }, { 274, 456, 0 } };
vector<vector<Point>> drawPloygons;
void drowPolygon(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	int j = 0;
	for (auto& ploygon : drawPloygons)
	{
		for (int i = 0; i < ploygon.size(); ++i)
		{
			j = i + 1 >= ploygon.size() ? 0 : i + 1;
			glBegin(GL_LINES);
			glVertex2i(ploygon[i].x, ploygon[i].y);
			glVertex2i(ploygon[j].x, ploygon[j].y);
			glEnd();
		}
	}
	glFlush();
}
int crossProduct(const Vector& vector1, const Vector& vector2)
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}
// 计算两条直线的交点（直线公式：Ax+By+C=0）
bool linesPoint(float A1, float B1, float C1, float A2, float B2, float C2, Point& point)
{
	if (equal(A1 * B2, A2 * B1))
		return false;

	point.x = (B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1);
	point.y = (A2 * C1 - A1 * C2) / (A1 * B2 - A2 * B1);
	point.z = 0;
	return true;
}
void cutPolygon(vector<Point>& ploygon)
{
	vector<Vector> vectors;
	for (int i = 0; i < ploygon.size(); i++)
	{
		int j = i + 1 < ploygon.size() ? i + 1 : 0;
		vectors.push_back({ ploygon[j].x - ploygon[i].x, ploygon[j].y - ploygon[i].y });
	}
	int j;
	int index = -1;
	for (int i = 0; i < vectors.size(); ++i)
	{
		j = i + 1 >= vectors.size() ? 0 : i + 1;
		float xp = crossProduct(vectors[i], vectors[j]);
		if (xp < 0)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		drawPloygons.push_back(ploygon);
		return;
	}
	Point cutStartP = ploygon[index];
	Point cutEndP = ploygon[index + 1 < ploygon.size() ? index + 1 : 0];
	float A1 = cutEndP.y - cutStartP.y;
	float B1 = cutStartP.x - cutEndP.x;
	float C1 = cutEndP.x * cutStartP.y - cutStartP.x * cutEndP.y;

	Point cutPoint;
	float minDis = 0;
	int curIndex = 0;
	for (int i = 0; i < ploygon.size(); i++)
	{
		int j = i + 1 < ploygon.size() ? i + 1 : 0;
		float A2 = ploygon[j].y - ploygon[i].y;
		float B2 = ploygon[i].x - ploygon[j].x;
		float C2 = ploygon[j].x * ploygon[i].y - ploygon[i].x * ploygon[j].y;
		Point point;
		if (linesPoint(A1, B1, C1, A2, B2, C2, point))
		{
			float Xmax = ploygon[i].x > ploygon[j].x ? ploygon[i].x : ploygon[j].x;
			float Xmin = ploygon[i].x > ploygon[j].x ? ploygon[j].x : ploygon[i].x;
			float Ymax = ploygon[i].y > ploygon[j].y ? ploygon[i].y : ploygon[j].y;
			float Ymin = ploygon[i].y > ploygon[j].y ? ploygon[j].y : ploygon[i].y;

			if (point.x > Xmin && point.x < Xmax && point.y > Ymin && point.y < Ymax)
			{
				if (crossProduct({ cutEndP.x - cutStartP.x, cutEndP.x - cutStartP.x }, { ploygon[j].x - ploygon[i].x, ploygon[j].y - ploygon[i].y }) > 0)
				{
					float dis = (cutEndP.y - point.y) * (cutEndP.y - point.y) + (cutEndP.x - point.x) * (cutEndP.x - point.x);
					if (dis < minDis || minDis == 0)
					{
						cutPoint = point;
						curIndex = i;
					}
				}
			}
		}
	}
	vector<Point> newPloygon = ploygon;
	newPloygon.insert(newPloygon.begin() + curIndex + 1, cutPoint);

	vector<Point> newPloygon1;
	vector<Point> newPloygon2;

	bool f = false;
	for (int i = 0; i < newPloygon.size(); i++)
	{
		if (newPloygon[i] == cutPoint || newPloygon[i] == cutEndP)
		{
			if (!f)
			{
				newPloygon1.push_back(newPloygon[i]);
			}
			else
			{
				newPloygon2.push_back(newPloygon[i]);
			}
			f = !f;
		}		

		if (!f)
		{
			newPloygon1.push_back(newPloygon[i]);
		}
		else
		{
			newPloygon2.push_back(newPloygon[i]);
		}
	}
	cutPolygon(newPloygon1);
	cutPolygon(newPloygon2);
}
void code_4_exercise_19()
{
	cutPolygon(ploygon);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drowPolygon);
}
#endif

#ifdef CHAPTER_4_EXERCISE_20
struct Vector
{
	float x, y;
};
struct Point
{
	GLfloat x, y, z;
};
vector<Point> ploygon = { { 105, 44, 0 }, { 328, 250, 0 }, { 148, 135, 0 }, { 55, 200, 0 } };
vector<vector<Point>> drawPloygons;
inline bool operator==(const Point& p1, const Point& p2)
{
	return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}
inline bool operator<(const Point& p1, const Point& p2)
{
	return p1.x < p2.x && p1.y < p2.y && p1.z < p2.z;
}
// 计算两条直线的交点（直线公式：Ax+By+C=0）
bool linesPoint(float A1, float B1, float C1, float A2, float B2, float C2, Point& point)
{
	if (equal(A1 * B2, A2 * B1))
		return false;

	point.x = (B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1);
	point.y = (A2 * C1 - A1 * C2) / (A1 * B2 - A2 * B1);
	point.z = 0;
	return true;
}
void drowPolygon(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	int j = 0;
	for (auto& ploygon : drawPloygons)
	{
		for (int i = 0; i < ploygon.size(); ++i)
		{
			j = i + 1 >= ploygon.size() ? 0 : i + 1;
			glBegin(GL_LINES);
			glVertex2i(ploygon[i].x, ploygon[i].y);
			glVertex2i(ploygon[j].x, ploygon[j].y);
			glEnd();
		}
	}
	glFlush();
}
vector<Point> offset(Vector vec, const vector<Point>& ploygon)
{
	vector<Point> ret;
	for (auto& point : ploygon)
		ret.push_back({ point.x + vec.x, point.y + vec.y, 0 });
	return ret;
}
vector<Point> rotate(Point origin, float radius, const vector<Point>& ploygon)
{
	vector<Point> ret;
	for (auto& point : ploygon)
	{
		Point newPoint = point;

		Vector vec = { point.x - origin.x, point.y - origin.y };
		if (!equal(vec.x, 0))
		{
			float oldRadius = std::atan(vec.y / vec.x);
			float newRadius = oldRadius + radius;
			newPoint.x = std::sqrt((vec.x * vec.x + vec.y * vec.y) / (1 + std::tan(newRadius)));
			newPoint.y = newPoint.x * std::tan(newRadius);
		}
		ret.push_back(newPoint);
	}
	return ret;
}
void cutPolygon(const vector<Point>& ploygon)
{
	vector<Point> newPloygon;
	for (int i = 0; i < ploygon.size(); i++)
	{
		int rotateIndex = (i + 1) % ploygon.size();
		int testIndex = (i + 2) % ploygon.size();
		newPloygon = offset({ -ploygon[i].x, -ploygon[i].y }, ploygon);
		newPloygon = rotate({ 0, 0 }, -std::atan(newPloygon[rotateIndex].y / newPloygon[rotateIndex].x), newPloygon);
		if (newPloygon[testIndex].y < 0)
		{
			float A1 = 0;
			float B1 = 1;
			float C1 = 0;

			Point cutPoint;
			int curIndex = 0;
			for (int j = testIndex; j < newPloygon.size(); j++)
			{
				int nextIndex = (j + 1) % newPloygon.size();
				float A2 = newPloygon[nextIndex].y - newPloygon[j].y;
				float B2 = newPloygon[j].x - newPloygon[nextIndex].x;
				float C2 = newPloygon[nextIndex].x * newPloygon[j].y - newPloygon[j].x * newPloygon[nextIndex].y;
				Point point;
				if (linesPoint(A1, B1, C1, A2, B2, C2, point))
				{
					float Xmax = newPloygon[j].x > newPloygon[nextIndex].x ? newPloygon[j].x : newPloygon[nextIndex].x;
					float Xmin = newPloygon[j].x > newPloygon[nextIndex].x ? newPloygon[nextIndex].x : newPloygon[j].x;
					float Ymax = newPloygon[j].y > newPloygon[nextIndex].y ? newPloygon[j].y : newPloygon[nextIndex].y;
					float Ymin = newPloygon[j].y > newPloygon[nextIndex].y ? newPloygon[nextIndex].y : newPloygon[j].y;

					if (point.x > Xmin && point.x < Xmax && point.y > Ymin && point.y < Ymax)
					{
						cutPoint = point;
						curIndex = j;
						break;
					}
				}
			}
			vector<Point> newPoints = newPloygon;
			newPoints.insert(newPoints.begin() + curIndex + 1, cutPoint);

			vector<Point> newPloygon1;
			vector<Point> newPloygon2;
			Point cutEndP = newPloygon[rotateIndex];
			bool f = false;
			for (int i = 0; i < newPoints.size(); i++)
			{
				if (newPoints[i] == cutPoint || newPoints[i] == cutEndP)
				{
					if (!f)
					{
						newPloygon1.push_back(newPoints[i]);
					}
					else
					{
						newPloygon2.push_back(newPoints[i]);
					}
					f = !f;
				}

				if (!f)
				{
					newPloygon1.push_back(newPoints[i]);
				}
				else
				{
					newPloygon2.push_back(newPoints[i]);
				}
			}
			cutPolygon(newPloygon1);
			cutPolygon(newPloygon2);
			return;
		}
	}
	drawPloygons.push_back(ploygon);
}
void code_4_exercise_20()
{
	cutPolygon(ploygon);
	glLoadIdentity();
	gluOrtho2D(-winWidth / 2, winWidth, -winHeight / 2, winHeight);
	glutDisplayFunc(drowPolygon);
}
#endif

#ifdef CHAPTER_4_EXERCISE_21
struct Vector
{
	float x, y;
};
struct Point
{
	GLfloat x, y, z;
};
vector<Point> ploygon = { { 420, 430, 0 }, { 225, 170, 0 }, { 201, 335, 0 }, { 453, 374, 0 },
{ 447, 212, 0 }, { 135, 201, 0 }, { 388, 274, 0 } };
Point testP1 = { 356, 303, 0 }, rayP1 = { 608, 397, 0 };
Point testP2 = { 250, 223, 0 }, rayP2 = { 229, 439, 0 };
Point testP3 = { 376, 225, 0 }, rayP3 = { 235, 548, 0 };
inline bool operator==(const Point& p1, const Point& p2)
{
	return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}
inline bool operator<(const Point& p1, const Point& p2)
{
	return p1.x < p2.x && p1.y < p2.y && p1.z < p2.z;
}
int crossProduct(const Vector& vector1, const Vector& vector2)
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}
// 计算两条直线的交点（直线公式：Ax+By+C=0）
bool linesPoint(float A1, float B1, float C1, float A2, float B2, float C2, Point& point)
{
	if (equal(A1 * B2, A2 * B1))
		return false;

	point.x = (B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1);
	point.y = (A2 * C1 - A1 * C2) / (A1 * B2 - A2 * B1);
	point.z = 0;
	return true;
}
void drowPolygon(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	int j = 0;
	for (int i = 0; i < ploygon.size(); ++i)
	{
		j = i + 1 >= ploygon.size() ? 0 : i + 1;
		glBegin(GL_LINES);
		glVertex2i(ploygon[i].x, ploygon[i].y);
		glVertex2i(ploygon[j].x, ploygon[j].y);
		glEnd();
	}
	glBegin(GL_LINES);
	glVertex2i(testP1.x, testP1.y);
	glVertex2i(rayP1.x, rayP1.y);
	glVertex2i(testP2.x, testP2.y);
	glVertex2i(rayP2.x, rayP2.y);
	glVertex2i(testP3.x, testP3.y);
	glVertex2i(rayP3.x, rayP3.y);
	glEnd();
	glFlush();
}
void check(Point checkPoint, Point rayPoint, const vector<Point>& ploygon)
{
	int count = 0;
	float A1 = rayPoint.y - checkPoint.y;
	float B1 = checkPoint.x - rayPoint.x;
	float C1 = rayPoint.x * checkPoint.y - checkPoint.x * rayPoint.y;
	for (int i = 0; i < ploygon.size(); i++)
	{
		int j = i + 1 < ploygon.size() ? i + 1 : 0;

		float A2 = ploygon[j].y - ploygon[i].y;
		float B2 = ploygon[i].x - ploygon[j].x;
		float C2 = ploygon[j].x * ploygon[i].y - ploygon[i].x * ploygon[j].y;
		Point point;
		if (linesPoint(A1, B1, C1, A2, B2, C2, point))
		{
			float EXmax = ploygon[j].x > ploygon[i].x ? ploygon[j].x : ploygon[i].x;
			float EXmin = ploygon[j].x > ploygon[i].x ? ploygon[i].x : ploygon[j].x;
			float EYmax = ploygon[j].y > ploygon[i].y ? ploygon[j].y : ploygon[i].y;
			float EYmin = ploygon[j].y > ploygon[i].y ? ploygon[i].y : ploygon[j].y;

			float CheckXmax = rayPoint.x > checkPoint.x ? rayPoint.x : checkPoint.x;
			float CheckXmin = rayPoint.x > checkPoint.x ? checkPoint.x : rayPoint.x;
			float CheckYmax = rayPoint.y > checkPoint.y ? rayPoint.y : checkPoint.y;
			float CheckYmin = rayPoint.y > checkPoint.y ? checkPoint.y : rayPoint.y;

			if (EXmin < point.x && point.x < EXmax &&
				EYmin < point.y && point.y < EYmax &&
				CheckXmin < point.x && point.x < CheckXmax &&
				CheckYmin < point.y && point.y < CheckYmax)
			{
				if (crossProduct({ rayPoint.x - checkPoint.x, rayPoint.y - checkPoint.y },
				{ ploygon[j].x - ploygon[i].x, ploygon[j].y - ploygon[i].y }) > 0)
				{
					count++;
				}
				else
				{
					count--;
				}
			}
		}
	}
	printf("点(%f, %f)是%s\n", checkPoint.x, checkPoint.y, count != 0 ? "内点" : "外点");
}
void code_4_exercise_21()
{
	check(testP1, rayP1, ploygon);
	check(testP2, rayP2, ploygon);
	check(testP3, rayP3, ploygon);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drowPolygon);
}
#endif

#ifdef CHAPTER_4_EXERCISE_22
struct Vector
{
	float x, y;
};
struct Point
{
	GLfloat x, y, z;
};
vector<Point> ploygon = { { 420, 430, 0 }, { 225, 170, 0 }, { 201, 335, 0 }, { 453, 374, 0 },
{ 447, 212, 0 }, { 135, 201, 0 }, { 388, 274, 0 } };
Point testP1 = { 356, 303, 0 }, rayP1 = { 608, 397, 0 };
Point testP2 = { 250, 223, 0 }, rayP2 = { 229, 439, 0 };
Point testP3 = { 376, 225, 0 }, rayP3 = { 235, 548, 0 };
inline bool operator==(const Point& p1, const Point& p2)
{
	return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}
inline bool operator<(const Point& p1, const Point& p2)
{
	return p1.x < p2.x && p1.y < p2.y && p1.z < p2.z;
}
// 计算两条直线的交点（直线公式：Ax+By+C=0）
bool linesPoint(float A1, float B1, float C1, float A2, float B2, float C2, Point& point)
{
	if (equal(A1 * B2, A2 * B1))
		return false;

	point.x = (B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1);
	point.y = (A2 * C1 - A1 * C2) / (A1 * B2 - A2 * B1);
	point.z = 0;
	return true;
}
int crossProduct(const Vector& vector1, const Vector& vector2)
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}
int dotProduct(const Vector& vector1, const Vector& vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y;
}
void drowPolygon(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	int j = 0;
	for (int i = 0; i < ploygon.size(); ++i)
	{
		j = i + 1 >= ploygon.size() ? 0 : i + 1;
		glBegin(GL_LINES);
		glVertex2i(ploygon[i].x, ploygon[i].y);
		glVertex2i(ploygon[j].x, ploygon[j].y);
		glEnd();
	}
	glBegin(GL_LINES);
	glVertex2i(testP1.x, testP1.y);
	glVertex2i(rayP1.x, rayP1.y);
	glVertex2i(testP2.x, testP2.y);
	glVertex2i(rayP2.x, rayP2.y);
	glVertex2i(testP3.x, testP3.y);
	glVertex2i(rayP3.x, rayP3.y);
	glEnd();
	glFlush();
}
void check(Point checkPoint, Point rayPoint, const vector<Point>& ploygon)
{
	Vector vecTest = { rayPoint.x - checkPoint.x, rayPoint.y - checkPoint.y };
	Vector vec = { -vecTest.y, vecTest.x };
	int count = 0;
	float A1 = rayPoint.y - checkPoint.y;
	float B1 = checkPoint.x - rayPoint.x;
	float C1 = rayPoint.x * checkPoint.y - checkPoint.x * rayPoint.y;
	for (int i = 0; i < ploygon.size(); i++)
	{
		int j = i + 1 < ploygon.size() ? i + 1 : 0;

		float A2 = ploygon[j].y - ploygon[i].y;
		float B2 = ploygon[i].x - ploygon[j].x;
		float C2 = ploygon[j].x * ploygon[i].y - ploygon[i].x * ploygon[j].y;
		Point point;
		if (linesPoint(A1, B1, C1, A2, B2, C2, point))
		{
			float EXmax = ploygon[j].x > ploygon[i].x ? ploygon[j].x : ploygon[i].x;
			float EXmin = ploygon[j].x > ploygon[i].x ? ploygon[i].x : ploygon[j].x;
			float EYmax = ploygon[j].y > ploygon[i].y ? ploygon[j].y : ploygon[i].y;
			float EYmin = ploygon[j].y > ploygon[i].y ? ploygon[i].y : ploygon[j].y;

			float CheckXmax = rayPoint.x > checkPoint.x ? rayPoint.x : checkPoint.x;
			float CheckXmin = rayPoint.x > checkPoint.x ? checkPoint.x : rayPoint.x;
			float CheckYmax = rayPoint.y > checkPoint.y ? rayPoint.y : checkPoint.y;
			float CheckYmin = rayPoint.y > checkPoint.y ? checkPoint.y : rayPoint.y;

			if (EXmin < point.x && point.x < EXmax &&
				EYmin < point.y && point.y < EYmax &&
				CheckXmin < point.x && point.x < CheckXmax &&
				CheckYmin < point.y && point.y < CheckYmax)
			{
				if (dotProduct(vec, { ploygon[j].x - ploygon[i].x, ploygon[j].y - ploygon[i].y }) > 0)
				{
					count++;
				}
				else
				{
					count--;
				}
			}
		}
	}
	printf("点(%f, %f)是%s\n", checkPoint.x, checkPoint.y, count != 0 ? "内点" : "外点");
}
void code_4_exercise_22()
{
	check(testP1, rayP1, ploygon);
	check(testP2, rayP2, ploygon);
	check(testP3, rayP3, ploygon);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drowPolygon);
}
#endif

#ifdef CHAPTER_4_EXERCISE_23
struct Vector
{
	float x, y;
};
struct Point
{
	GLfloat x, y, z;
};
vector<Point> ploygon = { { 420, 430, 0 }, { 225, 170, 0 }, { 201, 335, 0 }, { 453, 374, 0 },
{ 447, 212, 0 }, { 135, 201, 0 }, { 388, 274, 0 } };
Point testP1 = { 395, 379, 0 }, rayP1 = { 564, 499, 0 };
Point testP2 = { 286, 306, 0 }, rayP2 = { 349, 534, 0 };
Point testP3 = { 359, 301, 0 }, rayP3 = { 604, 398, 0 };
Point testP4 = { 382, 224, 0 }, rayP4 = { 235, 548, 0 };
Point testP5 = { 251, 220, 0 }, rayP5 = { 231, 441, 0 };
Point testP6 = { 200, 211, 0 }, rayP6 = { 77, 122, 0 };
Point testP7 = { 229, 193, 0 }, rayP7 = { 313, 555, 0 };
inline bool operator==(const Point& p1, const Point& p2)
{
	return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}
inline bool operator<(const Point& p1, const Point& p2)
{
	return p1.x < p2.x && p1.y < p2.y && p1.z < p2.z;
}
int crossProduct(const Vector& vector1, const Vector& vector2)
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}
// 计算两条直线的交点（直线公式：Ax+By+C=0）
bool linesPoint(float A1, float B1, float C1, float A2, float B2, float C2, Point& point)
{
	if (equal(A1 * B2, A2 * B1))
		return false;

	point.x = (B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1);
	point.y = (A2 * C1 - A1 * C2) / (A1 * B2 - A2 * B1);
	point.z = 0;
	return true;
}
void drowPolygon(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	int j = 0;
	for (int i = 0; i < ploygon.size(); ++i)
	{
		j = i + 1 >= ploygon.size() ? 0 : i + 1;
		glBegin(GL_LINES);
		glVertex2i(ploygon[i].x, ploygon[i].y);
		glVertex2i(ploygon[j].x, ploygon[j].y);
		glEnd();
	}
	glBegin(GL_LINES);
	glVertex2i(testP1.x, testP1.y);
	glVertex2i(rayP1.x, rayP1.y);
	glVertex2i(testP2.x, testP2.y);
	glVertex2i(rayP2.x, rayP2.y);
	glVertex2i(testP3.x, testP3.y);
	glVertex2i(rayP3.x, rayP3.y);
	glVertex2i(testP4.x, testP4.y);
	glVertex2i(rayP4.x, rayP4.y);
	glVertex2i(testP5.x, testP5.y);
	glVertex2i(rayP5.x, rayP5.y);
	glVertex2i(testP6.x, testP6.y);
	glVertex2i(rayP6.x, rayP6.y);
	glVertex2i(testP7.x, testP7.y);
	glVertex2i(rayP7.x, rayP7.y);
	glEnd();
	glFlush();
}
void check(Point checkPoint, Point rayPoint, const vector<Point>& ploygon)
{
	int count = 0;
	float A1 = rayPoint.y - checkPoint.y;
	float B1 = checkPoint.x - rayPoint.x;
	float C1 = rayPoint.x * checkPoint.y - checkPoint.x * rayPoint.y;
	for (int i = 0; i < ploygon.size(); i++)
	{
		int j = i + 1 < ploygon.size() ? i + 1 : 0;

		float A2 = ploygon[j].y - ploygon[i].y;
		float B2 = ploygon[i].x - ploygon[j].x;
		float C2 = ploygon[j].x * ploygon[i].y - ploygon[i].x * ploygon[j].y;
		Point point;
		if (linesPoint(A1, B1, C1, A2, B2, C2, point))
		{
			float EXmax = ploygon[j].x > ploygon[i].x ? ploygon[j].x : ploygon[i].x;
			float EXmin = ploygon[j].x > ploygon[i].x ? ploygon[i].x : ploygon[j].x;
			float EYmax = ploygon[j].y > ploygon[i].y ? ploygon[j].y : ploygon[i].y;
			float EYmin = ploygon[j].y > ploygon[i].y ? ploygon[i].y : ploygon[j].y;

			float CheckXmax = rayPoint.x > checkPoint.x ? rayPoint.x : checkPoint.x;
			float CheckXmin = rayPoint.x > checkPoint.x ? checkPoint.x : rayPoint.x;
			float CheckYmax = rayPoint.y > checkPoint.y ? rayPoint.y : checkPoint.y;
			float CheckYmin = rayPoint.y > checkPoint.y ? checkPoint.y : rayPoint.y;

			if (EXmin < point.x && point.x < EXmax &&
				EYmin < point.y && point.y < EYmax &&
				CheckXmin < point.x && point.x < CheckXmax &&
				CheckYmin < point.y && point.y < CheckYmax)
			{
				if (crossProduct({ rayPoint.x - checkPoint.x, rayPoint.y - checkPoint.y },
				{ ploygon[j].x - ploygon[i].x, ploygon[j].y - ploygon[i].y }) > 0)
				{
					count++;
				}
				else
				{
					count--;
				}
			}
		}
	}
	printf("点(%f, %f)是%s,非零环绕数是：%d\n", checkPoint.x, checkPoint.y, count != 0 ? "内点" : "外点", count);
}
void code_4_exercise_23()
{
	check(testP1, rayP1, ploygon);
	check(testP2, rayP2, ploygon);
	check(testP3, rayP3, ploygon);
	check(testP4, rayP4, ploygon);
	check(testP5, rayP5, ploygon);
	check(testP6, rayP6, ploygon);
	check(testP7, rayP7, ploygon);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drowPolygon);
}
#endif

#ifdef CHAPTER_4_EXERCISE_24
struct Point
{
	GLfloat x, y, z;
};
void bitmapString(const char* s, Point position)
{
	glRasterPos2i(position.x, position.y);
	for (; *s != 0; s++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *s);
	}
}
void strokeString(const char* s)
{
	for (; *s != 0; s++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *s);
	}
}
void drowFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);
	bitmapString("hello world1", { 100, 100 });
	strokeString("hello world2");
	glFlush();
}
void code_4_exercise_24()
{
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drowFunc);
}
#endif

#ifdef CHAPTER_4_EXERCISE_25
struct Point
{
	GLfloat x, y, z;
};
void polymarker (const char* s, vector<Point> positions)
{
	for (auto position : positions)
	{
		glRasterPos2i(position.x, position.y);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *s);
	}
}
void drowFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);
	polymarker("*", { { 100, 100 }, { 200, 200 }, { 300, 300 } });
	glFlush();
}
void code_4_exercise_25()
{
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drowFunc);
}
#endif

#ifdef CHAPTER_4_EXERCISE_26
const double TWO_PI = 6.2831853;
GLsizei winWidth = 400, winHeight = 400;
GLuint regHex;
class screenPt
{
private:
	GLint x, y;
public:
	screenPt()
	{
		x = y = 0;
	}
	void setCoords(GLint xCoord, GLint yCoord)
	{
		x = xCoord;
		y = yCoord;
	}
	GLint getx() const
	{
		return x;
	}
	GLint gety() const
	{
		return y;
	}
};
static void init(void)
{
	screenPt hexVertex, circCtr;
	GLdouble theta;
	GLint k;
	circCtr.setCoords(winWidth / 2, winHeight / 2);
	glClearColor(1.0, 1.0, 1.0, 0.0);

	regHex = glGenLists(1);
	glNewList(regHex, GL_COMPILE);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for (k = 0; k < 6; k++)
	{
		theta = TWO_PI *k / 6.0;
		hexVertex.setCoords(circCtr.getx() + 150 * cos(theta), circCtr.gety() + 150 * sin(theta));
		glVertex2i(hexVertex.getx(), hexVertex.gety());
	}
	glEnd();
	glEndList();
}
void regHexagon(void)
{
	printf("regHexagon\n");
	glClear(GL_COLOR_BUFFER_BIT);
	glCallList(regHex);
	glFlush();
}
void winReshapeFcn(int newWidth, int newHeight)
{
	printf("winReshapeFcn\n");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0, (GLdouble)winWidth, 0.0, (GLdouble)winHeight);
	glClear(GL_COLOR_BUFFER_BIT);
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Reshape-Function & Display-List Example");
	init();
	glutDisplayFunc(regHexagon);
	glutReshapeFunc(winReshapeFcn);
	glutMainLoop();
}
#endif

#ifdef CHAPTER_4_EXERCISE_27
GLsizei winWidth = 600, winHeight = 500;
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 600.0, 0.0, 500.0);
}
void drawRect(vector<int> values)
{
	GLint offsetX = 30, offsetY=150;
	GLint d = 30;
	GLint space = (winWidth - values.size() * d) / (values.size() + 1);
	
	glBegin(GL_LINES);
	glVertex2i(offsetX, offsetY);
	glVertex2i(100000, offsetY);
	glVertex2i(offsetX, offsetY);
	glVertex2i(offsetX, 100000);
	glEnd();

	for (GLint i = 0; i < values.size(); i++)
		glRecti((i + 1)*space + i*d + offsetX, offsetY, (i + 1)*space + (i + 1)*d + offsetX, values[i] + offsetY);
	glColor3f(0.0, 0.0, 0.0);
}
void displayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	drawRect({ 100, 200, 300});
	glFlush();
}
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));
	glClear(GL_COLOR_BUFFER_BIT);
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Line Chart Data Plot");
	init();
	glutDisplayFunc(displayFunc);
	glutReshapeFunc(winReshapeFcn);
	glutMainLoop();
}
#endif

#ifdef CHAPTER_4_EXERCISE_28
GLsizei winWidth = 600, winHeight = 500;
struct Rect
{
	int x, y, w, h;
};
struct Data
{
	int value;
	string label;
};
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 600.0, 0.0, 500.0);
}
void drawRect(Rect rect, vector<Data> datas)
{
	GLint d = 30;
	GLint space = (rect.w - datas.size() * d) / (datas.size() + 1);

	glBegin(GL_LINE_LOOP);
	glVertex2i(rect.x, rect.y);
	glVertex2i(rect.x + rect.w, rect.y);
	glVertex2i(rect.x + rect.w, rect.y + rect.h);
	glVertex2i(rect.x, rect.y + rect.h);
	glEnd();

	for (GLint i = 0; i < datas.size(); i++)
	{
		int leftX = (i + 1)*space + i*d + rect.x;
		int leftY = rect.y;
		int RightX = (i + 1)*space + (i + 1)*d + rect.x;
		int RightY = datas[i].value + rect.y;
		glRecti(leftX, leftY, RightX, RightY);
		
		glRasterPos2i(leftX, RightY + 10);
		char c[8];
		sprintf_s(c, "%d", datas[i].value);
		for (const char* p = c; *p != 0; p++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *p);

		glRasterPos2i(leftX, leftY - 10);
		for (int j = 0; j < datas[i].label.size(); j++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, datas[i].label[j]);
	}
	glColor3f(0.0, 0.0, 0.0);
}
void displayFunc(void)
{
	printf("displayFunc\n");
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	drawRect({ 100, 100, 300, 300 }, { { 100, "abc" }, { 200, "def" }, { 300, "ghi" } });
	glFlush();
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Line Chart Data Plot");
	init();
	glutDisplayFunc(displayFunc);
	glutMainLoop();
}
#endif

#ifdef CHAPTER_4_EXERCISE_29
GLsizei winWidth = 600, winHeight = 500;
struct Rect
{
	int x, y, w, h;
};
struct Data
{
	int value;
	string label;
};
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 600.0, 0.0, 500.0);
}
void drawLine(Rect rect, vector<Data> datas)
{
	int space = rect.w / (datas.size() + 1);

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(rect.x, rect.y);
	glVertex2i(rect.x + rect.w, rect.y);
	glVertex2i(rect.x + rect.w, rect.y + rect.h);
	glVertex2i(rect.x, rect.y + rect.h);
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < datas.size(); i++)
		glVertex2i((i + 1)*space + rect.x, datas[i].value + rect.y);
	glEnd();

	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < datas.size(); i++)
	{
		glRasterPos2i((i + 1)*space + rect.x-4, datas[i].value + rect.y-4);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '*');
	}

	for (int i = 0; i < datas.size(); i++)
	{
		glRasterPos2i((i + 1)*space + rect.x-10, datas[i].value + rect.y + 10);
		char c[8];
		sprintf_s(c, "%d", datas[i].value);
		for (const char* p = c; *p != 0; p++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *p);		
	}

	for (int i = 0; i < datas.size(); i++)
	{
		glRasterPos2i((i + 1)*space + rect.x-10, rect.y - 10);
		for (int j = 0; j < datas[i].label.size(); j++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, datas[i].label[j]);
	}

}
void lineGraph(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawLine({ 100, 100, 300, 300 }, { { 100, "abc" }, { 200, "def" }, { 100, "ghi" } });
	glFlush();
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Line Chart Data Plot");
	init();
	glutDisplayFunc(lineGraph);
	glutMainLoop();
}
#endif

#ifdef CHAPTER_4_EXERCISE_30
const GLdouble twoPi = 6.283185;
struct Point
{
	double x, y;
};
GLsizei winWidth = 400, winHeight = 300;
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
}
void drawCircle(Point center, int radius)
{
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 1000; i++)
	{
		double angle = i * twoPi / 1000;
		Point point;
		point.x = center.x + radius * cos(angle);
		point.y = center.y + radius * sin(angle);
		glVertex2d(point.x, point.y);
	}
	glEnd();
}
void drawPie(vector<float> values)
{
	Point center, point;
	center.x = winWidth / 2;
	center.y = winHeight / 2;
	GLint radius = winWidth / 4;
	GLdouble angle, lastAngle = 0.0;
	GLfloat sum = 0.0;

	drawCircle(center, radius);
	for (int i = 0; i < values.size(); i++)
		sum += values[i];
	for (int i = 0; i < values.size(); i++)
	{
		angle = twoPi * values[i] / sum + lastAngle;
		point.x = center.x + radius * cos(angle);
		point.y = center.y + radius * sin(angle);
		glBegin(GL_LINES);
		glVertex2i(center.x, center.y);
		glVertex2i(point.x, point.y);
		glEnd();
		lastAngle = angle;

		if (twoPi / 4 < angle && angle < twoPi * 3 / 4)
			point.x -= 50;
		if (twoPi / 2 < angle && angle < twoPi)
			point.y -= 10;
		glRasterPos2i(point.x, point.y);
		char c[16];
		sprintf_s(c, "%.2f", values[i]);
		for (const char* p = c; *p != 0; p++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *p);
	}
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	drawPie({ 10.0, 7.0, 13.0, 5.0, 13.0, 14.0, 3.0, 16.0, 5.0, 3.0, 17.0, 8.0 });
	glFlush();
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Pie Chart");
	init();
	glutDisplayFunc(displayFcn);
	glutMainLoop();
}
#endif

#ifdef CHAPTER_4_ADD_EXERCISE_2
struct Vector
{
	float x, y;
};
struct Point
{
	GLfloat x, y, z;
};
inline bool operator==(const Point& p1, const Point& p2)
{
	return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}
inline bool operator<(const Point& p1, const Point& p2)
{
	return p1.x < p2.x && p1.y < p2.y && p1.z < p2.z;
}
vector<Point> ploygon = { { 156, 338, 0 }, { 331, 151, 0 }, { 568, 151, 0 }, { 607, 428, 0 }, { 398, 266, 0 }, { 300, 387, 0 } };
vector<vector<Point>> ploygons;
vector<vector<Point>> drawPloygons;
void drowPolygon(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	int j = 0;
	for (auto& ploygon : drawPloygons)
	{
		for (int i = 0; i < ploygon.size(); ++i)
		{
			j = i + 1 >= ploygon.size() ? 0 : i + 1;
			glBegin(GL_LINES);
			glVertex2i(ploygon[i].x, ploygon[i].y);
			glVertex2i(ploygon[j].x, ploygon[j].y);
			glEnd();
		}
	}
	glFlush();
}
int crossProduct(const Vector& vector1, const Vector& vector2)
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}
// 计算两条直线的交点（直线公式：Ax+By+C=0）
bool linesPoint(float A1, float B1, float C1, float A2, float B2, float C2, Point& point)
{
	if (equal(A1 * B2, A2 * B1))
		return false;

	point.x = (B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1);
	point.y = (A2 * C1 - A1 * C2) / (A1 * B2 - A2 * B1);
	point.z = 0;
	return true;
}
void cutPolygon(const vector<Point>& ploygon)
{
	vector<Vector> vectors;
	for (int i = 0; i < ploygon.size(); i++)
	{
		int j = i + 1 < ploygon.size() ? i + 1 : 0;
		vectors.push_back({ ploygon[j].x - ploygon[i].x, ploygon[j].y - ploygon[i].y });
	}
	int j;
	int index = -1;
	for (int i = 0; i < vectors.size(); ++i)
	{
		j = i + 1 >= vectors.size() ? 0 : i + 1;
		float xp = crossProduct(vectors[i], vectors[j]);
		if (xp < 0)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		ploygons.push_back(ploygon);
		return;
	}
	Point cutStartP = ploygon[index];
	Point cutEndP = ploygon[index + 1 < ploygon.size() ? index + 1 : 0];
	float A1 = cutEndP.y - cutStartP.y;
	float B1 = cutStartP.x - cutEndP.x;
	float C1 = cutEndP.x * cutStartP.y - cutStartP.x * cutEndP.y;

	Point cutPoint;
	float minDis = 0;
	int curIndex = 0;
	for (int i = 0; i < ploygon.size(); i++)
	{
		int j = i + 1 < ploygon.size() ? i + 1 : 0;
		float A2 = ploygon[j].y - ploygon[i].y;
		float B2 = ploygon[i].x - ploygon[j].x;
		float C2 = ploygon[j].x * ploygon[i].y - ploygon[i].x * ploygon[j].y;
		Point point;
		if (linesPoint(A1, B1, C1, A2, B2, C2, point))
		{
			float Xmax = ploygon[i].x > ploygon[j].x ? ploygon[i].x : ploygon[j].x;
			float Xmin = ploygon[i].x > ploygon[j].x ? ploygon[j].x : ploygon[i].x;
			float Ymax = ploygon[i].y > ploygon[j].y ? ploygon[i].y : ploygon[j].y;
			float Ymin = ploygon[i].y > ploygon[j].y ? ploygon[j].y : ploygon[i].y;

			if (point.x > Xmin && point.x < Xmax && point.y > Ymin && point.y < Ymax)
			{
				if (crossProduct({ cutEndP.x - cutStartP.x, cutEndP.x - cutStartP.x }, { ploygon[j].x - ploygon[i].x, ploygon[j].y - ploygon[i].y }) > 0)
				{
					float dis = (cutEndP.y - point.y) * (cutEndP.y - point.y) + (cutEndP.x - point.x) * (cutEndP.x - point.x);
					if (dis < minDis || minDis == 0)
					{
						cutPoint = point;
						curIndex = i;
					}
				}
			}
		}
	}
	vector<Point> newPloygon = ploygon;
	newPloygon.insert(newPloygon.begin() + curIndex + 1, cutPoint);

	vector<Point> newPloygon1;
	vector<Point> newPloygon2;

	bool f = false;
	for (int i = 0; i < newPloygon.size(); i++)
	{
		if (newPloygon[i] == cutPoint || newPloygon[i] == cutEndP)
		{
			if (!f)
			{
				newPloygon1.push_back(newPloygon[i]);
			}
			else
			{
				newPloygon2.push_back(newPloygon[i]);
			}
			f = !f;
		}

		if (!f)
		{
			newPloygon1.push_back(newPloygon[i]);
		}
		else
		{
			newPloygon2.push_back(newPloygon[i]);
		}
	}
	cutPolygon(newPloygon1);
	cutPolygon(newPloygon2);
}
void cutTriangle(const vector<Point>& ploygon)
{
	vector<Point> points = ploygon;

	if (points.size() < 3)
		return;
	if (!equal(crossProduct({ points[1].x - points[0].x, points[1].y - points[0].y }, { points[2].x - points[1].x, points[2].y - points[1].y }), 0))
	{
		drawPloygons.push_back({ points[0], points[1], points[2] });
	}
	points.erase(points.begin() + 1);
	cutTriangle(points);
}
void code_4_add_exercise_2()
{
	cutPolygon(ploygon);
	for (auto& ploy : ploygons)
	{
		cutTriangle(ploy);
	}
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	glutDisplayFunc(drowPolygon);
}
#endif

//////////////////////////////////////////////////////////////////////////
// CHAPTER_4_COMMON

#ifdef CHAPTER_4_COMMON

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
}

void main(int argc, char** argv)
{
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("An Example OpenGL Program");
	int glewCode = glewInit();
	if (glewCode != GLEW_OK)
	{
		printf("glewInit err:%d\n", glewCode);
		return;
	}

	init();

#ifdef CHAPTER_4_15_2
	code_4_15_2();
#endif

#ifdef CHAPTER_4_Test
	glutDisplayFunc(drawPoint);
	polygonTest();
#endif

#ifdef CHAPTER_4_EXERCISE_1
	code_4_exercise_1();
#endif

#ifdef CHAPTER_4_EXERCISE_2
	code_4_exercise_2();
#endif

#ifdef CHAPTER_4_EXERCISE_3
	code_4_exercise_3();
#endif

#ifdef CHAPTER_4_EXERCISE_4
	code_4_exercise_4();
#endif

#ifdef CHAPTER_4_EXERCISE_5
	code_4_exercise_5();
#endif

#ifdef CHAPTER_4_EXERCISE_6
	code_4_exercise_6();
#endif

#ifdef CHAPTER_4_EXERCISE_7
	code_4_exercise_7();
#endif

#ifdef CHAPTER_4_EXERCISE_8
	code_4_exercise_8();
#endif

#ifdef CHAPTER_4_EXERCISE_9
	code_4_exercise_9();
#endif

#ifdef CHAPTER_4_EXERCISE_10
	code_4_exercise_10();
#endif

#ifdef CHAPTER_4_EXERCISE_11
	code_4_exercise_11();
#endif

#ifdef CHAPTER_4_EXERCISE_12
	code_4_exercise_12();
#endif

#ifdef CHAPTER_4_EXERCISE_13
	code_4_exercise_13();
#endif

#ifdef CHAPTER_4_EXERCISE_14
	code_4_exercise_14();
#endif

#ifdef CHAPTER_4_EXERCISE_15
	code_4_exercise_15();
#endif

#ifdef CHAPTER_4_EXERCISE_16
	code_4_exercise_16();
#endif

#ifdef CHAPTER_4_EXERCISE_17
	code_4_exercise_17();
#endif

#ifdef CHAPTER_4_EXERCISE_18
	code_4_exercise_18();
#endif

#ifdef CHAPTER_4_EXERCISE_19
	code_4_exercise_19();
#endif

#ifdef CHAPTER_4_EXERCISE_20
	code_4_exercise_20();
#endif

#ifdef CHAPTER_4_EXERCISE_21
	code_4_exercise_21();
#endif

#ifdef CHAPTER_4_EXERCISE_22
	code_4_exercise_22();
#endif

#ifdef CHAPTER_4_EXERCISE_23
	code_4_exercise_23();
#endif

#ifdef CHAPTER_4_EXERCISE_24
	code_4_exercise_24();
#endif

#ifdef CHAPTER_4_EXERCISE_25
	code_4_exercise_25();
#endif

#ifdef CHAPTER_4_ADD_EXERCISE_2
	code_4_add_exercise_2();
#endif

	glutMainLoop();
}
#endif
#endif 