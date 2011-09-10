/**
 * @file    adaptors.hpp
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

#ifndef ADAPTORS_HPP
#define ADAPTORS_HPP

#include <wntr/adaptors.hpp>
#include <QtDBus/QDBusMessage>

using Wintermute::Adaptor;

namespace Wintermute {
    namespace Network {
        struct Message;
        struct BroadcastAdaptor;
        struct SystemAdaptor;

        class BroadcastAdaptor : public Adaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.Network.Broadcast")

            public:
                explicit BroadcastAdaptor();

            signals:
                void started();
                void stopped();

            public slots:
                virtual void quit(const QDBusMessage&) const;
                Q_INVOKABLE const bool isBroadcasting(const QDBusMessage&) const;
                Q_INVOKABLE Q_NOREPLY void sendBroadcast(const QDBusMessage&);
                Q_NOREPLY void stopBroadcasting(const QDBusMessage&);
                Q_NOREPLY void startBroadcasting(const QDBusMessage&);
        };

        class SystemAdaptor : public Adaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.Network.System")

            public:
                explicit SystemAdaptor();

            signals:
                void messageRecieved(const Network::Message& );
                void started();
                void stopped();

            public slots:
                Q_INVOKABLE Q_NOREPLY void sendMessage(const Network::Message&,const QDBusMessage&);
                Q_INVOKABLE const bool isActive(const QDBusMessage&) const;
                virtual void quit(const QDBusMessage&) const;
        };
    }
};

#endif // ADAPTORS_HPP
