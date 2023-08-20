#include "json.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <ctime>

using namespace std;
using json = nlohmann::json;

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<semaphore.h>
#include<atomic>

#include "group.hpp"
#include "user.hpp"
#include "public.hpp"

// 记录当前用户信息
User g_currentUser;

// 记录当前用户好友信息
vector<User> g_currentUserFriendList;

// 记录当前用户的群组列表信息
vector<Group> g_currentUserGroupList;

//控制聊天页面结束
bool isMainmenuRuning = false;

//用于读写线程通信的信号量
sem_t rwsem;

//登录状态，atomic防止线程安全问题
atomic_bool g_loginState{false};


// 系统支持的客户端命令列表
unordered_map<string, string> commandMap = {
    {"help", "显示所有支持的命令，格式help"},
    {"chat", "一对一聊天，格式chat:friendid:message"},
    {"addfriend", "添加好友，格式addfriend:friendid"},
    {"creategroup", "创建群组，格式creategroup:groupname:groupdesc"},
    {"addgroup", "加入群聊，格式addgroup:groupid"},
    {"groupchat", "群聊，格式groupchat:groupid:message"},
    {"loginout", "注销，格式loginout"}};


// 显示当前登录成功的用户信息
void showCurrentUserData()
{
    cout << "=================login user===========" << endl;
    cout << "current login user => id:" << g_currentUser.get_id() << " name:" << g_currentUser.get_name() << endl;
    cout << "------------------friend list----------" << endl;
    if (!g_currentUserFriendList.empty())
    {
        for (auto &it : g_currentUserFriendList)
        {
            cout << "id:" << it.get_id() << " name:" << it.get_name() << " state:" << it.get_state() << endl;
        }
    }
    cout << "-------------------group list-----------" << endl;
    if (!g_currentUserGroupList.empty())
    {
        for (auto &it : g_currentUserGroupList)
        {
            cout << "Groupid" << it.get_id() << " GroupName:" << it.get_name() << " GroupDesc:" << it.get_desc() << endl;
            cout << "Group user:" << endl;
            for (auto &user : it.get_User())
            {
                cout << "groupuserid:" << user.get_id() << " name:" << user.get_name() << " state:" << user.get_state() << " GroupRole:" << user.get_role() << endl;
            }
        }
    }
    cout << "========================================" << endl;
}

//处理登录业务逻辑
void doLoginResponse(json &response)
{    
    if (0 != response["errno"].get<int>())
    {
        cerr << response["errmsg"] << endl;
        g_loginState = false;
    }
    else
    {
        // 记录当前用户id和name名字
        g_currentUser.set_id(response["id"].get<int>());
        g_currentUser.set_name(response["name"]);

        // 记录当前用户好友列表
        if (response.contains("friends"))
        {
            vector<string> vec1 = response["friends"];
            for (string &str : vec1)
            {
                json js = json::parse(str);
                User user;
                user.set_id(js["id"].get<int>());
                user.set_name(js["name"]);
                user.set_state(js["state"]);
                g_currentUserFriendList.push_back(user);
            }
        }

        // 记录当前用户的群组列表信息22::44
        if (response.contains("groups"))
        {
            vector<string> vec1 = response["groups"];
            for (string &groupstr : vec1)
            {
                json groupjs = json::parse(groupstr);
                Group group;
                group.set_id(groupjs["id"].get<int>());
                group.set_name(groupjs["groupname"]);
                group.set_desc(groupjs["groupdesc"]);

                vector<string> vec2 = groupjs["users"];
                for (string &groupuserstr : vec2)
                {
                    json userjs = json::parse(groupuserstr);
                    GroupUser user;
                    user.set_id(userjs["id"].get<int>());
                    user.set_name(userjs["name"]);
                    user.set_state(userjs["state"]);
                    user.set_role(userjs["role"]);
                    group.get_User().push_back(user);
                }
                g_currentUserGroupList.push_back(group);
            }
        }

        // 显示用户基本信息
        showCurrentUserData();

        // 显示当前用户的离线信息  个人聊天信息或者群组信息
        if (response.contains("offlinemsg"))
        {
            vector<string> vec = response["offlinemsg"];
            for (auto &it : vec)
            {
                json js = json::parse(it);
                cout << js["time"] << " [" << js["id"] << "]" << js["name"] << " said: " << js["msg"] << endl;
            }
        }
        g_loginState = true;
    }
}

void doRegResponse(json &response)
{     
    if (0 != response["errno"].get<int>())
    {
        cerr << "is already exit , register error!" << endl;
    }
    else
    {
        cout << "register is success,userid is " << response["id"] << ", do not forget it!" << endl;
    }
}

