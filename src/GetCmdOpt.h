/* $Header: /cvsnt/repository/source/Common/GetCmdOpt/GetCmdOpt.h,v 1.2 2009/01/02 11:15:48 RETI\mrubin Exp $ */

#ifndef GETCMDOPT_H_1800E12E_979F_4841_B34C_87DECB9F6237
#define GETCMDOPT_H_1800E12E_979F_4841_B34C_87DECB9F6237

#include <string>
#include <vector>

class GetCmdOpt
{
public:
	GetCmdOpt (int argc, char *argv [], std::string keyPrefix = "--");
	~GetCmdOpt ();

	bool GetInt (const char *key, int& value, int defaultVal);
	bool GetIntVector (const char *key, std::vector<int> &vec);
	bool GetDouble (const char *key, double& value, double defaultVal);
	bool GetDoubleVector (const char *key, std::vector<double> &vec);
	bool GetString (const char *key, std::string &value, const char * defaultVal);
	bool GetStringVector (const char *key, std::vector<std::string> &vec);
	bool GetBool (const char *key, bool &value ,bool defaultVal);
	bool KeyExists(const char* key);

protected:
	int m_argc;
	char **m_argv;
	std::string m_keyPrefix;

	bool FindKey (const char *key, int *pIndex);
	bool ReturnValueToken (int iIndex, const char **pToken);
	bool TokenStartWithKeyPrefix(char *token);
	int ReturnMaximumIntValue();
	bool ConvertStringToInt(const char *s, int &i);

private:
	GetCmdOpt (const GetCmdOpt&);
	GetCmdOpt& operator= (const GetCmdOpt&);
};


#endif	/* GETCMDOPT_H_1800E12E_979F_4841_B34C_87DECB9F6237 */