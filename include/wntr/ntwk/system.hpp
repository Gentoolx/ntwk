/**
 * @file    server.hpp
 * @author  Jacky Alcine <jackyalcine@gmail.com>
 * @date 8/26/2011
 *
 * Copyright (C) 2011 by Jacky Alcine
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Linguistics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Linguistics; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <QObject>
#include <QLocalServer>
#include "message.hpp"

namespace Wintermute {
    namespace Network {
        struct System;
        struct Server;
        struct Client;

        /**
         * @brief Represents the management system of Wintermute's network interactions.
         *
         * @attention Currently, the implementation of the system is down.
         *
         * @todo Implement a system similar to the one in WntrData with the attached
         * data sources and what not. Of course, a base class will need to be implemented
         * for the server class (Server) and the client class (Client) and they would
         * both need to hook into the central system class. (Server) A boiler plate has been
         * defined below.
         *
         * @class Server "include/wntr/ntwk/system.hpp"
         */
        class System : public QObject {
            Q_OBJECT

            friend class Server;

            signals:
                /**
                 * @brief
                 *
                 * @fn messageRecieved
                 * @param
                 */
                void messageRecieved( const Message& );

            private:
                static System s_system; /**< An instance of the System, so that signals and slots can be done. */

            protected:
                /**
                 * @brief
                 *
                 * @fn System
                 */
                System() { }
                /**
                 * @brief
                 *
                 * @fn System
                 * @param
                 */
                System( const System& p_sytm ) { }
                /**
                 * @brief
                 *
                 * @fn System
                 */
                virtual ~System() { }
            public:
                /**
                 * @brief
                 *
                 * @fn send
                 * @param
                 */
                void send( const Message& ) { }
                /**
                 * @brief
                 *
                 * @fn start
                 */
                void start() { }
                /**
                 * @brief
                 *
                 * @fn stop
                 */
                void stop() { }
        };

        /**
         * @brief An abstract server class for connecting with an AngelNet.
         *
         * This class provides a model that of which classes derviving this class can
         * use to implement another protocol to be amended into the system.
         *
         * @class Server system.hpp "include/wntr/ntwk/system.hpp"
         */
        class Server : public QObject {
            Q_OBJECT
            Q_PROPERTY(const QString Protocol READ protocol)

            signals:
                /**
                 * @brief
                 *
                 * @fn messageRecieved
                 * @param
                 */
                void messageRecieved( const Message& );

            public:
                /**
                 * @brief
                 *
                 * @fn Server
                 */
                Server() { }
                /**
                 * @brief
                 *
                 * @fn Server
                 * @param
                 */
                Server(const Server &p_srvr) { }
                /**
                 * @brief
                 *
                 * @fn ~Server
                 */
                virtual ~Server() = 0;
                /**
                 * @brief
                 *
                 * @fn sendBroadcast
                 */
                virtual void sendBroadcast() = 0;
                /**
                 * @brief
                 *
                 * @fn isActive
                 */
                virtual const bool isActive() = 0;
                /**
                 * @brief
                 *
                 * @fn start
                 */
                virtual void start() = 0;
                /**
                 * @brief
                 *
                 * @fn stop
                 */
                virtual void stop() = 0;
                /**
                 * @brief
                 *
                 * @fn protocol
                 */
                virtual const QString protocol() = 0;
        };

        /**
         * @brief
         *
         * @class Client system.hpp "include/wntr/ntwk/system.hpp"
         */
        class Client : public QObject {
            Q_OBJECT
            Q_PROPERTY(const QString protocol READ protocol)

            public:
                /**
                 * @brief
                 *
                 * @fn Client
                 */
                Client() { }
                /**
                 * @brief
                 *
                 * @fn Client
                 * @param
                 */
                Client(const Client& p_clnt) { }
                /**
                 * @brief
                 *
                 * @fn ~Client
                 */
                virtual ~Client() = 0;
                /**
                 * @brief
                 *
                 * @fn protocol
                 */
                virtual const QString protocol() const = 0;
        };

        /**
         * @brief Represents a local socket based server geared towards the AngelNet.
         *
         *
         * @see Server
         * @class LocalServer system.hpp "include/wntr/ntwk/system.hpp"
         */
        class LocalServer : public Server {
            Q_OBJECT

            public:
                /**
                 * @brief
                 *
                 * @fn LocalServer
                 */
                LocalServer() { }
                /**
                 * @brief
                 *
                 * @fn LocalServer
                 * @param
                 */
                LocalServer(const LocalServer& p_srvr) { }
                /**
                 * @brief
                 *
                 * @fn ~LocalServer
                 */
                virtual ~LocalServer() { }
                /**
                 * @brief
                 *
                 * @fn sendBroadcast
                 */
                virtual void sendBroadcast () { }
                /**
                 * @brief
                 *
                 * @fn start
                 */
                virtual void start () { }
                /**
                 * @brief
                 *
                 * @fn stop
                 */
                virtual void stop () { }
                /**
                 * @brief
                 *
                 * @fn isActive
                 */
                virtual const bool isActive () const { }
                /**
                 * @brief
                 *
                 * @fn protocol
                 * @return const QString
                 */
                virtual const QString protocol() const { return "local"; }

            private:
                bool m_active;
                QLocalServer m_srvr;
        };
    }
}

#endif // SERVER_HPP
