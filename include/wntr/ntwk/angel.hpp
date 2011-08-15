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
 *
 * @file angel.hpp
 * @author Jacky Alcine
 * @created April 3, 2011, 3:52 PM
 *
 * Definition of some easier-to-type names for the system.
 */

#ifndef ANGEL_HPP
#define	ANGEL_HPP

#include "angel/node.hpp"

namespace Wintermute {
    namespace Network {
        /**
         * @brief typedef for nodes in the Angel network.
         * A shorthand notation referring to nodes in the AngelNet as AngelNodes.
         */
        typedef Wintermute::Network::Angel::Node AngelNode;
        /**
         * @brief typdef of ids of AngelNodes.
         */
        typedef AngelNode::ID AngelID;
    }
}

#endif	/* ANGEL_HPP */

// kate: indent-mode cstyle; space-indent on; indent-width 4; 
