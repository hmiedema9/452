#include Windows.h
#include iostream

using namespace std;

int main()
{
	LARGE_INTEGER startingTime, endingTime,testingTime,elapsedMilliseconds, frequency;

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startingTime);
	for (int i = 0; i  1000000; i++);
	QueryPerformanceCounter(&endingTime);

	double frq = 1.0f  (frequency.QuadPart1000);
	elapsedMilliseconds.QuadPart = endingTime.QuadPart - startingTime.QuadPart;
	elapsedMilliseconds.QuadPart = 1000; convert to milliseconds
	elapsedMilliseconds.QuadPart = frequency.QuadPart;

	cout  frequency  frequency.QuadPart   tickssecond  endl;
	cout  period   frq   millisecond  endl;
	cout  elapsed time of for loop   (double)elapsedMilliseconds.QuadPart1000   milliseconds  endl;

	elapsed time to small to be measured so we call it 1000000 times and divide 
	difference by result to get elapsed
	QueryPerformanceCounter(&startingTime);
	for (int i = 0; i  1000000;i++)	
		QueryPerformanceCounter(&testingTime);
	QueryPerformanceCounter(&endingTime);

	elapsedMilliseconds.QuadPart = endingTime.QuadPart - startingTime.QuadPart;
	elapsedMilliseconds.QuadPart = 1000; convert to milliseconds
	elapsedMilliseconds.QuadPart = frequency.QuadPart;

	cout  overhead of counter call  ((double)elapsedMilliseconds.QuadPart1000000)1000 milliseonds  endl;

	getchar();
	return 0;
}
