//
// Created by myheng on 24-12-1.
//
#pragma once



#include <grpcpp/grpcpp.h>
#include <queue>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"
#include <atomic>
#include "ConfigMgr.h"


using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;



class RPConPool {
public:
    RPConPool(size_t poolSize, std::string host, std::string port);

    ~RPConPool();

    std::unique_ptr<VarifyService::Stub> getConnection();

    void returnConnection(std::unique_ptr<VarifyService::Stub> context);

    void Close();

private:
    std::atomic<bool> b_stop_;
    size_t poolSize_;
    std::string host_;
    std::string port_;
    std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
    std::mutex mutex_;
    std::condition_variable cond_;
};


class VarifyGrpcClient:public Singleton<VarifyGrpcClient>
{
    friend class Singleton<VarifyGrpcClient>;
public:

    GetVarifyRsp GetVarifyCode(std::string email);

private:
    VarifyGrpcClient();

    std::unique_ptr<RPConPool> pool_;
};


