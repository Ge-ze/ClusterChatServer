#include"log.hpp"
#include"connection.hpp"


//初始化数据库连接
Connection::Connection()
{
    _conn = mysql_init(nullptr);
}

//释放数据库连接资源
Connection::~Connection()
{
    if(_conn != nullptr)
    {
        mysql_close(_conn);
    }
}

//连接数据库
bool Connection::connect(string ip, unsigned short port, string user, string password, string dbname)
{
    MYSQL *p = mysql_real_connect(_conn,ip.c_str(),user.c_str(),password.c_str(),dbname.c_str(),port,nullptr,0);
    if (p != nullptr)
    {
        // 代码支持中文，因为默认是ASCII
        mysql_query(_conn, "set names gbk");
        LOG("connect mysql success!");
    }
    else
    {
        LOG("connect mysql fail!");
    }

    return p;
}

//更新操作 insert、delete、update
bool Connection::update(string sql)
{
    if(mysql_query(_conn,sql.c_str()))
    {
        LOG("更新失败" + sql +" "+ mysql_error(_conn));
        return false;
    }
    return true;
}

//查询操作 select
MYSQL_RES* Connection::query(string sql)
{
    if(mysql_query(_conn,sql.c_str()))
    {
        LOG("查询失败" + sql +" "+ mysql_error(_conn));         
        return nullptr;
    }
    return mysql_use_result(_conn);
}