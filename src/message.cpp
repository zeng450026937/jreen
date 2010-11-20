/****************************************************************************
 *  message.cpp
 *
 *  Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>
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

#include "message.h"
#include "message_p.h"
#include "delayeddelivery.h"
#include "util.h"
#include <QStringList>

namespace jreen
{

Message::Message(Type type, const JID& to, const QString &body, const QString &subject, const QString &thread, const QString &xmllang)
	: Stanza(*new MessagePrivate)
{
	Q_D(Message);
	d->subtype = type;
	d->to = to;
	d->thread = thread;
	d->body[xmllang] = body;
	d->subject[xmllang] = subject;
}

Message::Message(const QDomElement &node) : Stanza(node, new MessagePrivate)
{
}

Message::Message(MessagePrivate &p) : Stanza(p)
{

}

Message::Type Message::subtype() const
{
	Q_D(const Message);
	return d->subtype;
}

const QString &Message::body(const QString &lang) const
{
	Q_D(const Message);
	return d->body.value(lang);
}

const QString &Message::subject(const QString &lang) const
{
	Q_D(const Message);
	return d->subject.value(lang);
}

const QString &Message::thread() const
{
	Q_D(const Message);
	return d->thread;
}

void Message::setThread(const QString &thread)
{
	Q_D(Message);
	d->thread = thread;
}

void Message::setID(const QString &id)
{
	Q_D(Message);
	d->id = id;
}

const DelayedDelivery *Message::when() const
{
	return findExtension<DelayedDelivery>().data();
}

void Message::writeXml(QXmlStreamWriter *writer) const
{
}
}
