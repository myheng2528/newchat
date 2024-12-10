//
// Created by myheng on 24-11-29.
//
#include <iostream>
#include "CServer.h"
#include "const.h"
#include "ConfigMgr.h"
#include "RedisMgr.h"
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/exception.h>


void TestRedisMgr() {
    assert(RedisMgr::GetInstance()->Set("blogwebsite","llfc.club"));
    std::string value="";
    assert(RedisMgr::GetInstance()->Get("blogwebsite", value) );
    assert(RedisMgr::GetInstance()->Get("nonekey", value) == false);
    assert(RedisMgr::GetInstance()->HSet("bloginfo","blogwebsite", "llfc.club"));
    assert(RedisMgr::GetInstance()->HGet("bloginfo","blogwebsite") != "");
    assert(RedisMgr::GetInstance()->ExistsKey("bloginfo"));
    assert(RedisMgr::GetInstance()->Del("bloginfo"));
    assert(RedisMgr::GetInstance()->Del("bloginfo"));
    assert(RedisMgr::GetInstance()->ExistsKey("bloginfo") == false);
    assert(RedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue1"));
    assert(RedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue2"));
    assert(RedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue3"));
    assert(RedisMgr::GetInstance()->RPop("lpushkey1", value));
    assert(RedisMgr::GetInstance()->RPop("lpushkey1", value));
    assert(RedisMgr::GetInstance()->LPop("lpushkey1", value));
    assert(RedisMgr::GetInstance()->LPop("lpushkey2", value)==false);
}

void testMysql(){
    try {
        // 初始化 MySQL Driver
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();

        // 连接到 MySQL 数据库
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3308", "root", "123456"));
        con->setSchema("test");

        // 创建一个测试表
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("CREATE TABLE IF NOT EXISTS test_table ("
                      "id INT AUTO_INCREMENT PRIMARY KEY, "
                      "name VARCHAR(50), "
                      "age INT)");

        // 插入测试数据
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO test_table (name, age) VALUES (?, ?)"));
        pstmt->setString(1, "Alice");
        pstmt->setInt(2, 25);
        pstmt->execute();

        pstmt->setString(1, "Bob");
        pstmt->setInt(2, 30);
        pstmt->execute();

        // 查询数据
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM test_table"));
        while (res->next()) {
            std::cout << "ID: " << res->getInt("id")
                 << ", Name: " << res->getString("name")
                 << ", Age: " << res->getInt("age") << std::endl;
        }

    } catch (sql::SQLException &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "SQLState: " << e.getSQLState() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
    }
}



int main()
{
    //TestRedis();
    //TestRedisMgr();
    testMysql();
    auto gCfgMgr = ConfigMgr::Inst();
    std::string gate_port_str = gCfgMgr["GateServer"]["Port"];
    unsigned short gate_port = atoi(gate_port_str.c_str());
    try
    {
        //unsigned short port = static_cast<unsigned short>(8080);
        net::io_context ioc{ 1 };
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {

            if (error) {
                return;
            }
            ioc.stop();
        });
        std::make_shared<CServer>(ioc, gate_port)->Start();
        ioc.run();
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}