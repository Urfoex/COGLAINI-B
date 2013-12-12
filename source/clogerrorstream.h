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


#ifndef CLOGERRORSTREAM_H
#define CLOGERRORSTREAM_H

#include <ostream>
#include <memory>

class CLogErrorStream {

public:
	CLogErrorStream();
	CLogErrorStream( const CLogErrorStream& other );
	virtual ~CLogErrorStream();
	virtual CLogErrorStream& operator=( const CLogErrorStream& other );
	virtual bool operator==( const CLogErrorStream& other ) const;

	/// @warning you need to close the stream yourself
	void setLogStream( std::ostream* log );
	/// @warning you need to close the stream yourself
	void setErrorStream( std::ostream* error );

	std::ostream& logStream();
	std::ostream& errorStream();

private:
	std::ostream* m_log;
	std::ostream* m_error;

};

#endif // CLOGERRORSTREAM_H
