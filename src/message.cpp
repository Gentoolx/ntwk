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
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <sstream>

using namespace std;
using namespace boost::posix_time;
using namespace Wintermute::Network;

using std::string;
using std::map;
using std::istringstream;
using boost::posix_time::ptime;
using Wintermute::Network::Message;

namespace Wintermute {
	namespace Network {
		int Message::_c = 0;

		Message::Message () {
			__init ( );
		}

		Message::Message(const string& serializingText){
			istringstream inData(serializingText);
			{
				/// @todo Define Boost de-serialization information here.
			}
		}

		Message::Message (const ValueMap &collection) {
			this->data = collection;
			__init();
		}

		Message::Message (const string& property, QVariant* value) {
			__init ( );
			data.insert ( ValueMap::value_type ( new string(property.c_str ()), value ) );
		}

		Message::~Message () {
			data.clear ( );
		}

		void Message::__init () {
			ptime now ( microsec_clock::universal_time ( ) );
			cout <<"(ntwk) [Message] Generated message #" << Message::_c << "; created on " << to_iso_string(now) << "." << endl;
			this->setProperty ( string("_ts"), new QVariant(to_iso_extended_string(now).c_str()));
			Message::_c++;
		}

		const ptime Message::getCreationTime () const {
			if (this->hasProperty ("_ts")){
				QString ts = this->getProperty ("_ts")->toString ();
				cout <<ts.toStdString () << endl;
				return from_iso_string ( ts.toStdString().c_str () );
			}
			else {
				cout <<"NULL message." << endl;
				ptime now;
				return now;
			}
		}

		QVariant* Message::getProperty (const string& property) {
			if ( !hasProperty ( property ) )
				return NULL;

			return this->data.find ( new string(property.c_str ()))->second;
		}

		const QVariant* Message::getProperty (const string& property) const{
			if ( !hasProperty ( property ) )
				return NULL;

			return this->data.find ( new string(property.c_str ()))->second;
		}

		template<class Archive>
		void Message::serialize(Archive& ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(data);
		}

		const bool Message::hasProperty (const string& propertyName) const {

			return !(this->data.find ( new string(propertyName.c_str ())) == this->data.end ( ));
		}

		void Message::setProperty (const string& propertyName, QVariant* propertyValue) {

			this->data.insert ( ValueMap::value_type ( new string(propertyName.c_str ()), propertyValue ) );
		}

		const string Message::getMessageType () const {
			const QVariant* a = this->getProperty ( "typ" );
			if ( a == NULL )
				return "unknwn";
			else
				return a->toString ( ).toStdString ();
		}

		const string Message::toString () const {
			ostringstream outData;

			{
				boost::archive::xml_oarchive outXml(outData);
				//outXml << boost::serialization::make_nvp<Message>("Message",*this);
			}

			return outData.str ();
		}

		Message* Message_create (){
			return new Message;
		}

		Message* Message_createFromString(const char *data){
			const string theData(data);
			return new Message(theData);
		}

		Message* Message_createProperty(const char *name, QVariant *property){
			return new Message((string(name)),property);
		}

		const bool Message_hasProperty (const Message &msg, const char *propertyName){
			return msg.hasProperty (propertyName);
		}

		const QVariant* Message_getProperty (const Message &msg, const char *propertyName){
			return msg.getProperty ((string(propertyName)));
		}

		const char* Message_getMessageType (const Message &msg){
			return (msg.getMessageType ()).c_str ();
		}

		const char* Message_toString (const Message &msg){
			return (msg.toString ()).c_str ();
		}
	}
}
