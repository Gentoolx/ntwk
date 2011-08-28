/**
 * @author Jacky Alcine <jacky.alcine@thesii.org>
 * @legalese
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
 * @endlegalese
 *
 */

#include "wntrntwk.hpp"
#include <boost/python.hpp>

using namespace boost::python;
using namespace Wintermute;
using namespace Wintermute::Network;

namespace Wintermute {
    namespace Network {
        BOOST_PYTHON_MODULE ( wntrntwk ) {
            class_<Message> ( "Message",init<const QString&, QVariant*>() )
            .def ( "setProperty",&Message::setProperty )
            .def( "getProperty",&Message::property )
            .def ( "getMessageType",&Message::getMessageType )
            .def ( "toString",&Message::toString );

            class_<BroadcastMessage, bases<Message> > ( "BroadcastMessage",init<const BroadcastType>() )
            .def ( "getBroadcastType",&BroadcastMessage::getBroadcastType );

            class_<Broadcast, boost::noncopyable > ( "Broadcast" ,no_init )
            .def ( "initialize",&Broadcast::initialize )
            .def ( "isActive",&Broadcast::isActive )
            .def ( "deinitialize",&Broadcast::deinitialize );
        }

    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;
