//
// Created by myheng on 24-12-3.
//

#pragma once

#include <memory>
#include "Singleton.h"
#include <hiredis/hiredis.h>
#include <atomic>
#include <queue>
#include <condition_variable>
#include "ConfigMgr.h"

class RedisConPool {
public:
    RedisConPool(size_t poolSize, const char* host, int port, const char* pwd);
    ~RedisConPool();
    redisContext* getConnection();
    void returnConnection(redisContext* context);
    void Close();

private:
    std::atomic<bool> b_stop_;
    size_t poolSize_;
    const char* host_;
    int port_;
    std::queue<redisContext*> connections_;
    std::mutex mutex_;
    std::condition_variable cond_;
};


class RedisMgr: public Singleton<RedisMgr>,
                public std::enable_shared_from_this<RedisMgr>
{
    friend class Singleton<RedisMgr>;
public:
    ~RedisMgr();
    bool Get(const std::string &key, std::string& value);
    bool Set(const std::string &key, const std::string &value);
    bool LPush(const std::string &key, const std::string &value);
    bool LPop(const std::string &key, std::string& value);
    bool RPush(const std::string& key, const std::string& value);
    bool RPop(const std::string& key, std::string& value);
    bool HSet(const std::string &key, const std::string  &hkey, const std::string &value);
    bool HSet(const char* key, const char* hkey, const char* hvalue, size_t hvaluelen);
    std::string HGet(const std::string &key, const std::string &hkey);
    bool Del(const std::string &key);
    bool ExistsKey(const std::string &key);
    void Close();
private:
    RedisMgr();
    std::unique_ptr<RedisConPool> _con_pool;
};
