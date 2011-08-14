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

 * @file broadcast.cpp
 * @author Jacky Alcine <jackyalcine@gmail.com>
 * @created April 9, 2011, 1:45 PM
 */

#include "broadcast.hpp"
using namespace std;

using std::cout;
using std::endl;

namespace Wintermute {
	namespace Network {
		udp::socket* Broadcast::socket_ = NULL;
		mutable_buffer* Broadcast::inBuffer_ = NULL;
		boost::thread* Broadcast::thread_ = NULL;
		io_service* Broadcast::io_service_ = NULL;
		deadline_timer* Broadcast::timer_ = NULL;
		udp::endpoint* Broadcast::endpoint_ = new udp::endpoint(udp::v4 (),2011);
		boost::posix_time::milliseconds* Broadcast::timeout_ = new boost::posix_time::milliseconds(500);

		void Broadcast::destroy( )
		{
			cout << "(ntwk) [Broadcast] Destroying... " << endl;
			stop();
			delete timer_;
			delete io_service_;
			delete socket_;
		}

		void Broadcast::initialize( )
		{
			cout << "(ntwk) [Broadcast] Initializing... " << endl;
			start();
		}

		void Broadcast::start(){
			io_service_ = new io_service;
			socket_ = new udp::socket(*Broadcast::io_service_);
			openSocket();
			cout << "(ntwk) [Broadcast] Starting broadcasting activity... " << endl;
			timer_ = new deadline_timer(*Broadcast::io_service_, *timeout_);
			timer_->async_wait(Broadcast::timerElasped);
			io_service_->run ();
		}

		void Broadcast::stop(){
			cout << "(ntwk) [Broadcast] Stopping broadcasting activity... " << endl;
			timer_->cancel ();
			io_service_->stop ();
			thread_->interrupt ();
		}

		void Broadcast::sendSignal(){
			/// @todo Send out one broadcast message and queue another to be sent.
			BroadcastMessage aMsg;
			const string theData(aMsg.toString ());
			cout << "(ntwk) [Broadcast] Attempting to send message '" << aMsg.toString () << "'" << endl;
			socket_->async_send(buffer(theData), Broadcast::handleWrite);
		}

		void Broadcast::readSignal(){
			/// @todo Read ONE broadcast message and queue to read another.
			delete inBuffer_;
			inBuffer_ = new mutable_buffer;
			boost::system::error_code e;
			cout << "(ntwk) [Broadcast] Attempting to read any messages.." << endl;

			if (socket_->available () > 0)
				socket_->async_receive(buffer(*inBuffer_), Broadcast::handleRead);
		}

		void Broadcast::openSocket(){
			socket_->open (endpoint_->protocol ());
			socket_->set_option (socket_base::broadcast(true));
			socket_->set_option (socket_base::keep_alive(true));
			socket_->set_option (socket_base::reuse_address(true));
			socket_->bind (*endpoint_);
		}

		void Broadcast::timerElasped (const boost::system::error_code &e){
			if (!e){
				socket_->async_connect(*endpoint_,Broadcast::handleConnect);
				if (thread_ == NULL){
					thread_ = new thread(boost::bind(&io_service::run,boost::ref(*Broadcast::io_service_)));
					thread_->join();
				}
			} else {
				cout << "(ntwk) [Broadcast] The timer encountered an error; " << e.message () << endl;
			}
		}

		void Broadcast::handleConnect(const boost::system::error_code& e){
			if (!e){
				cout << "(ntwk) [Broadcast] Connected to endpoint (" << socket_->local_endpoint () << " | " << socket_->remote_endpoint () << ")"<< endl;
				sendSignal ();
				readSignal ();
				cout << "(ntwk) [Broadcast] Information queued to be read and sent." << endl;
				timer_->expires_from_now (*timeout_);
				timer_->async_wait(Broadcast::timerElasped);
			} else {
				cout << "(ntwk) [Broadcast] Couldn't connect to endpoint (" << *endpoint_ << "); " << e.message () << endl;
				openSocket();
				socket_->async_connect(*endpoint_,Broadcast::handleConnect);
			}
		}

		void Broadcast::handleWrite (const boost::system::error_code &e, size_t bytes){
			if (!e) {
				cout << "(ntwk) [Broadcast] Wrote " << bytes << " bytes." << endl;
			} else {
				cout << "(ntwk) [Broadcast] Error writing to socket; " << e.message () << endl;
			}
		}

		void Broadcast::handleRead (const boost::system::error_code &e, size_t bytes){
			if (!e) {
				if (bytes != 0){
					cout << "(ntwk) [Broadcast] Read " << bytes << " bytes." << endl;
				}
				else {
					cout << "(ntwk) [Broadcast] No incoming data." << endl;
				}

				const char* inData = buffer_cast<const char*>(*inBuffer_);
				if (inData != NULL){
					cout << "(ntwk) [Broadcast] Recieved: " << inData << endl;
				}
			} else {
				cout << "(ntwk) [Broadcast] Error reading from socket; " << e.message () << endl;
			}
		}

		const bool Broadcast::isActive (){
			return (socket_ != NULL && socket_->is_open ());
		}

		BroadcastMessage::BroadcastMessage( const BroadcastType& brdtype ) :
			Message( )
		{
			this->setProperty( string("typ") ,new QVariant("brdcst") );
			this->setProperty( string("brdcst_typ") , new QVariant((int)brdtype) );
		}

		const BroadcastType BroadcastMessage::getBroadcastType( ) const
		{
			const int val = this->getProperty( "brdcst_typ" )->toInt ();
			return static_cast< const BroadcastType > ( val );
		}

		/** exposing to C **/

		void Broadcast_initialize (){ Broadcast::initialize ();	}

		void Broadcast_destroy (){ Broadcast::destroy (); }

		BroadcastMessage* BroadcastMessage_create (const BroadcastType typ){
			return new BroadcastMessage(typ);
		}

		void BroadcastMessage_free (BroadcastMessage const *msg){
			delete msg;
		}

		const BroadcastType BroadcastMessage_getBroadcastType (BroadcastMessage const &msg){
			return msg.getBroadcastType ();
		}
		/** end expose **/
	}
}
