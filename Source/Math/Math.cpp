#include "Math/Vec2.hpp"
#include "Math/Vec3.hpp"
#include "Math/Vec4.hpp"
#include "Math/Vec8.hpp"
#include <iostream>
//#include <chrono>

I32 main() {

	Vec2i v2i();
	Vec2f v2f();

	Vec3i v3i();
	Vec3f v3f();

	Vec4i v4i();
	Vec4f v4f();

	Vec8i v8i();
	Vec8f v8f();

	v2i += 1;


	v2f += 1;

	v3i += 1;
	v3f += 1;

	v4i += 1;
	v4f += 1;

	v8i += 1;
	v8f += 1;

	/*auto Start = std::chrono::high_resolution_clock::now();
	auto Stop = std::chrono::high_resolution_clock::now();
	std::cout << " SSE + : " << std::chrono::duration_cast<std::chrono::milliseconds>(Stop - Start).count() << std::endl;

	Start = std::chrono::high_resolution_clock::now();
	Stop = std::chrono::high_resolution_clock::now();
	std::cout << " naive + : " << std::chrono::duration_cast<std::chrono::milliseconds>(Stop - Start).count() << std::endl;*/
	system("pause");
	return 0;
}