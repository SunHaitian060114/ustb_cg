 

#include"account.h"
#include"AccountRecord.h"
   Account::Account(){}
   Account::~Account(){}
double Account::accumulate(Date date)
{
   double ac = floor(((minus_day(this->lastDate,date)*balance)) * 100 + 0.5) / 100;//这段时间内的余额累积量=日期差*余额
   lastDate.copy_date(date);//更新最新日期
   return ac;
}//计算从上一次到这次的余额贡献值(这一段时间的累积)

double Account::total = 0.0;

const string& Account::getID() const
{
   return id;
}

double Account::getBalance()
{
   return balance;
}

double Account::getTotal()
{
    return total;
}

void Account::query(const Date& date1, const Date& date2)
{
   for (auto record = recordMap.lower_bound(date1) ; record != recordMap.upper_bound(date2) ;  record++)
   {
      // 如果交易日期在指定范围内____first 是 Date，second 是 AccountRecord
      if ((record->first < date2 && date1 < record->first ) || record->first == date1 || record->first == date2)
      {
         // 显示交易信息，例如账户ID、余额等
         cout << record->first << "\t#";
         cout << record->second.getAccountID() << "\t"
         << record->second.getAmount() << "\t"
         << record->second.getBalance() << "\t"
         << record->second.getDesc() << endl;
      }
   }
}

int minus_day(Date A, Date B)
{
    int mid = abs(todays(A)-todays(B));    
    return mid;
}

std::multimap<Date, AccountRecord> Account::recordMap;  // 定义静态成员

// ——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

void SavingsAccount::record(Date date, double amount)
{
   balance += amount;
   lastDate.copy_date(date);
}//结算在哪天存款剩余多少

double SavingsAccount::getRate()
{
   return rate;
}



void SavingsAccount::deposit(Date date, double amount, string desc) {
   accumulation += accumulate(date);
   amount = floor(amount * 100 + 0.5) / 100;
   balance += amount;
   total += amount;
   aim = desc;
   cout << date.Get_year() << "-" << date.Get_month() << "-" << date.Get_day() 
        << "\t#" << id << "\t" <<  std::fixed << std::setprecision(0) << amount << "\t" <<  std::fixed << std::setprecision(0) << balance << "\t" << desc << endl;
   Account::recordMap.emplace(date, AccountRecord(date, this, amount, balance, desc));
}

void SavingsAccount::withdraw(Date date, double amount, string desc) {
   if(amount<=0)
   {
      throw AccountException("amount<=0",this);
   }
   if(balance<amount)
   {
      throw AccountException("balance<amount",this);
   }
   accumulation += accumulate(date);
   amount = floor(amount * 100 + 0.5) / 100;
   balance -= amount;
   total -= amount;
   aim = desc;
   cout << date.Get_year() << "-" << date.Get_month() << "-" << date.Get_day() 
        << "\t#" << id << "\t-" <<  std::fixed << std::setprecision(0) << amount << "\t" <<  std::fixed << std::setprecision(0) << balance << "\t" << desc << endl;
   Account::recordMap.emplace(date, AccountRecord(date, this, -amount, balance, desc));
}


void SavingsAccount::settle(Date date) {
   // 计算从上一次操作到当前日期的天数
   int days = minus_day(lastDate, date);
   accumulation += balance * days;  // 关键：余额 × 天数


   // 1月1日结算年利息
   if (date.Get_month() == 1 && date.Get_day() == 1) {
       double interest = floor((accumulation * rate / 366) * 100 + 0.5) / 100;
       balance += interest;
       total += interest;  // 利息增加到Total
       cout << date.Get_year() << "-" << date.Get_month() << "-" << date.Get_day() 
            << "\t#" << id << "\t" <<  std::fixed << std::setprecision(2) << interest << "\t" <<  std::fixed << std::setprecision(1) << balance << "\tinterest" << endl;
       accumulation = 0;
   }
   lastDate.copy_date(date);
   
}

void SavingsAccount::show()
{
   cout << id << "\t" << "Balance: " <<  std::fixed << std::setprecision(1) << balance;
}//如图

SavingsAccount::SavingsAccount(Date date, string id, double rate)
{
    this->id = id;
    this->lastDate.copy_date(date);
    this->rate = rate;
    this->balance = 0;
    this->accumulation = 0;
    cout << lastDate.Get_year() << "-" << lastDate.Get_month() << "-" << lastDate.Get_day() << "\t#" << id << " created" << endl;
 }

SavingsAccount::SavingsAccount()
{
   this->balance = 0;
   this->accumulation = 0;
}

SavingsAccount::~SavingsAccount()
{

}

double SavingsAccount::accumulate (Date date) 
{
   double ac = floor(((minus_day(this->lastDate,date)*balance)) * 100 + 0.5) / 100;//这段时间内的余额累积量=日期差*余额
   lastDate.copy_date(date);//更新最新日期
   return ac;
}//计算从上一次到这次的余额贡献值(这一段时间的累积)

// ——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————

CreditAccount::CreditAccount(){}

CreditAccount::CreditAccount(Date date1, string id1, double credit1, double rate1, double fee1) 
    : credit(rate1), rate(credit1), fee(fee1)  // 直接使用日利率，不转换
{
   this->balance = 0;
    lastDate.copy_date(date1);
    id = id1;
    cout << lastDate.Get_year() << "-" << lastDate.Get_month() << "-" << lastDate.Get_day() 
         << "\t#" << id << " created" << endl;
}

