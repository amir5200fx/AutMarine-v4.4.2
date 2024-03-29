#include <fileName.hxx>

#include <wordList.hxx>
#include <DynamicList.hxx>
#include <OSspecific.hxx>

const char* const AutLib::fileName::typeName = "fileName";

AutLib::debug::debugSwitch
AutLib::fileName::debug
(
	fileName::typeName,
	0
);

const AutLib::fileName AutLib::fileName::null;

AutLib::fileName::fileName(const wordList& lst)
{
	for (label elemI = 0; elemI < (lst).size(); elemI++)
	{
		operator=((*this) / lst[elemI]);
	}
}

AutLib::fileName::Type AutLib::fileName::type() const
{
	return ::AutLib::type(*this);
}


//
// * remove repeated slashes
//       /abc////def        -->   /abc/def
//
// * remove '/./'
//       /abc/def/./ghi/.   -->   /abc/def/./ghi
//       abc/def/./         -->   abc/def
//
// * remove '/../'
//       /abc/def/../ghi/jkl/nmo/..   -->   /abc/ghi/jkl
//       abc/../def/ghi/../jkl        -->   abc/../def/jkl
//
// * remove trailing '/'
//
bool AutLib::fileName::clean()
{
	// the top slash - we are never allowed to go above it
	register string::size_type top = this->find('/');

	// no slashes - nothing to do
	if (top == string::npos)
	{
		return false;
	}

	// start with the '/' found:
	register char prev = '/';
	register string::size_type nChar = top + 1;
	register string::size_type maxLen = this->size();

	for
		(
			register string::size_type src = nChar;
			src < maxLen;
			/*nil*/
			)
	{
		register char c = operator[](src++);

		if (prev == '/')
		{
			// repeated '/' - skip it
			if (c == '/')
			{
				continue;
			}

			// could be '/./' or '/../'
			if (c == '.')
			{
				// found trailing '/.' - skip it
				if (src >= maxLen)
				{
					continue;
				}


				// peek at the next character
				register char c1 = operator[](src);

				// found '/./' - skip it
				if (c1 == '/')
				{
					src++;
					continue;
				}

				// it is '/..' or '/../'
				if (c1 == '.' && (src + 1 >= maxLen || operator[](src + 1) == '/'))
				{
					string::size_type parent;

					// backtrack to find the parent directory
					// minimum of 3 characters:  '/x/../'
					// strip it, provided it is above the top point
					if
						(
							nChar > 2
							&& (parent = this->rfind('/', nChar - 2)) != string::npos
							&& parent >= top
							)
					{
						nChar = parent + 1;   // retain '/' from the parent
						src += 2;
						continue;
					}

					// bad resolution, eg 'abc/../../'
					// retain the sequence, but move the top to avoid it being
					// considered a valid parent later
					top = nChar + 2;
				}
			}
		}
		operator[](nChar++) = prev = c;
	}

	// remove trailing slash
	if (nChar > 1 && operator[](nChar - 1) == '/')
	{
		nChar--;
	}

	this->resize(nChar);

	return (nChar != maxLen);
}


AutLib::fileName AutLib::fileName::clean() const
{
	fileName fName(*this);
	fName.clean();
	return fName;
}



//  Return file name (part beyond last /)
//
//  behaviour compared to /usr/bin/basename:
//    input           name()          basename
//    -----           ------          --------
//    "foo"           "foo"           "foo"
//    "/foo"          "foo"           "foo"
//    "foo/bar"       "bar"           "bar"
//    "/foo/bar"      "bar"           "bar"
//    "/foo/bar/"     ""              "bar"
//
AutLib::word AutLib::fileName::name() const
{
	size_type i = rfind('/');

	if (i == npos)
	{
		return *this;
	}
	else
	{
		return substr(i + 1, npos);
	}
}


//  Return directory path name (part before last /)
//
//  behaviour compared to /usr/bin/dirname:
//    input           path()          dirname
//    -----           ------          -------
//    "foo"           "."             "."
//    "/foo"          "/"             "foo"
//    "foo/bar"       "foo"           "foo"
//    "/foo/bar"      "/foo"          "/foo"
//    "/foo/bar/"     "/foo/bar/"     "/foo"
//
AutLib::fileName AutLib::fileName::path() const
{
	size_type i = rfind('/');

	if (i == npos)
	{
		return ".";
	}
	else if (i)
	{
		return substr(0, i);
	}
	else
	{
		return "/";
	}
}


//  Return file name without extension (part before last .)
AutLib::fileName AutLib::fileName::lessExt() const
{
	size_type i = find_last_of("./");

	if (i == npos || i == 0 || operator[](i) == '/')
	{
		return *this;
	}
	else
	{
		return substr(0, i);
	}
}


//  Return file name extension (part after last .)
AutLib::word AutLib::fileName::ext() const
{
	size_type i = find_last_of("./");

	if (i == npos || i == 0 || operator[](i) == '/')
	{
		return word::null;
	}
	else
	{
		return substr(i + 1, npos);
	}
}


// Return the components of the file name as a wordList
// note that concatenating the components will not necessarily retrieve
// the original input fileName
//
//  behaviour
//    input           components()
//    -----           ------
//    "foo"           1("foo")
//    "/foo"          1("foo")
//    "foo/bar"       2("foo", "bar")
//    "/foo/bar"      2("foo", "bar")
//    "/foo/bar/"     2("foo", "bar")
//
AutLib::wordList AutLib::fileName::components(const char delimiter) const
{
	DynamicList<word> wrdList(20);

	size_type start = 0, end = 0;

	while ((end = find(delimiter, start)) != npos)
	{
		// avoid empty element (caused by doubled slashes)
		if (start < end)
		{
			wrdList.append(substr(start, end - start));
		}
		start = end + 1;
	}

	// avoid empty trailing element
	if (start < size())
	{
		wrdList.append(substr(start, npos));
	}

	// transfer to wordList
	return wordList(wrdList.xfer());
}


// Return a component of the file name
AutLib::word AutLib::fileName::component
(
	const size_type cmpt,
	const char delimiter
) const
{
	return components(delimiter)[(label)cmpt];
}

const AutLib::fileName& AutLib::fileName::operator=(const fileName& str)
{
	string::operator=(str);
	return *this;
}


const AutLib::fileName& AutLib::fileName::operator=(const word& str)
{
	string::operator=(str);
	return *this;
}


const AutLib::fileName& AutLib::fileName::operator=(const string& str)
{
	string::operator=(str);
	stripInvalid();
	return *this;
}


const AutLib::fileName& AutLib::fileName::operator=(const std::string& str)
{
	string::operator=(str);
	stripInvalid();
	return *this;
}


const AutLib::fileName& AutLib::fileName::operator=(const char* str)
{
	string::operator=(str);
	stripInvalid();
	return *this;
}

AutLib::fileName AutLib::operator/(const string& a, const string& b)
{
	if (a.size())           // First string non-null
	{
		if (b.size())       // Second string non-null
		{
			return fileName(a + '/' + b);
		}
		else                // Second string null
		{
			return a;
		}
	}
	else                    // First string null
	{
		if (b.size())       // Second string non-null
		{
			return b;
		}
		else                // Second string null
		{
			return fileName();
		}
	}
}