#include <iostream>
#include <string>
#include <sstream>
#include <napi.h>

#include "libs/arguments/Arguments.hpp"

using namespace std;

struct Options
{
    vector<string> source;
    string outdir = "";
    string name = "";
    string method = "";
    string chunkMethod = "";
    vector<string> flags;
    vector<string> attributes;

    bool hasFlag(string flag)
    {
        for (string f : flags)
        {
            if (f == flag)
            {
                return true;
            }
        }

        return false;
    }
};

Napi::String greetHello(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 2)
    {
        Napi::TypeError::New(env, "Wrong number of arguments")
            .ThrowAsJavaScriptException();
        cout << "Error";
        // return env.Null();
    };
    int argc = info[0].As<Napi::Number>();
    char **str = {};

    char *test = new char[100];

    Napi::Array arr = info[1].As<Napi::Array>();
    for (int i = 0; i < arr.Length(); i++)
    {
        cout << "interator " << i << endl;

        Napi::Value v = arr[i];
        string val = v.ToString();
        cout << "argv: " << val << endl;
        if (v.IsString())
        {
            string value = v.ToString();
            cout << "Value: " << value << endl;

            strcpy(test, value.c_str());

            str[i] = test;
            // cout << test << endl;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        cout << str[i][0] << endl;
    }

    Arguments args(argc, str);

    args.addArgument("source,i,", "input files");
    args.addArgument("outdir,o", "output directory");
    args.addArgument("method,m", "sampling method");
    args.addArgument("chunkMethod", "chunking method");
    args.addArgument("flags", "flags");
    args.addArgument("attributes", "attributes in output file");

    if (!args.has("source"))
    {
        cout << "/Converter <source> -o <outdir>" << endl;

        exit(1);
    }

    vector<string> source = args.get("source").as<vector<string>>();
    string outdir = args.get("outdir").as<string>();

    string method = args.get("method").as<string>("poisson");
    string chunkMethod = args.get("chunkMethod").as<string>("LASZIP");

    if (source.size() == 0)
    {
        cout << "/Converter <source> -o <outdir>" << endl;

        exit(1);
    }

    vector<string> flags = args.get("flags").as<vector<string>>();
    vector<string> attributes = args.get("attributes").as<vector<string>>();

    Options options;
    options.source = source;
    options.outdir = outdir;
    options.method = method;
    options.chunkMethod = chunkMethod;
    options.flags = flags;
    options.attributes = attributes;

    cout << "flags: ";
    for (string flag : options.flags)
    {
        cout << flag << ", ";
    }
    cout << endl;

    // cout << options;

    return Napi::String::New(env, "success");
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "greetHello"),
                Napi::Function::New(env, greetHello));
    return exports;
}

NODE_API_MODULE(greet, Init)