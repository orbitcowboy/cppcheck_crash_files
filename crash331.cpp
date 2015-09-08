BG_TString BG_TString::Format(const BG_TChar * fmt, ...)
{
  va_list args;
  va_start(args,fmt);
  BG_TString result(FormatVA(fmt,args));
  va_end(args);
  return result;
}

#if !(defined(EG_TARGET_OS_WIN))
BG_TString BG_TString::Format(const BG_TString & fmt, ...)
{
  const BG_TChar * f = fmt.c_str();
  va_list args;
  va_start(args,f);
  BG_TString result(FormatVA(f,args));
  va_end(args);
  return result;
}
#endif