// 接受线程
void readTaskHandler(int clientfd)
{
    while (1)
    {
        char buffer[1024] = {0};
        int len = recv(clientfd, buffer, 1024, 0);
        if (-1 == len)
        {
            close(clientfd);
            exit(-1);
        }

        // 反序列化接受到的消息 string-》json
        json js = json::parse(buffer);
        int msgtype = js["msgid"].get<int>(); 

        //接收一对一聊天消息
        if (ONE_CHAT_MSG == msgtype)
        {
            cout << js["time"].get<string>() << " [" << js["id"] << "]" << js["name"].get<string>() << " said:" << js["msg"].get<string>() << endl;
            continue;
        }
        
        //接收群聊消息
        if(GROUP_CHAT_MSG == msgtype)
        {
           cout << js["time"].get<string>()<<"groupMsg:{"<<"groupid: "<<js["groupid"]<< "} [" << js["id"] << "]" << js["name"].get<string>() << " said:" << js["msg"].get<string>() << endl;
            continue; 
        }

        //接受登录消息
        if(LOGIN_MSG_ACK == msgtype)
        {
            //处理登录业务逻辑 
            doLoginResponse(js);
            //并将结果告知主线程,成功或失败
            sem_post(&rwsem);
            continue;
        }

        //处理注册业务
        if(REG_MSG_ACK == msgtype)
        {
            doRegResponse(js);
            //通知注册结果
            sem_post(&rwsem);
            continue;
        }
    }
} 

