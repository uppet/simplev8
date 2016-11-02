

#include <stdio.h>
#include "v8.h"

using namespace v8;

int main(int argc, char **argv) {
    V8::Initialize();
    HandleScope scope;
    Persistent<Context> ctx = Context::New();
    printf("I'm md8..\n");
}
