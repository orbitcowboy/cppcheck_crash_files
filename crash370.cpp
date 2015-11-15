bool Preprocessor::ConcatenateIncludeName(SmallString<128> &FilenameBuffer, SourceLocation &End) {  
                   unsigned PreAppendSize = FilenameBuffer.size();  
                   FilenameBuffer.resize(PreAppendSize + CurTok.getLength());  
                   const char *BufPtr = &FilenameBuffer[PreAppendSize];  
                   return true;
