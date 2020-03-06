//#include "gtest/gtest.h"
#include <string>
#include <sstream>
#include "../src/GetCmdOpt.h"

static void SplitIntoArgs(std::vector<std::string>& qargs, std::string command)
{
	// Based on: https://stackoverflow.com/a/18675499/562242
	std::string str = command;
	std::string token;

	for (size_t i = 0; i<str.length(); i++) {
		char c = str[i];
		if (c == ' ') {
			if (!token.empty())
			{
				qargs.push_back(token);
				token.clear();
				continue;
			}
		}
		else if (c == '\"') {
			i++;
			while (str[i] != '\"') { token.push_back (str[i]); i++; }
		}
		else {
			token.push_back(c);
		}
	}

	if (!token.empty())
	{
		qargs.push_back(token);
	}
}

static void PrepareArgs (const char *commandLine, int &testArgc, char *** testArgv)
{
	int numArgs = 0;
	std::string cmd = commandLine;
	std::string temp;
	std::istringstream ss(cmd);
	std::vector<std::string> qargs;

	SplitIntoArgs(qargs, cmd);

	testArgc = (int) qargs.size() + 1;
	*testArgv = new char *[testArgc];
	*testArgv[0] = 0;	/* executable name, not needed */
	for (size_t i = 0; i < qargs.size(); ++i)
	{
		char *p = new char[qargs[i].size()+1];
		strcpy(p, qargs[i].c_str());
		(*testArgv)[i + 1] = p;
	}
}

class TestFixture : public ::testing::Test
{
public:
	GetCmdOpt *opts = 0;
	bool b;

	void CreateOpts(char *commandLine)
	{
		int testArgc = 0;
		char **testArgv = 0;

		PrepareArgs(commandLine, testArgc, &testArgv);
		opts = new GetCmdOpt(testArgc, testArgv);
	}

	void CreateOpts(char *commandLine, const char *keyStart)
	{
		int testArgc = 0;
		char **testArgv = 0;

		PrepareArgs(commandLine, testArgc, &testArgv);
		opts = new GetCmdOpt(testArgc, testArgv, keyStart);
	}

	void SetUp()
	{
	}

	void TearDown()
	{
		if (opts != 0)
		{
			delete opts;
		}
	}
};

TEST_F(TestFixture, Test_FullOptionList) {
	CreateOpts("--rows 10 --fleet 1 2 3 4 --cols 9 --Q --n --foobar --foo --rows --I ..\\foo\\bar c:\\a\\b\\c\\baz \"c:\\Program Files\\blah\" --ratio 12.34 --height 1.2 3.4 5.6 777.888999");

	int nRows;
	opts->GetInt("rows", nRows);
	ASSERT_TRUE(nRows == 10);

	int nCols;
	opts->GetInt("cols", nCols);
	ASSERT_TRUE(nCols == 9);

	opts->GetInt("cols", nCols);
	opts->GetInt("cols", nCols);
	ASSERT_TRUE(nCols == 9);

	double number;
	opts->GetDouble("ratio", number);
	ASSERT_TRUE(number == 12.34);

	std::vector<int> vecFleet;
	opts->GetIntVector("fleet", vecFleet);
	ASSERT_TRUE(vecFleet.size() == 4);
	ASSERT_TRUE(vecFleet[0] == 1);
	ASSERT_TRUE(vecFleet[1] == 2);
	ASSERT_TRUE(vecFleet[2] == 3);
	ASSERT_TRUE(vecFleet[3] == 4);

	std::vector<double> vecHeight;
	opts->GetDoubleVector("height", vecHeight);
	ASSERT_TRUE(vecHeight.size() == 4);
	ASSERT_TRUE(vecHeight[0] == 1.2);
	ASSERT_TRUE(vecHeight[1] == 3.4);
	ASSERT_TRUE(vecHeight[2] == 5.6);
	ASSERT_TRUE(vecHeight[3] == 777.888999);

	std::vector<std::string> vecIncludeDir;
	opts->GetStringVector("I", vecIncludeDir);
	ASSERT_TRUE(vecIncludeDir.size() == 3);
	ASSERT_TRUE(vecIncludeDir[0].compare("..\\foo\\bar") == 0);
	ASSERT_TRUE(vecIncludeDir[1].compare("c:\\a\\b\\c\\baz") == 0);
	ASSERT_TRUE(vecIncludeDir[2].compare("c:\\Program Files\\blah") == 0);

	bool b;

	ASSERT_TRUE(opts->GetBool("Q", b) && b);
	ASSERT_TRUE(opts->GetBool("n", b) && b);
	ASSERT_TRUE(opts->GetBool("foobar", b) && b);
	ASSERT_TRUE(opts->GetBool("foo", b) && b);
	ASSERT_TRUE(opts->GetBool("fleet", b) && b);
	ASSERT_FALSE(opts->GetBool("book", b));
}

