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

 * @file message.hpp
 * @author Jacky Alcine
 * @created April 3, 2011, 10:09 AM
 */

#ifndef MESSAGE_HPP
#define	MESSAGE_HPP

#include <map>
#include <string>
#include <algorithm>
#include <QVariant>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;
using namespace boost::posix_time;
using namespace boost::serialization;

using std::string;
using std::map;
using boost::serialization::access;
using boost::posix_time::ptime;

namespace Wintermute {
    namespace Network {
        struct Message;

        /**
         * @brief Foundational message class.
         * This class represents the underlying mechanics of network interactivites for Wintermute.
         */
        class Message {
            private:
                friend class boost::serialization::access;
                /**
                 * @brief typedef for ValueMap.
                 * Holds the mapping attributes of Wintermute.
                 */
                typedef map<const string* ,QVariant*> ValueMap;
                ValueMap data;
                static int _c;
                template<class Archive>
                /**
                 * @brief Serializes the message.
                 * Serializes the message into a compatiable format provided by the Archive.
                 * @param Archive|ar The archiving object.
                 * @param int|version The verison of the archiving process.
                 * @see <boost/serialization.hpp>
                 */
                void serialize(Archive& ar, unsigned int const version);


                /**
                 * @brief Initializes the message.
                 * Loads and corrects the few prerequisties of the Message.
                 */
                void __init();

            public:

                /**
                 * @brief Default constructor.
                 * Creates a new empty Message.
                 */
                Message();

                /**
                 * @brief Deserializing method.
                 * Creates a Message by deserializing the text passed from Boost.
                 * @fn Message
                 * @param  data The data representing this message.
                 */
                Message(const string& );

                /**
                 * @brief Constructor, single attribute.
                 * Creates a new message with one attribute.
                 * @param  attrName The name of the attribute.
                 * @param QVariant|attrValue The value of the attribute.
                 */
                Message(const string&, QVariant*);

                /**
                 * @brief Constructor, mutliple attributes.
                 * Creates a new message with multiple attributes.
                 * @param ValueMap|attrs The ValueMap to copy.
                 */
                Message(const ValueMap& );

                /**
                 * @brief Checks property existence.
                 * Determines if a property exists, and return a Boolean whether or not it does.
                 * @param  propertyName The property name to look for.
                 * @return TRUE if it exists, FALSE otherwise.
                 */
                const bool hasProperty(const string& ) const;

                /**
                 * @brief Obtains property.
                 * Gets the value of the property as a variant.
                 * @param  propertyName The property name to look for.
                 * @return A pointer to a QVariant if it exists; NULL otherwise.
                 */
                QVariant* getProperty(const string&);

                /**
                 * @brief Obtains property as a constant.
                 * Gets the value of the property as a variant, with read-only privileges.
                 * @param  propertyName The property name to look for.
                 * @return A pointer to a QVariant if it exists; NULL otherwise.
                 */
                const QVariant* getProperty(const string&) const;

                /**
                 * @brief Changes the value of a property.
                 * Sets the value of an existing property; creates it if it doesn't exists.
                 * @param  propertyName The name of the property to set (or create).
                 * @param QVariant|propertyValue The value of the property to set (or create).
                 */
                void setProperty(const string&, QVariant*);

                /**
                 * @brief Gets message type.
                 * Obtains the type of messsage that this message exposes.
                 * @note This method just calls getProperty('typ'); is provided as a convinence method.
                 * @return The type of this message, as a string.
                 */
                const string getMessageType() const;

                /**
                 * @brief
                 *
                 * @fn toString
                 */
                const string toString() const;

                /**
                 * @brief Gets time of spawning.
                 * Obtains the time that this message was created.
                 * @note This method merely serves as a convinence method; you can get the value by calling getProperty('_ts').
                 * @return The time of creation.
                 */
                const ptime getCreationTime() const;
                /**
                 * @brief Destructor.
                 * Destroys this message.
                 */
                ~Message();
        };

#ifdef __cplusplus
        extern "C" {

            /**
             * @brief Creates a Message.
             * Creates a new empty Message. Can be used to represent a NULL Message.
             * @return A Message.
             */
            Message* Message_create ();

            /**
             * @brief Deserializes a Message.
             * Generates a new message from serialized text.
             * @fn Message_createFromString
             * @param  The C-style string representing the serialized text.
             */
            Message* Message_createFromString ( const char* );

            /**
             * @brief Creates a single-property message.
             * Creates a new message with only one property.
             * @param  The C-style string representing the property name.
             * @param QVariant*|A pointer to the value to save.
             */
            Message* Message_createOneProperty ( const char*, QVariant* );

            /**
             * @brief Determines existence of property.
             * Finds out if a specified property is part of this Message's property list.
             * @param Message&|A constant reference to the Message to search.
             * @param  A C-style string representing the property name.
             */
            const bool Message_hasProperty( Message const&, const char* );

            /**
             * @brief Obtains property value.
             * Returns the value stored away in the Message by the specified property name.
             * @param Message&|
             * @param  
             */
            const QVariant* Message_getProperty( Message const &, const char* );

            /**
             * @brief
             *
             * @param
             * @param
             * @param
             */
            void Message_setProperty( Message&, const char*, QVariant* );

            /**
             * @brief
             *
             * @param
             */
            const char* Message_getMessageType(Message const & );

            /**
             * @brief
             *
             * @param
             */
            const char* Message_toString(Message const &);

            /**
             * @brief
             *
             * @param
             */
            const int Message_getCreationTime (Message const &);

        }
#endif
    }
}

#endif	/* MESSAGE_HPP */

