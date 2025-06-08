#include"date.h"

int get_day_permonth(int month, Date A)
{
    switch(month)
    {
    case 1: 
    case 3: 
    case 5: 
    case 7: 
    case 8: 
    case 10: 
    case 12:
        return 31;
        break;
    case 4: 
    case 6: 
    case 9: 
    case 11:
        return 30;
        break;

    case 2:
        if(is_leapyear(A.Get_year())){
            return 29;
            break;
        }
        else{
            return 28;
            break;
        }
    break;
    
    default:
        cout << "error" << endl;
        return 0;
        break;
    }
    return 0;
}


bool Date::validDate(int y, int m, int d)
{
    if (y < 1900 || m < 1 || m > 12 || d < 1) 
        return false;

    int maxDay = 31;
    if (m == 4 || m == 6 || m == 9 || m == 11) {
        maxDay = 30;
    } else if (m == 2) {
        maxDay = (y % 400 == 0 || (y % 100 != 0 && y % 4 == 0)) ? 29 : 28;
    }
    return d <= maxDay;
}
Date::Date()
{
    year = 1949;
    month = 10;
    day = 1;
}
Date::Date(int x, int y, int z)
{
    if (!validDate(x, y, z)) {
        throw std::invalid_argument("Invalid date");
    }
    year = x;
    month = y;
    day = z;
}
Date::~Date(){}



int is_leapyear(int year)
{
    if(year%4==0&&year%100!=0&&year%400==0) return 1;
    else return 0;
}

void Date::copy_date(const  Date & A)
{
    year = A.year;
    month = A.month;
    day = A.day;
}

long long todays(Date A)//计算自1949年起至这一天的天数
{
    long long sum = 0;
    for(int i=1949 ; i<A.Get_year() ; i++){
        if(is_leapyear(i)) sum+=366;
        else sum+=365;
    }
    for(int i=1 ; i<A.Get_month() ; i++){
        sum+=get_day_permonth(i,A);
    }
    sum+=A.Get_day();
    return sum;
}

void Date::show()
{
    cout << this->Get_year() << "-" << this->Get_month() << "-" << this->Get_day() << "\t";
}

int Date::getMaxDay()
{
    Date B(this->Get_year(),this->Get_month(),this->Get_day());
    int maxday = get_day_permonth(this->Get_month(),B);
    return maxday;
}

int Date::Get_year() const {return year;}
int Date::Get_month() const {return month;}
int Date::Get_day() const {return day;}

bool Date::operator<(const Date& rhs)const
{
    if(year != rhs.year) return year < rhs.year;
    if(month != rhs.month) return month < rhs.month;
    return day < rhs.day;
}

bool Date::operator==(const Date& rhs)const
{
    return year == rhs.year && month == rhs.month && day == rhs.day;
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << date.Get_year() << "-" << date.Get_month() << "-" << date.Get_day();
    return os;
}



Date Date::read()
{
    int y, m, d;
    char sep1, sep2;

    std::cin >> y >> sep1 >> m >> sep2 >> d;

    // 检查日期格式是否正确
    if (sep1 != '/' || sep2 != '/') {
        std::cin.setstate(std::ios::failbit);  // 设置失败状态
        return Date(0, 0, 0);  // 返回一个默认的 Date 对象
    }

    return Date(y, m, d);  // 返回正确读取的 Date 对象
}

