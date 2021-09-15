#include <iostream>
#include <time.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Datetime
{
public:
		int second;
		int minute;
		int hour;
		int day;
		int month;
		int year;

		int Set(int);
		int Get(int);

		void out();
		void System_time(Datetime& system);
		void Get_Next(Datetime& next);
		void Get_Prev(Datetime& prev);
};

/*int Datetime::Set(int arg)
{
	switch (arg)
	{
	default:
		break;
	}
}

int Datetime::Get(int arg)
{
	switch (arg)
	{
	default:
		break;
	}
}*/

void Datetime::out()
{
	printf("%i:%i:%i %i/%i/%i", hour, minute, second, day, month, year);
}

void Datetime::System_time(Datetime& system)
{
	time_t rawtime;
	tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	system.hour = timeinfo.tm_hour;
	system.minute = timeinfo.tm_min;
	system.second = timeinfo.tm_sec;
	system.day = timeinfo.tm_mday;
	system.month = timeinfo.tm_mon + 1;
	system.year = timeinfo.tm_year + 1900;
}

time_t Get_unix_time(Datetime A)
{
	tm timeinfo;
	timeinfo.tm_year = A.year - 1900;
	timeinfo.tm_mon = A.month - 1;
	timeinfo.tm_mday = A.day;
	timeinfo.tm_hour = A.hour;
	timeinfo.tm_min = A.minute;
	timeinfo.tm_sec = A.second;
	time_t unix_time = mktime(&timeinfo);
	return unix_time;
}

void Get_normal_time(time_t unix_time, Datetime& A)
{
	tm timeinfo;
	localtime_s(&timeinfo, &unix_time);
	A.hour = timeinfo.tm_hour;
	A.minute = timeinfo.tm_min;
	A.second = timeinfo.tm_sec;
	A.day = timeinfo.tm_mday;
	A.month = timeinfo.tm_mon + 1;
	A.year = timeinfo.tm_year + 1900;
}

void Datetime::Get_Next(Datetime& next)
{
	int min = 2419200 * 2;
	int max = 31536000;
	Get_normal_time(Get_unix_time(*this) + (min + rand() % (max - min + 1)), next);
}

void Datetime::Get_Prev(Datetime& prev)
{
	int min = 2419200 * 2;
	int max = 31536000;
	Get_normal_time(Get_unix_time(*this) - (min + rand() % (max - min + 1)), prev);
}

Datetime* Read_file_date(string file_name, int& n)
{
	ifstream file(file_name);
	string line;
	n = 0;
	while (getline(file, line))
	{
		n++;
	}
	Datetime* Date = new Datetime[n];
	int i = 0;
	file.clear();
	file.seekg(0, ios::beg);
	while (getline(file, line))
	{
		istringstream line_(line);
		line_ >> Date[i].hour >> Date[i].minute >> Date[i].second >> Date[i].day >> Date[i].month >> Date[i].year;
		i++;
	}
	file.close();
	return Date;
}

int main()
{
	srand(time(0));
	Datetime date, next, prev;
	int n;
	Datetime* Date = Read_file_date("data.txt", n);
	Datetime* Prev = new Datetime[n];
	date.System_time(date); 
	//printf("\n\nSystem time: "); 
	date.out();
	printf("Data from file:\n\n");
	for (int i = 0; i < n; i++)
	{
		Date[i].Get_Next(next);
		Date[i].Get_Prev(prev);
		printf("%i)\t", i + 1);
		Date[i].out();
		printf("\t");
		next.out();
		printf("\t");
		prev.out();
		printf("\n");
		Prev[i] = prev;
	}

	printf("\n\tVariant 7\n"); //Выбрать из массива исходных дат те, для которых предыдущие не попадают на другой месяц.Вывести информацию о таких датах на экран в формате : <номер> <дата> <предыдущая дата>
	for (int i = 0; i < n; i++)
	{
		if (Date[i].month != Prev[i].month)
		{
			printf("%i)\t", i + 1);
			Date[i].out();
			printf("\t");
			prev.out();
			printf("\n");
		}
	}

	delete[] Date;
	delete[] Prev;
	return 0;
}