CreditAccount::~CreditAccount(){}

double CreditAccount::accumulate (Date date) 
{

   double ac;
   if(balance>=0){
      ac = 0;
   }
   else{
      ac = floor(((minus_day(this->lastDate,date)*balance)) * 100 + 0.5) / 100;
      lastDate.copy_date(date);
   }
   return ac;
}

void CreditAccount::deposit(Date date, double amount, string desc) {
   settle(date);
   amount = floor(amount * 100 + 0.5) / 100;
   balance += amount;
   total += amount;
   aim = desc;
   cout << date.Get_year() << "-" << date.Get_month() << "-" << date.Get_day() 
        << "\t#" << id << "\t" <<  std::fixed << std::setprecision(0) << amount << "\t" <<  std::fixed << std::setprecision(0) << balance << "\t" << desc << endl;
   Account::recordMap.emplace(date, AccountRecord(date, this, amount, balance, desc));
}

void CreditAccount::withdraw(Date date, double amount, string desc) {
   if (amount <= 0) {
        throw AccountException("amount <= 0", this);
   }
   if (balance - amount < -credit) {
      throw AccountException("balance - amount < -credit", this);
   }
   settle(date);
   amount = floor(amount * 100 + 0.5) / 100;
   balance -= amount;
   total -= amount;
   aim = desc;
   cout << date.Get_year() << "-" << date.Get_month() << "-" << date.Get_day() 
        << "\t#" << id << "\t-" <<  std::fixed << std::setprecision(0) << amount << "\t" <<  std::fixed << std::setprecision(0) << balance << "\t" << desc << endl;
   Account::recordMap.emplace(date, AccountRecord(date, this, -amount, balance, desc));
}

void CreditAccount::settle(Date date) {
    int days = minus_day(lastDate, date);
    if (balance < 0 && days > 0) {
        // 使用年利率按天计算
      //   double interest = floor(abs(balance) * (rate / 365.0) * days * 100 + 0.5) / 100;
      double interest = abs(balance)* rate * days;
        // 限制最大利息不超过信用额度的一定比例
      //   interest = std::min(interest, credit * 0.1); 
        balance -= interest;
        

        // 如果利息金额大于0.005才输出（防止输出-0.00）
        if (fabs(interest) >= 0.5) {
            cout << date.Get_year() << "-" << date.Get_month() << "-" << date.Get_day() 
                 << "\t#" << id << "\t-" << fixed << setprecision(0) << interest 
                 << "\t" << fixed << setprecision(0) << balance << "\tinterest" << endl;
               total -= interest;
               }

    }

    // 年费计算保持不变
    if (date.Get_month() == 1 && date.Get_day() == 1) {
        balance -= fee;
        total -= fee;
        // 同样避免输出极小值
        if (fabs(fee) >= 0.005) {
            cout << date.Get_year() << "-" << date.Get_month() << "-" << date.Get_day() 
                 << "\t#" << id << "\t-" << fixed << setprecision(0) << fee 
                 << "\t" << fixed << setprecision(0) << balance << "\tannual fee" << endl;
        }
    }

    lastDate.copy_date(date);
}


void CreditAccount::show() {
   double available = credit + (balance < 0 ? balance : 0);
   available = floor(available * 100 + 0.5) / 100;
   cout << id << "\tBalance: " <<  std::fixed << std::setprecision(0) << balance << "\tAvailable credit: " << available;
}


Account::MonthlyStats Account::getMonthlyStats(const Date& month) const {
    MonthlyStats stats{0, 0, month};
    auto range = recordMap.equal_range(month);
    
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second.getAccountID() == id) {
            double amount = it->second.getAmount();
            if (amount > 0) {
                stats.totalDeposit += amount;
            } else {
                stats.totalWithdraw += -amount;
            }
        }
    }
    return stats;
}

std::string Account::getReminders() const {
    std::string reminders;
    // 基础实现，子类可以覆盖
    return reminders;
}

// CreditAccount.cpp
std::string CreditAccount::getReminders() const {
    std::string reminders;
    if (balance < 0) {
        reminders += "提醒: 您的信用账户有欠款需要偿还\n";
    }
    return reminders;
}

vector<AccountRecord> Account::queryByMonth(int year, int month) {
    Date start(year, month, 1);  // 从该月的第一天开始
    Date end = (month == 12) ? Date(year + 1, 1, 1) : Date(year, month + 1, 1); // 到下个月的第一天

    vector<AccountRecord> records;
    auto it_low = recordMap.lower_bound(start);  // 获取记录从该月的第一天开始
    auto it_high = recordMap.lower_bound(end);   // 获取记录到下个月的第一天

    for (; it_low != it_high; ++it_low) {
        records.push_back(it_low->second);  // 添加符合条件的记录
    }

    return records;
}



std::vector<AccountRecord> Account::queryByAmount(const Date& month) {
    std::vector<AccountRecord> records;
    auto range = recordMap.equal_range(month);
    
    for (auto it = range.first; it != range.second; ++it) {
        records.push_back(it->second);
    }
    
    // 按金额排序（降序）
    std::sort(records.begin(), records.end(), 
        [](const AccountRecord& a, const AccountRecord& b) {
            return std::abs(a.getAmount()) > std::abs(b.getAmount());
        });
    
    return records;
}