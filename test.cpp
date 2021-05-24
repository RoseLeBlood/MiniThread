#include <mn_hash.hpp>
#include <stdio.h>

using namespace mn;

int main() {
	int a = 8;
	int g = hash<int>{}(a);
	int s = hash<const char*>{}("hallo");

	printf("hash from 8 is %d\n and from 'hello' %d", g, s);
	return 0;
}
