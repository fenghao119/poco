//
// Binary.h
//
// $Id$
//
// Library: MongoDB
// Package: MongoDB
// Module:  Binary
//
// Definition of the Binary class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_Binary_INCLUDED
#define MongoDB_Binary_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Element.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Buffer.h"
#include "Poco/StreamCopier.h"
#include "Poco/MemoryStream.h"
#include "Poco/UUID.h"
#include <sstream>


namespace Poco {
namespace MongoDB {


class MongoDB_API Binary
	/// Implements BSON Binary. It's a wrapper around a Poco::Buffer<unsigned char>.
{
public:
	typedef SharedPtr<Binary> Ptr;

	Binary();
		/// Constructor

	Binary(Poco::Int32 size, unsigned char subtype);
		/// Constructor

	Binary(const UUID& uuid);
		/// Constructor for setting a UUID in a binary element

	Binary(const std::string& data, unsigned char subtype = 0);
		/// Constructor for getting binary data from a string.
		
	Binary(const void* data, Poco::Int32 size, unsigned char subtype = 0);
		/// Constructor for getting binary data from a buffer.		

	virtual ~Binary();
		/// Destructor

	Buffer<unsigned char>& buffer();
		/// Returns a reference to the buffer

	unsigned char subtype() const;
		/// Returns the subtype

	void subtype(unsigned char type);
		/// Sets the subtype

	std::string toString(int indent = 0) const;
		/// Returns the binary encoded in Base64

	std::string toRawString() const;
		/// Returns the raw content as a string.

	UUID uuid() const;
		/// Returns the UUID when the binary subtype is 0x04.
		/// Otherwise BadCastException will be thrown

private:
	Buffer<unsigned char> _buffer;
	unsigned char _subtype;
};


inline unsigned char Binary::subtype() const
{
	return _subtype;
}


inline void Binary::subtype(unsigned char type)
{
	_subtype = type;
}


inline Buffer<unsigned char>& Binary::buffer()
{
	return _buffer;
}


inline std::string Binary::toRawString() const
{
	return std::string(reinterpret_cast<const char*>(_buffer.begin()), _buffer.size());
}


// BSON Embedded Document
// spec: binary
template<>
struct ElementTraits<Binary::Ptr>
{
	enum { TypeId = 0x05 };

	static std::string toString(const Binary::Ptr& value, int indent = 0)
	{
		return value.isNull() ? "" : value->toString();
	}
};


template<>
inline void BSONReader::read<Binary::Ptr>(Binary::Ptr& to)
{
	Poco::Int32 size;
	_reader >> size;

	to->buffer().resize(size);

	unsigned char subtype;
	_reader >> subtype;
	to->subtype(subtype);

	_reader.readRaw((char*) to->buffer().begin(), size);
}


template<>
inline void BSONWriter::write<Binary::Ptr>(Binary::Ptr& from)
{
	_writer << (Poco::Int32) from->buffer().size();
	_writer << from->subtype();
	_writer.writeRaw((char*) from->buffer().begin(), from->buffer().size());
}


} } // namespace Poco::MongoDB


#endif // MongoDB_Binary_INCLUDED
