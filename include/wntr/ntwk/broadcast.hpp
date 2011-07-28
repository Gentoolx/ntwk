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

 * @file broadcast.hpp
 * @author Jacky Alcine <jackyalcine@gmail.com>
 * @created April 9, 2011, 1:44 PM
 */

#ifndef BROADCAST_HPP
#define BROADCAST_HPP

#include <boost/asio/buffer.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/system/error_code.hpp>
#include "message.hpp"

using namespace boost::asio;
using namespace boost::this_thread;
using namespace boost::asio::ip;
using namespace Wintermute::Network;

using boost::asio::ip::udp;
using boost::asio::io_service;
using boost::asio::deadline_timer;
using boost::thread;
using boost::system::error_code;
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
			Unspecified = 0, IamAlive, Disconnected, Connecting, Identify
		};

		/**
		 * @brief Management class for broadcasting.
		 * This class mananges every aspect of Wintermute's ability to send
		 * broadcast signals out onto the network.
		 */
		class Broadcast {
			private:
				static io_service* io_service_;
				static deadline_timer* timer_;
				static udp::socket* socket_;
				static boost::thread* thread_;
				static udp::endpoint* endpoint_;
				static mutable_buffer* inBuffer_;
				static boost::posix_time::milliseconds* timeout_;
				/**
				 * @brief
				 *
				 * @fn openSocket
				 */
				static void openSocket();
				/**
				  * @brief Handler for timeout.
				  * @param boost::system::error_code|e The error, if any, passed.
				  */
				static void timerElasped(const error_code& );
				/**
				  * @brief Handler for async_accept.
				  * @see boost::asio::ip::tcp::acceptor::async_accept
				  * @param boost::system::error_code|e The error, if any, passed.
				  */
				static void handleConnect(const error_code& );
				/**
				  * @brief Handler for async_write.
				  * @see boost::asio::async_write
				  * @param boost::system::error_code|e The error, if any, passed.
				  * @param size_t|bytes The number of bytes sent.
				  */
				static void handleWrite(const error_code&, size_t );
				/**
				  * @brief Handler for async_read.
				  * @see boost::asio::async_read
				  * @param boost::system::error_code|e The error, if any, passed.
				  * @param size_t|bytes The number of bytes sent.
				  */
				static void handleRead(const error_code&, size_t );
				/**
				  @brief Starts the system.
				  * Opens up the sockets and begins broadcasting.
				  */
				static void start();
				/**
				  * @brief Stops the system.
				  * Closes the opened sockets and halts broadcasting.
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
				static void readSignal();
				static void doBroadcast();
			public:

				/**
				 * @brief Initializes the broadcast system.
				 * Creates the timer, acceptor, socket and begins broadcasting messsages.
				 */
				static void initialize( );

				/**
				 * @brief Destroys the broadcast system.
				 * Stops then destroys the timer, acceptor and socket.
				 */
				static void destroy( );

				/**
				 * @brief Determines if it's accepting.
				 * Determines if the socket exists, and if it does, if it's open.
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
				BroadcastMessage( const BroadcastType & = Unspecified );

				/**
				 * @brief Obtains the broadcast type.
				 * Gets the broadcast type that was stored by this broadcast message.
				 * @return BroadcastType The type of broadcast.
				 * @see BroadcastType
				 */
				const BroadcastType getBroadcastType( ) const;
		};

#ifdef  __cplusplus
		extern "C" {

			/**
			 * @brief
			 *
			 */
			void Broadcast_initialize();

			/**
			 * @brief
			 *
			 */
			void Broadcast_destroy();

			/**
			 * @brief
			 *
			 * @param BroadcastType|typ
			 */
			BroadcastMessage* BroadcastMessage_create(const BroadcastType = Unspecified);

			/**
			 * @brief
			 *
			 * @param BroadcastMessage|
			 */
			void BroadcastMessage_free(BroadcastMessage const * );

			/**
			 * @brief
			 *
			 * @param BroadcastMessage|
			 */
			const BroadcastType BroadcastMessage_getBroadcastType(BroadcastMessage const &);
		}
#endif
	}
}
#endif	/* BROADCAST_HPP */

