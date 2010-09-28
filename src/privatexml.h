/****************************************************************************
 *  privatexml.h
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

#ifndef PRIVATEXML_H
#define PRIVATEXML_H

#include <QObject>
#include "jreen.h"

J_BEGIN_NAMESPACE

class Client;
class IQ;
struct PrivateXmlPrivate;

// XEP-0049

class PrivateXml : public QObject
{
	Q_OBJECT
	J_DECLARE_PRIVATE(PrivateXml)
public:
	enum Result
	{
		StoreOk,
		StoreError,
		RequestError,
		RequestOk
	};
	PrivateXml( Client *client );
	// SLOT(newResult(QDomElement,PrivateXml::Result,QSharedPointer<Error>))
	void request( const QString &name, const QString &xmlns, QObject *handler, const char *member );
	void store( const QDomElement &node, QObject *handler, const char *member );
private slots:
	void handleIQ( const IQ &iq, int context );
private:
	class Query;
	PrivateXmlPrivate *j_ptr;
};

J_END_NAMESPACE

#endif // PRIVATEXML_H
