INITIALIZE_AG_DEPENDENCY(AliasAnalysis)

void f()
{
    int o1;
    {}
}



/*
https://sourceforge.net/apps/trac/cppcheck/ticket/3762

$ cppcheck Crash2.cpp
Checking Crash2.cpp...
Speicherzugriffsfehler

from llvm compiler : lib/Analysis/AliasAnalysisEvaluator.cpp
*/
