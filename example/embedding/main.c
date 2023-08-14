//For more details, visit https://wren.io/embedding/

#include <stdio.h>
#include "wren.h"

static void writeFn(WrenVM* vm, const char* text)
{
  printf("%s", text);
}

void errorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
  switch (errorType)
  {
    case WREN_ERROR_COMPILE:
    {
      printf("[%s line %d] [Error] %s\n", module, line, msg);
    } break;
    case WREN_ERROR_STACK_TRACE:
    {
      printf("[%s line %d] in %s\n", module, line, msg);
    } break;
    case WREN_ERROR_RUNTIME:
    {
      printf("[Runtime Error] %s\n", msg);
    } break;
  }
}

int main()
{

  WrenConfiguration config;
  wrenInitConfiguration(&config);
    config.writeFn = &writeFn;
    config.errorFn = &errorFn;
  WrenVM* vm = wrenNewVM(&config);

  const char* module = "main";
  const char* script = "class Skynet {\n"
"  static makeFiber(num, size, div) {\n"
"    return Fiber.new {\n"
"      if (size == 1) {\n"
"        Fiber.yield(num)\n"
"      } else {\n"
"        var fibers = []\n"
"        for (i in 0...div) {\n"
"          var subNum = num + i * (size / div)\n"
"          fibers.add(makeFiber(subNum, size / div, div))\n"
"        }\n"
"\n"
"        var sum = 0\n"
"        for (task in fibers) {\n"
"          sum = sum + task.call()\n"
"        }\n"
"        Fiber.yield(sum)\n"
"      }\n"
"    }\n"
"  }\n"
"}\n"
"\n"
"var start = System.clock\n"
"var result = Skynet.makeFiber(0, 1000000, 10).call()\n"
"var end = System.clock\n"
"System.print(\"Result: %(result) in %(end - start) s\")\n";


  WrenInterpretResult result = wrenInterpret(vm, module, script);

  switch (result)
  {
    case WREN_RESULT_COMPILE_ERROR:
      { printf("Compile Error!\n"); } break;
    case WREN_RESULT_RUNTIME_ERROR:
      { printf("Runtime Error!\n"); } break;
    case WREN_RESULT_SUCCESS:
      { printf("Success!\n"); } break;
    case WREN_RESULT_MAX_OPERATIONS:
      printf("Max operations reached!\n");
      break;
  }

  wrenFreeVM(vm);

}