// 获取系统时间
string getCurrentTime()
{
    auto tt = chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm *ptm = localtime(&tt);
    char date[60] = {0};
    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d",
            (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
            (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
    return string(date);
}
 

void help(int fd = 0, string str = "")
{
    for (auto &it : commandMap)
    {
        cout << it.first << " " << it.second << endl;
    }
}

void chat(int fd, string str)
{
    int friendid;
    string msg;

    int idx = str.find(":");
    if(-1 == idx)
    {
        cerr<<"chat invalid command"<<endl;
        return;
    }
    else
    {
        friendid = atoi(str.substr(0,idx).c_str());
        msg = str.substr(idx+1,str.size()-idx);
    }

    json js;
    js["msgid"] = ONE_CHAT_MSG;
    js["id"] = g_currentUser.get_id();
    js["name"]=g_currentUser.get_name();
    js["to"] = friendid;
    js["msg"]=msg;
    js["time"]=getCurrentTime();

    string buff = js.dump();
    int len = send(fd, buff.c_str(), strlen(buff.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "send chat msg error-> " << buff << endl;
    } 
}

void addfriend(int fd, string str)
{
    int friendid = atoi(str.c_str());
    json js;
    js["msgid"] = ADD_FRIEND_MSG;
    js["id"] = g_currentUser.get_id();
    js["friendid"] = friendid;

    // 序列化
    string buff = js.dump();
    int len = send(fd, buff.c_str(), strlen(buff.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "send addfriend msg error-> " << buff << endl;
    }
}

void creategroup(int fd, string str)
{
    string groupname;
    string groupdesc;
    int idx = str.find(":");
    if(-1 == idx)
    {
        cerr<<"creategroup invalid command"<<endl;
        return;
    }
    else
    {
        groupname = str.substr(0,idx);
        groupdesc = str.substr(idx+1,str.size()-idx);
    }

    json js;
    js["msgid"]=CREATE_GROUP_MSG;
    js["id"]=g_currentUser.get_id();
    js["groupname"]=groupname;
    js["groupdesc"]=groupdesc;

    string buff = js.dump();
    int len = send(fd,buff.c_str(),strlen(buff.c_str())+1,0);
    if(-1 == len)
    {
        cerr<<"send creategroup msg error-> "<<buff<<endl;        
    }
}

void addgroup(int fd, string str)
{
    int groupid = atoi(str.c_str());
    json js;
    js["msgid"] = ADD_GROUP_MSG;
    js["id"] = g_currentUser.get_id();
    js["groupid"] = groupid;

    string buff = js.dump();
    int len = send(fd, buff.c_str(), strlen(buff.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "send addgroup msg error-> " << buff << endl;
    }
}

void groupchat(int fd, string str)
{
    int groupid;     
    string msg;

    int idx = str.find(":");
    if(-1 == idx)
    {
        cerr<<"groupchat invalid command"<<endl;
        return;
    }
    else
    {
        groupid = atoi(str.substr(0,idx).c_str());
        msg = str.substr(idx+1,str.size()-idx);
    }

    json js;
    js["msgid"] = GROUP_CHAT_MSG;
    js["id"] = g_currentUser.get_id();
    js["name"]= g_currentUser.get_name();
    js["groupid"] = groupid;    
    js["msg"]=msg;
    js["time"]=getCurrentTime();

    string buff = js.dump();
    int len = send(fd, buff.c_str(), strlen(buff.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "send groupchat msg error-> " << buff << endl;
    } 
}

void loginout(int fd, string str)
{
    json js;
    js["msgid"]=LOGINOUT_MSG;
    js["id"]=g_currentUser.get_id();
    string buff = js.dump();

    int len = send(fd,buff.c_str(),strlen(buff.c_str())+1,0);
    if(-1 == len)
    {
        cerr<<"send longinout msg error-> "<<buff<<endl;
    }
    else
    {
        isMainmenuRuning = false;
        //释放信号量资源
        sem_destroy(&rwsem);
        g_currentUserFriendList.clear();
        g_currentUserGroupList.clear();
    }
}

// 注册系统支持的客户端命令处理
unordered_map<string, function<void(int, string)>> commandHanderMap = {
    {"help", help},
    {"chat", chat},
    {"addfriend", addfriend},
    {"creategroup", creategroup},
    {"addgroup", addgroup},
    {"groupchat", groupchat},
    {"loginout", loginout}}; 

// 主聊天页面程序
void mainMenu(int clientfd)
{
    // 显示系统支持的命令,其含有默认值可不传参数
    help();
    char buffer[1024] = {0};
    while (isMainmenuRuning)
    {
        cin.getline(buffer, 1024);
        string commandbuf(buffer);
        // 存储命令
        string command;
        int idx = commandbuf.find(":");
        if (-1 == idx)
        {
            command = commandbuf;
        }
        else
        {
            command = commandbuf.substr(0, idx);
        }

        auto it = commandHanderMap.find(command);
        if (it == commandHanderMap.end())
        {
            cerr << "invalid input command!" << endl;
            continue;
        }

        // 调用相应命令的事件处理回调
        it->second(clientfd, commandbuf.substr(idx + 1, commandbuf.size() - idx));
    }
}

// main线程作为发送线程，子线程作为接受线程
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "command invalied! example: ./chatclient 127.0.0.1 6000" << endl;
        exit(-1);
    }

    // 解析传入的ip和端口号
    char *ip = argv[1];
    int port = atoi(argv[2]);

    // 创建client端的socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == clientfd)
    {
        cerr << "socket create error!" << endl;
        exit(-1);
    }

    // 填写client需要连接的server信息ip和端口
    sockaddr_in server;
    memset(&server, 0, sizeof(sockaddr_in));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);

    // client与server进行连接
    if (-1 == connect(clientfd, (sockaddr *)&server, sizeof(sockaddr_in)))
    {
        cerr << "connect is error!" << endl;
        close(clientfd);
        exit(-1);
    }

    //初始化信号量
    sem_init(&rwsem,0,0);

    // 登录成功，启动线程负责接受数据
    std::thread readTask(readTaskHandler, clientfd);
    readTask.detach();


    // main线程用于接受用户输入和发送数据
    while (1)
    {
        // 显示菜单页面
        cout << "============================================" << endl;
        cout << "1.Login" << endl;
        cout << "2.regist" << endl;
        cout << "3.quit" << endl;
        cout << "============================================" << endl;
        cout << "choice:";
        int choice = 0;
        cin >> choice;
        // 读取缓冲区的回车符
        cin.get();

        switch (choice)
        {
        case 1:
        {
            int id = 0;
            char password[50] = {0};
            cout << "id:";
            cin >> id;
            // 读掉缓冲区残留的回车符
            cin.get();
            cout << "password:";
            cin.getline(password, 50);

            json js;
            js["msgid"] = LOGIN_MSG;
            js["id"] = id;
            js["password"] = password;

            string request = js.dump();
            g_loginState = false;

            int len = send(clientfd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (-1 == len)
            {
                cerr << "send login msg is error!" << endl;
            }     
         
            //阻塞等待信号量，由子线程处理完登录响应后，通知这里
            sem_wait(&rwsem);
            if(g_loginState)
            {
               isMainmenuRuning = true;
               // 进入聊天主页面
               mainMenu(clientfd);   
            }      
                
        }
        break;
        case 2:
        {
            string name;
            string password;
            cout << "username:";
            cin >> name;
            cout << "userpassword:";
            cin >> password;

            json js;
            js["msgid"] = REG_MSG;
            js["name"] = name;
            js["password"] = password;
            string request = js.dump();

            int len = send(clientfd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (len == -1)
            {
                cerr << "send is error!" << request << endl;
            }

            //阻塞等待信号量，由子线程处理注册响应后，通知这里
            sem_wait(&rwsem);
        }
        break;
        case 3:
        {
            close(clientfd);
            //信号量资源释放
            sem_destroy(&rwsem);
            exit(0);
        }
        break;
        default:
        {
            cerr << "invalied input!" << endl;
        }
        break;
        }
    }

    return 0;
}
