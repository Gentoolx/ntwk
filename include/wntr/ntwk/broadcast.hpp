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

 * @file broadcast.hpp
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @date April 9, 2011, 1:44 PM
 */

#ifndef BROADCAST_HPP
#define BROADCAST_HPP

#include "message.hpp"

using namespace Wintermute::Network;

using Wintermute::Network::Message;

namespace Wintermute {
    namespace Network {
        struct Broadcast;
        struct BroadcastMessage;

        /**
         * @brief Broadcasting types.
         * Represents the types of broadcast messages that can be sent.
         * @see Broadcast
         * @see BroadcastMessage;
         */
        enum BroadcastType {
            Unspecified = 0,
            Ping
        };

        /**
         * @brief Management class for broadcasting.
         * This class mananges every aspect of Wintermute's ability to send
         * broadcast signals out onto the network.
         */
        class Broadcast {
            private:
                static bool s_actv;
            public:
                /**
                 * @brief Initializes the broadcast system.
                 */
                static void initialize( );

                /**
                 * @brief Destroys the broadcast system.
                 */
                static void deinitialize( );
                /**
                  @brief Starts the system.
                  */
                static void start();
                /**
                  * @brief Stops the system.
                  */
                static void stop();
                /**
                  * @brief Send broadcast signal.
                  * Determines if a broadcast signal can be sent, and if so, sends it.
                  */
                static void sendSignal();
                /**
                  * @brief Recieves broadcast signal.
                  * Determines if a broadcast signal can be read, and if so, recieves it.
                  */
                static void readSignal(const Message& );
                /**
                 * @brief Determines if it's accepting.
                 */
                static const bool isActive( );
        };

        /**
         * @brief A message designed to represent existence on a network.
         * @see Message
         * Inherited from Message, this class handles the work of representing, determining and querying the
         * existence of other Wintermute processes in the local and remote network.
         */
        class BroadcastMessage : public Message {
            public:

                /**
                 * @brief Constructor.
                 * Creates a new BroadcastMessage of type Unspecified.
                 * @param BroadcastType|type A reference to a broadcast type, defaults to Unspecified.
                 * @see BroadcastType
                 */
                BroadcastMessage ( const BroadcastType & = Unspecified );

                /**
                 * @brief Obtains the broadcast type.
                 * Gets the broadcast type that was stored by this broadcast message.
                 * @return BroadcastType The type of broadcast.
                 * @see BroadcastType
                 */
                const BroadcastType getBroadcastType( ) const;
        };
    }
}
#endif	/* BROADCAST_HPP */

// kate: indent-mode cstyle; space-indent on; indent-width 4;
