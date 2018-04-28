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

	bool GetInt (char *p, int& i);
	bool GetIntVector (char *p, std::vector<int> &vec);
	bool GetNumber (char *p, double& d);
	bool GetNumberVector (char *p, std::vector<double> &vec);
	bool GetString (char *p, std::string &s);
	bool GetStringVector (char *p, std::vector<std::string> &vec);
	bool GetBool (char *p);

protected:
	int m_argc;
	char **m_argv;
	std::string m_keyStart;

	bool FindKey (char *p, int *pIndex);
	bool ReturnValueToken (int iIndex, const char **pToken);
	bool TokenStartWithKeyPrefix(char *token);

private:
	GetCmdOpt (const GetCmdOpt&);
	GetCmdOpt& operator= (const GetCmdOpt&);
};


#endif	/* GETCMDOPT_H_1800E12E_979F_4841_B34C_87DECB9F6237 */

