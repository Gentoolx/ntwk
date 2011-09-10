/*
 *  Author: Wintermute Developers <wintermute-devel@lists.launchpad.net>
 *
 *  Copyright 2011 (c) Wintermute Developers
 *
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
 */

#ifndef WNTRNTWK_HPP
#define WNTRNTWK_HPP

#include "angel.hpp"
#include "message.hpp"
#include "broadcast.hpp"
#include "system.hpp"
#include "adaptors.hpp"

namespace Wintermute {
    namespace Network {
        struct Interface;

        class Interface : public QObject {
            Q_OBJECT
            Q_DISABLE_COPY(Interface)
            static Interface* s_ints;

            public:
                Interface();
                static const bool isActive();
                static Interface* instance();

            public slots:
                Q_NOREPLY void start() const;
                Q_NOREPLY void stop() const;

            signals:
                void started() const;
                void stopped() const;
        };
    }
}


#endif /* WNTRNTWK_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;
