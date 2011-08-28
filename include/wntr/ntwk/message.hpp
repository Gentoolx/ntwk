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

#include <map>
#include <string>
#include <algorithm>
#include <QVariant>

using namespace std;

using std::map;
using std::string;

namespace Wintermute {
    namespace Network {
        struct Message;

        /**
         * @brief Foundational message class.
         * This class represents the underlying mechanics of network interactivites for Wintermute.
         */
        class Message : public QObject {
            Q_OBJECT

            private:
                static long s_count; /**< Holds the number of messages ever sent since the process started. */

                /**
                 * @brief Initializes the message.
                 * Loads and corrects the few prerequisties of the Message.
                 */
                void __init();

            public:

                /**
                 * @brief Empty constructor.
                 * Creates a new empty Message.
                 */
                Message();

                /**
                 * @brief Constructor, single attribute.
                 * Creates a new message with one attribute.
                 * @param  attrName The name of the attribute.
                 * @param QVariant|attrValue The value of the attribute.
                 */
                explicit Message ( const QString&, QVariant* );

                /**
                 * @brief
                 *
                 * @fn Message
                 * @param
                 */
                Message ( const Message& );

                /**
                 * @brief Destructor.
                 * Destroys this message.
                 */
                ~Message();

                /**
                 * @brief Gets message type.
                 * Obtains the type of messsage that this message exposes.
                 * @note This method just calls getProperty('typ'); is provided as a convinence method.
                 * @return The type of this message, as a string.
                 */
                const QString getMessageType() const;

                /**
                 * @brief
                 *
                 * @fn toString
                 */
                const QString toString() const;

                /**
                 * @brief Gets time of spawning.
                 * Obtains the time that this message was created.
                 * @note This method merely serves as a convinence method; you can get the value by calling getProperty('_ts').
                 * @return The time of creation.
                 */
                const QDateTime getCreationTime() const;

                /**
                 * @brief Deserializing method.
                 * Creates a Message by deserializing the text passed from Boost.
                 * @fn Message
                 * @param  data The data representing this message.
                 */
                static Message* fromString ( const QString& );
        };
    }
}

Q_DECLARE_METATYPE(Wintermute::Network::Message)

#endif	/* MESSAGE_HPP */

// kate: indent-mode cstyle; space-indent on; indent-width 4;
