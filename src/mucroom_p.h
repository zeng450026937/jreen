/****************************************************************************
 *
 *  This file is part of qutIM
 *
 *  Copyright (c) 2010 by Nigmatullin Ruslan <euroelessar@gmail.com>
 *
 ***************************************************************************
 *                                                                         *
 *   This file is part of free software; you can redistribute it and/or    *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************
 ****************************************************************************/

#ifndef MUCROOM_P_H
#define MUCROOM_P_H

#include "mucroom.h"
#include "mucmessagesession_p.h"

namespace jreen
{
	class MUCRoomPrivate
	{
	public:
		Q_DECLARE_PUBLIC(MUCRoom)
		
		MUCRoomPrivate(MUCRoom *room) :
				q_ptr(room), client(0), affiliation(MUCRoom::AffiliationOutcast),
				role(MUCRoom::RoleNone), maxChars(-1), maxStanzas(-1), seconds(-1)
		{
		}
		
		static MUCRoomPrivate *get(MUCRoom *room) { return room->d_func(); }
		void handlePresence(const Presence &pres);
		void handleMessage(const Message &msg);
		
		MUCRoom *q_ptr;
		Client *client;
		JID jid;
		MUCRoom::Affiliation affiliation;
		MUCRoom::Role role;
		QString password;
		int maxChars;
		int maxStanzas;
		int seconds;
		QDateTime since;
		MUCMessageSession *session;
		QString subject;
	};
	
	class MUCRoomQuery : public StanzaExtension
	{
		J_EXTENSION(jreen::MUCRoomQuery, "")
	public:
		MUCRoomQuery(const QString &password)
			: m_password(password), m_maxChars(-1), m_maxStanzas(-1), m_seconds(-1) {}
		void setMaxChars(int maxChars) { m_maxChars = qMax(-1, maxChars); }
		void setMaxStanzas(int maxStanzas) { m_maxStanzas = qMax(-1, maxStanzas); }
		void setSeconds(int seconds) { m_seconds = qMax(-1, seconds); }
		void setSince(const QDateTime &since) { m_since = since; }
	private:
		QString m_password;
		int m_maxChars;
		int m_maxStanzas;
		int m_seconds;
		QDateTime m_since;
		friend class MUCRoomQueryFactory;
	};
	
	class MUCRoomItem
	{
	public:
		MUCRoomItem() : affiliation(MUCRoom::AffiliationNone), role(MUCRoom::RoleNone) {}
		
		MUCRoom::Affiliation affiliation;
		MUCRoom::Role role;
		JID jid;
		QString reason;
		JID actor;
		QString nick;
	};
	
	class MUCRoomUserQuery : public StanzaExtension
	{
		J_EXTENSION(jreen::MUCRoomUserQuery, "")
	public:
		MUCRoomUserQuery() : flags(0)
		{
		}
		enum Flag
		{
			NonAnonymous         = 0x0001,
			SemiAnonymous        = 0x0002,
			FullyAnonymous       = 0x0004,
			AffiliationChangeWNR = 0x0008,
			Self                 = 0x0010,
			LoggingEnabled       = 0x0020,
			LoggingDisabled      = 0x0040,
			NewRoom              = 0x0080,
			NickAssigned         = 0x0100,
			Banned               = 0x0200,
			NickChanged          = 0x0400,
			Kicked               = 0x0800,
			AffiliationChanged   = 0x1000,
			MembershipRequired   = 0x2000,
			RoomSegfaulted       = 0x4000
		};
		MUCRoomItem item;
		JID alternate;
		int flags;
		QString status;
	};
	
	class MUCRoomAdminQuery : public StanzaExtension
	{
		J_EXTENSION(jreen::MUCRoomAdminQuery, "")
	public:
		MUCRoomAdminQuery() {}
		MUCRoomAdminQuery(const QString &nick, MUCRoom::Affiliation a, const QString &reason)
		{
			MUCRoomItem item;
			item.affiliation = a;
			item.nick = nick;
			item.reason = reason;
			items << item;
		}
		MUCRoomAdminQuery(const QString &nick, MUCRoom::Role r, const QString &reason)
		{
			MUCRoomItem item;
			item.role = r;
			item.nick = nick;
			item.reason = reason;
			items << item;
		}
		QList<MUCRoomItem> items;
	};
	
	class MUCRoomOwnerQuery : public StanzaExtension
	{
		J_EXTENSION(jreen::MUCRoomOwnerQuery, "")
	public:
		MUCRoomOwnerQuery() {}
		MUCRoomOwnerQuery(const DataForm::Ptr &f) : form(f) {}
		DataForm::Ptr form;
	};
}

#endif // MUCROOM_P_H
