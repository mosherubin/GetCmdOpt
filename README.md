# GetCmdOpt
Simple, light, cross-platform C++ class for parsing command line options

# Quick start

This is a lightweight C++ option parser library, written in standard C++ to be used cross platform (avoiding the problem that GetOpts is not natively supported on Windows).  ```GetCmdOpt``` supports retrieving multiple values for a single option key.

Command line options have a mandatory ```key``` followed by zero or more optional ```values```, all delimited by spaces.  Values can be integers, doubles, or strings.  Here are some examples of key/value pairs that can be parsed from the command line:

````
--width 174
--days 3 4 7 8
--distance 3.1415
--distance 3.1415 7.2 -22 123.456
--dirs foo/bar
--dirs foo/bar baz\\wam "c:\\Program Files\\SuperApp"
--enable-trace --somethingelse
--flag 1
--flag 0
--flag qwerty
````

The key prefix, by default '--', can be modified by the developer (a default of '--' is preferred over '-' to allow negative numbers as values without mixing up the parser).

## Basics

    #include "GetCmdOpt.h"

Create a GetCmdOpt instance from the main() function's ```argc``` and ```argv``` arguments.

    int main(int argc, char *argv[])
    {
	    GetCmdOpt *opt = new GetCmdOpt(argc, argv);
      ...
    }

At this point the command line has been parsed.  The application should now call the appropriate ```GetCmdOpt``` public functions to return the keys/values it needs.

By default, command-line options are declared by prefixing them with the string '--' followed by zero or more values.  Here is an example of a valid and complete command line:

    --rows 10 --fleet 1 2 3 --flag --verbose 0 --I ..\\foo\\bar \"c:\\Program Files\\blah\" --ratio 12.34 --height 1.2 3.4 777.888999

## Parsing Functions Suported by GetCmdOpts

| Type | Option Form | Comment | Use Function ... |
|---|---|---|---|
| Integer | ```--width 174``` | Single integer value | GetInt |
| Integer Vector | ```--days 3 4 7 8``` | Multiple integer values | GetIntVector |
| Double | ```--distance 3.1415``` | Single double value | GetNumber |
| Double Vector | ```--distance 3.1415 7.2 -22 123.456``` | Multiple double values | GetNumberVector |
| String | ```--dirs foo/bar``` | Single string value | GetString |
| String Vector | ```--dirs foo/bar baz\\wam "c:\\Program Files\\SuperApp"``` | Multiple string values | GetStringVector |
| Bool | ```--enable-trace --somethingelse``` | true, ```enable-trace``` has no value | GetBool |
| Bool | ```--flag 1``` | true, ```--enable-trace``` value is not "0" | GetBool |
| Bool | ```--flag 0``` | false, ```--enable-trace``` value is "0" | GetBool |
| Bool | ```--flag qwerty``` | true, ```--enable-trace``` value is not "0" | GetBool |

## Member Functions

### Single-Value Parse: GetInt, GetString, GetNumber

### Multiple-Value Parse: GetIntVector, getStringVector, GetNumberVector

### Parsing Boolean Keys

## Coding Example

```
#include "GetCmdOpt.h"

// Demoing GetCmdOpt with the command line:
//
//     --rows 10 --fleet 1 2 3 4 --cols 9 --enable-trace --foobar 1 --foo 0 
//     --day Tuesday --I ..\foo\bar c:\a\b\c\baz "c:\Program Files\blah" 
//     --ratio 12.34 --height 1.2 3.4 5.6 777.888999

void DisplayCommandLine(int argc, char *argv[])
{
	std::string cmdline;

	for (int i = 1; i < argc; ++i)
	{
		bool hasEmbeddedSpace = (strchr(argv[i], ' ') != 0);

		if (hasEmbeddedSpace) cmdline += "\"";
		cmdline += argv[i];
		if (hasEmbeddedSpace) cmdline += "\"";
		if (i < argc - 1)
			cmdline += " ";
	}
	printf("Command line:\n\n\t%s\n\n", cmdline.c_str());
}

int main(int argc, char *argv[])
{
	GetCmdOpt *opt = new GetCmdOpt(argc, argv);
	int i;

	int rows;
	int cols;
	std::vector<int> vecFleet;
	std::string day;
	std::vector<std::string> vecInclude;
	double ratio;
	std::vector<double> vecHeights;

	DisplayCommandLine(argc, argv);

	opt->GetInt("rows", rows);
	opt->GetInt("cols", cols);
	opt->GetIntVector("fleet", vecFleet);
	opt->GetString("day", day);
	opt->GetStringVector("I", vecInclude);
	opt->GetNumber("ratio", ratio);
	opt->GetNumberVector("height", vecHeights);

	printf ("rows = %d\n", rows);
	printf ("cols = %d\n", cols);
	printf ("vecFleet:\n");
	for (i = 0; i < (int) vecFleet.size(); ++i)
	{
		printf("\t[%d] %d\n", i, vecFleet[i]);
	}
	printf("enable-trace is %s\n", opt->GetBool("enable-trace") ? "TRUE" : "FALSE");
	printf("foobar is %s\n", opt->GetBool("foobar") ? "TRUE" : "FALSE");
	printf("foo is %s\n", opt->GetBool("foo") ? "TRUE" : "FALSE");

	printf("day is \"%s\"\n", day.c_str());
	printf("vecInclude:\n");
	for (i = 0; i < (int)vecInclude.size(); ++i)
	{
		printf("\t[%d] %s\n", i, vecInclude[i].c_str());
	}

	printf("ratio is \"%f\"\n", ratio);
	printf("vecHeights:\n");
	for (i = 0; i < (int)vecHeights.size(); ++i)
	{
		printf("\t[%d] %f\n", i, vecHeights[i]);
	}
}
```

Here is the expected output of the above program:

```
Command line:

        --rows 10 --fleet 1 2 3 4 --cols 9 --enable-trace --foobar 1 --foo 0 --day Tuesday --I ..\foo\bar c:\a\b\c\baz "c:\Program Files\blah" --ratio 12.34 --height 1.2 3.4 5.6 777.888999

rows = 10
cols = 9
vecFleet:
        [0] 1
        [1] 2
        [2] 3
        [3] 4
enable-trace is TRUE
foobar is TRUE
foo is FALSE
day is "Tuesday"
vecInclude:
        [0] ..\foo\bar
        [1] c:\a\b\c\baz
        [2] c:\Program Files\blah
ratio is "12.340000"
vecHeights:
        [0] 1.200000
        [1] 3.400000
        [2] 5.600000
        [3] 777.888999
```

# TODO list

* <need to fill in>
