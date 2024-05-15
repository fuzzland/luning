void __Instr_onEnterFunc(const char *FuncName)
{
    printf("\n__Instr_onEnterFunc@@@%s\n", FuncName);
}

void __Instr_onArg(const void *Arg, int ArgIndex)
{
    printf("\n__Instr_onArg@@@%d@@@%p\n", ArgIndex, Arg);
}

void __Instr_beforeExit(const char *FuncName, const void *ReturnValue)
{
    printf("\n__Instr_beforeExit@@@%s@@@%p\n", FuncName, ReturnValue);
}