TEST_F(TestFixture, Test_NoOptions) {
	CreateOpts("");

	int nRows;
	ASSERT_FALSE(opts->GetInt("rows", nRows));

	std::vector<int> vecFleet;
	ASSERT_FALSE(opts->GetIntVector("fleet", vecFleet));
}

TEST_F(TestFixture, Test_String_SameKeyMultipleTimes) {
	CreateOpts("--I AAA BBB --I CCC DDD EEE FFF --I GGGG H II JJJ");

	std::vector<std::string> vec;
	opts->GetStringVector("I", vec);

	ASSERT_TRUE (vec.size() == 10);
	ASSERT_TRUE(vec[0].compare("AAA") == 0);
	ASSERT_TRUE(vec[1].compare("BBB") == 0);
	ASSERT_TRUE(vec[2].compare("CCC") == 0);
	ASSERT_TRUE(vec[3].compare("DDD") == 0);
	ASSERT_TRUE(vec[4].compare("EEE") == 0);
	ASSERT_TRUE(vec[5].compare("FFF") == 0);
	ASSERT_TRUE(vec[6].compare("GGGG") == 0);
	ASSERT_TRUE(vec[7].compare("H") == 0);
	ASSERT_TRUE(vec[8].compare("II") == 0);
	ASSERT_TRUE(vec[9].compare("JJJ") == 0);
}

TEST_F(TestFixture, Test_Int_NegativeValues) {
	CreateOpts("--X 1 2 -3 4 5 -6");

	std::vector<int> vec;
	opts->GetIntVector("X", vec);

	ASSERT_TRUE(vec.size() == 6);
	ASSERT_TRUE(vec[0] = 1);
	ASSERT_TRUE(vec[1] = 2);
	ASSERT_TRUE(vec[2] = -3);
	ASSERT_TRUE(vec[3] = 4);
	ASSERT_TRUE(vec[4] = 5);
	ASSERT_TRUE(vec[5] = -6);
}

TEST_F(TestFixture, Test_Int_SameKeyMultipleTimes) {
	CreateOpts("--X 1 2 --X 3 4 5 6 --X 7 8 9 10");

	std::vector<int> vec;
	opts->GetIntVector("X", vec);

	ASSERT_TRUE(vec.size() == 10);
	ASSERT_TRUE(vec[0] = 1);
}

TEST_F(TestFixture, Test_Double_SameKeyMultipleTimes) {
	CreateOpts("--D 1.2 2.34 --D 3 4.5 5.67 6.789012 --D 7.89 8.9012 9.01234 0");

	std::vector<double> vec;
	opts->GetDoubleVector("D", vec);

	ASSERT_TRUE(vec.size() == 10);
	ASSERT_TRUE(vec[0] == 1.2);
	ASSERT_TRUE(vec[1] == 2.34);
	ASSERT_TRUE(vec[2] == 3);
	ASSERT_TRUE(vec[3] == 4.5);
	ASSERT_TRUE(vec[4] == 5.67);
	ASSERT_TRUE(vec[5] == 6.789012);
	ASSERT_TRUE(vec[6] == 7.89);
	ASSERT_TRUE(vec[7] == 8.9012);
	ASSERT_TRUE(vec[8] == 9.01234);
	ASSERT_TRUE(vec[9] == 0);
}

