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
 * @date April 3, 2011, 10:09 AM
 */

#ifndef MESSAGE_HPP
#define	MESSAGE_HPP

#include <QVariant>
#include <QDateTime>
#include <QVariantMap>

using namespace std;

using std::map;
using std::string;

namespace Wintermute {
    namespace Network {
        struct Message;

        /**
         * @brief Foundational message class of WntrNtwk.
         *
         * This class represents the underlying mechanics of network
         * interactivites for Wintermute. It provides a property-based
         * system that can convert QVariant-compatiable objects into
         * strings and reproduce the message on the other end of the
         * transmission.
         *
         * This class can stand on its own, but derivations of this class
         * (BroadcastMessage and RequestMessage) provide a shorter syntax
         * and more of a ease of use to implement. Simple constructing goes
         * as follows:
         *
         * @code
         * Message l_msg;                   // We prefer that you use pointers.
         * l_msg.setAttribute("Foo",3);
         * l_msg.setAttribute("Bar","Hello world!");
         * qDebug() << l_msg.toString();    // { 'Foo' : 3, 'Bar' : 'Hello world!' };
         * @endcode
         *
         * @attention Encryption of said messages should be required, if not
         *            at least signing of messages. QCA provides MD5, SHA-1, and OpenGPG.
         *            We're good.
         * @todo Add operating overloading for searching through messages (operator[](const QString)).
         */
        class Message : public QObject {
            Q_OBJECT
            Q_PROPERTY(const QString Type READ type)
            Q_PROPERTY(const QDateTime CreationTime READ creationTime)

            private:
                static long s_count; /**< Holds the number of messages ever sent since the process started. */

                /**
                 * @brief Initializes the message.
                 *
                 * Loads and corrects the few prerequisties of the Message.
                 */
                void __init();

            public:

                /**
                 * @brief Empty constructor.
                 *
                 * Creates a new empty Message.
                 */
                Message();

                /**
                 * @brief Constructor, single attribute.
                 *
                 * Creates a new message with one attribute.
                 * @param p_attrName The name of the attribute.
                 * @param p_attrValue The value of the attribute.
                 */
                explicit Message ( const QString&, QVariant* );

                /**
                 * @brief Copy constructor.
                 *
                 * Creates a copy of a Message.
                 * @param p_msg The Message to be copied.
                 */
                Message ( const Message& );

                /**
                 * @brief Destructor.
                 *
                 * Destroys this message.
                 */
                ~Message();

                /**
                 * @brief Gets message type.
                 *
                 * Obtains the type of messsage that this message exposes.
                 * @note This method just calls getProperty('Type'); is provided as a convinence method.
                 * @return The type of this message, as a string.
                 */
                const QString type() const;

                /**
                 * @brief Converts this Message into a string.
                 *
                 * Using JSON (Java Script Object Notation), the value of the properties
                 * of this Message are converted into a serializable format.
                 * @fn toString
                 */
                const QString toString() const;

                /**
                 * @brief Gets time of spawning.
                 *
                 * Obtains the time that this message was created.
                 * @note This method merely serves as a convinence method; you can get the value by calling getProperty('TimeStamp').
                 * @return The time of creation.
                 */
                const QDateTime creationTime() const;

                /**
                 * @brief Converts a string into a Message.
                 *
                 * Creates a Message by deserializing the text that should represent JSON.
                 * @fn Message
                 * @param data The data representing this message.
                 * @return A shiny new Message.
                 */
                static Message* fromString ( const QString& );
        };
    }
}

Q_DECLARE_METATYPE(Wintermute::Network::Message)

#endif	/* MESSAGE_HPP */

// kate: indent-mode cstyle; space-indent on; indent-width 4;
