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

 * @file node.hpp
 * @author Jacky Alcine
 * @created April 3, 2011, 3:53 PM
 */

#ifndef NODE_HPP
#define	NODE_HPP

namespace Wintermute {
    namespace Network {
        namespace Angel {

            /**
             * @brief
             *
             * @class Node node.hpp <wntr/ntwk/angel/node.hpp>
             */
            class Node {
            public:
                /**
                 * @brief
                 *
                 * @class ID node.hpp <wntr/ntwk/angel/node.hpp>
                 */
                class ID {
                public:
                    /**
                     * @brief
                     *
                     * @fn ID
                     */
                    ID();
                    /**
                     * @brief
                     *
                     * @fn ID
                     * @param
                     */
                    ID(const ID&);
                    /**
                     * @brief
                     *
                     * @fn ~ID
                     */
                    ~ID();
                };
                /**
                 * @brief
                 *
                 * @fn Node
                 */
                Node();
                /**
                 * @brief
                 *
                 * @fn ~Node
                 */
                ~Node();
            };
        }
    }
}

#endif	/* NODE_HPP */