TEST_F(TestFixture, Test_Double_NegativeValues) {
	CreateOpts("--X 1.1 2.2 -3.3 4.4 5.5 -6");

	std::vector<double> vec;
	opts->GetDoubleVector("X", vec);

	ASSERT_TRUE(vec.size() == 6);
	ASSERT_TRUE(vec[0] = 1.1);
	ASSERT_TRUE(vec[1] = 2.2);
	ASSERT_TRUE(vec[2] = -3.3);
	ASSERT_TRUE(vec[3] = 4.4);
	ASSERT_TRUE(vec[4] = 5.5);
	ASSERT_TRUE(vec[5] = -6.0);
}

TEST_F(TestFixture, Test_Double_ChangeKeyPrefix) {
	CreateOpts("#$X 1.1 2.2 -3.3 4.4 5.5 -6", "#$");

	std::vector<double> vec;
	opts->GetDoubleVector("X", vec);

	ASSERT_TRUE(vec.size() == 6);
	ASSERT_TRUE(vec[0] = 1.1);
	ASSERT_TRUE(vec[1] = 2.2);
	ASSERT_TRUE(vec[2] = -3.3);
	ASSERT_TRUE(vec[3] = 4.4);
	ASSERT_TRUE(vec[4] = 5.5);
	ASSERT_TRUE(vec[5] = -6.0);
}

TEST_F(TestFixture, Test_Bool_No_Value) {
	CreateOpts("--flag");

	ASSERT_TRUE(opts->GetBool("flag", b) && b);
}

TEST_F(TestFixture, Test_Bool_No_Value_Followed_by_Key) {
	CreateOpts("--flag --key");

	ASSERT_TRUE(opts->GetBool("flag", b) && b);
}

TEST_F(TestFixture, Test_Bool_Zero_Value) {
	CreateOpts("--flag 0");

	ASSERT_TRUE(opts->GetBool("flag", b) && !b);
}

TEST_F(TestFixture, Test_Bool_Non_Zero_Value_1) {
	CreateOpts("--flag 1");

	ASSERT_TRUE(opts->GetBool("flag", b) && b);
}

TEST_F(TestFixture, Test_Bool_Non_Zero_Value_2) {
	CreateOpts("--flag 7");

	ASSERT_TRUE(opts->GetBool("flag", b) && b);
}

TEST_F(TestFixture, Test_Bool_Non_Zero_Value_3) {
	CreateOpts("--flag foobar");

	ASSERT_TRUE(opts->GetBool("flag", b) && b);
}

TEST_F(TestFixture, Test_Bool_Identical_Keys_Different_Values_1) {
	CreateOpts("--flag --flag 0");

	ASSERT_TRUE(opts->GetBool("flag", b) && b);
}

TEST_F(TestFixture, Test_Bool_Identical_Keys_Different_Values_2) {
	CreateOpts("--flag 0 --flag");

	ASSERT_TRUE(opts->GetBool("flag", b) && !b);
}

TEST_F(TestFixture, Test_Bool_Identical_Keys_Identical_Values) {
	CreateOpts("--flag 1 --flag");

	ASSERT_TRUE(opts->GetBool("flag", b) && b);
}

TEST_F(TestFixture, Test_Bool_Identical_Keys) {
	CreateOpts("--flag false");

	bool b;
	ASSERT_TRUE(opts->GetBool("flag", b) && b);
}

TEST_F(TestFixture, Test_Int_Too_Large) {
	CreateOpts("--val 2147483648");

	int val;

	ASSERT_FALSE(opts->GetInt("val", val));
}

TEST_F(TestFixture, Test_Int_Invalid) {
	CreateOpts("--val foobar");

	int val;

	ASSERT_FALSE(opts->GetInt("val", val));
}

TEST_F(TestFixture, Test_Double_Invalid) {
	CreateOpts("--val foobar");

	double val;

	ASSERT_FALSE(opts->GetDouble("val", val));
}

TEST_F(TestFixture, Test_String_Empty) {
	CreateOpts("--str --val 123");

	std::string str = "qqq";

	ASSERT_FALSE(opts->GetString("str", str));
}
