/*
储蓄账户类包括：
a. 数据成员： 账户id, 余额balance, 年利率rate等
b. 成员函数：显示账户信息show，存款deposit，取款withdraw，结算利息settle等
类图如下所示：

要求：
①  使用多文件处理机制，即step_1.cpp（如下所示），account.h（自己编写），account.cpp（自己编写）
②  年利的计算：由于账户的余额是不断变化的，因此：
a.  年利  =  余额 * 年利率 （错误）
b.  年利 = 日均余额 * 年利率 （正确）  //引入日均余额
     日均余额 = 一年当中每天的余额累计起来/一年的总天数
     例如，如果年利率是1.5%，某账户第5天存入5000元，第45天存入5500元，第90天结算利息。
                         i.   它第5天到第45天之间的余额为5000元 （持续40天）
                        ii.   第45天到第90天之间的余额为10500元  （持续45天）
                       iii.   90天的利息是（40*5000+45*10500）*1.5%/365=27.64元
c. 对变动值amount四舍五入保留两位小数
   amount = floor(amount * 100 + 0.5) / 100;
d. 为简便起见，日期用一个整数来表示。该整数是一个以日为单位的相对日期，例如如果以开户日为1，那么开户日后的第3天就用4来表示。这样通过将两个日期相减就可以得到两个日期相差天数，在计算年利时非常方便。

运行截图如下：

*/
#pragma once
#include "date.h"
#include "AccountException.h"
class AccountRecord;

class Account
{
   public:
   string id;//账户id
   double balance;//余额            // xxx
   double accumulation;//利息总数
   virtual double accumulate(Date date);
   static double total;
   Date lastDate;//上次存款时间
   string aim;                      //xxx
   const string& getID() const;
   double getBalance();
   static double getTotal();
   virtual void show()=0;//展示信息
   virtual void deposit(Date date, double amount, string aim0)=0;//存款
   virtual void withdraw(Date date, double amount, string aim0)=0;//取款
   virtual void settle(Date date)=0;
   Account();
   ~Account();
   static multimap<Date, AccountRecord> recordMap;  // 保存所有账目信息的容器

   static void query(const Date& date1, const Date& date2);

   struct MonthlyStats {
        double totalDeposit;
        double totalWithdraw;
        Date month;
    };
    
    virtual MonthlyStats getMonthlyStats(const Date& month) const;
    virtual std::string getReminders() const;

    static std::vector<AccountRecord> queryByDate(const Date& month);
    static std::vector<AccountRecord> queryByAmount(const Date& month);

};





class SavingsAccount : public Account
{
   private:
   double rate;//年利率
   public:
   SavingsAccount();
   ~SavingsAccount();
   void record(Date date, double amount);
   SavingsAccount(Date date, string id, double rate);
   double getRate();
   double accumulate (Date date) override;
   void show()override;//展示信息
   void deposit(Date date, double amount, string aim0)override;//存款
   void withdraw(Date date, double amount, string aim0)override;//取款
   void settle(Date date)override;//结算利息
};

class CreditAccount : public Account
{
   private:
   double rate;
   double credit;
   double fee;
   public:
   CreditAccount();
   CreditAccount(Date date1, string id1, double credit1, double rate1, double fee1);
   ~CreditAccount();
   double accumulate (Date date) override;
   void show()override;//展示信息
   void deposit(Date date, double amount, string aim0)override;//存款
   void withdraw(Date date, double amount, string aim0)override;//取款
   void settle(Date date)override;//结算利息
   std::string getReminders() const override;
};

int minus_day(Date A, Date B);