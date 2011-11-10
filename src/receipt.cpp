/****************************************************************************
 *  receipt.cpp
 *
 *  Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>
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

#include "receipt.h"
#include "jstrings.h"
#include <QStringList>

namespace Jreen
{

class ReceiptPrivate
{
public:
	Receipt::Type type;
	QString id;
};

Receipt::Receipt(Type type,const QString &id) : d_ptr(new ReceiptPrivate)
{
	Q_D(Receipt);
	d->type = type;
	d->id = id;
}

Receipt::~Receipt()
{
}

Receipt::Type Receipt::type() const
{
	return d_func()->type;
}

QString Receipt::id() const
{
	return d_func()->id;
}

}
