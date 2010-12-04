/****************************************************************************
 *  vcard.h
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

#ifndef VCARD_H
#define VCARD_H
#include "stanzaextension.h"
#include "nickname.h"
#include <QDateTime>

//implementation of XEP 0054
//http://xmpp.org/extensions/xep-0054.html
namespace jreen {

class VCardPrivate;

class JREEN_EXPORT VCard : public StanzaExtension
{
	J_EXTENSION(jreen::VCard,
		   "/iq/vcard[@xmlns='vcard-temp']")
	Q_DECLARE_PRIVATE(VCard)
public:
	/**
	* Addressing type indicators.
	* @note @c AddrTypeDom and @c AddrTypeIntl are mutually exclusive. If both are present,
	* @c AddrTypeDom takes precendence.
	* @note Also note that not all adress types are applicable everywhere. For example,
	* @c AddrTypeIsdn does not make sense for a postal address. Check XEP-0054
	* for details.
	*/
	enum AddressType
	{
		AddrTypeHome	=	0x1,		/**< Home address. */
		AddrTypeWork	=	0x2,		/**< Work address. */
		AddrTypePref	=	0x4,		/**< Preferred address. */
		AddrTypeX400	=	0x8,		/**< X.400 address. */
		AddrTypeInet	=	0x10,		/**< Internet address. */
		AddrTypeParcel	=	0x20,		/**< Parcel address. */
		AddrTypePostal	=	0x40,		/**< Postal address. */
		AddrTypeDom		=	0x80,		/**< Domestic(?) address. */
		AddrTypeIntl	=	0x100,		/**< International(?) address. */
		AddrTypeVoice	=	0x200,		/**< Voice number. */
		AddrTypeFax		=	0x400,		/**< Fax number. */
		AddrTypePager	=	0x800,		/**< Pager. */
		AddrTypeMsg		=	0x1000,		/**< MSG(?) */
		AddrTypeCell	=	0x2000,		/**< Cell phone number. */
		AddrTypeVideo	=	0x4000,		/**< Video chat(?). */
		AddrTypeBbs		=	0x8000,		/**< BBS. */
		AddrTypeModem	=	0x10000,	/**< Modem. */
		AddrTypeIsdn	=	0x20000,	/**< ISDN. */
		AddrTypePcs		=	0x40000		/**< PCS. */
	};
	/**
	* Classifies the VCard.
	*/
	enum Classification
	{
		ClassNone = 0,		/**< Not classified. */
		ClassPublic,		/**< Public. */
		ClassPrivate,		/**< Private. */
		ClassConfidential	/**< Confidential. */
	};

	class NamePrivate;
	class PhotoPrivate;
	class AddressPrivate;
	class TelephonePrivate;
	class EMailPrivate;

	class Name
	{
	public:
		Name();
		Name(const Name &o);
		Name(NamePrivate &p);
		~Name();
		Name &operator =(const Name &o);
		
		QString family() const;
		void setFamily(const QString &);
		QString given() const;
		void setGiven(const QString &);
		QString middle() const;
		void setMiddle(const QString &);
		QString prefix() const;
		void setPrefix(const QString &);
		QString suffix() const;
		void setSuffix(const QString &);
	private:
		QSharedDataPointer<NamePrivate> d_ptr;
		friend class NamePrivate;
	};

	class Photo
	{
	public:
		Photo();
		Photo(const Photo &o);
		Photo(PhotoPrivate &p);
		~Photo();
		Photo &operator =(const Photo &o);
		
		QString external() const;
		void setExternal(const QString &);
		void setData(const QByteArray &data, const QString &mimeType = QString());
		QByteArray data() const;
		QString mimeType() const;
	private:
		QSharedDataPointer<PhotoPrivate> d_ptr;
		friend class PhotoPrivate;
	};
	
	class Telephone
	{
	public:
		enum Type
		{
			Home      = 0x0001, 
			Work      = 0x0002,
			Voice     = 0x0004,
			Fax       = 0x0008,
			Pager     = 0x0010,
			Msg       = 0x0020,
			Cell      = 0x0040,
			Video     = 0x0080,
			BBS       = 0x0100,
			Modem     = 0x0200,
			ISDN      = 0x0400,
			PCS       = 0x0800,
			Preferred = 0x1000
		};
		typedef QFlags<Type> Types;
		
		Telephone();
		Telephone(const Telephone &o);
		Telephone(TelephonePrivate &p);
		~Telephone();
		Telephone &operator =(const Telephone &o);

		bool testType(Type t) const;
		void setType(Type t, bool value);
		QString number() const;
		void setNumber(const QString &);
	private:
		QSharedDataPointer<TelephonePrivate> d_ptr;
		friend class TelephonePrivate;
	};

	class EMail
	{
	public:
		enum Type
		{
			Home      = 0x01,
			Work      = 0x02,
			Internet  = 0x04,
			Preferred = 0x08,
			X400      = 0x10,
		};
		EMail();
		EMail(const EMail &o);
		EMail(EMailPrivate &p);
		~EMail();
		EMail &operator =(const EMail &o);
	private:
		QSharedDataPointer<EMailPrivate> d_ptr;
		friend class EMailPrivate;
	};

	class Address
	{
	public:
		enum Type
		{
			Home          = 0x01,
			Work          = 0x02,
			Postal        = 0x04,
			Parcel        = 0x08,
			Domestic      = 0x10,
			International = 0x20,
			Preferred     = 0x40
		};
		Address();
		Address(const Address &o);
		Address(AddressPrivate &p);
		~Address();
		Address &operator =(const Address &o);
	private:
		QSharedDataPointer<AddressPrivate> d_ptr;
		friend class AddressPrivate;
	};

	VCard(const QString &formattedName = QString(), Classification classification = ClassNone);
	VCard(VCardPrivate &p);
	~VCard();
	/**
	* Sets the formatted name.
	* @param name The formatted name.
	*/
	inline void setFormattedName(const QString &fn);
	/**
	* Returns the formatted name.
	* @return The formatted name.
	*/
	const QString &formattedName() const;
	void setName(const QString& family, const QString& given,
				 const QString& middle = QString(),
				 const QString& prefix = QString(),
				 const QString& suffix = QString());
	void setName(const Name &name);
	/**
   * Returns a full name.
   * @return A full name.
   */
	const Name& name() const;
	/**
	* Sets a nickname.
	* @param nickname The nickname.
	*/
	void setNickname(const QString& nickname);
	/**
	* Returns the nickname.
	* @return The nickname.
	*/
	QString nickname() const;
	/**
	* Sets the birthday.
	* @param bday The birthday
	*/
	void setBday(const QDateTime& bday);
	/**
	* Returns the birthday.
	* @return The birthday.
	*/
	QDateTime bday() const;
	/**
	* Sets a URI to a photo.
	* @param extval The URI to the photo.
	*/
	void setPhoto(const QString& extval);
	/**
	* Sets the photo directly.
	* @param type Format type parameter.
	* @param binval The binary photo data.
	* @note To remove the photo from the VCard call this function without any parameters.
	*/
	void setPhoto(const Photo &photo);
	/**
	* Returns photo information.
	* @return Info about the photo.
	*/
	Photo photo() const;
	/**
	* Sets a URI to a organization logo.
	* @param extval The URI to the logo.
	*/

private:
	QScopedPointer<VCardPrivate> d_ptr;
};

} // namespace jreen

Q_DECLARE_OPERATORS_FOR_FLAGS(jreen::VCard::Telephone::Types)

#endif // VCARD_H
