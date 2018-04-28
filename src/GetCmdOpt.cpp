/* $Header: /cvsnt/repository/source/Common/GetCmdOpt/GetCmdOpt.cpp,v 1.4 2009/01/03 22:18:40 RETI\mrubin Exp $ */

#include <assert.h>
#include <vector>
#include <string>

#include "GetCmdOpt.h"

#pragma message ("Need to differentiate between a boolean flag and a similarly-named integer flag")

GetCmdOpt::GetCmdOpt (int argc, char *argv [], std::string keyStart)
{
	int i;

	m_argc = argc - 1;
	m_argv = new char * [argc - 1];
	m_keyStart = keyStart;

	// Make a local copy of all arguments.  Ignore the 0-th argument, as
	// that is the name of the executable
	for (i=1; i<argc; i++)
	{
		m_argv [i-1] = new char [strlen (argv [i]) + 1];
		strcpy (m_argv [i-1], argv [i]);
	}
}

GetCmdOpt::~GetCmdOpt ()
{
	int i;

	for (i=0; i<m_argc; i++)
		delete m_argv [i];

	delete[] m_argv;
}

bool GetCmdOpt::GetInt (char *p, int& i)
{
	int iIndex;
	const char *pToken;
	bool Ret = false;

	assert (p);
	if (p == 0)
		return false;

	if (!FindKey (p, &iIndex))
		goto exit;

	// So we found the option key.  Return next field as value, if valid.
	if (ReturnValueToken (iIndex + 1, &pToken))
	{
		i = atoi (pToken);
		Ret = true;
	}

exit:
	return Ret;
}

bool GetCmdOpt::GetIntVector (char *p, std::vector<int> &vec)
{
	// Return all integer instances of a specific key.

	const char *pToken;
	int i;

	assert (p);
	if (p == 0)
		return false;

	// TODO: The following code is a duplicate of FindKey(), simplify the code!
	std::string strKey;

	vec.clear();

	// Prepare the key string we're looking for
	strKey += m_keyStart;
	strKey += p;

	for (i=0; i<m_argc; i++)
	{
		if (strcmp (strKey.c_str (), m_argv [i]) == 0)
		{
			// We've found a matching key, copy all values to <vec>
			++i;
			while (ReturnValueToken(i, &pToken))
			{
				vec.push_back(atoi(pToken));
				++i;
			}

			// Decrement the loop counter to point to the last value token processed
			--i;
		}
	}

	return (vec.size () > 0);
}

bool GetCmdOpt::GetNumber (char *p, double& d)
{
	int iIndex;
	const char *pToken;
	bool Ret = false;

	if (!FindKey (p, &iIndex))
		goto exit;

	// So we found the option key.  Return next field as value, if valid.
	if (ReturnValueToken (iIndex + 1, &pToken))
	{
		if (sscanf (pToken, "%lf", &d) != EOF)
			Ret = true;
	}

exit:
	return Ret;
}

bool GetCmdOpt::GetNumberVector (char *p, std::vector<double> &vec)
{
	// Return all integer instances of a specific key.

	const char *pToken;
	int i;

	assert (p);
	if (p == 0)
		return false;

	// TODO: The following code is a duplicate of FindKey(), simplify the code!
	std::string strKey;

	// Prepare the key string we're looking for
	strKey += m_keyStart;
	strKey += p;

	for (i=0; i<m_argc; i++)
	{
		if (strcmp (strKey.c_str (), m_argv [i]) == 0)
		{
			// We've found a matching key, copy all values to <vec>
			++i;
			while (ReturnValueToken (i, &pToken))
			{
				double d;

				if (sscanf (pToken, "%lf", &d) != EOF)
					vec.push_back (d);
				++i;
			}

			// Decrement the loop counter to point to the last value token processed
			--i;
		}
	}

	return (vec.size () > 0);
}

bool GetCmdOpt::GetString (char *p, std::string &s)
{
	// Return a string following a key.  Double-quote delimited strings are
	// automatically handled because DOS parses them correctly.

	const char *pToken;
	int iIndex;

	assert (p);
	if (p == 0)
		return false;

	if (!FindKey (p, &iIndex))
		return false;

	// So we found the option key.  Return next field as value, if valid.
	if (ReturnValueToken (iIndex + 1, &pToken))
	{
		s.assign (pToken);
		return true;
	}

	return false;
}

bool GetCmdOpt::GetStringVector (char *p, std::vector<std::string>& vec)
{
	// Return all string instances of a specific key.  Double-quote delimited strings are
	// automatically handled because DOS parses them correctly.

	const char *pToken;
	int i;

	assert (p);
	if (p == 0)
		return false;

	std::string strKey;

	// Prepare the key string we're looking for
	strKey += m_keyStart;
	strKey += p;

	for (i=0; i<m_argc; i++)
	{
		if (strcmp (strKey.c_str (), m_argv [i]) == 0)
		{
			// We've found a matching key, copy all values to <vec>
			++i;
			while (ReturnValueToken(i, &pToken))
			{
				std::string s;

				s = pToken;
				vec.push_back(s);
				++i;
			}

			// Decrement the loop counter to point to the last value token processed
			--i;
		}
	}

	return (vec.size () > 0);
}

bool GetCmdOpt::GetBool (char *p)
{
	bool Ret = false;

	assert (p);
	if (!p)
		goto exit;

	if (FindKey (p, NULL))
		Ret = true;

exit:
	return Ret;
}

bool GetCmdOpt::FindKey (char *p, int *piIndex)
{
	int i;
	std::string strKey;
	bool Ret = false;

	assert (p);
	if (!p)
		goto exit;

	// Prepare the key string we're looking for
	strKey += m_keyStart;
	strKey += p;

	for (i=0; i<m_argc; i++)
	{
		if (strcmp (strKey.c_str (), m_argv [i]) == 0)
		{
			// We've found a matching key, return the index
			if (piIndex)
				*piIndex = i;
			Ret = true;
			goto exit;
		}
	}

exit:
	return Ret;
}

bool GetCmdOpt::TokenStartWithKeyPrefix(char *token)
{
	if (
			(token != 0)
			&&
			(strlen(token) >= 3)
			&&
			(strncmp(token, m_keyStart.c_str(), m_keyStart.size()) == 0)
		)
		return true;
	else
		return false;
}

// ReturnValueToken: If token indexed by <iIndex> is a value and not a key,
//                   return the value and true.
bool GetCmdOpt::ReturnValueToken (int iIndex, const char **pToken)
{
	bool Ret = false;

	assert (pToken);
	if (!pToken)
		goto exit;

	// If <iIndex> is a valid token index, return a pointer to the token.
	if
		(
			(iIndex < m_argc)
			&&
			!TokenStartWithKeyPrefix(m_argv[iIndex])
		)
	{
		*pToken = m_argv [iIndex];
		Ret = true;
	}

exit:
	return Ret;
}
