namespace clang {
namespace clangd {

template <class> class UniqueFunction;

template <typename T> using Callback = UniqueFunction<void(llvm ::Expected<T>)>;

template <class Ret, class... Args> class UniqueFunction<Ret(Args...)> {};

} // namespace clangd
} // namespace clang

void ClangdServer ::findHover(PathRef File, Position Pos, Callback<Hover> CB) {
  auto FS = FSProvider.getFileSystem();
  auto Action = [Pos, FS](Callback<Hover> CB,
                          llvm ::Expected<InputsAndAST> InpAST) {
    CB(clangd ::getHover(InpAST->AST, Pos));
  };
}

// from https://trac.cppcheck.net/ticket/8601
