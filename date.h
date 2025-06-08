#pragma once
#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

#include <stdexcept>

// 用 std::enable_if 来限制类型，确保只对 Date 类型启用


class Date
{
    public:
    Date();
    Date(int x, int y, int z);
    ~Date();
    int Get_year() const;
    int Get_month() const;
    int Get_day() const;
    string aim;
    void copy_date(const  Date & A);
    void show();
    int getMaxDay();
    bool operator<(const Date& rhs)const;
    bool operator==(const Date& rhs)const;
    friend std::ostream& operator<<(std::ostream& os, const Date& date);
    
    static Date read();

    bool validDate(int y, int m, int d);
    private:
    int year,month,day;


    //运算符重载
    
};


template <typename T>
typename std::enable_if<std::is_same<T, Date>::value, std::ostream&>::type
operator<<(std::ostream& os, const T& date);

long long todays(Date A);
int get_day_permonth(int month, Date A);
int is_leapyear(int year);