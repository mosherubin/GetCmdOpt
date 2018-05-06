/* $Header: /cvsnt/repository/source/Common/GetCmdOpt/GetCmdOpt.h,v 1.2 2009/01/02 11:15:48 RETI\mrubin Exp $ */

#ifndef GETCMDOPT_H_1800E12E_979F_4841_B34C_87DECB9F6237
#define GETCMDOPT_H_1800E12E_979F_4841_B34C_87DECB9F6237

#include <string>
#include <vector>

class GetCmdOpt
{
public:
	GetCmdOpt (int argc, char *argv [], std::string keyStart = "--");
	~GetCmdOpt ();

	bool GetInt (const char *p, int& i);
	bool GetIntVector (const char *p, std::vector<int> &vec);
	bool GetNumber (const char *p, double& d);
	bool GetNumberVector (const char *p, std::vector<double> &vec);
	bool GetString (const char *p, std::string &s);
	bool GetStringVector (const char *p, std::vector<std::string> &vec);
	bool GetBool (const char *p);

protected:
	int m_argc;
	char **m_argv;
	std::string m_keyStart;

	bool FindKey (const char *p, int *pIndex);
	bool ReturnValueToken (int iIndex, const char **pToken);
	bool TokenStartWithKeyPrefix(char *token);
	int ReturnMaximumIntValue();
	bool ConvertStringToInt(const char *s, int &i);

private:
	GetCmdOpt (const GetCmdOpt&);
	GetCmdOpt& operator= (const GetCmdOpt&);
};


#endif	/* GETCMDOPT_H_1800E12E_979F_4841_B34C_87DECB9F6237 */

