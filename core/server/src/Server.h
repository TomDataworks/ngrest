/*
 *  Copyright 2016 Utkin Dmitry <loentar@gmail.com>
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 *  This file is part of ngrest: http://github.com/loentar/ngrest
 */

#ifndef NGREST_SERVER_H
#define NGREST_SERVER_H

#ifdef USE_GET_WRITE_QUEUE
#ifndef WIN32
#include <sys/select.h>
#else
#include <winsock2.h>
#endif
#endif

#include "servercommon.h"

#ifdef HAS_EPOLL
struct epoll_event;
#endif

namespace ngrest {

class ClientCallback;

/**
 * @brief simple socket server class with support of epoll or select
 */
class Server
{
public:
    /**
     * @brief constructor
     */
    Server();

    /**
     * @brief destructor
     */
    ~Server();

    /**
     * @brief create server with arguments
     * @param args arguments to pass to server
     * @return true - server successfully created
     */
    bool create(const StringMap& args);

    /**
     * @brief set callback to handle client events
     * @param callback client callback
     */
    void setClientCallback(ClientCallback* callback);

    /**
     * @brief start server with epoll event loop (or with select)
     * @return server exit status
     */
    int exec();

    /**
     * @brief stops the server
     */
    void quit();

private:
    Socket createServerSocket(const std::string& ip, const std::string& port);
    bool setupNonblock(Socket fd);
    bool handleIncomingConnection();
    void handleRequest(Socket fd);

private:
    bool isStopping = false;
    Socket fdServer = 0;
    int fdEpoll = 0;
#ifdef HAS_EPOLL
    epoll_event* event = nullptr;
    epoll_event* events = nullptr;
#else
    fd_set activeFds;
#endif
    ClientCallback* callback = nullptr;
    std::string ip;
    std::string port;
};

}

#endif // NGREST_SERVER_H
