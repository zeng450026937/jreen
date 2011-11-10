/****************************************************************************
 *  ping.h
 *
 *  Copyright (c) 2010 by Sidorov Aleksey <sauron@citadelspb.com>
 *
 ***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#ifndef PING_H
#define PING_H
#include "stanzaextension.h"
#include "jid.h"

namespace Jreen {

//XEP-0199
//http://xmpp.org/extensions/xep-0199.html
class PingPrivate;
class JREEN_EXPORT Ping : public Payload
{
	Q_DECLARE_PRIVATE(Ping)
	J_PAYLOAD(Jreen::Ping)
public:
	Ping();
	~Ping();
	
private:
	QScopedPointer<PingPrivate> d_ptr;
};

} // namespace Jreen

#endif // PING_H
