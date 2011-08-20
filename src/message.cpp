/*
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

 * @file message.cpp
 * @author Jacky Alcine
 * @created April 3, 2011, 10:10 AM
 */

#include "message.hpp"
#include <QtDebug>
#include <QDateTime>

using namespace std;
using namespace Wintermute::Network;

using std::string;
using std::map;
using Wintermute::Network::Message;

namespace Wintermute {
    namespace Network {
        int Message::s_count = 0;

        Message::Message () {
            __init ( );
        }

        /// @todo Find a means of copying all of the custom properties set from &msg to this.
        Message::Message ( const Message &msg ) {
            __init();
        }

        Message::Message ( const string& property, QVariant* value ) {
            __init ( );
        }

        Message::~Message () {
        }

        void Message::__init () {
            QDateTime now = QDateTime::currentDateTimeUtc ();
            qDebug() << "(ntwk) [Message] Generated message #" << Message::s_count << "; created on" << now << ".";
            this->setProperty ( "TimeStamp" , now );
            Message::s_count++;
        }

        const QDateTime Message::getCreationTime () const {
            QVariant l_vrt = this->property ( "TimeStamp" ).toDateTime();
            if (l_vrt.isValid ()) return l_vrt.toDateTime ();
            else NULL;
        }

        const string Message::getMessageType () const {
            QVariant l_vrt = this->property ( "MessageType" ).toDateTime();
            if (l_vrt.isValid ()) return l_vrt.toString ().toStdString ();
            else return "";
        }

        const string Message::toString () const {

        }

        /// @todo Implement the deconstruction method from JSON to a Message here.
        Message* Message::fromString ( const string& serializingText ) {

        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
