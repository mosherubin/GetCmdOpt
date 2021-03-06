# GetCmdOpt
Simple, light, cross-platform C++ class for parsing command line options

# Quick start

This is a lightweight C++ option parser library, written in standard C++ to be used cross platform (avoiding the problem that GetOpts is not natively supported on Windows).  `GetCmdOpt` supports retrieving multiple values for a single option key.

Command line options have a mandatory `key` followed by zero or more optional `values`, all delimited by spaces.  Values can be integers, doubles, or strings.  Here are some examples of key/value pairs that can be parsed from the command line.  Here's what a real-life command line might look like:

```
--width 174 --days 3 4 7 8 --radius 3.1415 --distance 3.1415 7.2 -22 123.456 --folder foo/bar --dirs foo/bar baz\\wam "c:\\Program Files\\SuperApp" --enable-trace --somethingelse --flag-1 1 --flag-2 0 --flag-3 qwerty
```

Here is the same command line, parsed into key/value(s) pairs:

```
--width 174
--days 3 4 7 8
--radius 3.1415
--distance 3.1415 7.2 -22 123.456
--folder foo/bar
--dirs foo/bar baz\\wam "c:\\Program Files\\SuperApp"
--enable-trace
--somethingelse
--flag-1 1
--flag-2 0
--flag-3 qwerty
```

The key prefix, by default '--', can be modified by the developer (a default of '--' is preferred over '-' to allow negative numbers as values without mixing up the parser).

## Basics

    #include "GetCmdOpt.h"

Create a GetCmdOpt instance from the main() function's `argc` and `argv` arguments.

    int main(int argc, char *argv[])
    {
        GetCmdOpt *opt = new GetCmdOpt(argc, argv);
      ...
    }

At this point the command line has been parsed.  The application should now call the appropriate `GetCmdOpt` public functions to return the keys/values it needs.  See the section entitled "Parsing Functions Supported by GetCmdOpt" for the functions you can now call on `opt`.

By default, command-line options are declared by prefixing them with the string '--' followed by zero or more values.  Here is an example of a valid and complete command line:

    --rows 10 --fleet 1 2 3 --flag --verbose 0 --I ..\\foo\\bar \"c:\\Program Files\\blah\" --ratio 12.34 --height 1.2 3.4 777.888999

## GetCmdOpt Member Functions

`GetCmdOpts` supports the parsing of integer, double, string, and boolean command line values.  For the first three data types (i.e., integer, double, and string) it supports retrieving multiple values for a single key.  here is a table showing the functions:

| | Integer | Double | String | Boolean |
|:---:|:---:|:---:|:---:|:---:|
| **Single** | GetInt | GetDouble | GetString | GetBool |
| **Vector** | GetIntVector | GetDoubleVector | GetStringVector  | --- |

The following table shows what the command key/values would look like:

| Type | Option Form | Comment | Use Function ... |
|---|---|---|---|
| Integer | `--width 174` | Single integer value | GetInt |
| Integer Vector | `--days 3 4 7 8` | Multiple integer values | GetIntVector |
| Double | `--radius 3.1415` | Single double value | GetDouble |
| Double Vector | `--distance 3.1415 7.2 -22 123.456` | Multiple double values | GetDoubleVector |
| String | `--folder foo/bar` | Single string value | GetString |
| String Vector | `--dirs foo/bar baz\\wam "c:\\Program Files\\SuperApp"` | Multiple string values | GetStringVector |
| Bool | `--enable-trace --somethingelse` | true, `enable-trace` has no value | GetBool |
| Bool | `--flag-1 1` | true, `--enable-trace` value is not "0" | GetBool |
| Bool | `--flag-2 0` | false, `--enable-trace` value is "0" | GetBool |
| Bool | `--flag-3 qwerty` | true, `--enable-trace` value is not "0" | GetBool |

### Constructors

```
*GetCmdOpt (int argc, char *argv [], std::string keyPrefix = "--")*
```

**Description**

Use the `GetCmdOpt` constructor to create an object that can then be probed for command line options.  It accepts

**Parameters**

    argc       [in] The original `argc` passed by the operating system to the program

    argv       [in] The original `argv` passed by the operating system to the program
           
    keyPrefix  [in] An option string denoting the characters that are to prefix to all keys in the command line,
                identifying the command line token as a key string rather than a value string.  The default key
                prefix string is two successive hyphens ('--'), but this can be changed for a given program by
                specifying a different string.
    
### Single-Value Parse: GetInt, GetDouble, GetString, GetBool

```
*bool GetInt (const char *key, int& i)*  
*bool GetDouble (const char *key, double& d)*  
*bool GetString (const char *key, std::string &s)*  
*bool GetBool (const char *key, bool &b)*  
```

**Description**

Each of these functions takes a `key` and a reference to a variable of the desired data type (e.g., int, double, std::string, bool). If the function call succeeds, the parsed value will be returned in the out parameter.

If there are multiple occurrences of the key, only the first value will be returned.  Subsequence keys with the name will be ignored.

The `GetBool` function differs from the other functions that the key can be followed by an optional value.  If there is no value, the boolean value for that key defaults to `true`.  If the value is a string of zeroes (i.e., it is a string whose characters are all '0') then the boolean value is set to `false`.  In all other cases it is set to `true`.  The following table summarizes the rules for `GetBool`:

| Type | Option Form | Comment | Use Function ... |
|---|---|---|---|
| bool | `--enable-trace --somethingelse` | true, `enable-trace` has no value | GetBool |
| bool | `--foo 1` | true, `--foo` value is not "0" | GetBool |
| bool | `--bar 0` | false, `--bar` value is "0" | GetBool |
| bool | `--baz qwerty` | true, `--baz` value is not "0" | GetBool |

**Parameters**

    key    [in] A string corresponding to a command line option key, without the leading key prefix.  For example, 
           if the command line has an option "--date", the `key` string would be "date".
           
    value  [out] A reference to the desired data type expected
    
**Return Value**

On success, the function returns a boolean value of `true`, and the out parameter `value` is set to the parsed value.
If the parsing operation failed, a boolean value of `false` is returned.

### Multiple-Value Parse: GetIntVector, GetDoubleVector, GetStringVector

```
*bool GetIntVector (const char *p, std::vector<int> &vec)*
*bool GetDoubleVector (const char *p, std::vector<double> &vec)*
*bool GetStringVector (const char *p, std::vector<std::string> &vec)*
```

**Description**

Each of these functions takes a `key` and a reference to an `std::vector` variable of the desired data type.  If the function call succeeds, all values associated with the key will be returned in the out parameter.  If the key occurs more than once, all the values associated with all the key occurrences are returned.

**Parameters**

    key    [in] A string corresponding to a command line option key, without the leading key prefix.  For example, 
           if the command line has an option "--date", the `key` string would be "date".
           
    vec    [out] A reference to an `std::vector<>` matching the desired data type
    
**Return Value**

On success, the function returns a boolean value of `true`, and the out parameter `vec` contains all is set to the parsed value.
If the parsing operation failed, a boolean value of `false` is returned.

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
    opt->GetDouble("ratio", ratio);
    opt->GetDoubleVector("height", vecHeights);

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
vecInclude:2^31
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

# Tips / Tricks / Caveats

* The largest integer that `GetInt` can parse is `2147483647` (max int = 2^31 - 1)
* If a key occurs more than once, a `Get*Vector` function call will return **all** values associated with the key.
