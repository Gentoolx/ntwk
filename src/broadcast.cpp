/**
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.

 * @file broadcast.cpp
 * @date April 9, 2011, 1:45 PM
 */

#include "broadcast.hpp"
#include "system.hpp"
#include <QTimer>
#include <QtDebug>

using namespace std;

namespace Wintermute {
    namespace Network {
        Broadcast* Broadcast::s_brdcst = NULL;

        void Broadcast::initialize( ) {
            s_brdcst = new Broadcast;
            qDebug() << "(ntwk) [Broadcast] Initializing... ";

            connect(System::instance (),SIGNAL(messageRecieved(Message)),
                    s_brdcst,SLOT(readSignal(Message)));

            start();
        }

        void Broadcast::deinitialize( ) {
            delete s_brdcst;
            qDebug() << "(ntwk) [Broadcast] Destroying... ";
            System::instance()->disconnect(s_brdcst,SLOT(readSignal(const Message&)));
            stop();
        }

        void Broadcast::start() {
            qDebug() << "(ntwk) [Broadcast] Starting broadcasting activity... ";
            s_brdcst->sendSignal ();
        }

        void Broadcast::stop() {
            qDebug() << "(ntwk) [Broadcast] Stopping broadcasting activity... ";
        }

        void Broadcast::sendSignal() const {
            BroadcastMessage aMsg(BroadcastMessage::Online);
            //qDebug() << "(ntwk) [Broadcast] Attempting to send message '" << aMsg.toString () << "'";
            System::send(aMsg);
            QTimer::singleShot(200, s_brdcst, SLOT(sendSignal()));
        }

        void Broadcast::readSignal(const Message& p_msg) const {
            qDebug() << "(ntwk) [Broadcast] Attempting to read any messages..";
            if (p_msg.type() == "Broadcast") {
                BroadcastMessage l_msg(p_msg);
                switch (l_msg.broadcastType()) {
                case BroadcastMessage::Ping:
                    emit pingReply(l_msg.property ("Sender").toString ());
                    break;
                default:
                    break;
                }
            }
        }

        const bool Broadcast::isActive () {
            return (s_brdcst != NULL);
        }

        void Broadcast::ping(const QString& p_qualifier) {
            BroadcastMessage l_msg(BroadcastMessage::Ping);
            l_msg.setProperty ("Recipient",p_qualifier);
            System::send(l_msg);
        }

        /// @todo Just convert the address into a qualifier and send it to the other version of the method.
        void Broadcast::ping(const QHostAddress& p_addr) {
            Broadcast::ping("");
        }

        BroadcastMessage::BroadcastMessage( const Message& p_msg) : Message(p_msg) {

        }

        BroadcastMessage::BroadcastMessage ( const BroadcastType& brdtype ) : Message( ) {
            this->setProperty ( "Type" , "Broadcast" );
            this->setProperty ( "BroadcastType" , ( ( int ) brdtype ) );
            this->setProperty ( "Recipient" , "Wintermute2");
        }

        const BroadcastMessage::BroadcastType BroadcastMessage::broadcastType( ) const {
            const int val = this->property ( "BroadcastType" ).toInt ();
            return static_cast< const BroadcastType > ( val );
        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
