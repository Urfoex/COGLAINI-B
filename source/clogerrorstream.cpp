/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Manuel Bellersen <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "clogerrorstream.h"

#include <iostream>
#include <stdexcept>

CLogErrorStream::CLogErrorStream():
	m_log( &std::clog ),
	m_error( &std::cerr ) {

}

CLogErrorStream::CLogErrorStream( const CLogErrorStream& other ):
	m_log( other.m_log ),
	m_error( other.m_error ) {
	if( this == &other ) {
		return;
	}
}

CLogErrorStream::~CLogErrorStream() {
}

CLogErrorStream& CLogErrorStream::operator=( const CLogErrorStream& other ) {
	throw( std::logic_error( "Don't assign!\n" ) );

	if( &other == this ) {
		return *this;
	}

	return *this;
}

bool CLogErrorStream::operator==( const CLogErrorStream& other ) const {
///TODO: return ...;
	throw( std::logic_error( "Don't compare!\n" ) );
	return this == &other;
}

void CLogErrorStream::setErrorStream( std::ostream* error ) {
	m_error = error;
}

void CLogErrorStream::setLogStream( std::ostream* log ) {
	m_log = log;
}

std::ostream& CLogErrorStream::errorStream() {
	return *m_error;
}

std::ostream& CLogErrorStream::logStream() {
	return *m_log;
}
