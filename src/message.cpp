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
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @date April 3, 2011, 10:10 AM
 */

#include "message.hpp"
#include <qjson/qobjecthelper.h>
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <QtDebug>

using namespace std;
using namespace Wintermute::Network;

using std::string;
using std::map;
using Wintermute::Network::Message;

namespace Wintermute {
    namespace Network {
        long Message::s_count = 0;

        Message::Message () {
            __init ( );
        }

        Message::Message ( const Message &msg ) {
            QList<QByteArray> l_propNames = msg.dynamicPropertyNames ();
            foreach (const QString l_propName, l_propNames)
            this->setProperty (l_propName.toStdString ().c_str (),msg.property (l_propName.toStdString ().c_str ()));

            __init();
        }

        Message::Message ( const QString& property, QVariant* value ) {
            __init ( );
        }

        Message::~Message () {
        }

        void Message::__init () {
            QDateTime now = QDateTime::currentDateTimeUtc ();
            //qDebug() << "(ntwk) [Message] Generated message #" << Message::s_count << "; created on" << now << ".";
            this->setProperty ( "TimeStamp" , now );
            Message::s_count++;
        }

        const QDateTime Message::creationTime () const {
            QVariant l_vrt = this->property ( "TimeStamp" ).toDateTime();
            if (l_vrt.isValid ()) return l_vrt.toDateTime ();
            else NULL;
        }

        const QString Message::type () const {
            QVariant l_vrt = this->property ( "MessageType" ).toDateTime();
            if (l_vrt.isValid ()) return l_vrt.toString ();
            else return "";
        }

        /// @todo Implement the means of converting this object's properties to a QString. (Using QJson)
        const QString Message::toString () const {
            return "COOHIE!";
        }

        /// @todo Implement the means of converting a QString into a Message.
        Message* Message::fromString ( const QString& serializedText ) {
            return NULL;
        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
