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
#include "server.hpp"
#include <QtDebug>
using namespace std;

namespace Wintermute {
    namespace Network {
        void Broadcast::deinitialize( ) {
            qDebug() << "(ntwk) [Broadcast] Destroying... ";
            stop();
        }

        void Broadcast::initialize( ) {
            qDebug() << "(ntwk) [Broadcast] Initializing... ";
            start();
        }

        void Broadcast::start() {
            qDebug() << "(ntwk) [Broadcast] Starting broadcasting activity... ";
        }

        void Broadcast::stop() {
            qDebug() << "(ntwk) [Broadcast] Stopping broadcasting activity... ";
        }

        void Broadcast::sendSignal() {
            /// @todo Send out one broadcast message and queue another to be sent.
            BroadcastMessage aMsg;
            qDebug() << "(ntwk) [Broadcast] Attempting to send message '" << aMsg.toString () << "'";
        }

        void Broadcast::readSignal(const Message& p_msg) {
            /// @todo Read ONE broadcast message and queue to read another.
            qDebug() << "(ntwk) [Broadcast] Attempting to read any messages..";
        }

        const bool Broadcast::isActive () {
            return false;
        }

        BroadcastMessage::BroadcastMessage ( const BroadcastType& brdtype ) : Message( ) {
            this->setProperty ( "typ" , "brdcst" );
            this->setProperty ( "brdcst_typ" , ( ( int ) brdtype ) );
        }

        const BroadcastType BroadcastMessage::getBroadcastType( ) const {
            const int val = this->property ( "brdcst_typ" ).toInt ();
            return static_cast< const BroadcastType > ( val );
        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
