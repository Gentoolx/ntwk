/**
 * @file    adaptors.cpp
 * @author  Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @created 9/6/2011
 *
 *
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

#include "wntr/ntwk/adaptors.hpp"
#include <wntrntwk.hpp>
#include <QDBusConnection>

namespace Wintermute {
    namespace Network {
        SystemAdaptor::SystemAdaptor() : Adaptor(Network::Interface::instance ()) {
            setAutoRelaySignals (true);
            connect(this,SIGNAL(aboutToQuit()),Network::Interface::instance (),SLOT(stop()));
            QDBusConnection::sessionBus().connect ("org.thesii.Wintermute","/Master",
                                      "org.thesii.Wintermute.Master","aboutToQuit",
                                      this,SLOT(quit()));
        }

        const bool SystemAdaptor::isActive (const QDBusMessage& p_msg) const {
            QDBusConnection::sessionBus ().send(p_msg.createReply (Network::Interface::instance ()->isActive ()));
            return Network::Interface::instance()->isActive ();
        }

        void SystemAdaptor::sendMessage (const Network::Message &p_msg, const QDBusMessage& p_dmsg){
            Network::System::send (p_msg);
            QDBusConnection::sessionBus ().send(p_dmsg.createReply (QVariant::fromValue(p_msg.type ())));
        }

        void SystemAdaptor::quit (const QDBusMessage& p_msg) const {
            CoreAdaptor::haltSystem();
            QDBusConnection::sessionBus ().send(p_msg.createReply(true));
        }

        BroadcastAdaptor::BroadcastAdaptor() : Adaptor(Broadcast::instance()) {
            setAutoRelaySignals (true);
            connect(this,SIGNAL(aboutToQuit()),Broadcast::instance(),SLOT(stop()));
        }

        const bool BroadcastAdaptor::isBroadcasting (const QDBusMessage& p_msg) const {
            QDBusConnection::sessionBus ().send(p_msg.createReply(Broadcast::isActive ()));
            return Broadcast::isActive ();
        }

        void BroadcastAdaptor::startBroadcasting (const QDBusMessage& p_msg) {
            if (!Broadcast::isActive ()){
                Broadcast::start ();
                emit started ();
            }

            QDBusConnection::sessionBus ().send(p_msg.createReply (!(Broadcast::isActive () == true)));
        }

        void BroadcastAdaptor::stopBroadcasting (const QDBusMessage& p_msg) {
            if (Broadcast::isActive ()){
                Broadcast::stop ();
                emit stopped ();
            }

            QDBusConnection::sessionBus ().send(p_msg.createReply (!(Broadcast::isActive () == false)));
        }

        void BroadcastAdaptor::sendBroadcast (const QDBusMessage& p_msg){
            Broadcast::forceSignal ();
            QDBusConnection::sessionBus ().send(p_msg.createReply(true));
        }

        void BroadcastAdaptor::quit (const QDBusMessage& p_msg) const {
            CoreAdaptor::haltSystem();
            QDBusConnection::sessionBus ().send(p_msg.createReply(true));
        }
    }
}
