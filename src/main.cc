

#include <stdio.h>
#include "v8.h"

#include <string>
#include <fstream>
#include <streambuf>
#include <ios>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace v8;

Handle<Value> InvokePrint(const Arguments& args) {
    if (args.Length() != 1) {
        return Undefined();
    }
    Local<String> line = String::Cast(*args[0]);
    std::vector<uint16_t> buf(line->Length()+1);
    line->Write(buf.data(), 0, line->Length());
    std::wstring output;
    std::copy(buf.begin(), buf.begin() + line->Length(), std::back_inserter(output));
    output += L"\n";
    std::wcout<<output;
    return Undefined();;
}

Handle<Value> InvokeRead(const Arguments& args) {
    std::string line;
    std::cin>>line;
    if (std::cin.rdstate()  & std::ifstream::eofbit) {
        line = "eof";
        ThrowException(String::New("Exception eof"));
    }
    return String::New(line.c_str());
}

int main(int argc, char **argv) {
    V8::Initialize();
    std::string v8flags = "--nolazy";
    V8::SetFlagsFromString(v8flags.c_str(), v8flags.length());
    printf("I'm md8 1..\n");
    HandleScope hscope;
    const char *extensions[] = {"v8/print", "v8/load", "v8/quit"};
    ExtensionConfiguration cfg(3, extensions);
    Persistent<Context> ctx = Context::New(&cfg);
    Context::Scope scope(ctx);

    if (argc < 2) {
        printf("Need script..\n");
        return 1;
    }
    printf("I'm md8..\n");

    std::ifstream filestream(argv[1]);
    std::string str((std::istreambuf_iterator<char>(filestream)),
                    std::istreambuf_iterator<char>());

    v8::Local<v8::FunctionTemplate> tPrint = v8::FunctionTemplate::New(InvokePrint);
    v8::Local<v8::FunctionTemplate> tRead = v8::FunctionTemplate::New(InvokeRead);

    ctx->Global()->Set(String::New("mdprint"), tPrint->GetFunction());
    ctx->Global()->Set(String::New("readline"), tRead->GetFunction());

    ScriptOrigin source_origin(String::New(argv[1]));
    Local<String> source= String::New(str.c_str());

    Local<Script> script = Script::Compile(source, &source_origin);
    script->Run();
    return 0;
}
