class basic_fbstring {
    basic_fbstring& operator=(int il) {
        return assign();
    }
    basic_fbstring& assign() {
        return replace();
    }
    basic_fbstring& replaceImplDiscr() {
        return replace();
    }
    basic_fbstring& replace() {
        return replaceImplDiscr();
    }
};
