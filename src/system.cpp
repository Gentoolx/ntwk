/**
 * @file    server.cpp
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

#include "system.hpp"
#include <QUuid>
#include <QFile>
#include <QtDebug>
#include <QHostInfo>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QLocalServer>
#include <QLocalSocket>
#include <QUdpSocket>

namespace Wintermute {
    namespace Network {
        System System::s_system;
        System::ServerList System::s_servers;
        LocalServer LocalServer::s_lclSrv;
        TcpServer TcpServer::s_tcpSrv;

        System::System( ) { }

        System::System(const System& p_systm) { }

        void System::start () {
            foreach(Server* p_srvr, s_servers)
                p_srvr->start ();

            qDebug() << "(ntwk) [System]" << s_servers.size () << "server(s) started.";
        }

        void System::stop () {
            foreach(Server* p_srvr, s_servers)
                p_srvr->stop ();

            qDebug() << "(ntwk) [System]" << s_servers.size () << "server(s) stopped.";
        }

        System* System::instance () { return &s_system; }

        void System::send (const Message &p_msg, const Features &p_ftr){
            foreach(Server* p_srvr, s_servers){
                if (p_srvr->features () == p_ftr || p_ftr == None)
                    p_srvr->send (p_msg);
            }
        }

        void System::send (const Message &p_msg, const QString &p_ptrl){
            QStringList l_clnts = allServers();
            const int l_indx = l_clnts.indexOf(p_ptrl);

            if (l_indx != -1)
                s_servers.at (l_indx)->send(p_msg);
        }

        const QStringList System::allServers () {
            QStringList l_srvrs;

            foreach (const Server* l_srvr, s_servers)
                l_srvrs << l_srvr->protocol();

            return l_srvrs;
        }

        /// @todo Be sure to prohibit the user from having qualifiers that are reserved qualifiers.
        /// @todo Where should the qualifiers list be stored?
        /// @todo Doing a host-name lookup could return a list of hosts. Should we return that list?
        /// @todo QHostInfo seems to require an async host name lookup.
        const QHostAddress System::toHostAddress (const QString &p_qual){
            if (p_qual.toLower () == "broadcast")
                return QHostAddress::Broadcast;
            else if (p_qual.toLower () == "self")
                return QHostAddress::LocalHost;

            if (p_qual.indexOf ("wntr::") == 0){
            } else {
                QHostInfo l_hstInf = QHostInfo::fromName (p_qual);
                if (!l_hstInf.addresses().empty())
                    return l_hstInf.addresses ().front ();
                else
                    return QHostAddress::Broadcast;
            }
        }

        System::~System (){  }

        Server::Server() {
            System::s_servers.append (this);
        }

        Server::Server(const Server &p_srvr)  {
            System::s_servers.append (const_cast<Server*>(&p_srvr));
            connect(System::instance (),SIGNAL(messageRecieved(Message)), this,SIGNAL(messageRecieved(Message)));
        }

        Server::~Server () { }

        LocalServer::LocalServer() : Server(), m_srvr(NULL), m_sckt(NULL) { }

        LocalServer::LocalServer(const LocalServer &p_srvr) : Server(p_srvr),
            m_srvr(p_srvr.m_srvr), m_sckt(p_srvr.m_sckt) { }

        void LocalServer::disconnectFromSocket() {
            qDebug() << "(ntwk) [LocalServer] Stopping local server" << m_srvr->serverName ();
            m_srvr->close ();
        }

        void LocalServer::connectToSocket() {
            QUuid l_uuid = QUuid::createUuid();
            m_srvr->listen ("Wintermute_" + l_uuid.toString ());

            if (m_srvr->isListening ()){
                qDebug() << "(ntwk) [LocalServer] Starting local server with name" << m_srvr->serverName ();
                connect(m_srvr,SIGNAL(newConnection()),this,SLOT(handleConnection()));
            } else
                qDebug() << "(ntwk) [LocalServer] Failed to listen:" << m_srvr->errorString ();
        }

        void LocalServer::send(const Message& p_msg){
            m_msgQueue.enqueue(&p_msg);
            const QString l_hst = p_msg.property ("Recipient").toString ();

            if (l_hst.toLower() == "broadcast"){

            } else if (QFile::exists("/tmp/" + l_hst)) {
                m_sckt = new QLocalSocket(this);
                connect(m_sckt,SIGNAL(connected()), this,SLOT(handleConnected()));
                m_sckt->connectToServer(l_hst);
            } else {
                //qDebug() << "(ntwk) [LocalServer] Host not found at" << l_hst;
            }
        }

        /// @todo Send a queued Message, close the socket and then request that the Server sent the next message.
        /// @todo Determine if the method 'toUft8' is a better solution than 'toLocal8Bit'.
        void LocalServer::handleConnected(){
            if (m_sckt->isValid ()){
                qDebug() << "(ntwk) [LocalServer] Socket connected to" << m_sckt->serverName();
                const Message* l_msg = m_msgQueue.dequeue();
                const QByteArray l_buffer = l_msg->toString ().toUtf8 ();
                if (m_sckt->write (l_buffer) == l_buffer.size ()){
                    m_sckt->disconnectFromServer ();
                    m_sckt->deleteLater ();
                }
            } else
                qDebug() << "(ntwk) [LocalServer]" << m_sckt->errorString ();
        }

        void LocalServer::handleConnection() {
            while (m_srvr->hasPendingConnections ()){
                m_sckt = m_srvr->nextPendingConnection ();
                if (m_sckt->isValid () && m_sckt->isReadable () && m_sckt->size () > 0){
                    qDebug() << "(ntwk) [LocalServer] Incoming connection from" << m_sckt->serverName () << "; bytes:" << m_sckt->size ();
                    connect(m_sckt,SIGNAL(readyRead()),this,SLOT(handleRead()));
                } else
                    qDebug() << "(ntwk) [LocalServer] Incoming connection failed:" << m_sckt->errorString ();
            }
        }

        /// @todo Determine if the data's a properly formed Message before converting it into a Message.
        void LocalServer::handleRead() {
            if (m_sckt->isValid () && m_sckt->isReadable ()){
                qDebug() << "(ntwk) [LocalServer] Reading data from" << m_sckt << "; bytes:" << m_sckt->size ();
                QByteArray l_buffer = m_sckt->readAll ();
                qDebug() << l_buffer;
                const Message* l_msg = Message::fromString (l_buffer);
                emit messageRecieved (*l_msg);
            } else
                qDebug() << m_sckt->errorString ();
        }

        void LocalServer::stop () {
            if (m_srvr){
                const QString l_scktName = m_srvr->serverName ();
                disconnectFromSocket();
                m_srvr->blockSignals (true);
                m_srvr->deleteLater ();
                QFile::remove ("/tmp" + l_scktName);
            }
        }

        void LocalServer::start () {
            if (!m_srvr){
                m_srvr = new QLocalServer(this);
                connectToSocket();
            }
        }

        const QString LocalServer::protocol () const { return "local"; }

        const Features LocalServer::features () const { return Reliable; }

        const bool LocalServer::isActive () const { return m_srvr != NULL && m_srvr->isListening (); }

        LocalServer::~LocalServer () { }

        TcpServer::TcpServer() : m_srvr(NULL), m_sckt(NULL), m_brdcstSckt(NULL), Server(*this) { }

        TcpServer::TcpServer(const TcpServer& p_srvr) : m_srvr(p_srvr.m_srvr),
            m_brdcstSckt(NULL), m_sckt(p_srvr.m_sckt), Server(p_srvr) { }

        void TcpServer::start() {
            if (!m_srvr){
                m_srvr = new QTcpServer(this);
                m_brdcstSckt = new QTcpSocket(m_srvr);
                connectToSocket();
            }
        }

        void TcpServer::stop() {
            if (m_srvr){
                disconnectFromSocket();
                m_srvr->blockSignals(true);
                m_srvr->deleteLater();
                m_brdcstSckt->blockSignals(true);
                m_brdcstSckt->deleteLater();
            }
        }

        void TcpServer::connectToSocket() {
            //quint16 l_port = 1300;
            if (m_srvr->listen(QHostAddress::Broadcast))
                qDebug() << "(ntwk) [TcpServer] Started TCP server at" << m_srvr->serverAddress().toString() << ":" << m_srvr->serverPort();
            else
                qDebug() << "(ntwk) [TcpServer]" << m_srvr->errorString();
            connect(m_srvr,SIGNAL(newConnection()),this,SLOT(handleConnection()));

            m_brdcstSckt->connectToHost(QHostAddress::Broadcast,m_srvr->serverPort());
            connect(m_brdcstSckt,SIGNAL(readyRead()), this,SLOT(handleRead()));
        }

        void TcpServer::disconnectFromSocket() {
            m_brdcstSckt->disconnectFromHost();
            m_srvr->close();
        }

        void TcpServer::send(const Message& p_msg){
            m_msgQueue.enqueue(&p_msg);
            const QString l_hostName = p_msg.property ("Recipient").toString ();
            const QHostAddress l_hostAddr = System::toHostAddress (l_hostName);

            if (l_hostAddr != QHostAddress::Null){
                m_sckt = new QTcpSocket(this);
                connect(m_sckt,SIGNAL(connected()), this,SLOT(handleConnected()));
                m_sckt->connectToHost(l_hostAddr,QIODevice::ReadOnly);
            } else {
                //qDebug() << "(ntwk) [TcpServer] Host not found at" << l_hst;
            }
        }

        void TcpServer::handleConnection() {
            while (m_srvr->hasPendingConnections ()){
                m_sckt = m_srvr->nextPendingConnection ();
                if (m_sckt->isValid () && m_sckt->isReadable () && m_sckt->size () > 0){
                    qDebug() << "(ntwk) [TcpServer] Incoming connection from" << m_sckt->peerName () << "; bytes:" << m_sckt->size ();
                    connect(m_sckt,SIGNAL(readyRead()),this,SLOT(handleRead()));
                } else
                    qDebug() << "(ntwk) [TcpServer] Incoming connection failed:" << m_sckt->errorString ();
            }
        }

        void TcpServer::handleConnected() {
            if (m_sckt->isValid ()){
                qDebug() << "(ntwk) [TcpServer] Socket connected to (" << m_sckt->peerName() << ")"<< m_sckt->peerAddress() << ":" << m_sckt->peerPort();
                const Message* l_msg = m_msgQueue.dequeue();
                const QByteArray l_buffer = l_msg->toString ().toUtf8 ();
                if (m_sckt->write (l_buffer) == l_buffer.size ()){
                    m_sckt->disconnectFromHost ();
                    m_sckt->deleteLater ();
                }
            } else
                qDebug() << "(ntwk) [TcpServer]" << m_sckt->errorString ();
        }

        void TcpServer::handleRead() {
            if (m_sckt->isValid () && m_sckt->isReadable ()){
                qDebug() << "(ntwk) [TcpServer] Reading data from" << m_sckt << "; bytes:" << m_sckt->size ();
                QByteArray l_buffer = m_sckt->readAll ();
                qDebug() << l_buffer;
                const Message* l_msg = Message::fromString (l_buffer);
                emit messageRecieved (*l_msg);
            } else
                qDebug() << m_sckt->errorString ();

            if (m_brdcstSckt->isValid() && m_brdcstSckt->isReadable()){
                QByteArray l_buffer = m_sckt->readAll ();
                qDebug() << l_buffer;
                const Message* l_msg = Message::fromString (l_buffer);
                emit messageRecieved (*l_msg);
            }
        }

        const QString TcpServer::protocol() const { return "tcp"; }

        const Features TcpServer::features() const { return Reliable; }

        const bool TcpServer::isActive() const { return m_srvr != NULL && m_srvr->isListening(); }

        TcpServer::~TcpServer() { }
    }
}
