#ifndef __USER_H__
#define __USER_H__
#include <k_total_head.h>

/*用户功能暂定
    1.设置要素值
    2.用户同一个判断、用户赋值、获取各要素值
    3.不可单独修改某一个要素的值(需整个用户一起修改)

*/
class CUser
{

public:
    CUser();
    ~CUser();
    CUser(int user_id,char *account,char *password,
        char* user_name,int16_t user_age);
    CUser(const CUser &other);

    //为用户设置对应的值
    void set_user_info(int user_id,char *account,char *password,
        char* user_name,int16_t user_age);
    // 用account(账号)来判断是否相等
    bool operator==(const CUser &other);
    CUser &operator=(const CUser &other);

    // 返回对应的变量值
    int get_id() const;
    char *get_account() const;
    char *get_password() const;
    char *get_name() const;
    int16_t get_age() const;
    void print();

private:
    int _user_id;
    char _account[7];
    char _password[16];
    char _user_name[255];
    int16_t _user_age;
};

#endif
