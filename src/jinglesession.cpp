/****************************************************************************
**
** qutIM instant messenger
**
** Copyright (C) 2011 Ruslan Nigmatullin <euroelessar@ya.ru>
**
*****************************************************************************
**
** $QUTIM_BEGIN_LICENSE$
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see http://www.gnu.org/licenses/.
** $QUTIM_END_LICENSE$
**
****************************************************************************/

#include "jinglesession_p.h"
#include "client.h"
#include "util.h"

namespace Jreen
{

JingleSessionContent *JingleSessionPrivate::findContent(const QString &name)
{
	for (int i = 0; i < contents.size(); ++i) {
		if (contents.at(i).name == name)
			return &contents[i];
	}
	return 0;
}

void JingleSessionPrivate::handle(const Jingle::Ptr &jingle)
{
	qDebug() << Q_FUNC_INFO;
	if (jingle->action == Jingle::SessionAccept) {
		foreach (const Jingle::Content &remoteContent, jingle->contents) {
			JingleSessionContent *content = findContent(remoteContent.name);
			if (!content) {
				qWarning("Unknown content with name %s", qPrintable(content->name));
				continue;
			}
			if (!remoteContent.description) {
				qWarning("Unknown content description with name %s", qPrintable(content->name));
				continue;
			}
			content->contentObject->handleDescription(remoteContent.description);
			if (remoteContent.transports.size() != 1) {
				qWarning("Content %s has %d transports", qPrintable(content->name),
				         remoteContent.transports.size());
			}
			if (!remoteContent.transports.value(0)) {
				qCritical("Content %s has no transports", qPrintable(content->name));
				continue;
			}
			const JingleTransportInfo::Ptr &transportInfo = remoteContent.transports.at(0);
			for (int i = content->transports.size() - 1; i >= 0; --i) {
				if (content->transports[i]->payloadType() != transportInfo->payloadType()) {
					content->transports.removeAt(i);
					delete content->transportObjects.takeAt(i);
				}
			}
			if (!remoteContent.transports.value(0)) {
				qCritical("Content %s has no needed transport", qPrintable(content->name));
				continue;
			}
			JingleContentPrivate *p = JingleContentPrivate::get(content->contentObject);
			p->setTransport(content->transportObjects[0]);
			p->transport->setRemoteInfo(transportInfo, true);
		}
	} else if (jingle->action == Jingle::SessionTerminate) {
		emit q_func()->terminated();
	}
}

void JingleSessionPrivate::_q_localInfoReady(const Jreen::JingleTransportInfo::Ptr &info)
{
	JingleTransport *transport = qobject_cast<JingleTransport*>(q_func()->sender());
	for (int i = 0; i < contents.size(); ++i) {
		JingleSessionContent &content = contents[i];
		int index = content.transportObjects.indexOf(transport);
		if (index != -1) {
			Q_ASSERT(!content.transports.at(index));
			--needMore;
			--content.needMore;
			content.transports[index] = info;
			QObject::disconnect(transport, SIGNAL(localInfoReady(Jreen::JingleTransportInfo::Ptr)),
			                    q_func(), SLOT(_q_localInfoReady(Jreen::JingleTransportInfo::Ptr)));
		}
	}
	if (needMore == 0)
		q_func()->initiate();
}

JingleSession::JingleSession(const JID &responder, const QStringList &contents, Client *client)
    : QObject(client->jingleManager()), d_ptr(new JingleSessionPrivate)
{
	Q_D(JingleSession);
	d->q_ptr = this;
	d->incoming = 0;
	d->client = client;
	d->other = responder;
	d->sid = Util::randomStringHash(16);
	JingleManagerPrivate *manager = JingleManagerPrivate::get(client->jingleManager());
	manager->sessions.insert(d->sid, this);
	manager->sessionsByJid.insert(responder, this);
	for (int i = 0; i < contents.size(); ++i) {
		if (addContent(contents.at(i))) {
//			const JingleSessionContent &content = d->contents.last();
		}
	}
	if (d->needMore == 0 && d->contents.size() > 0)
		initiate();
}

JingleSession::JingleSession(const Payload::Ptr &payload, Client *client)
    : QObject(client->jingleManager()), d_ptr(new JingleSessionPrivate)
{
	Q_D(JingleSession);
	Q_ASSERT(se_cast<Jingle*>(payload.data()));
	d->client = client;
	Jingle::Ptr jingle = payload.staticCast<Jingle>();
	d->other = jingle->initiator;
	d->sid = jingle->sid;
	JingleManagerPrivate *manager = JingleManagerPrivate::get(client->jingleManager());
	manager->sessions.insert(d->sid, this);
	manager->sessionsByJid.insert(jingle->initiator, this);
}

void JingleSession::initiate()
{
	Q_D(JingleSession);
	Jingle::Ptr jingle = Jingle::Ptr::create();
	jingle->initiator = d->client->jid();
	jingle->action = Jingle::SessionInitiate;
	jingle->sid = d->sid;
	for (int i = 0; i < d->contents.size(); ++i)
		jingle->contents << d->contents.at(i);
	IQ iq(IQ::Set, d->other);
	iq.addExtension(jingle);
	IQReply *reply = d->client->send(iq);
	Q_UNUSED(reply);
}

void JingleSession::terminate()
{
	Q_D(JingleSession);
	Jingle::Ptr jingle = Jingle::Ptr::create();
	jingle->sid = d->sid;
	jingle->action = Jingle::SessionTerminate;
	IQ iq(IQ::Set, d->other);
	iq.addExtension(jingle);
	IQReply *reply = d->client->send(iq);
	connect(reply, SIGNAL(received(Jreen::IQ)), SIGNAL(terminated()));
}

bool JingleSession::isIncoming() const
{
	return d_func()->incoming;
}

JID JingleSession::jid() const
{
	return d_func()->other;
}

JingleContent *JingleSession::content(const QString &id) const
{
	Q_D(const JingleSession);
	for (int i = 0; i < d->contents.size(); ++i) {
		if (d->contents.at(i).name == id)
			return d->contents.at(i).contentObject;
	}
	return 0;
}

QStringList JingleSession::contents() const
{
	Q_D(const JingleSession);
	QStringList result;
	for (int i = 0; i < d->contents.size(); ++i)
		result << d->contents.at(i).name;
	return result;
}

JingleSession::~JingleSession()
{
	Q_D(JingleSession);
	JingleManagerPrivate *manager = JingleManagerPrivate::get(d->client->jingleManager());
	manager->sessions.remove(d->sid);
	manager->sessionsByJid.remove(d->other);
}

bool JingleSession::addContent(const QString &media, const QString &id)
{
	Q_D(JingleSession);
	JingleManagerPrivate *manager = JingleManagerPrivate::get(d->client->jingleManager());
	JingleSessionContent content;
	content.creator = Jingle::Initiator;
	content.contentObject = manager->content(media, this);
	if (!content.contentObject) {
		qWarning("Unknown content %s", qPrintable(media));
		return false;
	}
	content.description = content.contentObject->defaultDescription();
	content.name = id.isEmpty() ? Util::randomStringHash(8) : id;
	foreach (AbstractJingleTransportFactory *factory, manager->transports) {
		JingleTransport *transport = factory->createObject(content.contentObject);
		if (transport->localInfo().isNull()) {
			connect(transport, SIGNAL(localInfoReady(Jreen::JingleTransportInfo::Ptr)),
					SLOT(_q_localInfoReady(Jreen::JingleTransportInfo::Ptr)));
			content.transports << JingleTransportInfo::Ptr();
			content.needMore++;
		} else {
			content.transports << transport->localInfo();
		}
		content.transportObjects << transport;
	}
	d->contents << content;
	d->needMore += content.needMore;
	emit contentAdded(content.contentObject);
	return true;
}

void JingleSession::accept()
{
}

void JingleSession::decline()
{
}

}

#include "jinglesession.moc"
