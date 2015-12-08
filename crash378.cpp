class ArgumentSource {
  union Storage {
    struct {
      RValue Value;
      SILLocation Loc;
    } TheRV;
    struct {
      LValue Value;
      SILLocation Loc;
    } TheLV;
    Expr *TheExpr;
  } Storage;

  enum class Kind : unsigned char {
    RValue,
  } StoredKind;

public:
  ArgumentSource() : StoredKind(Kind::Expr) {
    Storage.TheExpr = nullptr;
  }

  ~ArgumentSource() {
    switch (StoredKind) {
    case Kind::RValue:
      asKnownRValue().~RValue();
      return;
    }
  }

  explicit operator bool() const & {
    switch (StoredKind) {
    case Kind::RValue:
      return bool(asKnownRValue());
    }
  }
  Expr *asKnownExpr() && {
    assert(StoredKind == Kind::RValue);
    Expr *result = Storage.TheExpr;
    Storage.TheExpr = nullptr;
    return result;
  }

  void forwardInto() {
    auto substFormalType = getSubstType();
  }
};
