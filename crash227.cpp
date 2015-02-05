//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//


#ifndef _EXPRESSION_NODE_
#define _EXPRESSION_NODE_

#ifdef FEATURE_PAL
#error This file isn't designed to build in PAL
#endif

#include "strike.h"
#include "sos.h"
//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

// ==++==
// 
 
// 
// ==--==
#ifndef __util_h__
#define __util_h__

#define LIMITED_METHOD_CONTRACT

// So we can use the PAL_TRY_NAKED family of macros without dependencies on utilcode.
inline void RestoreSOToleranceState() {}

#include <cor.h>
#include <CorSym.h>
#include <clrdata.h>
#include <palclr.h>

#if !defined(FEATURE_PAL)
#include <metahost.h>
#include <new>
#include <dia2.h>
#endif

#ifdef STRIKE
#if defined(_MSC_VER)
#pragma warning(disable:4200)
#pragma warning(default:4200)
#endif
#include "data.h"

#endif //STRIKE

#include "cor.h"
#include "cordebug.h"
#include "static_assert.h"


typedef LPCSTR  LPCUTF8;
typedef LPSTR   LPUTF8;

DECLARE_HANDLE(OBJECTHANDLE);

struct IMDInternalImport;

#if defined(_TARGET_WIN64_)
#define WIN64_8SPACES ""
#define WIN86_8SPACES "        "
#define POINTERSIZE "16"
#define POINTERSIZE_HEX 16
#define POINTERSIZE_BYTES 8
#define POINTERSIZE_TYPE "I64"
#else
#define WIN64_8SPACES "        "
#define WIN86_8SPACES ""
#define POINTERSIZE "8"
#define POINTERSIZE_HEX 8
#define POINTERSIZE_BYTES 4
#define POINTERSIZE_TYPE "I32"
#endif

#if defined(_MSC_VER)
#pragma warning(disable:4510 4512 4610)
#endif

#ifndef _ASSERTE
#ifdef _DEBUG
#define _ASSERTE(expr)         \
    do { if (!(expr) ) { ExtErr("_ASSERTE fired:\n\t%s\n", #expr); if (IsDebuggerPresent()) DebugBreak(); } } while (0)
#else
#define _ASSERTE(x)
#endif
#endif // ASSERTE

#ifdef _DEBUG
#define ASSERT_CHECK(expr, msg, reason)         \
        do { if (!(expr) ) { ExtOut(reason); ExtOut(msg); ExtOut(#expr); DebugBreak(); } } while (0)
#endif

// PREFIX macros - Begin

// SOS does not have support for Contracts.  Therefore we needed to duplicate
// some of the PREFIX infrastructure from inc\check.h in here.

// Issue - PREFast_:510  v4.51 does not support __assume(0)
#if (defined(_MSC_VER) && !defined(_PREFAST_)) || defined(_PREFIX_)
#if defined(_AMD64_)
// Empty methods that consist of UNREACHABLE() result in a zero-sized declspec(noreturn) method
// which causes the pdb file to make the next method declspec(noreturn) as well, thus breaking BBT
// Remove when we get a VC compiler that fixes VSW 449170
# define __UNREACHABLE() DebugBreak(); __assume(0);
#else
# define __UNREACHABLE() __assume(0)
#endif
#else
#define __UNREACHABLE()  do { } while(true)
#endif


#if defined(_PREFAST_) || defined(_PREFIX_) 
#define COMPILER_ASSUME_MSG(_condition, _message) if (!(_condition)) __UNREACHABLE();
#else

#if defined(DACCESS_COMPILE)
#define COMPILER_ASSUME_MSG(_condition, _message) do { } while (0)
#else

#if defined(_DEBUG)
#define COMPILER_ASSUME_MSG(_condition, _message) \
    ASSERT_CHECK(_condition, _message, "Compiler optimization assumption invalid")
#else
#define COMPILER_ASSUME_MSG(_condition, _message) __assume(_condition)
#endif // _DEBUG

#endif // DACCESS_COMPILE

#endif // _PREFAST_ || _PREFIX_

#define PREFIX_ASSUME(_condition) \
    COMPILER_ASSUME_MSG(_condition, "")

// PREFIX macros - End

class MethodTable;

#define MD_NOT_YET_LOADED ((DWORD_PTR)-1)
/*
 * HANDLES
 *
 * The default type of handle is a strong handle.
 *
 */
#define HNDTYPE_DEFAULT                         HNDTYPE_STRONG
#define HNDTYPE_WEAK_DEFAULT                    HNDTYPE_WEAK_LONG
#define HNDTYPE_WEAK_SHORT                      (0)
#define HNDTYPE_WEAK_LONG                       (1)
#define HNDTYPE_STRONG                          (2)
#define HNDTYPE_PINNED                          (3)
#define HNDTYPE_VARIABLE                        (4)
#define HNDTYPE_REFCOUNTED                      (5)
#define HNDTYPE_DEPENDENT                       (6)
#define HNDTYPE_ASYNCPINNED                     (7)
#define HNDTYPE_SIZEDREF                        (8)
#define HNDTYPE_WEAK_WINRT                      (9)

// Anything above this we consider abnormal and stop processing heap information
const int nMaxHeapSegmentCount = 1000;

class BaseObject
{
    MethodTable    *m_pMethTab;
};


const DWORD gElementTypeInfo[] = {
#define TYPEINFO(e,ns,c,s,g,ia,ip,if,im,gv)    s,
#include "cortypeinfo.h"
#undef TYPEINFO
};

// Under unix we need to implement ExtensionApis.
#ifdef FEATURE_PAL
#undef GetExpression
DWORD_PTR GetExpression(const char *exp);
#endif // FEATURE_PAL

typedef struct tagLockEntry
{
    tagLockEntry *pNext;    // next entry
    tagLockEntry *pPrev;    // prev entry
    DWORD dwULockID;
    DWORD dwLLockID;        // owning lock
    WORD wReaderLevel;      // reader nesting level    
} LockEntry;

#define MAX_CLASSNAME_LENGTH    1024

enum EEFLAVOR {UNKNOWNEE, MSCOREE, MSCORWKS, MSCOREND};

#include "sospriv.h"
extern IXCLRDataProcess *g_clrData;
extern ISOSDacInterface *g_sos;

#include "dacprivate.h"

#ifndef FEATURE_PAL
interface ICorDebugProcess;
extern ICorDebugProcess * g_pCorDebugProcess;
#endif // FEATURE_PAL

// This class is templated for easy modification.  We may need to update the CachedString
// or related classes to use wchar_t instead of char in the future.
template <class T, int count, int size>
class StaticData
{
public:
    StaticData()
    {
        for (int i = 0; i < count; ++i)
            InUse[i] = false;
    }

    // Whether the individual data pointers in the cache are in use.
    bool InUse[count];

    // The actual data itself.
    T Data[count][size];

    // The number of arrays in the cache.
    static const int Count;

    // The size of each individual array.
    static const int Size;
};

class CachedString
{
public:
    CachedString();
    CachedString(const CachedString &str);
    ~CachedString();

    const CachedString &operator=(const CachedString &str);

    // Returns the capacity of this string.
    size_t GetStrLen() const
    {
        return mSize;
    }

    // Returns a mutable character pointer.  Be sure not to write past the
    // length of this string.
    inline operator char *()
    {
        return mPtr;
    }

    // Returns a const char representation of this string.
    inline operator const char *() const
    {
        return GetPtr();
    }

    // To ensure no AV's, any time a constant pointer is requested, we will
    // return an empty string "" if we hit an OOM.  This will only happen
    // if we hit an OOM and do not check for it before using the string.
    // If you request a non-const char pointer out of this class, it may be
    // null (see operator char *).
    inline const char *GetPtr() const
    {
        if (!mPtr || IsOOM())
            return "";

        return mPtr;
    }

    // Returns true if we ran out of memory trying to allocate the string
    // or the refcount.
    bool IsOOM() const
    {
        return mIndex == -2;
    }
    
    // allocate a string of the specified size.  this will Clear() any
    // previously allocated string.  call IsOOM() to check for failure.
    void Allocate(int size);

private:
    // Copies rhs into this string.
    void Copy(const CachedString &rhs);

    // Clears this string, releasing any underlying memory.
    void Clear();

    // Creates a new string.
    void Create();

    // Sets an out of memory state.
    void SetOOM();

private:
    char *mPtr;

    // The reference count.  This may be null if there is only one copy
    // of this string.
    mutable unsigned int *mRefCount;

    // mIndex contains the index of the cached pointer we are using, or:
    // ~0 - poison value we initialize it to for debugging purposes
    // -1 - mPtr points to a pointer we have new'ed
    // -2 - We hit an oom trying to allocate either mCount or mPtr
    int mIndex;
    
    // contains the size of current string
    int mSize;

private:
    static StaticData<char, 4, 1024> cache;
};

// Things in this namespace should not be directly accessed/called outside of
// the output-related functions.
namespace Output
{
    extern unsigned int g_bSuppressOutput;
    extern unsigned int g_Indent;
    extern unsigned int g_DMLEnable;
    extern bool g_bDbgOutput;
    extern bool g_bDMLExposed;
    
    inline bool IsOutputSuppressed()
    { return g_bSuppressOutput > 0; }
    
    inline void ResetIndent()
    { g_Indent = 0; }
    
    inline void SetDebugOutputEnabled(bool enabled)
    { g_bDbgOutput = enabled; }
    
    inline bool IsDebugOutputEnabled()
    { return g_bDbgOutput; }

    inline void SetDMLExposed(bool exposed)
    { g_bDMLExposed = exposed; }
    
    inline bool IsDMLExposed()
    { return g_bDMLExposed; }

    enum FormatType
    {
        DML_None,
        DML_MethodTable,
        DML_MethodDesc,
        DML_EEClass,
        DML_Module,
        DML_IP,
        DML_Object,
        DML_Domain,
        DML_Assembly,
        DML_ThreadID,
        DML_ValueClass,
        DML_DumpHeapMT,
        DML_ListNearObj,
        DML_ThreadState,
        DML_PrintException,
        DML_RCWrapper,
        DML_CCWrapper,
        DML_ManagedVar,
    };

    /**********************************************************************\
    * This function builds a DML string for a ValueClass.  If DML is       *
    * enabled, this function returns a DML string based on the format      *
    * type.  Otherwise this returns a string containing only the hex value *
    * of addr.                                                             *
    *                                                                      *
    * Params:                                                              *
    *   mt - the method table of the ValueClass                            *
    *   addr - the address of the ValueClass                               *
    *   type - the format type to use to output this object                *
    *   fill - whether or not to pad the hex value with zeros              *
    *                                                                      *
    \**********************************************************************/
    CachedString BuildVCValue(CLRDATA_ADDRESS mt, CLRDATA_ADDRESS addr, FormatType type, bool fill = true);
    

    /**********************************************************************\
    * This function builds a DML string for an object.  If DML is enabled, *
    * this function returns a DML string based on the format type.         *
    * Otherwise this returns a string containing only the hex value of     *
    * addr.                                                                *
    *                                                                      *
    * Params:                                                              *
    *   addr - the address of the object                                   *
    *   type - the format type to use to output this object                *
    *   fill - whether or not to pad the hex value with zeros              *
    *                                                                      *
    \**********************************************************************/
    CachedString BuildHexValue(CLRDATA_ADDRESS addr, FormatType type, bool fill = true);

    /**********************************************************************\
    * This function builds a DML string for an managed variable name.      *
    * If DML is enabled, this function returns a DML string that will      *
    * enable the expansion of that managed variable using the !ClrStack    *
    * command to display the variable's fields, otherwise it will just     *
    * return the variable's name as a string.
    *                                                                      *
    * Params:                                                              *
    *   expansionName - the current variable expansion string              *
    *   frame - the frame that contains the variable of interest           *
    *   simpleName - simple name of the managed variable                   *
    *                                                                      *
    \**********************************************************************/
    CachedString BuildManagedVarValue(__in_z LPWSTR expansionName, ULONG frame, __in_z LPWSTR simpleName, FormatType type);
    CachedString BuildManagedVarValue(__in_z LPWSTR expansionName, ULONG frame, int indexInArray, FormatType type);    //used for array indices (simpleName = "[<indexInArray>]")
}

class NoOutputHolder
{
public:
    NoOutputHolder(BOOL bSuppress = TRUE);
    ~NoOutputHolder();

private:
    BOOL mSuppress;
};

class EnableDMLHolder
{
public:
    EnableDMLHolder(BOOL enable);
    ~EnableDMLHolder();

private:
    BOOL mEnable;
};

size_t CountHexCharacters(CLRDATA_ADDRESS val);

// Normal output.
void DMLOut(PCSTR format, ...);         /* Prints out DML strings. */
void IfDMLOut(PCSTR format, ...);       /* Prints given DML string ONLY if DML is enabled; prints nothing otherwise. */
void ExtOut(PCSTR Format, ...);         /* Prints out to ExtOut (no DML). */
void ExtWarn(PCSTR Format, ...);        /* Prints out to ExtWarn (no DML). */
void ExtErr(PCSTR Format, ...);         /* Prints out to ExtErr (no DML). */
void ExtDbgOut(PCSTR Format, ...);      /* Prints out to ExtOut in a checked build (no DML). */
void WhitespaceOut(int count);          /* Prints out "count" number of spaces in the output. */

// Change indent for ExtOut
inline void IncrementIndent()  { Output::g_Indent++; }
inline void DecrementIndent()  { if (Output::g_Indent > 0) Output::g_Indent--; }
inline void ExtOutIndent()  { WhitespaceOut(Output::g_Indent << 2); }

// DML Generation Methods
#define DMLListNearObj(addr) Output::BuildHexValue(addr, Output::DML_ListNearObj).GetPtr()
#define DMLDumpHeapMT(addr) Output::BuildHexValue(addr, Output::DML_DumpHeapMT).GetPtr()
#define DMLMethodTable(addr) Output::BuildHexValue(addr, Output::DML_MethodTable).GetPtr()
#define DMLMethodDesc(addr) Output::BuildHexValue(addr, Output::DML_MethodDesc).GetPtr()
#define DMLClass(addr) Output::BuildHexValue(addr, Output::DML_EEClass).GetPtr()
#define DMLModule(addr) Output::BuildHexValue(addr, Output::DML_Module).GetPtr()
#define DMLIP(ip) Output::BuildHexValue(ip, Output::DML_IP).GetPtr()
#define DMLObject(addr) Output::BuildHexValue(addr, Output::DML_Object).GetPtr()
#define DMLDomain(addr) Output::BuildHexValue(addr, Output::DML_Domain).GetPtr()
#define DMLAssembly(addr) Output::BuildHexValue(addr, Output::DML_Assembly).GetPtr()
#define DMLThreadID(id) Output::BuildHexValue(id, Output::DML_ThreadID, false).GetPtr()
#define DMLValueClass(mt, addr) Output::BuildVCValue(mt, addr, Output::DML_ValueClass).GetPtr()
#define DMLRCWrapper(addr) Output::BuildHexValue(addr, Output::DML_RCWrapper).GetPtr()
#define DMLCCWrapper(addr) Output::BuildHexValue(addr, Output::DML_CCWrapper).GetPtr()
#define DMLManagedVar(expansionName,frame,simpleName) Output::BuildManagedVarValue(expansionName, frame, simpleName, Output::DML_ManagedVar).GetPtr()

bool IsDMLEnabled();


#ifndef SOS_Assert
#define SOS_Assert(x)
#endif

void ConvertToLower(__out_ecount(len) char *buffer, size_t len);

extern const char * const DMLFormats[];
int GetHex(CLRDATA_ADDRESS addr, __out_ecount(len) char *out, size_t len, bool fill);

// A simple string class for mutable strings.  We cannot use STL, so this is a stand in replacement
// for std::string (though it doesn't use the same interface).
template <class T, size_t (__cdecl *LEN)(const T *), errno_t (__cdecl *COPY)(T *, size_t, const T * _Src)>
class BaseString
{
public:
    BaseString()
        : mStr(0), mSize(0), mLength(0)
    {
        const size_t size = 64;
        
        mStr = new T[size];
        mSize = size;
        mStr[0] = 0;
    }

    BaseString(const T *str)
        : mStr(0), mSize(0), mLength(0)
    {
        CopyFrom(str, LEN(str));
    }

    BaseString(const BaseString<T, LEN, COPY> &rhs)
        : mStr(0), mSize(0), mLength(0)
    {
        *this = rhs;
    }

    ~BaseString()
    {
        Clear();
    }

    const BaseString<T, LEN, COPY> &operator=(const BaseString<T, LEN, COPY> &rhs)
    {
        Clear();
        CopyFrom(rhs.mStr, rhs.mLength);
        return *this;
    }

    const BaseString<T, LEN, COPY> &operator=(const T *str)
    {
        Clear();
        CopyFrom(str, LEN(str));
        return *this;
    }

    const BaseString<T, LEN, COPY> &operator +=(const T *str)
    {
        size_t len = LEN(str);
        CopyFrom(str, len);
        return *this;
    }

    const BaseString<T, LEN, COPY> &operator +=(const BaseString<T, LEN, COPY> &str)
    {
        CopyFrom(str.mStr, str.mLength);
        return *this;
    }

    BaseString<T, LEN, COPY> operator+(const T *str) const
    {
        return BaseString<T, LEN, COPY>(mStr, mLength, str, LEN(str));
    }

    BaseString<T, LEN, COPY> operator+(const BaseString<T, LEN, COPY> &str) const
    {
        return BaseString<T, LEN, COPY>(mStr, mLength, str.mStr, str.mLength);
    }

    operator const T *() const
    {
        return mStr;
    }
    
    const T *c_str() const
    {
        return mStr;
    }

    size_t GetLength() const
    {
        return mLength;
    }

private:
    BaseString(const T * str1, size_t len1, const T * str2, size_t len2)
    : mStr(0), mSize(0), mLength(0)
    {
        const size_t size = len1 + len2 + 1 + ((len1 + len2) >> 1);
        mStr = new T[size];
        mSize = size;
        
        CopyFrom(str1, len1);
        CopyFrom(str2, len2);
    }
    
    void Clear()
    {
        mLength = 0;
        mSize = 0;
        if (mStr)
        {
            delete [] mStr;
            mStr = 0;
        }
    }

    void CopyFrom(const T *str, size_t len)
    {
        if (mLength + len + 1 >= mSize)
            Resize(mLength + len + 1);

        COPY(mStr+mLength, mSize-mLength, str);
        mLength += len;
    }

    void Resize(size_t size)
    {
        /* We always resize at least one half bigger than we need.  When CopyFrom requests a resize
         * it asks for the exact size that's needed to concatenate strings.  However in practice
         * it's common to add multiple strings together in a row, e.g.:
         *    String foo = "One " + "Two " + "Three " + "Four " + "\n";
         * Ensuring the size of the string is bigger than we need, and that the minimum size is 64,
         * we will cut down on a lot of needless resizes at the cost of a few bytes wasted in some
         * cases.
         */
        size += size >> 1;
        if (size < 64)
            size = 64;

        T *newStr = new T[size];

        if (mStr)
        {
            COPY(newStr, size, mStr);
            delete [] mStr;
        }
        else
        {
            newStr[0] = 0;
        }
        
        mStr = newStr;
        mSize = size;
    }
private:
    T *mStr;
    size_t mSize, mLength;
};

typedef BaseString<char, strlen, strcpy_s> String;
typedef BaseString<wchar_t, wcslen, wcscpy_s> WString;


template<class T>
void Flatten(__out_ecount(len) T *data, unsigned int len)
{
    for (unsigned int i = 0; i < len; ++i)
        if (data[i] < 32 || (data[i] > 126 && data[i] <= 255))
            data[i] = '.';
    data[len] = 0;
}

void Flatten(__out_ecount(len) char *data, unsigned int len);

/* Formats for the Format class.  We support the following formats:
 *      Pointer - Same as %p.
 *      Hex - Same as %x (same as %p, but does not output preceding zeros.
 *      PrefixHex - Same as %x, but prepends 0x.
 *      Decimal - Same as %d.
 * Strings and wide strings don't use this.
 */
class Formats
{
public:
    enum Format
    {
        Default,
        Pointer,
        Hex,
        PrefixHex,
        Decimal,
    };
};

enum Alignment
{
    AlignLeft,
    AlignRight
};

namespace Output
{
    /* Defines how a value will be printed.  This class understands how to format
     * and print values according to the format and DML settings provided.
     * The raw templated class handles the pointer/integer case.  Support for
     * character arrays and wide character arrays are handled by template
     * specializations.
     *
     * Note that this class is not used directly.  Instead use the typedefs and
     * macros which define the type of data you are outputing (for example ObjectPtr,
     * MethodTablePtr, etc).
     */
    template <class T>
    class Format
    {
    public:
        Format(T value)
            : mValue(value), mFormat(Formats::Default), mDml(Output::DML_None)
        {
        }

        Format(T value, Formats::Format format, Output::FormatType dmlType)
            : mValue(value), mFormat(format), mDml(dmlType)
        {
        }

        Format(const Format<T> &rhs)
            : mValue(rhs.mValue), mFormat(rhs.mFormat), mDml(rhs.mDml)
        {
        }

        /* Prints out the value according to the Format and DML settings provided.
         */
        void Output() const
        {
            if (IsDMLEnabled() && mDml != Output::DML_None)
            {
                const int len = GetDMLWidth(mDml);
                char *buffer = (char*)alloca(len);
            
                BuildDML(buffer, len, (CLRDATA_ADDRESS)mValue, mFormat, mDml);
                DMLOut(buffer);
            }
            else
            {
                if (mFormat == Formats::Default || mFormat == Formats::Pointer)
                {
                    ExtOut("%p", (__int64)mValue);
                }
                else
                {
                    const char *format = NULL;
                    if (mFormat == Formats::PrefixHex)
                    {
                        format = "0x%x";
                    }
                    else if (mFormat == Formats::Hex)
                    {
                        format = "%x";
                    }
                    else if (mFormat == Formats::Decimal)
                    {
                        format = "%d";
                    }

                    ExtOut(format, (__int32)mValue);
                }

            }
        }

        /* Prints out the value based on a specified width and alignment.
         * Params:
         *   align - Whether the output should be left or right justified.
         *   width - The output width to fill.
         * Note:
         *   This function guarantees that exactly width will be printed out (so if width is 24,
         *   exactly 24 characters will be printed), even if the output wouldn't normally fit
         *   in the space provided.  This function makes no guarantees as to what part of the
         *   data will be printed in the case that width isn't wide enough.
         */
        void OutputColumn(Alignment align, int width) const
        {
            bool leftAlign = align == AlignLeft;
            if (IsDMLEnabled() && mDml != Output::DML_None)
            {
                const int len = GetDMLColWidth(mDml, width);
                char *buffer = (char*)alloca(len);
            
                BuildDMLCol(buffer, len, (CLRDATA_ADDRESS)mValue, mFormat, mDml, leftAlign, width);
                DMLOut(buffer);
            }
            else
            {
                int precision = GetPrecision();
                if (mFormat == Formats::Default || mFormat == Formats::Pointer)
                {
                    if (precision > width)
                        precision = width;

                    ExtOut(leftAlign ? "%-*.*p" : "%*.*p", width, precision, (__int64)mValue);
                }
                else
                {
                    const char *format = NULL;
                    if (mFormat == Formats::PrefixHex)
                    {
                        format = leftAlign ? "0x%-*.*x" : "0x%*.*x";
                        width -= 2;
                    }
                    else if (mFormat == Formats::Hex)
                    {
                        format = leftAlign ? "%-*.*x" : "%*.*x";
                    }
                    else if (mFormat == Formats::Decimal)
                    {
                        format = leftAlign ? "%-*.*d" : "%*.*d";
                    }

                    if (precision > width)
                        precision = width;

                    ExtOut(format, width, precision, (__int32)mValue);
                }
            }
        }
    
        /* Converts this object into a Wide char string.  This allows you to write the following code:
         *    WString foo = L"bar " + ObjectPtr(obj);
         * Where ObjectPtr is a subclass/typedef of this Format class.
         */
        operator WString() const
        {
            String str = *this;
            const char *cstr = (const char *)str;
        
            int len = MultiByteToWideChar(CP_ACP, 0, cstr, -1, NULL, 0);
            wchar_t *buffer = (wchar_t *)alloca(len*sizeof(wchar_t));
        
            MultiByteToWideChar(CP_ACP, 0, cstr, -1, buffer, len);
        
            return WString(buffer);
        }
    
        /* Converts this object into a String object.  This allows you to write the following code:
         *    String foo = "bar " + ObjectPtr(obj);
         * Where ObjectPtr is a subclass/typedef of this Format class.
         */
        operator String() const
        {
            if (IsDMLEnabled() && mDml != Output::DML_None)
            {
                const int len = GetDMLColWidth(mDml, 0);
                char *buffer = (char*)alloca(len);
            
                BuildDMLCol(buffer, len, (CLRDATA_ADDRESS)mValue, mFormat, mDml, false, 0);
                return buffer;
            }
            else
            {
                char buffer[64];
                if (mFormat == Formats::Default || mFormat == Formats::Pointer)
                {
                    sprintf_s(buffer, _countof(buffer), "%p", (int *)mValue);
                    ConvertToLower(buffer, _countof(buffer));
                }
                else
                {
                    const char *format = NULL;
                    if (mFormat == Formats::PrefixHex)
                        format = "0x%x";
                    else if (mFormat == Formats::Hex)
                        format = "%x";
                    else if (mFormat == Formats::Decimal)
                        format = "%d";

                    sprintf_s(buffer, _countof(buffer), format, (__int32)mValue);
                    ConvertToLower(buffer, _countof(buffer));
                }
                
                return buffer;
            }
        }

    private:
        int GetPrecision() const
        {
            if (mFormat == Formats::Hex || mFormat == Formats::PrefixHex)
            {
                ULONGLONG val = mValue;
                int count = 0;
                while (val)
                {
                    val >>= 4;
                    count++;
                }

                if (count == 0)
                    count = 1;

                return count;
            }

            else if (mFormat == Formats::Decimal)
            {
                T val = mValue;
                int count = (val > 0) ? 0 : 1;
                while (val)
                {
                    val /= 10;
                    count++;
                }

                return count;
            }

            // mFormat == Formats::Pointer
            return sizeof(int*)*2;
        }

        static inline void BuildDML(__out_ecount(len) char *result, int len, CLRDATA_ADDRESS value, Formats::Format format, Output::FormatType dmlType)
        {
            BuildDMLCol(result, len, value, format, dmlType, true, 0);
        }
    
        static int GetDMLWidth(Output::FormatType dmlType)
        {
            return GetDMLColWidth(dmlType, 0);
        }
    
        static void BuildDMLCol(__out_ecount(len) char *result, int len, CLRDATA_ADDRESS value, Formats::Format format, Output::FormatType dmlType, bool leftAlign, int width)
        {
            char hex[64];
            int count = GetHex(value, hex, _countof(hex), format != Formats::Hex);
            int i = 0;
    
            if (!leftAlign)
            {
                for (; i < width - count; ++i)
                    result[i] = ' ';
        
                result[i] = 0;
            }
    
            int written = sprintf_s(result+i, len - i, DMLFormats[dmlType], hex, hex);
    
            SOS_Assert(written != -1);
            if (written != -1)
            {
                for (i = i + written; i < width; ++i)
                    result[i] = ' ';
        
                result[i] = 0;
            }
        }
    
        static int GetDMLColWidth(Output::FormatType dmlType, int width)
        {
            return 1 + 4*sizeof(int*) + (int)strlen(DMLFormats[dmlType]) + width;
        }

    private:
        T mValue;
        Formats::Format mFormat;
        Output::FormatType mDml;
     };

     /* Format class used for strings.
      */
    template <>
    class Format<const char *>
    {
    public:
        Format(const char *value)
            : mValue(value)
        {
        }

        Format(const Format<const char *> &rhs)
            : mValue(rhs.mValue)
        {
        }

        void Output() const
        {
            if (IsDMLEnabled())
                DMLOut("%s", mValue);
            else
                ExtOut("%s", mValue);
        }

        void OutputColumn(Alignment align, int width) const
        {
            int precision = (int)strlen(mValue);

            if (precision > width)
                precision = width;

            const char *format = align == AlignLeft ? "%-*.*s" : "%*.*s";
        
            if (IsDMLEnabled())
                DMLOut(format, width, precision, mValue);
            else
                ExtOut(format, width, precision, mValue);
        }

    private:
        const char *mValue;
    };

    /* Format class for wide char strings.
     */
    template <>
    class Format<const wchar_t *>
    {
    public:
        Format(const wchar_t *value)
            : mValue(value)
        {
        }

        Format(const Format<const wchar_t *> &rhs)
            : mValue(rhs.mValue)
        {
        }
    
        void Output() const
        {
            if (IsDMLEnabled())
                DMLOut("%S", mValue);
            else
                ExtOut("%S", mValue);
        }

        void OutputColumn(Alignment align, int width) const
        {
            int precision = (int)wcslen(mValue);
            if (precision > width)
                precision = width;

            const char *format = align == AlignLeft ? "%-*.*S" : "%*.*S";

            if (IsDMLEnabled())
                DMLOut(format, width, precision, mValue);
            else
                ExtOut(format, width, precision, mValue);
        }

    private:
        const wchar_t *mValue;
    };


    template <class T>
    void InternalPrint(const T &t)
    {
        Format<T>(t).Output();
    }

    template <class T>
    void InternalPrint(const Format<T> &t)
    {
        t.Output();
    }

    inline void InternalPrint(const char t[])
    {
        Format<const char *>(t).Output();
    }
}

#define DefineFormatClass(name, format, dml) \
    template <class T>                       \
    Output::Format<T> name(T value)          \
    { return Output::Format<T>(value, format, dml); }

DefineFormatClass(EEClassPtr, Formats::Pointer, Output::DML_EEClass);
DefineFormatClass(ObjectPtr, Formats::Pointer, Output::DML_Object);
DefineFormatClass(ExceptionPtr, Formats::Pointer, Output::DML_PrintException);
DefineFormatClass(ModulePtr, Formats::Pointer, Output::DML_Module);
DefineFormatClass(MethodDescPtr, Formats::Pointer, Output::DML_MethodDesc);
DefineFormatClass(AppDomainPtr, Formats::Pointer, Output::DML_Domain);
DefineFormatClass(ThreadState, Formats::Hex, Output::DML_ThreadState);
DefineFormatClass(ThreadID, Formats::Hex, Output::DML_ThreadID);
DefineFormatClass(RCWrapper, Formats::Pointer, Output::DML_RCWrapper);
DefineFormatClass(CCWrapper, Formats::Pointer, Output::DML_CCWrapper);
DefineFormatClass(InstructionPtr, Formats::Pointer, Output::DML_IP);

DefineFormatClass(Decimal, Formats::Decimal, Output::DML_None);
DefineFormatClass(Pointer, Formats::Pointer, Output::DML_None);
DefineFormatClass(PrefixHex, Formats::PrefixHex, Output::DML_None);
DefineFormatClass(Hex, Formats::Hex, Output::DML_None);

#undef DefineFormatClass

template <class T0>
void Print(const T0 &val0)
{
    Output::InternalPrint(val0);
}

template <class T0, class T1>
void Print(const T0 &val0, const T1 &val1)
{
    Output::InternalPrint(val0);
    Output::InternalPrint(val1);
}

template <class T0>
void PrintLn(const T0 &val0)
{
    Output::InternalPrint(val0);
    ExtOut("\n");
}

template <class T0, class T1>
void PrintLn(const T0 &val0, const T1 &val1)
{
    Output::InternalPrint(val0);
    Output::InternalPrint(val1);
    ExtOut("\n");
}

template <class T0, class T1, class T2>
void PrintLn(const T0 &val0, const T1 &val1, const T2 &val2)
{
    Output::InternalPrint(val0);
    Output::InternalPrint(val1);
    Output::InternalPrint(val2);
    ExtOut("\n");
}


/* This class handles the formatting for output which is in a table format.  To use this class you define
 * how the table is formatted by setting the number of columns in the table, the default column width,
 * the default column alignment, the indentation (whitespace) for the table, and the amount of padding
 * (whitespace) between each column. Once this has been setup, you output rows at a time or individual
 * columns to build the output instead of manually tabbing out space.
 * Also note that this class was built to work with the Format class.  When outputing data, use the
 * predefined output types to specify the format (such as ObjectPtr, MethodDescPtr, Decimal, etc).  This
 * tells the TableOutput class how to display the data, and where applicable, it automatically generates
 * the appropriate DML output.  See the DefineFormatClass macro.
 */
class TableOutput
{
public:

    TableOutput()
        : mColumns(0), mDefaultWidth(0), mIndent(0), mPadding(0), mCurrCol(0), mDefaultAlign(AlignLeft),
          mWidths(0), mAlignments(0)
      {
      }
    /* Constructor.
     * Params:
     *   numColumns - the number of columns the table has
     *   defaultColumnWidth - the default width of each column
     *   alignmentDefault - whether columns are by default left aligned or right aligned
     *   indent - the amount of whitespace to prefix at the start of the row (in characters)
     *   padding - the amount of whitespace to place between each column (in characters)
     */
    TableOutput(int numColumns, int defaultColumnWidth, Alignment alignmentDefault = AlignLeft, int indent = 0, int padding = 1)
        : mColumns(numColumns), mDefaultWidth(defaultColumnWidth), mIndent(indent), mPadding(padding), mCurrCol(0), mDefaultAlign(alignmentDefault),
          mWidths(0), mAlignments(0)
    {
    }

    ~TableOutput()
    {
        Clear();
    }

    /* See the documentation for the constructor.
     */
    void ReInit(int numColumns, int defaultColumnWidth, Alignment alignmentDefault = AlignLeft, int indent = 0, int padding = 1);

    /* Sets the amount of whitespace to prefix at the start of the row (in characters). 
     */
    void SetIndent(int indent)
    {
        SOS_Assert(indent >= 0);

        mIndent = indent;
    }

    /* Sets the exact widths for the the given columns.
     * Params:
     *   columns - the number of columns you are providing the width for, starting at the first column
     *   ... - an int32 for each column (given by the number of columns in the first parameter).
     * Example:
     *    If you have 5 columns in the table, you can set their widths like so:
     *       tableOutput.SetWidths(5, 2, 3, 5, 7, 13);
     * Note:
     *    It's fine to pass a value for "columns" less than the number of columns in the table.  This
     *    is useful when you set the default column width to be correct for most of the table, and need
     *    to make a minor adjustment to a few.
     */
    void SetWidths(int columns, ...);

    /* Individually sets a column to the given width.
     * Params:
     *   col - the column to set, 0 indexed
     *   width - the width of the column (note this must be non-negative)
     */
    void SetColWidth(int col, int width);

    /* Individually sets the column alignment.
     * Params:
     *   col - the column to set, 0 indexed
     *   align - the new alignment (left or right) for the column
     */
    void SetColAlignment(int col, Alignment align);

    
    /* The WriteRow family of functions allows you to write an entire row of the table at once.
     * The common use case for the TableOutput class is to individually output each column after
     * calculating what the value should contain.  However, this would be tedious if you already
     * knew the contents of the entire row which usually happenes when you are printing out the
     * header for the table.  To use this, simply pass each column as an individual parameter,
     * for example:
     *    tableOutput.WriteRow("First Column", "Second Column", Decimal(3), PrefixHex(4), "Fifth Column");
     */
    template <class T0, class T1>
    void WriteRow(T0 t0, T1 t1)
    {
        WriteColumn(0, t0);
        WriteColumn(1, t1);
    }

    template <class T0, class T1, class T2>
    void WriteRow(T0 t0, T1 t1, T2 t2)
    {
        WriteColumn(0, t0);
        WriteColumn(1, t1);
        WriteColumn(2, t2);
    }


    template <class T0, class T1, class T2, class T3>
    void WriteRow(T0 t0, T1 t1, T2 t2, T3 t3)
    {
        WriteColumn(0, t0);
        WriteColumn(1, t1);
        WriteColumn(2, t2);
        WriteColumn(3, t3);
    }

    
    template <class T0, class T1, class T2, class T3, class T4>
    void WriteRow(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4)
    {
        WriteColumn(0, t0);
        WriteColumn(1, t1);
        WriteColumn(2, t2);
        WriteColumn(3, t3);
        WriteColumn(4, t4);
    }
    
    template <class T0, class T1, class T2, class T3, class T4, class T5>
    void WriteRow(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
    {
        WriteColumn(0, t0);
        WriteColumn(1, t1);
        WriteColumn(2, t2);
        WriteColumn(3, t3);
        WriteColumn(4, t4);
        WriteColumn(5, t5);
    }

    template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
    void WriteRow(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
    {
        WriteColumn(0, t0);
        WriteColumn(1, t1);
        WriteColumn(2, t2);
        WriteColumn(3, t3);
        WriteColumn(4, t4);
        WriteColumn(5, t5);
        WriteColumn(6, t6);
        WriteColumn(7, t7);
        WriteColumn(8, t8);
        WriteColumn(9, t9);
    }

    /* The WriteColumn family of functions is used to output individual columns in the table.
     * The intent is that the bulk of the table will be generated in a loop like so:
     *   while (condition) {
     *      int value1 = CalculateFirstColumn();
     *      table.WriteColumn(0, value1);
     *
     *      String value2 = CalculateSecondColumn();
     *      table.WriteColumn(1, value2);
     *   }
     * Params:
     *   col - the column to write, 0 indexed
     *   t - the value to write
     * Note:
     *   You should generally use the specific instances of the Format class to generate output.
     *   For example, use the "Decimal", "Pointer", "ObjectPtr", etc.  When passing data to this
     *   function.  This tells the Table class how to display the value.
     */
    template <class T>
    void WriteColumn(int col, const Output::Format<T> &t)
    {
        SOS_Assert(col >= 0);
        SOS_Assert(col < mColumns);

        if (col != mCurrCol)
            OutputBlankColumns(col);

        if (col == 0)
            OutputIndent();
        
        bool lastCol = col == mColumns - 1;

        if (!lastCol)
            t.OutputColumn(GetColAlign(col), GetColumnWidth(col));
        else
            t.Output();

        ExtOut(lastCol ? "\n" : GetWhitespace(mPadding));

        if (lastCol)
            mCurrCol = 0;
        else
            mCurrCol = col+1;
    }
    
    template <class T>
    void WriteColumn(int col, T t)
    {
        WriteColumn(col, Output::Format<T>(t));
    }

    void WriteColumn(int col, const String &str)
    {
        WriteColumn(col, Output::Format<const char *>(str));
    }

    void WriteColumn(int col, const WString &str)
    {
        WriteColumn(col, Output::Format<const wchar_t *>(str));
    }

    void WriteColumn(int col, __in_z WCHAR *str)
    {
        WriteColumn(col, Output::Format<const wchar_t *>(str));
    }

    void WriteColumn(int col, const WCHAR *str)
    {
        WriteColumn(col, Output::Format<const wchar_t *>(str));
    }
    
    inline void WriteColumn(int col, __in_z char *str)
    {
        WriteColumn(col, Output::Format<const char *>(str));
    }

    /* Writes a column using a printf style format.  You cannot use the Format class with
     * this function to specify how the output should look, use printf style formatting
     * with the appropriate parameters instead.
     */
    void WriteColumnFormat(int col, const char *fmt, ...)
    {
        SOS_Assert(strstr(fmt, "%S") == NULL);

        char result[128];
        
        va_list list;
        va_start(list, fmt);
        vsprintf_s(result, _countof(result), fmt, list);
        va_end(list);

        WriteColumn(col, result);
    }
    
    void WriteColumnFormat(int col, const wchar_t *fmt, ...)
    {
        wchar_t result[128];
        
        va_list list;
        va_start(list, fmt);
        vswprintf_s(result, _countof(result), fmt, list);
        va_end(list);

        WriteColumn(col, result);
    }

    /* This function is a shortcut for writing the next column.  (That is, the one after the
     * one you just wrote.)
     */
    template <class T>
    void WriteColumn(T t)
    {
        WriteColumn(mCurrCol, t);
    }

private:
    void Clear();
    void AllocWidths();
    int GetColumnWidth(int col);
    Alignment GetColAlign(int col);
    const char *GetWhitespace(int amount);
    void OutputBlankColumns(int col);
    void OutputIndent();

private:
    int mColumns, mDefaultWidth, mIndent, mPadding, mCurrCol;
    Alignment mDefaultAlign;
    int *mWidths;
    Alignment *mAlignments;
};

HRESULT GetMethodDefinitionsFromName(DWORD_PTR ModulePtr, IXCLRDataModule* mod, const char* name, IXCLRDataMethodDefinition **ppMethodDefinitions, int numMethods, int *numMethodsNeeded);
HRESULT GetMethodDescsFromName(DWORD_PTR ModulePtr, IXCLRDataModule* mod, const char* name, DWORD_PTR **pOut, int *numMethodDescs);

HRESULT FileNameForModule (DacpModuleData *pModule, __out_ecount (MAX_PATH) WCHAR *fileName);
HRESULT FileNameForModule (DWORD_PTR pModuleAddr, __out_ecount (MAX_PATH) WCHAR *fileName);
void IP2MethodDesc (DWORD_PTR IP, DWORD_PTR &methodDesc, JITTypes &jitType,
                    DWORD_PTR &gcinfoAddr);
const char *ElementTypeName (unsigned type);
void DisplayFields (CLRDATA_ADDRESS cdaMT, DacpMethodTableData *pMTD, DacpMethodTableFieldData *pMTFD,
                    DWORD_PTR dwStartAddr = 0, BOOL bFirst=TRUE, BOOL bValueClass=FALSE);
int GetObjFieldOffset(CLRDATA_ADDRESS cdaObj, __in_z LPWSTR wszFieldName, BOOL bFirst=TRUE);
int GetObjFieldOffset(CLRDATA_ADDRESS cdaObj, CLRDATA_ADDRESS cdaMT, __in_z LPWSTR wszFieldName, BOOL bFirst=TRUE);

BOOL IsValidToken(DWORD_PTR ModuleAddr, mdTypeDef mb);
void NameForToken_s(DacpModuleData *pModule, mdTypeDef mb, __out_ecount (capacity_mdName) WCHAR *mdName, size_t capacity_mdName, 
                  bool bClassName=true);
void NameForToken_s(DWORD_PTR ModuleAddr, mdTypeDef mb, __out_ecount (capacity_mdName) WCHAR *mdName, size_t capacity_mdName, 
                  bool bClassName=true);
HRESULT NameForToken_s(mdTypeDef mb, IMetaDataImport *pImport, __out_ecount (capacity_mdName) WCHAR *mdName,  size_t capacity_mdName, 
                     bool bClassName);
HRESULT NameForTokenNew_s(mdTypeDef mb, IMDInternalImport *pImport, __out_ecount (capacity_mdName) WCHAR *mdName,  size_t capacity_mdName, 
                     bool bClassName);

void vmmap();
void vmstat();

#ifndef FEATURE_PAL
///////////////////////////////////////////////////////////////////////////////////////////////////
// Support for managed stack tracing
//

DWORD_PTR GetDebuggerJitInfo(DWORD_PTR md);

///////////////////////////////////////////////////////////////////////////////////////////////////
#endif // FEATURE_PAL

template <typename SCALAR>
inline
int bitidx(SCALAR bitflag)
{
    for (int idx = 0; idx < static_cast<int>(sizeof(bitflag))*8; ++idx)
    {
        if (bitflag & (1 << idx))
        {
            _ASSERTE((bitflag & (~(1 << idx))) == 0);
            return idx;
        }
    }
    return -1;
}

HRESULT
DllsName(
    ULONG_PTR addrContaining,
    __out_ecount (MAX_PATH) WCHAR *dllName
    );

inline
BOOL IsElementValueType (CorElementType cet)
{
    return cet >= ELEMENT_TYPE_BOOLEAN && cet <= ELEMENT_TYPE_R8 
        || cet == ELEMENT_TYPE_VALUETYPE || cet == ELEMENT_TYPE_I || cet == ELEMENT_TYPE_U;
}


#define safemove(dst, src) \
SafeReadMemory (TO_TADDR(src), &(dst), sizeof(dst), NULL)

extern "C" PDEBUG_DATA_SPACES g_ExtData;

template <class T>
class ArrayHolder    
{
public:
    ArrayHolder(T *ptr)
        : mPtr(ptr)
    {
    }

    ~ArrayHolder()
    {
        Clear();
    }
    
    ArrayHolder(const ArrayHolder &rhs)
    {
        mPtr = const_cast<ArrayHolder *>(&rhs)->Detach();
    }

    ArrayHolder &operator=(T *ptr)
    {
        Clear();
        mPtr = ptr;
        return *this;
    }

    const T &operator[](int i) const
    {
        return mPtr[i];
    }

    T &operator[](int i)
    {
        return mPtr[i];
    }

    operator const T *() const
    {
        return mPtr;
    }

    operator T *()
    {
        return mPtr;
    }

    T **operator&()
    {
        return &mPtr;
    }

    T *GetPtr()
    {
        return mPtr;
    }

    T *Detach()
    {
        T *ret = mPtr;
        mPtr = NULL;
        return ret;
    }

private:
    void Clear()
    {
        if (mPtr)
        {
            delete [] mPtr;
            mPtr = NULL;
        }
    }

private:
    T *mPtr;
};



// This class acts a smart pointer which calls the Release method on any object
// you place in it when the ToRelease class falls out of scope.  You may use it
// just like you would a standard pointer to a COM object (including if (foo),
// if (!foo), if (foo == 0), etc) except for two caveats:
//     1. This class never calls AddRef and it always calls Release when it
//        goes out of scope.
//     2. You should never use & to try to get a pointer to a pointer unless
//        you call Release first, or you will leak whatever this object contains
//        prior to updating its internal pointer.
template<class T>
class ToRelease
{
public:
    ToRelease()
        : m_ptr(NULL)
    {}
    
    ToRelease(T* ptr)
        : m_ptr(ptr)
    {}
    
    ~ToRelease()
    {
        Release();
    }

    void operator=(T *ptr)
    {
        Release();

        m_ptr = ptr;
    }

    T* operator->()
    {
        return m_ptr;
    }

    operator T*()
    {
        return m_ptr;
    }

    T** operator&()
    {
        return &m_ptr;
    }

    T* GetPtr() const
    {
        return m_ptr;
    }

    T* Detach()
    {
        T* pT = m_ptr;
        m_ptr = NULL;
        return pT;
    }
    
    void Release()
    {
        if (m_ptr != NULL)
        {
            m_ptr->Release();
            m_ptr = NULL;
        }
    }

private:
    T* m_ptr;    
};

struct ModuleInfo
{
    ULONG64 baseAddr;
    ULONG64 size;
    BOOL hasPdb;
};
extern ModuleInfo moduleInfo[];

BOOL InitializeHeapData();
BOOL IsServerBuild ();
UINT GetMaxGeneration();
UINT GetGcHeapCount();
BOOL GetGcStructuresValid();

ULONG GetILSize(DWORD_PTR ilAddr); // REturns 0 if error occurs
HRESULT DecodeILFromAddress(IMetaDataImport *pImport, TADDR ilAddr);
void DecodeIL(IMetaDataImport *pImport, BYTE *buffer, ULONG bufSize);
void DecodeDynamicIL(BYTE *data, ULONG Size, DacpObjectData& tokenArray);

BOOL IsRetailBuild (size_t base);
EEFLAVOR GetEEFlavor ();
#ifndef FEATURE_PAL
HRESULT InitCorDebugInterface();
VOID UninitCorDebugInterface();
BOOL GetEEVersion(VS_FIXEDFILEINFO *pFileInfo);
BOOL GetSOSVersion(VS_FIXEDFILEINFO *pFileInfo);
#endif

BOOL IsDumpFile ();

// IsMiniDumpFile will return true if 1) we are in
// a small format minidump, and g_InMinidumpSafeMode is true.
extern BOOL g_InMinidumpSafeMode;

BOOL IsMiniDumpFile();
void ReportOOM();

BOOL SafeReadMemory (TADDR offset, PVOID lpBuffer, ULONG cb, PULONG lpcbBytesRead);
#if !defined(_TARGET_WIN64_)
// on 64-bit platforms TADDR and CLRDATA_ADDRESS are identical
inline BOOL SafeReadMemory (CLRDATA_ADDRESS offset, PVOID lpBuffer, ULONG cb, PULONG lpcbBytesRead)
{ return SafeReadMemory(TO_TADDR(offset), lpBuffer, cb, lpcbBytesRead); }
#endif

BOOL NameForMD_s (DWORD_PTR pMD, __out_ecount (capacity_mdName) WCHAR *mdName, size_t capacity_mdName);
BOOL NameForMT_s (DWORD_PTR MTAddr, __out_ecount (capacity_mdName) WCHAR *mdName, size_t capacity_mdName);

wchar_t *CreateMethodTableName(TADDR mt, TADDR cmt = NULL);

void isRetAddr(DWORD_PTR retAddr, DWORD_PTR* whereCalled);
DWORD_PTR GetValueFromExpression (__in __in_z const char *const str);

#ifndef FEATURE_PAL
// ensure we always allocate on the process heap
FORCEINLINE void* __cdecl operator new(size_t size) throw()
{ return HeapAlloc(GetProcessHeap(), 0, size); }
FORCEINLINE void __cdecl operator delete(void* pObj) throw()
{ HeapFree(GetProcessHeap(), 0, pObj); }

FORCEINLINE void* __cdecl operator new[](size_t size) throw()
{ return HeapAlloc(GetProcessHeap(), 0, size); }
FORCEINLINE void __cdecl operator delete[](void* pObj) throw()
{ HeapFree(GetProcessHeap(), 0, pObj); }
#endif

enum ModuleHeapType
{
    ModuleHeapType_ThunkHeap,
    ModuleHeapType_LookupTableHeap
};

HRESULT PrintDomainHeapInfo(const char *name, CLRDATA_ADDRESS adPtr, DWORD_PTR *size, DWORD_PTR *wasted = 0);
DWORD_PTR PrintModuleHeapInfo(DWORD_PTR *moduleList, int count, ModuleHeapType type, DWORD_PTR *wasted = 0);
void PrintHeapSize(DWORD_PTR total, DWORD_PTR wasted);
void DomainInfo(DacpAppDomainData *pDomain);
void AssemblyInfo(DacpAssemblyData *pAssembly);
DWORD_PTR LoaderHeapInfo(CLRDATA_ADDRESS pLoaderHeapAddr, DWORD_PTR *wasted = 0);
DWORD_PTR JitHeapInfo();
DWORD_PTR VSDHeapInfo(CLRDATA_ADDRESS appDomain, DWORD_PTR *wasted = 0);

DWORD GetNumComponents(TADDR obj);

struct GenUsageStat
{
    size_t allocd;
    size_t freed;
    size_t unrooted;
};

struct HeapUsageStat
{
    GenUsageStat  genUsage[4]; // gen0, 1, 2, LOH
};

extern DacpUsefulGlobalsData g_special_usefulGlobals;
BOOL GCHeapUsageStats(const DacpGcHeapDetails& heap, BOOL bIncUnreachable, HeapUsageStat *hpUsage);

class HeapStat
{
protected:
    struct Node
    {
        DWORD_PTR data;
        DWORD count;
        size_t totalSize;
        Node* left;
        Node* right;
        Node ()
            : data(0), count(0), totalSize(0), left(NULL), right(NULL)
        {
        }
    };
    BOOL bHasStrings;
    Node *head;
    BOOL fLinear;
public:
    HeapStat ()
        : bHasStrings(FALSE), head(NULL), fLinear(FALSE)
    {}
    ~HeapStat()
    {
        Delete();
    }
    // TODO: Change the aSize argument to size_t when we start supporting
    // TODO: object sizes above 4GB
    void Add (DWORD_PTR aData, DWORD aSize);
    void Sort ();
    void Print (const char* label = NULL);
    void Delete ();
    void HasStrings(BOOL abHasStrings)
        {
            bHasStrings = abHasStrings;
        }
private:
    int CompareData(DWORD_PTR n1, DWORD_PTR n2);
    void SortAdd (Node *&root, Node *entry);
    void LinearAdd (Node *&root, Node *entry);
    void ReverseLeftMost (Node *root);
    void Linearize();
};

class CGCDesc;
// The information MethodTableCache returns.
struct MethodTableInfo
{
    bool IsInitialized()       { return BaseSize != 0; }

    DWORD BaseSize;           // Caching BaseSize and ComponentSize for a MethodTable
    DWORD ComponentSize;      // here has HUGE perf benefits in heap traversals.
    BOOL  bContainsPointers;
    DWORD_PTR* GCInfoBuffer;  // Start of memory of GC info
    CGCDesc* GCInfo;    // Just past GC info (which is how it is stored)
    bool  ArrayOfVC;
};

class MethodTableCache
{
protected:

    struct Node
    {
        DWORD_PTR data;            // This is the key (the method table pointer)
        MethodTableInfo info;  // The info associated with this MethodTable
        Node* left;
        Node* right;
        Node (DWORD_PTR aData) : data(aData), left(NULL), right(NULL)
        {
            info.BaseSize = 0;
            info.ComponentSize = 0;
            info.bContainsPointers = false;
            info.GCInfo = NULL;
            info.ArrayOfVC = false;
            info.GCInfoBuffer = NULL;
        }
    };
    Node *head;
public:
    MethodTableCache ()
        : head(NULL)
    {}
    ~MethodTableCache() { Clear(); }

    // Always succeeds, if it is not present it adds an empty Info struct and returns that
    // Thus you must call 'IsInitialized' on the returned value before using it
    MethodTableInfo* Lookup(DWORD_PTR aData);

    void Clear ();
private:
    int CompareData(DWORD_PTR n1, DWORD_PTR n2);    
    void ReverseLeftMost (Node *root);    
};

extern MethodTableCache g_special_mtCache;

struct DumpArrayFlags
{
    DWORD_PTR startIndex;
    DWORD_PTR Length;
    BOOL bDetail;
    LPSTR strObject;
    BOOL bNoFieldsForElement;
    
    DumpArrayFlags ()
        : startIndex(0), Length((DWORD_PTR)-1), bDetail(FALSE), strObject (0), bNoFieldsForElement(FALSE)
    {}
    ~DumpArrayFlags ()
    {
        if (strObject)
            delete [] strObject;
    }
}; //DumpArrayFlags



// -----------------------------------------------------------------------

#define BIT_SBLK_IS_HASH_OR_SYNCBLKINDEX    0x08000000
#define BIT_SBLK_FINALIZER_RUN              0x40000000
#define BIT_SBLK_SPIN_LOCK                  0x10000000
#define SBLK_MASK_LOCK_THREADID             0x000003FF   // special value of 0 + 1023 thread ids
#define SBLK_MASK_LOCK_RECLEVEL             0x0000FC00   // 64 recursion levels
#define SBLK_APPDOMAIN_SHIFT                16           // shift right this much to get appdomain index
#define SBLK_MASK_APPDOMAININDEX            0x000007FF   // 2048 appdomain indices
#define SBLK_RECLEVEL_SHIFT                 10           // shift right this much to get recursion level
#define BIT_SBLK_IS_HASHCODE            0x04000000
#define MASK_HASHCODE                   ((1<<HASHCODE_BITS)-1)
#define SYNCBLOCKINDEX_BITS             26
#define MASK_SYNCBLOCKINDEX             ((1<<SYNCBLOCKINDEX_BITS)-1)

HRESULT GetMTOfObject(TADDR obj, TADDR *mt);

struct needed_alloc_context 
{
    BYTE*   alloc_ptr;   // starting point for next allocation
    BYTE*   alloc_limit; // ending point for allocation region/quantum
};

struct AllocInfo
{
    needed_alloc_context *array;
    int num;                     // number of allocation contexts in array

    AllocInfo()
        : array(NULL)
        , num(0)
    {}
    void Init()
    {
        extern void GetAllocContextPtrs(AllocInfo *pallocInfo);
        GetAllocContextPtrs(this);
    }
    ~AllocInfo()
    { 
        if (array != NULL) 
            delete[] array; 
    }
};

struct GCHandleStatistics
{
    HeapStat hs;
    
    DWORD strongHandleCount;
    DWORD pinnedHandleCount;
    DWORD asyncPinnedHandleCount;
    DWORD refCntHandleCount;
    DWORD weakLongHandleCount;
    DWORD weakShortHandleCount;
    DWORD variableCount;
    DWORD sizedRefCount;
    DWORD dependentCount;
    DWORD weakWinRTHandleCount;
    DWORD unknownHandleCount;
    GCHandleStatistics()
        : strongHandleCount(0), pinnedHandleCount(0), asyncPinnedHandleCount(0), refCntHandleCount(0),
          weakLongHandleCount(0), weakShortHandleCount(0), variableCount(0), sizedRefCount(0),
          dependentCount(0), weakWinRTHandleCount(0), unknownHandleCount(0)
    {}
    ~GCHandleStatistics()
    {
        hs.Delete();
    }
};

struct SegmentLookup
{
    DacpHeapSegmentData *m_segments;
    int m_iSegmentsSize;
    int m_iSegmentCount;
        
    SegmentLookup();
    ~SegmentLookup();

    void Clear();
    BOOL AddSegment(DacpHeapSegmentData *pData);
    CLRDATA_ADDRESS GetHeap(CLRDATA_ADDRESS object, BOOL& bFound);
};

class GCHeapSnapshot
{
private:
    BOOL m_isBuilt;
    DacpGcHeapDetails *m_heapDetails;
    DacpGcHeapData m_gcheap;
    SegmentLookup m_segments;

    BOOL AddSegments(DacpGcHeapDetails& details);
public:
    GCHeapSnapshot();

    BOOL Build();
    void Clear();
    BOOL IsBuilt() { return m_isBuilt; }

    DacpGcHeapData *GetHeapData() { return &m_gcheap; }
    
    int GetHeapCount() { return m_gcheap.HeapCount; }    
    
    DacpGcHeapDetails *GetHeap(CLRDATA_ADDRESS objectPointer);
    int GetGeneration(CLRDATA_ADDRESS objectPointer);

    
};
extern GCHeapSnapshot g_snapshot;
    
BOOL IsSameModuleName (const char *str1, const char *str2);
BOOL IsModule (DWORD_PTR moduleAddr);
BOOL IsMethodDesc (DWORD_PTR value);
BOOL IsMethodTable (DWORD_PTR value);
BOOL IsStringObject (size_t obj);
BOOL IsObjectArray (DWORD_PTR objPointer);
BOOL IsObjectArray (DacpObjectData *pData);

/* Returns a list of all modules in the process.
 * Params:
 *      name - The name of the module you would like.  If mName is NULL the all modules are returned.
 *      numModules - The number of modules in the array returned.
 * Returns:
 *      An array of modules whose length is *numModules, NULL if an error occurred.  Note that if this
 *      function succeeds but finds no modules matching the name given, this function returns a valid
 *      array, but *numModules will equal 0.
 * Note:
 *      You must clean up the return value of this array by calling delete [] on it, or using the
 *      ArrayHolder class.
 */
DWORD_PTR *ModuleFromName(__in __in_z __in_opt LPSTR name, int *numModules);
void GetInfoFromName(DWORD_PTR ModuleAddr, const char* name);
void GetInfoFromModule (DWORD_PTR ModuleAddr, ULONG token, DWORD_PTR *ret=NULL);

    
typedef void (*VISITGCHEAPFUNC)(DWORD_PTR objAddr,size_t Size,DWORD_PTR methodTable,LPVOID token);
BOOL GCHeapsTraverse(VISITGCHEAPFUNC pFunc, LPVOID token, BOOL verify=true);

/////////////////////////////////////////////////////////////////////////////////////////////////////////

struct strobjInfo
{
    size_t  methodTable;
    DWORD   m_StringLength;
};

// Just to make figuring out which fill pointer element matches a generation
// a bit less confusing. This gen_segment function is ported from gc.cpp.
inline unsigned int gen_segment (int gen)
{
    return (DAC_NUMBERGENERATIONS - gen - 1);
}

inline CLRDATA_ADDRESS SegQueue(DacpGcHeapDetails& heapDetails, int seg)
{
    return heapDetails.finalization_fill_pointers[seg - 1];
}

inline CLRDATA_ADDRESS SegQueueLimit(DacpGcHeapDetails& heapDetails, int seg)
{
    return heapDetails.finalization_fill_pointers[seg];
}

#define FinalizerListSeg (DAC_NUMBERGENERATIONS+1)
#define CriticalFinalizerListSeg (DAC_NUMBERGENERATIONS)

void GatherOneHeapFinalization(DacpGcHeapDetails& heapDetails, HeapStat *stat, BOOL bAllReady, BOOL bShort);

CLRDATA_ADDRESS GetAppDomainForMT(CLRDATA_ADDRESS mtPtr);
CLRDATA_ADDRESS GetAppDomain(CLRDATA_ADDRESS objPtr);
void GCHeapInfo(const DacpGcHeapDetails &heap, DWORD_PTR &total_size);
BOOL GCObjInHeap(TADDR taddrObj, const DacpGcHeapDetails &heap, 
    TADDR_SEGINFO& trngSeg, int& gen, TADDR_RANGE& allocCtx, BOOL &bLarge);

BOOL VerifyObject(const DacpGcHeapDetails &heap, const DacpHeapSegmentData &seg, DWORD_PTR objAddr, DWORD_PTR MTAddr, size_t objSize, 
    BOOL bVerifyMember);
BOOL VerifyObject(const DacpGcHeapDetails &heap, DWORD_PTR objAddr, DWORD_PTR MTAddr, size_t objSize, 
    BOOL bVerifyMember);

BOOL IsMTForFreeObj(DWORD_PTR pMT);
void DumpStackObjectsHelper (TADDR StackTop, TADDR StackBottom, BOOL verifyFields);


enum ARGTYPE {COBOOL,COSIZE_T,COHEX,COSTRING};
struct CMDOption
{
    const char* name;
    void *vptr;
    ARGTYPE type;
    BOOL hasValue;
    BOOL hasSeen;
};
struct CMDValue
{
    void *vptr;
    ARGTYPE type;
};
BOOL GetCMDOption(const char *string, CMDOption *option, size_t nOption,
                  CMDValue *arg, size_t maxArg, size_t *nArg);

void DumpMDInfo(DWORD_PTR dwStartAddr, CLRDATA_ADDRESS dwRequestedIP = 0, BOOL fStackTraceFormat = FALSE);
void DumpMDInfoFromMethodDescData(DacpMethodDescData * pMethodDescData, BOOL fStackTraceFormat);
void GetDomainList(DWORD_PTR *&domainList, int &numDomain);
HRESULT GetThreadList(DWORD_PTR **threadList, int *numThread);
CLRDATA_ADDRESS GetCurrentManagedThread(); // returns current managed thread if any
void GetAllocContextPtrs(AllocInfo *pallocInfo);

void ReloadSymbolWithLineInfo();

size_t FunctionType (size_t EIP);

size_t Align (size_t nbytes);
// Aligns large objects
size_t AlignLarge (size_t nbytes);

ULONG OSPageSize ();
size_t NextOSPageAddress (size_t addr);

// This version of objectsize reduces the lookup of methodtables in the DAC.
// It uses g_special_mtCache for it's work.
BOOL GetSizeEfficient(DWORD_PTR dwAddrCurrObj, 
    DWORD_PTR dwAddrMethTable, BOOL bLarge, size_t& s, BOOL& bContainsPointers);

// ObjSize now uses the methodtable cache for its work too.
size_t ObjectSize (DWORD_PTR obj, BOOL fIsLargeObject=FALSE);
size_t ObjectSize(DWORD_PTR obj, DWORD_PTR mt, BOOL fIsValueClass, BOOL fIsLargeObject=FALSE);

void CharArrayContent(TADDR pos, ULONG num, bool widechar);
void StringObjectContent (size_t obj, BOOL fLiteral=FALSE, const int length=-1);  // length=-1: dump everything in the string object.

UINT FindAllPinnedAndStrong (DWORD_PTR handlearray[],UINT arraySize);
void PrintNotReachableInRange(TADDR rngStart, TADDR rngEnd, BOOL bExcludeReadyForFinalization, 
    HeapStat* stat, BOOL bShort);

const char *EHTypeName(EHClauseType et);

template<typename T>
inline const LPCSTR GetTransparency(const T &t)
{
    if (!t.bHasCriticalTransparentInfo)
    {
        return "Not calculated";
    }
    else if (t.bIsCritical && !t.bIsTreatAsSafe)
    {
        return "Critical";
    }
    else if (t.bIsCritical)
    {
        return "Safe critical";
    }
    else
    {
        return "Transparent";
    }
}

struct StringHolder
{
    LPSTR data;
    StringHolder() : data(NULL) { }
    ~StringHolder() { if(data) delete [] data; }
};


ULONG TargetPlatform();
ULONG DebuggeeType();

inline BOOL IsKernelDebugger ()
{
    return DebuggeeType() == DEBUG_CLASS_KERNEL;
}

void    ResetGlobals(void);
HRESULT LoadClrDebugDll(void);
extern "C" void UnloadClrDebugDll(void);

extern IMetaDataImport* MDImportForModule (DacpModuleData *pModule);
extern IMetaDataImport* MDImportForModule (DWORD_PTR pModule);

//*****************************************************************************
//
// **** CQuickBytes
// This helper class is useful for cases where 90% of the time you allocate 512
// or less bytes for a data structure.  This class contains a 512 byte buffer.
// Alloc() will return a pointer to this buffer if your allocation is small
// enough, otherwise it asks the heap for a larger buffer which is freed for
// you.  No mutex locking is required for the small allocation case, making the
// code run faster, less heap fragmentation, etc...  Each instance will allocate
// 520 bytes, so use accordinly.
//
//*****************************************************************************
template <DWORD SIZE, DWORD INCREMENT> 
class CQuickBytesBase
{
public:
    CQuickBytesBase() :
        pbBuff(0),
        iSize(0),
        cbTotal(SIZE)
    { }

    void Destroy()
    {
        if (pbBuff)
        {
            delete[] (BYTE*)pbBuff;
            pbBuff = 0;
        }
    }

    void *Alloc(SIZE_T iItems)
    {
        iSize = iItems;
        if (iItems <= SIZE)
        {
            cbTotal = SIZE;
            return (&rgData[0]);
        }
        else
        {
            if (pbBuff) 
                delete[] (BYTE*)pbBuff;
            pbBuff = new BYTE[iItems];
            cbTotal = pbBuff ? iItems : 0;
            return (pbBuff);
        }
    }

    // This is for conformity to the CQuickBytesBase that is defined by the runtime so
    // that we can use it inside of some GC code that SOS seems to include as well.
    //
    // The plain vanilla "Alloc" version on this CQuickBytesBase doesn't throw either,
    // so we'll just forward the call.
    void *AllocNoThrow(SIZE_T iItems)
    {
        return Alloc(iItems);
    }

    HRESULT ReSize(SIZE_T iItems)
    {
        void *pbBuffNew;
        if (iItems <= cbTotal)
        {
            iSize = iItems;
            return NOERROR;
        }

        pbBuffNew = new BYTE[iItems + INCREMENT];
        if (!pbBuffNew)
            return E_OUTOFMEMORY;
        if (pbBuff) 
        {
            memcpy(pbBuffNew, pbBuff, cbTotal);
            delete[] (BYTE*)pbBuff;
        }
        else
        {
            _ASSERTE(cbTotal == SIZE);
            memcpy(pbBuffNew, rgData, SIZE);
        }
        cbTotal = iItems + INCREMENT;
        iSize = iItems;
        pbBuff = pbBuffNew;
        return NOERROR;
        
    }

    operator PVOID()
    { return ((pbBuff) ? pbBuff : &rgData[0]); }

    void *Ptr()
    { return ((pbBuff) ? pbBuff : &rgData[0]); }

    SIZE_T Size()
    { return (iSize); }

    SIZE_T MaxSize()
    { return (cbTotal); }

    void        *pbBuff;
    SIZE_T      iSize;              // number of bytes used
    SIZE_T      cbTotal;            // total bytes allocated in the buffer
    // use UINT64 to enforce the alignment of the memory
    UINT64 rgData[(SIZE+sizeof(UINT64)-1)/sizeof(UINT64)];
};

#define     CQUICKBYTES_BASE_SIZE           512
#define     CQUICKBYTES_INCREMENTAL_SIZE    128

class CQuickBytesNoDtor : public CQuickBytesBase<CQUICKBYTES_BASE_SIZE, CQUICKBYTES_INCREMENTAL_SIZE>
{
};

class CQuickBytes : public CQuickBytesNoDtor
{
public:
    CQuickBytes() { }

    ~CQuickBytes()
    {
        Destroy();
    }
};

template <DWORD CQUICKBYTES_BASE_SPECIFY_SIZE> 
class CQuickBytesNoDtorSpecifySize : public CQuickBytesBase<CQUICKBYTES_BASE_SPECIFY_SIZE, CQUICKBYTES_INCREMENTAL_SIZE>
{
};

template <DWORD CQUICKBYTES_BASE_SPECIFY_SIZE> 
class CQuickBytesSpecifySize : public CQuickBytesNoDtorSpecifySize<CQUICKBYTES_BASE_SPECIFY_SIZE>
{
public:
    CQuickBytesSpecifySize() { }

    ~CQuickBytesSpecifySize()
    {
        CQuickBytesNoDtorSpecifySize<CQUICKBYTES_BASE_SPECIFY_SIZE>::Destroy();
    }
};


#define STRING_SIZE 10
class CQuickString : public CQuickBytesBase<STRING_SIZE, STRING_SIZE> 
{
public:
    CQuickString() { }

    ~CQuickString()
    {
        Destroy();
    }
    
    void *Alloc(SIZE_T iItems)
    {
        return CQuickBytesBase<STRING_SIZE, STRING_SIZE>::Alloc(iItems*sizeof(WCHAR));
    }

    HRESULT ReSize(SIZE_T iItems)
    {
        return CQuickBytesBase<STRING_SIZE, STRING_SIZE>::ReSize(iItems * sizeof(WCHAR));
    }

    SIZE_T Size()
    {
        return CQuickBytesBase<STRING_SIZE, STRING_SIZE>::Size() / sizeof(WCHAR);
    }

    SIZE_T MaxSize()
    {
        return CQuickBytesBase<STRING_SIZE, STRING_SIZE>::MaxSize() / sizeof(WCHAR);
    }

    WCHAR* String()
    {
        return (WCHAR*) Ptr();
    }

};

enum GetSignatureStringResults
{
    GSS_SUCCESS,
    GSS_ERROR,
    GSS_INSUFFICIENT_DATA,
};

GetSignatureStringResults GetMethodSignatureString (PCCOR_SIGNATURE pbSigBlob, ULONG ulSigBlob, DWORD_PTR dwModuleAddr, CQuickBytes *sigString);
GetSignatureStringResults GetSignatureString (PCCOR_SIGNATURE pbSigBlob, ULONG ulSigBlob, DWORD_PTR dwModuleAddr, CQuickBytes *sigString);
void GetMethodName(mdMethodDef methodDef, IMetaDataImport * pImport, CQuickBytes *fullName);

#ifndef _TARGET_WIN64_
#define     itoa_s_ptr _itoa_s
#define     itow_s_ptr _itow_s
#define     itoa_ptr   _itoa
#define     itow_ptr   _itow
#else
#define     itoa_s_ptr _i64toa_s
#define     itow_s_ptr _i64tow_s
#define     itoa_ptr   _i64toa
#define     itow_ptr   _i64tow
#endif

#ifdef FEATURE_PAL
extern "C"
int  _itoa_s( int inValue, char* outBuffer, size_t inDestBufferSize, int inRadix );
extern "C"
int  _ui64toa_s( unsigned __int64 inValue, char* outBuffer, size_t inDestBufferSize, int inRadix );
#endif // FEATURE_PAL

struct MemRange
{
    MemRange (ULONG64 s = NULL, size_t l = 0, MemRange * n = NULL) 
        : start(s), len (l), next (n)
        {}

    bool InRange (ULONG64 addr)
    {
        return addr >= start && addr < start + len;
    }
        
    ULONG64 start;
    size_t len;
    MemRange * next;
}; //struct MemRange

class StressLogMem
{
private:
    // use a linked list for now, could be optimazied later
    MemRange * list;

    void AddRange (ULONG64 s, size_t l)
    {
        list = new MemRange (s, l, list);
    }
    
public:
    StressLogMem () : list (NULL)
        {}
    ~StressLogMem ();
    bool Init (ULONG64 stressLogAddr, IDebugDataSpaces* memCallBack);
    bool IsInStressLog (ULONG64 addr);
}; //class StressLogMem

#ifndef FEATURE_PAL
// An adapter class that DIA consumes so that it can read PE data from the an image
// This implementation gets the backing data from the image loaded in debuggee memory
// that has been layed out identical to the disk format (ie not seperated by section)
class PEOffsetMemoryReader : IDiaReadExeAtOffsetCallback
{
public:
    PEOffsetMemoryReader(TADDR moduleBaseAddress);

    // IUnknown implementation
    HRESULT __stdcall QueryInterface(REFIID riid, VOID** ppInterface);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();
    
    // IDiaReadExeAtOffsetCallback implementation
    HRESULT __stdcall ReadExecutableAt(DWORDLONG fileOffset, DWORD cbData, DWORD* pcbData, BYTE data[]);

private:
    TADDR m_moduleBaseAddress;
    volatile ULONG m_refCount;
};

// An adapter class that DIA consumes so that it can read PE data from the an image
// This implementation gets the backing data from the image loaded in debuggee memory
// that has been layed out in LoadLibrary format
class PERvaMemoryReader : IDiaReadExeAtRVACallback
{
public:
    PERvaMemoryReader(TADDR moduleBaseAddress);

    // IUnknown implementation
    HRESULT __stdcall QueryInterface(REFIID riid, VOID** ppInterface);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();
    
    // IDiaReadExeAtOffsetCallback implementation
    HRESULT __stdcall ReadExecutableAtRVA(DWORD relativeVirtualAddress, DWORD cbData, DWORD* pcbData, BYTE data[]);

private:
    TADDR m_moduleBaseAddress;
    volatile ULONG m_refCount;
};

class SymbolReader
{
private:
    ISymUnmanagedReader* m_pSymReader;

private:
    HRESULT GetNamedLocalVariable(ISymUnmanagedScope * pScope, ICorDebugILFrame * pILFrame, mdMethodDef methodToken, ULONG localIndex, __inout_ecount(paramNameLen) WCHAR* paramName, ULONG paramNameLen, ICorDebugValue** ppValue);

public:
    SymbolReader() : m_pSymReader (NULL) {}
    ~SymbolReader()
    {
        if(m_pSymReader != NULL)
        {
            m_pSymReader->Release();
            m_pSymReader = NULL;
        }
    }

    HRESULT LoadSymbols(IMetaDataImport * pMD, ICorDebugModule * pModule);
    HRESULT LoadSymbols(IMetaDataImport * pMD, ULONG64 baseAddress, __in_z WCHAR* pModuleName, BOOL isInMemory);
    HRESULT GetNamedLocalVariable(ICorDebugFrame * pFrame, ULONG localIndex, __inout_ecount(paramNameLen) WCHAR* paramName, ULONG paramNameLen, ICorDebugValue** ppValue);
    HRESULT SymbolReader::ResolveSequencePoint(__in_z WCHAR* pFilename, ULONG32 lineNumber, mdMethodDef* pToken, ULONG32* pIlOffset);
};
#endif

HRESULT
GetLineByOffset(
        __in  ULONG64 IP,
        __out ULONG *pLinenum,
        __out_ecount(cbFileName) LPSTR lpszFileName,
        __in ULONG cbFileName);

/// X86 Context
#define X86_SIZE_OF_80387_REGISTERS      80
#define X86_MAXIMUM_SUPPORTED_EXTENSION     512

typedef struct {
    DWORD   ControlWord;
    DWORD   StatusWord;
    DWORD   TagWord;
    DWORD   ErrorOffset;
    DWORD   ErrorSelector;
    DWORD   DataOffset;
    DWORD   DataSelector;
    BYTE    RegisterArea[X86_SIZE_OF_80387_REGISTERS];
    DWORD   Cr0NpxState;
} X86_FLOATING_SAVE_AREA;

typedef struct {

    DWORD ContextFlags;
    DWORD   Dr0;
    DWORD   Dr1;
    DWORD   Dr2;
    DWORD   Dr3;
    DWORD   Dr6;
    DWORD   Dr7;

    X86_FLOATING_SAVE_AREA FloatSave;

    DWORD   SegGs;
    DWORD   SegFs;
    DWORD   SegEs;
    DWORD   SegDs;

    DWORD   Edi;
    DWORD   Esi;
    DWORD   Ebx;
    DWORD   Edx;
    DWORD   Ecx;
    DWORD   Eax;

    DWORD   Ebp;
    DWORD   Eip;
    DWORD   SegCs;
    DWORD   EFlags;
    DWORD   Esp;
    DWORD   SegSs;

    BYTE    ExtendedRegisters[X86_MAXIMUM_SUPPORTED_EXTENSION];

} X86_CONTEXT;

typedef struct {
    ULONGLONG Low;
    LONGLONG High;
} M128A_XPLAT;


/// AMD64 Context
typedef struct {
    WORD   ControlWord;
    WORD   StatusWord;
    BYTE  TagWord;
    BYTE  Reserved1;
    WORD   ErrorOpcode;
    DWORD ErrorOffset;
    WORD   ErrorSelector;
    WORD   Reserved2;
    DWORD DataOffset;
    WORD   DataSelector;
    WORD   Reserved3;
    DWORD MxCsr;
    DWORD MxCsr_Mask;
    M128A_XPLAT FloatRegisters[8];

#if defined(_WIN64)
    M128A_XPLAT XmmRegisters[16];
    BYTE  Reserved4[96];
#else
    M128A_XPLAT XmmRegisters[8];
    BYTE  Reserved4[220];

    DWORD   Cr0NpxState;
#endif

} AMD64_XMM_SAVE_AREA32;

typedef struct {

    DWORD64 P1Home;
    DWORD64 P2Home;
    DWORD64 P3Home;
    DWORD64 P4Home;
    DWORD64 P5Home;
    DWORD64 P6Home;

    DWORD ContextFlags;
    DWORD MxCsr;

    WORD   SegCs;
    WORD   SegDs;
    WORD   SegEs;
    WORD   SegFs;
    WORD   SegGs;
    WORD   SegSs;
    DWORD EFlags;

    DWORD64 Dr0;
    DWORD64 Dr1;
    DWORD64 Dr2;
    DWORD64 Dr3;
    DWORD64 Dr6;
    DWORD64 Dr7;

    DWORD64 Rax;
    DWORD64 Rcx;
    DWORD64 Rdx;
    DWORD64 Rbx;
    DWORD64 Rsp;
    DWORD64 Rbp;
    DWORD64 Rsi;
    DWORD64 Rdi;
    DWORD64 R8;
    DWORD64 R9;
    DWORD64 R10;
    DWORD64 R11;
    DWORD64 R12;
    DWORD64 R13;
    DWORD64 R14;
    DWORD64 R15;

    DWORD64 Rip;

    union {
        AMD64_XMM_SAVE_AREA32 FltSave;
        struct {
            M128A_XPLAT Header[2];
            M128A_XPLAT Legacy[8];
            M128A_XPLAT Xmm0;
            M128A_XPLAT Xmm1;
            M128A_XPLAT Xmm2;
            M128A_XPLAT Xmm3;
            M128A_XPLAT Xmm4;
            M128A_XPLAT Xmm5;
            M128A_XPLAT Xmm6;
            M128A_XPLAT Xmm7;
            M128A_XPLAT Xmm8;
            M128A_XPLAT Xmm9;
            M128A_XPLAT Xmm10;
            M128A_XPLAT Xmm11;
            M128A_XPLAT Xmm12;
            M128A_XPLAT Xmm13;
            M128A_XPLAT Xmm14;
            M128A_XPLAT Xmm15;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;

    M128A_XPLAT VectorRegister[26];
    DWORD64 VectorControl;

    DWORD64 DebugControl;
    DWORD64 LastBranchToRip;
    DWORD64 LastBranchFromRip;
    DWORD64 LastExceptionToRip;
    DWORD64 LastExceptionFromRip;

} AMD64_CONTEXT;

typedef struct{
    __int64 LowPart;
    __int64 HighPart;
} FLOAT128_XPLAT;


/// ARM Context
#define ARM_MAX_BREAKPOINTS_CONST     8
#define ARM_MAX_WATCHPOINTS_CONST     4
typedef struct {

    DWORD ContextFlags;

    DWORD R0;
    DWORD R1;
    DWORD R2;
    DWORD R3;
    DWORD R4;
    DWORD R5;
    DWORD R6;
    DWORD R7;
    DWORD R8;
    DWORD R9;
    DWORD R10;
    DWORD R11;
    DWORD R12;

    DWORD Sp;
    DWORD Lr;
    DWORD Pc;
    DWORD Cpsr;

    DWORD Fpscr;
    union {
        M128A_XPLAT Q[16];
        ULONGLONG D[32];
        DWORD S[32];
    } DUMMYUNIONNAME;

    DWORD Bvr[ARM_MAX_BREAKPOINTS_CONST];
    DWORD Bcr[ARM_MAX_BREAKPOINTS_CONST];
    DWORD Wvr[ARM_MAX_WATCHPOINTS_CONST];
    DWORD Wcr[ARM_MAX_WATCHPOINTS_CONST];

} ARM_CONTEXT;

// On ARM this mask is or'ed with the address of code to get an instruction pointer
#ifndef THUMB_CODE
#define THUMB_CODE 1
#endif

//ARM64TODO: Verify the correctness of the following for ARM64
///ARM64 Context
#define ARM64_MAX_BREAKPOINTS     8
#define ARM64_MAX_WATCHPOINTS     2
typedef struct {
    
    DWORD ContextFlags;
    DWORD Cpsr;       // NZVF + DAIF + CurrentEL + SPSel
    union {
        struct {
            DWORD64 X0;
            DWORD64 X1;
            DWORD64 X2;
            DWORD64 X3;
            DWORD64 X4;
            DWORD64 X5;
            DWORD64 X6;
            DWORD64 X7;
            DWORD64 X8;
            DWORD64 X9;
            DWORD64 X10;
            DWORD64 X11;
            DWORD64 X12;
            DWORD64 X13;
            DWORD64 X14;
            DWORD64 X15;
            DWORD64 X16;
            DWORD64 X17;
            DWORD64 X18;
            DWORD64 X19;
            DWORD64 X20;
            DWORD64 X21;
            DWORD64 X22;
            DWORD64 X23;
            DWORD64 X24;
            DWORD64 X25;
            DWORD64 X26;
            DWORD64 X27;
            DWORD64 X28;
       };

       DWORD64 X[29];
   };

   DWORD64 Fp;
   DWORD64 Lr;
   DWORD64 Sp;
   DWORD64 Pc;


   M128A_XPLAT V[32];
   DWORD Fpcr;
   DWORD Fpsr;

   DWORD Bcr[ARM64_MAX_BREAKPOINTS];
   DWORD64 Bvr[ARM64_MAX_BREAKPOINTS];
   DWORD Wcr[ARM64_MAX_WATCHPOINTS];
   DWORD64 Wvr[ARM64_MAX_WATCHPOINTS];

} ARM64_CONTEXT;

typedef struct _CROSS_PLATFORM_CONTEXT {

    _CROSS_PLATFORM_CONTEXT() {}

    union {
        X86_CONTEXT       X86Context;
        AMD64_CONTEXT     Amd64Context;
        ARM_CONTEXT       ArmContext;
        ARM64_CONTEXT     Arm64Context;
    };

} CROSS_PLATFORM_CONTEXT, *PCROSS_PLATFORM_CONTEXT;



WString BuildRegisterOutput(const SOSStackRefData &ref, bool printObj = true);
WString MethodNameFromIP(CLRDATA_ADDRESS methodDesc, BOOL bSuppressLines=FALSE);
HRESULT GetGCRefs(ULONG osID, SOSStackRefData **ppRefs, unsigned int *pRefCnt, SOSStackRefError **ppErrors, unsigned int *pErrCount);
WString GetFrameFromAddress(TADDR frameAddr, IXCLRDataStackWalk *pStackwalk=0);

/* This cache is used to read data from the target process if the reads are known
 * to be sequential.
 */
class LinearReadCache
{
public:
    LinearReadCache(ULONG pageSize = 0x10000);
    ~LinearReadCache();

    /* Reads an address out of the target process, caching the page of memory read.
     * Params:
     *   addr - The address to read out of the target process.
     *   t - A pointer to the data to stuff it in.  We will read sizeof(T) data
     *       from the process and write it into the location t points to.  This
     *       parameter must be non-null.
     * Returns:
     *   True if the read succeeded.  False if it did not, usually as a result
     *   of the memory simply not being present in the target process.
     * Note:
     *   The state of *t is undefined if this function returns false.  We may
     *   have written partial data to it if we return false, so you must
     *   absolutely NOT use it if Read returns false.
     */
    template <class T>
    bool Read(TADDR addr, T *t, bool update = true)
    {
        _ASSERTE(t);

        // Unfortunately the ctor can fail the alloc for the byte array.  In this case
        // we'll just fall back to non-cached reads.
        if (mPage == NULL)
            return MisalignedRead(addr, t);

        // Is addr on the current page?  If not read the page of memory addr is on.
        // If this fails, we will fall back to a raw read out of the process (which
        // is what MisalignedRead does).
        if ((addr < mCurrPageStart) || (addr - mCurrPageStart > mCurrPageSize))
            if (!update || !MoveToPage(addr))
                return MisalignedRead(addr, t);

        // If MoveToPage succeeds, we MUST be on the right page.
        _ASSERTE(addr >= mCurrPageStart);
        
        // However, the amount of data requested may fall off of the page.  In that case,
        // fall back to MisalignedRead.
        TADDR offset = addr - mCurrPageStart;
        if (offset + sizeof(T) > mCurrPageSize)
            return MisalignedRead(addr, t);

        // If we reach here we know we are on the right page of memory in the cache, and
        // that the read won't fall off of the end of the page.
#ifdef _DEBUG
        mReads++;
#endif

        *t = *reinterpret_cast<T*>(mPage+offset);
        return true;
    }

    void EnsureRangeInCache(TADDR start, unsigned int size)
    {
        if (mCurrPageStart == start)
        {
            if (size <= mCurrPageSize)
                return;
            
            // Total bytes to read, don't overflow buffer.
            unsigned int total = size + mCurrPageSize;
            if (total + mCurrPageSize > mPageSize)
                total = mPageSize-mCurrPageSize;

            // Read into the middle of the buffer, update current page size.
            ULONG read = 0;
            HRESULT hr = g_ExtData->ReadVirtual(mCurrPageStart+mCurrPageSize, mPage+mCurrPageSize, total, &read);
            mCurrPageSize += read;

            if (hr != S_OK)
            {
                mCurrPageStart = 0;
                mCurrPageSize = 0;
            }
        }
        else
        {
            MoveToPage(start, size);
        }
    }
    
    void ClearStats()
    {
#ifdef _DEBUG
        mMisses = 0;
        mReads = 0;
        mMisaligned = 0;
#endif
    }
    
    void PrintStats(const char *func)
    {
#ifdef _DEBUG
        char buffer[1024];
        sprintf_s(buffer, _countof(buffer), "Cache (%s): %d reads (%2.1f%% hits), %d misses (%2.1f%%), %d misaligned (%2.1f%%).\n",
                                             func, mReads, 100*(mReads-mMisses)/(float)(mReads+mMisaligned), mMisses,
                                             100*mMisses/(float)(mReads+mMisaligned), mMisaligned, 100*mMisaligned/(float)(mReads+mMisaligned));
        OutputDebugStringA(buffer);
#endif
    }
    
private:
    /* Sets the cache to the page specified by addr, or false if we could not move to
     * that page.
     */
    bool MoveToPage(TADDR addr, unsigned int size = 0x18);

    /* Attempts to read from the target process if the data is possibly hanging off
     * the end of a page.
     */
    template<class T>
    inline bool MisalignedRead(TADDR addr, T *t)
    {
        ULONG fetched = 0;
        HRESULT hr = g_ExtData->ReadVirtual(addr, (BYTE*)t, sizeof(T), &fetched);

        if (FAILED(hr) || fetched != sizeof(T))
            return false;

        mMisaligned++;
        return true;
    }

private:
    TADDR mCurrPageStart;
    ULONG mPageSize, mCurrPageSize;
    BYTE *mPage;
    
    int mMisses, mReads, mMisaligned;
};


///////////////////////////////////////////////////////////////////////////////////////////
//
// Methods for creating a database out of the gc heap and it's roots in xml format or CLRProfiler format
//

#ifndef FEATURE_PAL
#include <hash_map>
#include <hash_set>
#include <list>
#endif

class TypeTree;
enum { FORMAT_XML=0, FORMAT_CLRPROFILER=1 };
enum { TYPE_START=0,TYPE_TYPES=1,TYPE_ROOTS=2,TYPE_OBJECTS=3,TYPE_HIGHEST=4};
class HeapTraverser
{
private:
    TypeTree *m_pTypeTree;
    size_t m_curNID;
    FILE *m_file;
    int m_format; // from the enum above
    size_t m_objVisited; // for UI updates
    bool m_verify;
    LinearReadCache mCache;
    
#ifndef FEATURE_PAL
    std::hash_map<TADDR, std::list<TADDR>> mDependentHandleMap;
#endif
    
public:           
    HeapTraverser(bool verify);
    ~HeapTraverser();

    FILE *getFile() { return m_file; }

    BOOL Initialize();
    BOOL CreateReport (FILE *fp, int format);

private:    
    // First all types are added to a tree
    void insert(size_t mTable);
    size_t getID(size_t mTable);    
    
    // Functions for writing to the output file.
    void PrintType(size_t ID,LPCWSTR name);

    void PrintObjectHead(size_t objAddr,size_t typeID,size_t Size);
    void PrintObjectMember(size_t memberValue, bool dependentHandle);
    void PrintObjectTail();

    void PrintRootHead();
    void PrintRoot(LPCWSTR kind,size_t Value);
    void PrintRootTail();
    
    void PrintSection(int Type,BOOL bOpening);

    // Root and object member helper functions
    void FindGCRootOnStacks();
    void PrintRefs(size_t obj, size_t methodTable, size_t size);
    
    // Callback functions used during traversals
    static void GatherTypes(DWORD_PTR objAddr,size_t Size,DWORD_PTR methodTable, LPVOID token);
    static void PrintHeap(DWORD_PTR objAddr,size_t Size,DWORD_PTR methodTable, LPVOID token);
    static void PrintOutTree(size_t methodTable, size_t ID, LPVOID token);
    void TraceHandles();
};

#ifndef FEATURE_PAL

class GCRootImpl
{
private:
    struct MTInfo
    {
        TADDR MethodTable;
        wchar_t  *TypeName;

        TADDR *Buffer;
        CGCDesc *GCDesc;

        bool ArrayOfVC;
        bool ContainsPointers;
        size_t BaseSize;
        size_t ComponentSize;
        
        const wchar_t *GetTypeName()
        {
            if (!TypeName)
                TypeName = CreateMethodTableName(MethodTable);
            
            if (!TypeName)
                return L"<error>";
            
            return TypeName;
        }

        MTInfo()
            : MethodTable(0), TypeName(0), Buffer(0), GCDesc(0),
              ArrayOfVC(false), ContainsPointers(false), BaseSize(0), ComponentSize(0)
        {
        }

        ~MTInfo()
        {
            if (Buffer)
                delete [] Buffer;

            if (TypeName)
                delete [] TypeName;
        }
    };

    struct RootNode
    {
        RootNode *Next;
        RootNode *Prev;
        TADDR Object;
        MTInfo *MTInfo;

        bool FilledRefs;
        bool FromDependentHandle;
        RootNode *GCRefs;
        
        
        const wchar_t *GetTypeName()
        {
            if (!MTInfo)
                return L"<unknown>";
                
            return MTInfo->GetTypeName();
        }

        RootNode()
            : Next(0), Prev(0)
        {
            Clear();
        }

        void Clear()
        {
            if (Next && Next->Prev == this)
                Next->Prev = NULL;

            if (Prev && Prev->Next == this)
                Prev->Next = NULL;

            Next = 0;
            Prev = 0;
            Object = 0;
            MTInfo = 0;
            FilledRefs = false;
            FromDependentHandle = false;
            GCRefs = 0;
        }
        
        void Remove(RootNode *&list)
        {
            RootNode *curr_next = Next;
            
            // We've already considered this object, remove it.
            if (Prev == NULL)
            {
                // If we've filtered out the head, update it.
                list = curr_next;

                if (curr_next)
                    curr_next->Prev = NULL;
            }
            else
            {
                // Otherwise remove the current item from the list
                Prev->Next = curr_next;

                if (curr_next)
                    curr_next->Prev = Prev;
            }
        }	
    };

public:
    static void GetDependentHandleMap(std::hash_map<TADDR, std::list<TADDR>> &map);

public:
    // Finds all objects which root "target" and prints the path from the root
    // to "target".  If all is true, all possible paths to the object are printed.
    // If all is false, only completely unique paths will be printed.
    int PrintRootsForObject(TADDR obj, bool all, bool noStacks);

    // Finds a path from root to target if it exists and prints it out.  Returns
    // true if it found a path, false otherwise.
    bool PrintPathToObject(TADDR root, TADDR target);

    // Calculates the size of the closure of objects kept alive by root.
    size_t ObjSize(TADDR root);

    // Walks each root, printing out the total amount of memory held alive by it.
    void ObjSize();

    // Returns the set of all live objects in the process.
    const std::hash_set<TADDR> &GetLiveObjects(bool excludeFQ = false);

    // See !FindRoots.
    int FindRoots(int gen, TADDR target);

private:
    // typedefs
    typedef void (*ReportCallback)(TADDR root, RootNode *path, bool printHeader);

    // Book keeping and debug.
    void ClearAll();
    void ClearNodes();
    void ClearSizeData();

    // Printing roots
    int PrintRootsOnHandleTable(int gen = -1);
    int PrintRootsOnAllThreads();
    int PrintRootsOnThread(DWORD osThreadId);
    int PrintRootsOnFQ(bool notReadyForFinalization = false);
    int PrintRootsInOlderGen();
    int PrintRootsInRange(LinearReadCache &cache, TADDR start, TADDR stop, ReportCallback func, bool printHeader);

    // Calculate gc root
    RootNode *FilterRoots(RootNode *&list);
    RootNode *FindPathToTarget(TADDR root);
    RootNode *GetGCRefs(RootNode *path, RootNode *node);
    
    void InitDependentHandleMap();

    //Reporting:
    void ReportOneHandlePath(const SOSHandleData &handle, RootNode *node, bool printHeader);
    void ReportOnePath(DWORD thread, const SOSStackRefData &stackRef, RootNode *node, bool printThread, bool printFrame);
    static void ReportOneFQEntry(TADDR root, RootNode *path, bool printHeader);
    static void ReportOlderGenEntry(TADDR root, RootNode *path, bool printHeader);
    void ReportSizeInfo(const SOSHandleData &handle, TADDR obj);
    void ReportSizeInfo(DWORD thread, const SOSStackRefData &ref, TADDR obj);

    // Data reads:
    TADDR ReadPointer(TADDR location);
    TADDR ReadPointerCached(TADDR location);

    // Object/MT data:
    MTInfo *GetMTInfo(TADDR mt);
    DWORD GetComponents(TADDR obj, TADDR mt);
    size_t GetSizeOfObject(TADDR obj, MTInfo *info);

    // RootNode management:
    RootNode *NewNode(TADDR obj = 0, MTInfo *mtinfo = 0, bool fromDependent = false);
    void DeleteNode(RootNode *node);

private:
    
    bool mAll,  // Print all roots or just unique roots?
         mSize; // Print rooting information or total size info?

    std::list<RootNode*> mCleanupList;  // A list of RootNode's we've newed up.  This is only used to delete all of them later.
    std::list<RootNode*> mRootNewList;  // A list of unused RootNodes that are free to use instead of having to "new" up more.
    
    std::hash_map<TADDR, MTInfo*> mMTs;     // The MethodTable cache which maps from MT -> MethodTable data (size, gcdesc, string typename)
    std::hash_map<TADDR, RootNode*> mTargets;   // The objects that we are searching for.
    std::hash_set<TADDR> mConsidered;       // A hashtable of objects we've already visited.
    std::hash_map<TADDR, size_t> mSizes;   // A mapping from object address to total size of data the object roots.
    
    std::hash_map<TADDR, std::list<TADDR>> mDependentHandleMap;
    
    LinearReadCache mCache;     // A linear cache which stops us from having to read from the target process more than 1-2 times per object.
};

#endif // !FEATURE_PAL

//
// Helper class used for type-safe bitflags
//   T - the enum type specifying the individual bit flags
//   U - the underlying/storage type
// Requirement:
//   sizeof(T) <= sizeof(U)
//
template <typename T, typename U>
struct Flags
{
    typedef T UnderlyingType;
    typedef U BitFlagEnumType;

    static_assert_no_msg(sizeof(BitFlagEnumType) <= sizeof(UnderlyingType));

    Flags(UnderlyingType v)
        : m_val(v)
    { }

    Flags(BitFlagEnumType v)
        : m_val(v)
    { }

    Flags(const Flags& other)
        : m_val(other.m_val)
    { }

    Flags& operator = (const Flags& other)
    { m_val = other.m_val; return *this; }

    Flags operator | (Flags other) const
    { return Flags<T, U>(m_val | other._val); }

    void operator |= (Flags other)
    { m_val |= other.m_val; }

    Flags operator & (Flags other) const
    { return Flags<T, U>(m_val & other.m_val); }

    void operator &= (Flags other)
    { m_val &= other.m_val; }

    Flags operator ^ (Flags other) const
    { return Flags<T, U>(m_val ^ other._val); }

    void operator ^= (Flags other)
    { m_val ^= other.m_val; }

    BOOL operator == (Flags other) const
    { return m_val == other.m_val; }

    BOOL operator != (Flags other) const
    { return m_val != other.m_val; }


private:
    UnderlyingType m_val;
};

#ifndef FEATURE_PAL

// Flags defining activation policy for COM objects
enum CIOptionsBits 
{
    cciLatestFx     = 0x01,     // look in the most recent .NETFx installation
    cciMatchFx      = 0x02,     // NYI: Look in the .NETFx installation matching the debuggee's runtime
    cciAnyFx        = 0x04,     // look in any .NETFx installation
    cciFxMask       = 0x0f,
    cciDbiColocated = 0x10,     // NYI: Look next to the already loaded DBI module
    cciDacColocated = 0x20,     // Look next to the already loaded DAC module
    cciDbgPath      = 0x40,     // Look in all folders in the debuggers symbols and binary path
};

typedef Flags<DWORD, CIOptionsBits> CIOptions;

/**********************************************************************\
* Routine Description:                                                 *
*                                                                      *
* CreateInstanceCustom() provides a way to activate a COM object w/o   *
* triggering the FeatureOnDemand dialog. In order to do this we        *
* must avoid using  the CoCreateInstance() API, which, on a machine    *
* with v4+ installed and w/o v2, would trigger this.                   *
* CreateInstanceCustom() activates the requested COM object according  *
* to the specified passed in CIOptions, in the following order         *
* (skipping the steps not enabled in the CIOptions flags passed in):   *
*    1. Attempt to activate the COM object using a framework install:  *
*       a. If the debugger machine has a V4+ shell shim use the shim   *
*          to activate the object                                      *
*       b. Otherwise simply call CoCreateInstance                      *
*    2. If unsuccessful attempt to activate looking for the dllName in *
*       the same folder as the DAC was loaded from                     *
*    3. If unsuccessful attempt to activate the COM object looking in  *
*       every path specified in the debugger's .exepath and .sympath   *
\**********************************************************************/
HRESULT CreateInstanceCustom(
                        REFCLSID clsid,
                        REFIID   iid,
                        LPCWSTR  dllName,
                        CIOptions cciOptions,
                        void** ppItf);


//------------------------------------------------------------------------
// A typesafe version of GetProcAddress
//------------------------------------------------------------------------
template <typename T>
BOOL
GetProcAddressT(
    __in PCSTR FunctionName,
    __in_opt PCWSTR DllName,
    __inout T* OutFunctionPointer,
    __inout HMODULE* InOutDllHandle
    )
{
    _ASSERTE(InOutDllHandle != NULL);
    _ASSERTE(OutFunctionPointer != NULL);

    T FunctionPointer = NULL;
    HMODULE DllHandle = *InOutDllHandle;
    if (DllHandle == NULL)
    {
        DllHandle = LoadLibraryExW(DllName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
        if (DllHandle != NULL)
            *InOutDllHandle = DllHandle;
    }
    if (DllHandle != NULL)
    {
        FunctionPointer = (T) GetProcAddress(DllHandle, FunctionName);
    }
    *OutFunctionPointer = FunctionPointer;
    return FunctionPointer != NULL;
}


#endif // FEATURE_PAL

struct ImageInfo
{
    ULONG64 modBase;
};

HRESULT
GetClrModuleImages(
    __in IXCLRDataModule* Module,
    __in CLRDataModuleExtentType DesiredType,
    __out ImageInfo* FirstAdd);

// Helper class used in ClrStackFromPublicInterface() to keep track of explicit EE Frames
// (i.e., "internal frames") on the stack.  Call Init() with the appropriate
// ICorDebugThread3, and this class will initialize itself with the set of internal
// frames.  You can then call PrintPrecedingInternalFrames during your stack walk to
// have this class output any internal frames that "precede" (i.e., that are closer to
// the leaf than) the specified ICorDebugFrame.
class InternalFrameManager
{
private:
    // TODO: Verify constructor AND destructor is called for each array element
    // TODO: Comment about hard-coding 1000
    ToRelease<ICorDebugInternalFrame2> m_rgpInternalFrame2[1000];
    ULONG32 m_cInternalFramesActual;
    ULONG32 m_iInternalFrameCur;

public:
    InternalFrameManager();
    HRESULT Init(ICorDebugThread3 * pThread3);
    HRESULT PrintPrecedingInternalFrames(ICorDebugFrame * pFrame);

private:
    HRESULT PrintCurrentInternalFrame();
};

#endif // __util_h__




#define MAX_EXPRESSION 500
#define MAX_ERROR 500


// Represents one node in a tree of expressions and sub-expressions
// These nodes are used in the !watch expandable expression tree
// Each node consists of a string based C#-like expression and its
// evaluation within the current context of the debuggee.
//
// These nodes are also intended for eventual use in ClrStack -i expression tree
// but ClrStack -i hasn't yet been refactored to use them
//
// Each node can evaluate to:
// nothing - if an error occurs during expression parsing or the expression
//           names don't match to anything in the debuggee
// a debuggee value - these are values that are backed in memory of the debuggee
//                    (ICorDebugValue objects) or build time constants which are
//                    stored in the assembly metadata.
// a debuggee type - instead of refering to a particular instance of a type (the
//                   value case above), nodes can directly refer to a type definition
//                   represented by an ICorDebugType object
class ExpressionNode
{
public:

    typedef VOID (*ExpressionNodeVisitorCallback)(ExpressionNode* pExpressionNode, int depth, VOID* pUserData);
    
    // Returns the complete expression being evaluated to get the value for this node
    // The returned pointer is a string interior to this object - once you release
    // all references to this object the string is invalid.
    WCHAR* GetAbsoluteExpression();

    // Returns the sub expression that logically indicates how the parent expression
    // was built upon to reach this node. This relative value has no purpose other
    // than an identifier and to convey UI meaning to the user. At present typical values
    // are the name of type, a local, a parameter, a field, an array index, or '<basetype>'
    // for a baseclass casting operation
    // The returned pointer is a string interior to this object - once you release
    // all references to this object the string is invalid.
    WCHAR* GetRelativeExpression();

    // Returns a text representation of the type of value that this node refers to
    // It is possible this node doesn't evaluate to anything and therefore has no
    // type
    // The returned pointer is a string interior to this object - once you release
    // all references to this object the string is invalid.
    WCHAR* GetTypeName();

    // Returns a text representation of the value for this node. It is possible that
    // this node doesn't evaluate to anything and therefore has no value text.
    // The returned pointer is a string interior to this object - once you release
    // all references to this object the string is invalid.
    WCHAR* GetTextValue();

    // If there is any error during the evaluation of this node's expression, it is
    // returned here.
    // The returned pointer is a string interior to this object - once you release
    // all references to this object the string is invalid.
    WCHAR* GetErrorMessage();
    
    // Factory function for creating the expression node at the root of a tree
    static HRESULT CreateExpressionNode(__in_z WCHAR* pExpression, ExpressionNode** ppExpressionNode);

    // Performs recursive expansion within the tree for nodes that are along the path to varToExpand.
    // Expansion involves calulating a set of child expressions from the current expression via
    // field dereferencing, array index dereferencing, or casting to a base type.
    // For example if a tree was rooted with expression 'foo.bar' and varToExpand is '(Baz)foo.bar[9]'
    // then 'foo.bar', 'foo.bar[9]', and '(Baz)foo.bar[9]' nodes would all be expanded.
    HRESULT Expand(__in_z WCHAR* varToExpand);

    // Standard depth first search tree traversal pattern with a callback
    VOID DFSVisit(ExpressionNodeVisitorCallback pFunc, VOID* pUserData, int depth=0);

private:
    // for nodes that evaluate to a type, this is that type
    // for nodes that evaluate to a debuggee value, this is the type of that
    // value or one of its base types. It represents the type the value should
    // displayed and expanded as.
    ToRelease<ICorDebugType> pTypeCast;

    // for nodes that evaluate to a memory backed debuggee value, this is that value
    ToRelease<ICorDebugValue> pValue;

    // if this node gets expanded and it has thread-static or context-static sub-fields,
    // this frame disambiguates which thread and context to use.
    ToRelease<ICorDebugILFrame> pILFrame;

    // TODO: exactly which metadata is this supposed to be? try to get rid of this
    ToRelease<IMetaDataImport> pMD;

    // PERF: this could be a lot more memory efficient
    WCHAR pTextValue[MAX_EXPRESSION];
    WCHAR pErrorMessage[MAX_ERROR];
    WCHAR pAbsoluteExpression[MAX_EXPRESSION];
    WCHAR pRelativeExpression[MAX_EXPRESSION];
    WCHAR pTypeName[MAX_EXPRESSION];

    // if this value represents a build time constant debuggee value, this is a pointer
    // to the value data stored in metadata and its size
    UVCP_CONSTANT pDefaultValue;
    ULONG cchDefaultValue;

    // Pointer in a linked list of sibling nodes that all share the same parent
    ExpressionNode* pNextSibling;
    // Pointer to the first child node of this node, other children can be found
    // by following the child's sibling list.
    ExpressionNode* pChild;

    typedef VOID (*VariableEnumCallback)(ICorDebugValue* pValue, WCHAR* pName, WCHAR* pErrorMessage, VOID* pUserData);
    typedef VOID (*FrameEnumCallback)(ICorDebugFrame* pFrame, VOID* pUserData);
    
    // Indicates how a child node was derived from its parent
    enum ChildKind
    {
        ChildKind_Field,
        ChildKind_Index,
        ChildKind_BaseClass
    };

    // Creates a new expression with a given debuggee value and frame
    ExpressionNode(__in_z WCHAR* pExpression, ICorDebugValue* pValue, ICorDebugILFrame* pFrame);

    // Creates a new expression that has an error and no value
    ExpressionNode(__in_z WCHAR* pExpression, __in_z WCHAR* pErrorMessage);

    // Creates a new child expression
    ExpressionNode(__in_z WCHAR* pParentExpression, ChildKind ck, __in_z WCHAR* pRelativeExpression, ICorDebugValue* pValue, ICorDebugType* pType, ICorDebugILFrame* pFrame, UVCP_CONSTANT pDefaultValue = NULL, ULONG cchDefaultValue = 0);

    // Common member initialization for the constructors
    VOID Init(ICorDebugValue* pValue, ICorDebugType* pTypeCast, ICorDebugILFrame* pFrame);

    // Retreves the correct IMetaDataImport for the type represented in this node and stores it
    // in pMD.
    HRESULT PopulateMetaDataImport();

    // Determines the string representation of pType and stores it in typeName
    static HRESULT CalculateTypeName(ICorDebugType * pType, __inout_ecount(typeNameLen) WCHAR* typeName, DWORD typeNameLen);
    

    // Appends angle brackets and the generic argument list to a type name
    static HRESULT AddGenericArgs(ICorDebugType * pType, __inout_ecount(typeNameLen) WCHAR* typeName, DWORD typeNameLen);

    // Determines the text name for the type of this node and caches it
    HRESULT PopulateType();

    // Node expansion helpers

    // Inserts a new child at the end of the linked list of children
    // PERF: This has O(N) insert time but these lists should never be large
    VOID AddChild(ExpressionNode* pNewChild);

    // Helper that determines if the current node is on the path of nodes represented by
    // expression varToExpand
    BOOL ShouldExpandVariable(__in_z WCHAR* varToExpand);

    // Expands this array node by creating child nodes with expressions refering to individual array elements
    HRESULT ExpandSzArray(ICorDebugValue* pInnerValue, __in_z WCHAR* varToExpand);

    // Expands this struct/class node by creating child nodes with expressions refering to individual field values
    // and one node for the basetype value
    HRESULT ExpandFields(ICorDebugValue* pInnerValue, __in_z WCHAR* varToExpand);
    
    // Value Population functions

    //Helper for unwrapping values
    static HRESULT DereferenceAndUnboxValue(ICorDebugValue * pInputValue, ICorDebugValue** ppOutputValue, BOOL * pIsNull = NULL);

    // Returns TRUE if the value derives from System.Enum
    static BOOL IsEnum(ICorDebugValue * pInputValue);

    // Calculates the value text for nodes that have enum values
    HRESULT PopulateEnumValue(ICorDebugValue* pEnumValue, BYTE* enumValue);

    // Helper that fetches the text value of a string ICorDebugValue
    HRESULT GetDebuggeeStringValue(ICorDebugValue* pInputValue, __inout_ecount(cchBuffer) WCHAR* wszBuffer, DWORD cchBuffer);

    // Helper that fetches the text value of a string build-time literal
    HRESULT GetConstantStringValue(__inout_ecount(cchBuffer) WCHAR* wszBuffer, DWORD cchBuffer);

    // Helper that caches the textual value for nodes that evaluate to array objects
    HRESULT PopulateSzArrayValue(ICorDebugValue* pInputValue);

    // Helper that caches the textual value for nodes of any type
    HRESULT PopulateTextValueHelper();

    // Caches the textual value of this node
    HRESULT PopulateTextValue();


    // Expression parsing and search

    // In/Out parameters for the EvaluateExpressionFrameScanCallback
    typedef struct _EvaluateExpressionFrameScanData
    {
        WCHAR* pIdentifier;
        ToRelease<ICorDebugValue> pFoundValue;
        ToRelease<ICorDebugILFrame> pFoundFrame;
        ToRelease<ICorDebugILFrame> pFirstFrame;
        WCHAR* pErrorMessage;
        DWORD cchErrorMessage;
    } EvaluateExpressionFrameScanData;

    //Callback that searches a frame to determine if it contains a local variable or parameter of a given name
    static VOID EvaluateExpressionFrameScanCallback(ICorDebugFrame* pFrame, VOID* pUserData);

    //Callback checks to see if a given local/parameter has name pName
    static VOID EvaluateExpressionVariableScanCallback(ICorDebugValue* pValue, __in_z WCHAR* pName, __out_z WCHAR* pErrorMessage, VOID* pUserData);

    //Factory method that recursively parses pExpression and create an ExpressionNode
    //  pExpression -          the entire expression being parsed
    //  pExpressionRemainder - the portion of the expression that remains to be parsed in this
    //                         recursive invocation
    //  charactersParsed     - the number of characters that have been parsed from pExpression
    //                         so far (todo: this is basically the difference between remainder and
    //                         full expression, do we need it?)
    //  pParsedValue         - A debuggee value that should be used as the context for interpreting
    //                         pExpressionRemainder
    //  pParsedType          - A debuggee type that should be used as the context for interpreting
    //                         pExpressionRemainder. 
    //  pParsedDefaultValue  - A fixed value from metadata that should be used as context for
    //                         interpretting pExpressionRemainder
    //  cchParsedDefaultValue- Size of pParsedDefaultValue
    //  pFrame               - A debuggee IL frame that disambiguates the thread and context needed
    //                         to evaluate a thread-static or context-static value
    //  ppExpressionNode     - OUT - the resulting expression node
    //
    //
    static HRESULT CreateExpressionNodeHelper(__in_z WCHAR* pExpression,
                                              __in_z WCHAR* pExpressionParseRemainder,
                                              DWORD charactersParsed,
                                              ICorDebugValue* pParsedValue,
                                              ICorDebugType* pParsedType,
                                              UVCP_CONSTANT pParsedDefaultValue,
                                              ULONG cchParsedDefaultValue,
                                              ICorDebugILFrame* pFrame,
                                              ExpressionNode** ppExpressionNode);

    // Splits apart a C#-like expression and determines the first identifier in the string and updates expression to point
    // at the remaining unparsed portion
    static HRESULT ParseNextIdentifier(__in_z WCHAR** expression,
                                       __inout_ecount(cchIdentifierName) WCHAR* identifierName,
                                       DWORD cchIdentifierName,
                                       __inout_ecount(cchErrorMessage) WCHAR* errorMessage,
                                       DWORD cchErrorMessage,
                                       DWORD* charactersParsed,
                                       BOOL* isArrayIndex);


    // Iterate through all parameters in the ILFrame calling the callback function for each of them
    static HRESULT EnumerateParameters(IMetaDataImport * pMD,
                                       mdMethodDef methodDef,
                                       ICorDebugILFrame * pILFrame,
                                       VariableEnumCallback pCallback,
                                       VOID* pUserData);

    // Enumerate all locals in the given ILFrame, calling the callback method for each of them
    static HRESULT EnumerateLocals(IMetaDataImport * pMD,
                                   mdMethodDef methodDef,
                                   ICorDebugILFrame * pILFrame,
                                   VariableEnumCallback pCallback,
                                   VOID* pUserData);

    // Iterates over all frames on the current thread's stack, calling the callback function for each of them
    static HRESULT EnumerateFrames(FrameEnumCallback pCallback, VOID* pUserData);

    // Determines the corresponding ICorDebugType for a given primitive type
    static HRESULT FindTypeFromElementType(CorElementType et, ICorDebugType** ppType);

    // Gets the appropriate element type encoding for well-known fully qualified type names
    // This doesn't work for arbitrary types, just types that have CorElementType short forms.
    static HRESULT GetCanonicalElementTypeForTypeName(__in_z WCHAR* pTypeName, CorElementType *et);

    // Searches the debuggee for any ICorDebugType that matches the given fully qualified name
    // This will search across all AppDomains and Assemblies
    static HRESULT FindTypeByName(__in_z WCHAR* pTypeName, ICorDebugType** ppType);

    // Searches the debuggee for any ICorDebugType that matches the given fully qualified name
    // This will search across all Assemblies in the given AppDomain
    static HRESULT FindTypeByName(ICorDebugAppDomain* pAppDomain, __in_z WCHAR* pTypeName, ICorDebugType** ppType);

    // Searches the assembly for any ICorDebugType that matches the given fully qualified name
    static HRESULT FindTypeByName(ICorDebugAssembly* pAssembly, __in_z WCHAR* pTypeName, ICorDebugType** ppType);

    // Searches a given module for any ICorDebugType that matches the given fully qualified type name
    static HRESULT FindTypeByName(ICorDebugModule* pModule, __in_z WCHAR* pTypeName, ICorDebugType** ppType);

    // Checks whether the given token is or refers to type System.ValueType or System.Enum
    static HRESULT IsTokenValueTypeOrEnum(mdToken token, IMetaDataImport* pMetadata, BOOL* pResult);
};

#endif



#ifndef IfFailRet
#define IfFailRet(EXPR) do { Status = (EXPR); if(FAILED(Status)) { return (Status); } } while (0)
#endif

// Returns the complete expression being evaluated to get the value for this node
// The returned pointer is a string interior to this object - once you release
// all references to this object the string is invalid.
WCHAR* ExpressionNode::GetAbsoluteExpression() { return pAbsoluteExpression; }

// Returns the sub expression that logically indicates how the parent expression
// was built upon to reach this node. This relative value has no purpose other
// than an identifier and to convey UI meaning to the user. At present typical values
// are the name of type, a local, a parameter, a field, an array index, or '<basetype>'
// for a baseclass casting operation
// The returned pointer is a string interior to this object - once you release
// all references to this object the string is invalid.
WCHAR* ExpressionNode::GetRelativeExpression() { return pRelativeExpression; }

// Returns a text representation of the type of value that this node refers to
// It is possible this node doesn't evaluate to anything and therefore has no
// type
// The returned pointer is a string interior to this object - once you release
// all references to this object the string is invalid.
WCHAR* ExpressionNode::GetTypeName() { PopulateType(); return pTypeName; }

// Returns a text representation of the value for this node. It is possible that
// this node doesn't evaluate to anything and therefore has no value text.
// The returned pointer is a string interior to this object - once you release
// all references to this object the string is invalid.
WCHAR* ExpressionNode::GetTextValue() { PopulateTextValue(); return pTextValue; }

// If there is any error during the evaluation of this node's expression, it is
// returned here.
// The returned pointer is a string interior to this object - once you release
// all references to this object the string is invalid.
WCHAR* ExpressionNode::GetErrorMessage() { return pErrorMessage; }

// Factory function for creating the expression node at the root of a tree
HRESULT ExpressionNode::CreateExpressionNode(__in_z WCHAR* pExpression, ExpressionNode** ppExpressionNode)
{
    *ppExpressionNode = NULL;
    HRESULT Status = CreateExpressionNodeHelper(pExpression,
        pExpression,
        0,
        NULL,
        NULL,
        NULL,
        0,
        NULL,
        ppExpressionNode);
    if(FAILED(Status) && *ppExpressionNode == NULL)
    {

        WCHAR pErrorMessage[MAX_ERROR];
        _snwprintf_s(pErrorMessage, MAX_ERROR, _TRUNCATE, L"Error 0x%x while parsing expression", Status);
        *ppExpressionNode = new ExpressionNode(pExpression, pErrorMessage);
        Status = S_OK;
        if(*ppExpressionNode == NULL)
            Status = E_OUTOFMEMORY;
    }
    return Status;
}

// Performs recursive expansion within the tree for nodes that are along the path to varToExpand.
// Expansion involves calulating a set of child expressions from the current expression via
// field dereferencing, array index dereferencing, or casting to a base type.
// For example if a tree was rooted with expression 'foo.bar' and varToExpand is '(Baz)foo.bar[9]'
// then 'foo.bar', 'foo.bar[9]', and '(Baz)foo.bar[9]' nodes would all be expanded.
HRESULT ExpressionNode::Expand(__in_z WCHAR* varToExpand)
{
    if(!ShouldExpandVariable(varToExpand))
        return S_FALSE;
    if(pValue == NULL && pTypeCast == NULL)
        return S_OK;

    // if the node evaluates to a type, then the children are static fields of the type
    if(pValue == NULL)
        return ExpandFields(NULL, varToExpand);

    // If the value is a null reference there is nothing to expand
    HRESULT Status = S_OK;
    BOOL isNull = TRUE;
    ToRelease<ICorDebugValue> pInnerValue;
    IfFailRet(DereferenceAndUnboxValue(pValue, &pInnerValue, &isNull));
    if(isNull)
    {
        return S_OK;
    }


    CorElementType corElemType;
    IfFailRet(pValue->GetType(&corElemType));
    if (corElemType == ELEMENT_TYPE_SZARRAY)
    {
        //If its an array, add children representing the indexed elements
        return ExpandSzArray(pInnerValue, varToExpand);
    }
    else if(corElemType == ELEMENT_TYPE_CLASS || corElemType == ELEMENT_TYPE_VALUETYPE)
    {
        // If its a class or struct (not counting string, array, or object) then add children representing
        // the fields.
        return ExpandFields(pInnerValue, varToExpand);
    }
    else
    {
        // nothing else expands
        return S_OK;
    }
}

// Standard depth first search tree traversal pattern with a callback
VOID ExpressionNode::DFSVisit(ExpressionNodeVisitorCallback pFunc, VOID* pUserData, int depth)
{
    pFunc(this, depth, pUserData);
    ExpressionNode* pCurChild = pChild;
    while(pCurChild != NULL)
    {
        pCurChild->DFSVisit(pFunc, pUserData, depth+1);
        pCurChild = pCurChild->pNextSibling;
    }
}


// Creates a new expression with a given debuggee value and frame
ExpressionNode::ExpressionNode(__in_z WCHAR* pExpression, ICorDebugValue* pValue, ICorDebugILFrame* pFrame)
{
    Init(pValue, NULL, pFrame);
    _snwprintf_s(pAbsoluteExpression, MAX_EXPRESSION, _TRUNCATE, L"%s", pExpression);
    _snwprintf_s(pRelativeExpression, MAX_EXPRESSION, _TRUNCATE, L"%s", pExpression);
}

// Creates a new expression that has an error and no value
ExpressionNode::ExpressionNode(__in_z WCHAR* pExpression, __in_z WCHAR* pErrorMessage)
{
    Init(NULL, NULL, NULL);
    _snwprintf_s(pAbsoluteExpression, MAX_EXPRESSION, _TRUNCATE, L"%s", pExpression);
    _snwprintf_s(pRelativeExpression, MAX_EXPRESSION, _TRUNCATE, L"%s", pExpression);
    _snwprintf_s(this->pErrorMessage, MAX_ERROR, _TRUNCATE, L"%s", pErrorMessage);
}

// Creates a new child expression
ExpressionNode::ExpressionNode(__in_z WCHAR* pParentExpression, ChildKind ck, __in_z WCHAR* pRelativeExpression, ICorDebugValue* pValue, ICorDebugType* pType, ICorDebugILFrame* pFrame, UVCP_CONSTANT pDefaultValue, ULONG cchDefaultValue)
{
    Init(pValue, pType, pFrame);
    if(ck == ChildKind_BaseClass)
    {
        _snwprintf_s(pAbsoluteExpression, MAX_EXPRESSION, _TRUNCATE, L"%s", pParentExpression);
    }
    else
    {
        _snwprintf_s(pAbsoluteExpression, MAX_EXPRESSION, _TRUNCATE, ck == ChildKind_Field ? L"%s.%s" : L"%s[%s]", pParentExpression, pRelativeExpression);
    }
    _snwprintf_s(this->pRelativeExpression, MAX_EXPRESSION, _TRUNCATE, ck == ChildKind_Index ? L"[%s]" : L"%s", pRelativeExpression);
    this->pDefaultValue = pDefaultValue;
    this->cchDefaultValue = cchDefaultValue;
}

// Common member initialization for the constructors
VOID ExpressionNode::Init(ICorDebugValue* pValue, ICorDebugType* pTypeCast, ICorDebugILFrame* pFrame)
{
    this->pValue = pValue;
    this->pTypeCast = pTypeCast;
    this->pILFrame = pFrame;
    pChild = NULL;
    pNextSibling = NULL;
    pTextValue[0] = 0;
    pErrorMessage[0] = 0;
    pAbsoluteExpression[0] = 0;
    pRelativeExpression[0] = 0;
    pTypeName[0] = 0;

    pDefaultValue = NULL;
    cchDefaultValue = 0;

    // The ToRelease holders don't automatically AddRef
    if(pILFrame != NULL)
        pILFrame->AddRef();
    if(pTypeCast != NULL)
        pTypeCast->AddRef();
    if(pValue != NULL)
    {   
        pValue->AddRef();
        PopulateMetaDataImport();
    }
}

// Retreves the correct IMetaDataImport for the type represented in this node and stores it
// in pMD.
HRESULT ExpressionNode::PopulateMetaDataImport()
{
    if(pMD != NULL)
        return S_OK;

    HRESULT Status = S_OK;
    ToRelease<ICorDebugType> pType;
    if(pTypeCast != NULL)
    {
        pType = pTypeCast;
        pType->AddRef();
    }
    else
    {
        BOOL isNull;
        ToRelease<ICorDebugValue> pInnerValue;
        IfFailRet(DereferenceAndUnboxValue(pValue, &pInnerValue, &isNull));
        ToRelease<ICorDebugValue2> pValue2;
        IfFailRet(pInnerValue->QueryInterface(IID_ICorDebugValue2, (LPVOID *) &pValue2));

        IfFailRet(pValue2->GetExactType(&pType));

        // for array, pointer, and byref types we can't directly get a class, we must unwrap first
        CorElementType et;
        IfFailRet(pType->GetType(&et));
        while(et == ELEMENT_TYPE_ARRAY || et == ELEMENT_TYPE_SZARRAY || et == ELEMENT_TYPE_BYREF || et == ELEMENT_TYPE_PTR)
        {
            pType->GetFirstTypeParameter(&pType);
            IfFailRet(pType->GetType(&et));
        }
    }
    ToRelease<ICorDebugClass> pClass;
    IfFailRet(pType->GetClass(&pClass));
    ToRelease<ICorDebugModule> pModule;
    IfFailRet(pClass->GetModule(&pModule));
    ToRelease<IUnknown> pMDUnknown;
    IfFailRet(pModule->GetMetaDataInterface(IID_IMetaDataImport, &pMDUnknown));
    IfFailRet(pMDUnknown->QueryInterface(IID_IMetaDataImport, (LPVOID*) &pMD));
    return Status;
}

// Determines the string representation of pType and stores it in typeName
HRESULT ExpressionNode::CalculateTypeName(ICorDebugType * pType, __inout_ecount(typeNameLen) WCHAR* typeName, DWORD typeNameLen)
{
    HRESULT Status = S_OK;

    CorElementType corElemType;
    IfFailRet(pType->GetType(&corElemType));

    switch (corElemType)
    {
        //List of unsupported CorElementTypes:
        //ELEMENT_TYPE_END            = 0x0,
        //ELEMENT_TYPE_VAR            = 0x13,     // a class type variable VAR <U1>
        //ELEMENT_TYPE_GENERICINST    = 0x15,     // GENERICINST <generic type> <argCnt> <arg1> ... <argn>
        //ELEMENT_TYPE_TYPEDBYREF     = 0x16,     // TYPEDREF  (it takes no args) a typed referece to some other type
        //ELEMENT_TYPE_MVAR           = 0x1e,     // a method type variable MVAR <U1>
        //ELEMENT_TYPE_CMOD_REQD      = 0x1F,     // required C modifier : E_T_CMOD_REQD <mdTypeRef/mdTypeDef>
        //ELEMENT_TYPE_CMOD_OPT       = 0x20,     // optional C modifier : E_T_CMOD_OPT <mdTypeRef/mdTypeDef>
        //ELEMENT_TYPE_INTERNAL       = 0x21,     // INTERNAL <typehandle>
        //ELEMENT_TYPE_MAX            = 0x22,     // first invalid element type
        //ELEMENT_TYPE_MODIFIER       = 0x40,
        //ELEMENT_TYPE_SENTINEL       = 0x01 | ELEMENT_TYPE_MODIFIER, // sentinel for varargs
        //ELEMENT_TYPE_PINNED         = 0x05 | ELEMENT_TYPE_MODIFIER,
    default:
        swprintf_s(typeName, typeNameLen, L"(Unhandled CorElementType: 0x%x)\0", corElemType);
        break;

    case ELEMENT_TYPE_VALUETYPE:
    case ELEMENT_TYPE_CLASS:
        {
            //Defaults in case we fail...
            if(corElemType == ELEMENT_TYPE_VALUETYPE) swprintf_s(typeName, typeNameLen, L"struct\0");
            else swprintf_s(typeName, typeNameLen, L"class\0");

            mdTypeDef typeDef;
            ToRelease<ICorDebugClass> pClass;
            if(SUCCEEDED(pType->GetClass(&pClass)) && SUCCEEDED(pClass->GetToken(&typeDef)))
            {
                ToRelease<ICorDebugModule> pModule;
                IfFailRet(pClass->GetModule(&pModule));

                ToRelease<IUnknown> pMDUnknown;
                ToRelease<IMetaDataImport> pMD;
                IfFailRet(pModule->GetMetaDataInterface(IID_IMetaDataImport, &pMDUnknown));
                IfFailRet(pMDUnknown->QueryInterface(IID_IMetaDataImport, (LPVOID*) &pMD));

                if(SUCCEEDED(NameForToken_s(TokenFromRid(typeDef, mdtTypeDef), pMD, g_mdName, mdNameLen, false)))
                    swprintf_s(typeName, typeNameLen, L"%s\0", g_mdName);
            }
            AddGenericArgs(pType, typeName, typeNameLen);
        }
        break;
    case ELEMENT_TYPE_VOID:
        swprintf_s(typeName, typeNameLen, L"void\0");
        break;
    case ELEMENT_TYPE_BOOLEAN:
        swprintf_s(typeName, typeNameLen, L"bool\0");
        break;
    case ELEMENT_TYPE_CHAR:
        swprintf_s(typeName, typeNameLen, L"char\0");
        break;
    case ELEMENT_TYPE_I1:
        swprintf_s(typeName, typeNameLen, L"signed byte\0");
        break;
    case ELEMENT_TYPE_U1:
        swprintf_s(typeName, typeNameLen, L"byte\0");
        break;
    case ELEMENT_TYPE_I2:
        swprintf_s(typeName, typeNameLen, L"short\0");
        break;
    case ELEMENT_TYPE_U2:
        swprintf_s(typeName, typeNameLen, L"unsigned short\0");
        break;    
    case ELEMENT_TYPE_I4:
        swprintf_s(typeName, typeNameLen, L"int\0");
        break;
    case ELEMENT_TYPE_U4:
        swprintf_s(typeName, typeNameLen, L"unsigned int\0");
        break;
    case ELEMENT_TYPE_I8:
        swprintf_s(typeName, typeNameLen, L"long\0");
        break;
    case ELEMENT_TYPE_U8:
        swprintf_s(typeName, typeNameLen, L"unsigned long\0");
        break;
    case ELEMENT_TYPE_R4:
        swprintf_s(typeName, typeNameLen, L"float\0");
        break;
    case ELEMENT_TYPE_R8:
        swprintf_s(typeName, typeNameLen, L"double\0");
        break;
    case ELEMENT_TYPE_OBJECT:
        swprintf_s(typeName, typeNameLen, L"object\0");
        break;
    case ELEMENT_TYPE_STRING:
        swprintf_s(typeName, typeNameLen, L"string\0");
        break;
    case ELEMENT_TYPE_I:
        swprintf_s(typeName, typeNameLen, L"IntPtr\0");
        break;
    case ELEMENT_TYPE_U:
        swprintf_s(typeName, typeNameLen, L"UIntPtr\0");
        break;
    case ELEMENT_TYPE_SZARRAY:
    case ELEMENT_TYPE_ARRAY:
    case ELEMENT_TYPE_BYREF:
    case ELEMENT_TYPE_PTR:
        {
            // get a name for the type we are building from
            ToRelease<ICorDebugType> pFirstParameter;
            if(SUCCEEDED(pType->GetFirstTypeParameter(&pFirstParameter)))
                CalculateTypeName(pFirstParameter, typeName, typeNameLen);
            else
                swprintf_s(typeName, typeNameLen, L"<unknown>\0");

            // append the appropriate [], *, &
            switch(corElemType)
            {
            case ELEMENT_TYPE_SZARRAY: 
                wcsncat_s(typeName, typeNameLen, L"[]", typeNameLen);
                return S_OK;
            case ELEMENT_TYPE_ARRAY:
                {
                    ULONG32 rank = 0;
                    pType->GetRank(&rank);
                    wcsncat_s(typeName, typeNameLen, L"[", typeNameLen);
                    for(ULONG32 i = 0; i < rank - 1; i++)
                    {
                        // todo- could we print out exact boundaries?
                        wcsncat_s(typeName, typeNameLen, L",", typeNameLen);
                    }
                    wcsncat_s(typeName, typeNameLen, L"]", typeNameLen);
                }
                return S_OK;
            case ELEMENT_TYPE_BYREF:   
                wcsncat_s(typeName, typeNameLen, L"&", typeNameLen);
                return S_OK;
            case ELEMENT_TYPE_PTR:     
                wcsncat_s(typeName, typeNameLen, L"*", typeNameLen);
                return S_OK;
            }
        }
        break;
    case ELEMENT_TYPE_FNPTR:
        swprintf_s(typeName, typeNameLen, L"*(...)");
        break;
    case ELEMENT_TYPE_TYPEDBYREF:
        swprintf_s(typeName, typeNameLen, L"typedbyref");
        break;
    }
    return S_OK;
}


// Appends angle brackets and the generic argument list to a type name
HRESULT ExpressionNode::AddGenericArgs(ICorDebugType * pType, __inout_ecount(typeNameLen) WCHAR* typeName, DWORD typeNameLen)
{
    bool isFirst = true;
    ToRelease<ICorDebugTypeEnum> pTypeEnum;
    if(SUCCEEDED(pType->EnumerateTypeParameters(&pTypeEnum)))
    {
        ULONG numTypes = 0;
        ToRelease<ICorDebugType> pCurrentTypeParam;

        while(SUCCEEDED(pTypeEnum->Next(1, &pCurrentTypeParam, &numTypes)))
        {
            if(numTypes == 0) break;

            if(isFirst)
            {
                isFirst = false;
                wcsncat_s(typeName, typeNameLen, L"<", typeNameLen);
            }
            else wcsncat_s(typeName, typeNameLen, L",", typeNameLen);

            WCHAR typeParamName[mdNameLen];
            typeParamName[0] = L'\0';
            CalculateTypeName(pCurrentTypeParam, typeParamName, mdNameLen);
            wcsncat_s(typeName, typeNameLen, typeParamName, typeNameLen);
        }
        if(!isFirst)
            wcsncat_s(typeName, typeNameLen, L">", typeNameLen);
    }

    return S_OK;
}

// Determines the text name for the type of this node and caches it
HRESULT ExpressionNode::PopulateType()
{
    HRESULT Status = S_OK;
    if(pTypeName[0] != 0)
        return S_OK;

    //default value
    swprintf_s(pTypeName, MAX_EXPRESSION, L"<unknown>");

    // if we are displaying this type as a specific sub-type, use that
    if(pTypeCast != NULL)
        return CalculateTypeName(pTypeCast, pTypeName, MAX_EXPRESSION);

    // if there is no value then either we succesfully already determined the type
    // name, or this node has no value or type and thus no type name.
    if(pValue == NULL)
        return S_OK;

    // get the type from the value and then calculate a name based on that
    ToRelease<ICorDebugType> pType;
    ToRelease<ICorDebugValue2> pValue2;
    if(SUCCEEDED(pValue->QueryInterface(IID_ICorDebugValue2, (void**) &pValue2)) && SUCCEEDED(pValue2->GetExactType(&pType)))
        return CalculateTypeName(pType, pTypeName, MAX_EXPRESSION);

    return S_OK;
}

// Node expansion helpers

// Inserts a new child at the end of the linked list of children
// PERF: This has O(N) insert time but these lists should never be large
VOID ExpressionNode::AddChild(ExpressionNode* pNewChild)
{
    if(pChild == NULL)
        pChild = pNewChild;
    else
    {
        ExpressionNode* pCursor = pChild;
        while(pCursor->pNextSibling != NULL)
            pCursor = pCursor->pNextSibling;
        pCursor->pNextSibling = pNewChild;
    }
}

// Helper that determines if the current node is on the path of nodes represented by
// expression varToExpand
BOOL ExpressionNode::ShouldExpandVariable(__in_z WCHAR* varToExpand)
{
    if(pAbsoluteExpression == NULL || varToExpand == NULL) return FALSE;

    // if there is a cast operation, move past it
    WCHAR* pEndCast = wcschr(varToExpand, L')');
    varToExpand = (pEndCast == NULL) ? varToExpand : pEndCast+1; 

    size_t varToExpandLen = wcslen(varToExpand);
    size_t currentExpansionLen = wcslen(pAbsoluteExpression);
    if(currentExpansionLen > varToExpandLen) return FALSE;
    if(currentExpansionLen < varToExpandLen && 
        varToExpand[currentExpansionLen] != L'.' &&
        varToExpand[currentExpansionLen] != L'[')
        return FALSE;
    if(wcsncmp(pAbsoluteExpression, varToExpand, currentExpansionLen) != 0) return FALSE;

    return TRUE;
}

// Expands this array node by creating child nodes with expressions refering to individual array elements
HRESULT ExpressionNode::ExpandSzArray(ICorDebugValue* pInnerValue, __in_z WCHAR* varToExpand)
{
    HRESULT Status = S_OK;
    ToRelease<ICorDebugArrayValue> pArrayValue;
    IfFailRet(pInnerValue->QueryInterface(IID_ICorDebugArrayValue, (LPVOID*) &pArrayValue));

    ULONG32 nRank;
    IfFailRet(pArrayValue->GetRank(&nRank));
    if (nRank != 1)
    {
        _snwprintf_s(pErrorMessage, MAX_ERROR, _TRUNCATE, L"Multi-dimensional arrays NYI");
        return E_UNEXPECTED;
    }

    ULONG32 cElements;
    IfFailRet(pArrayValue->GetCount(&cElements));

    //TODO: do we really want all the elements? This could be huge!
    for (ULONG32 i=0; i < cElements; i++)
    {
        WCHAR index[20];
        swprintf_s(index, 20, L"%d", i);

        ToRelease<ICorDebugValue> pElementValue;
        IfFailRet(pArrayValue->GetElementAtPosition(i, &pElementValue));
        ExpressionNode* pExpr = new ExpressionNode(pAbsoluteExpression, ChildKind_Index, index, pElementValue, NULL, pILFrame);
        AddChild(pExpr);
        pExpr->Expand(varToExpand);
    }
    return S_OK;
}

// Expands this struct/class node by creating child nodes with expressions refering to individual field values
// and one node for the basetype value
HRESULT ExpressionNode::ExpandFields(ICorDebugValue* pInnerValue, __in_z WCHAR* varToExpand)
{
    HRESULT Status = S_OK;

    mdTypeDef currentTypeDef;
    ToRelease<ICorDebugClass> pClass;
    ToRelease<ICorDebugType> pType;
    ToRelease<ICorDebugModule> pModule;
    if(pTypeCast == NULL)
    {
        ToRelease<ICorDebugValue2> pValue2;
        IfFailRet(pInnerValue->QueryInterface(IID_ICorDebugValue2, (LPVOID *) &pValue2));
        IfFailRet(pValue2->GetExactType(&pType));
    }
    else
    {
        pType = pTypeCast;
        pType->AddRef();
    }
    IfFailRet(pType->GetClass(&pClass));
    IfFailRet(pClass->GetModule(&pModule));
    IfFailRet(pClass->GetToken(&currentTypeDef));

    ToRelease<IUnknown> pMDUnknown;
    ToRelease<IMetaDataImport> pMD;
    IfFailRet(pModule->GetMetaDataInterface(IID_IMetaDataImport, &pMDUnknown));
    IfFailRet(pMDUnknown->QueryInterface(IID_IMetaDataImport, (LPVOID*) &pMD));

    // If the current type has a base type that isn't object, enum, or ValueType then add a node for the base type
    WCHAR baseTypeName[mdNameLen] = L"\0";
    ToRelease<ICorDebugType> pBaseType;
    ExpressionNode* pBaseTypeNode = NULL;
    if(SUCCEEDED(pType->GetBase(&pBaseType)) && pBaseType != NULL && SUCCEEDED(CalculateTypeName(pBaseType, baseTypeName, mdNameLen)))
    {
        if(wcsncmp(baseTypeName, L"System.Enum", 11) == 0)
            return S_OK;
        else if(wcsncmp(baseTypeName, L"System.Object", 13) != 0 && wcsncmp(baseTypeName, L"System.ValueType", 16) != 0)
        {
            pBaseTypeNode = new ExpressionNode(pAbsoluteExpression, ChildKind_BaseClass, L"<baseclass>", pInnerValue, pBaseType, pILFrame);
            AddChild(pBaseTypeNode);
        }
    }

    // add nodes for all the fields in this object
    ULONG numFields = 0;
    HCORENUM fEnum = NULL;
    mdFieldDef fieldDef;
    BOOL fieldExpanded = FALSE;
    while(SUCCEEDED(pMD->EnumFields(&fEnum, currentTypeDef, &fieldDef, 1, &numFields)) && numFields != 0)
    {
        mdTypeDef         classDef = 0;
        ULONG             nameLen = 0;
        DWORD             fieldAttr = 0;
        WCHAR             mdName[mdNameLen];
        WCHAR             typeName[mdNameLen];
        CorElementType    fieldDefaultValueEt;
        UVCP_CONSTANT     pDefaultValue;
        ULONG             cchDefaultValue;
        if(SUCCEEDED(pMD->GetFieldProps(fieldDef, &classDef, mdName, mdNameLen, &nameLen, &fieldAttr, NULL, NULL, (DWORD*)&fieldDefaultValueEt, &pDefaultValue, &cchDefaultValue)))
        {
            ToRelease<ICorDebugType> pFieldType;
            ToRelease<ICorDebugValue> pFieldVal;

            // static fields (of any kind - AppDomain, thread, context, RVA)
            if (fieldAttr & fdStatic)
            {
                pType->GetStaticFieldValue(fieldDef, pILFrame, &pFieldVal);
            } 
            // non-static fields on an object instance
            else if(pInnerValue != NULL)
            {
                ToRelease<ICorDebugObjectValue> pObjValue;
                if (SUCCEEDED(pInnerValue->QueryInterface(IID_ICorDebugObjectValue, (LPVOID*) &pObjValue)))
                    pObjValue->GetFieldValue(pClass, fieldDef, &pFieldVal);
            }
            // skip over non-static fields on static types
            else
            {
                continue; 
            }

            // we didn't get a value yet and there is default value available
            // need to calculate the type because there won't be a ICorDebugValue to derive it from
            if(pFieldVal == NULL && pDefaultValue != NULL)
            {
                FindTypeFromElementType(fieldDefaultValueEt, &pFieldType);
            }

            ExpressionNode* pNewChildNode = new ExpressionNode(pAbsoluteExpression, ChildKind_Field, mdName, pFieldVal, pFieldType, pILFrame, pDefaultValue, cchDefaultValue);
            AddChild(pNewChildNode);
            if(pNewChildNode->Expand(varToExpand) != S_FALSE)
                fieldExpanded = TRUE;
        }
    }
    pMD->CloseEnum(fEnum);

    // Only recurse to expand the base type if all of these hold:
    // 1) base type exists
    // 2) no field was expanded
    // 3) the non-casting portion of the varToExpand doesn't match the current expression
    //    OR the cast exists and doesn't match

    if(pBaseTypeNode == NULL) return Status;
    if(fieldExpanded) return Status;

    WCHAR* pEndCast = wcschr(varToExpand, L')');
    WCHAR* pNonCast = (pEndCast == NULL) ? varToExpand : pEndCast+1;
    if(wcscmp(pNonCast, pAbsoluteExpression) != 0)
    {
        pBaseTypeNode->Expand(varToExpand);
        return Status;
    }

    if(varToExpand[0] == L'(' && pEndCast != NULL)
    {
        int cchCastTypeName = ((int)(pEndCast-1)-(int)varToExpand)/2;
        PopulateType();
        if(wcslen(pTypeName) != (cchCastTypeName) ||
            wcsncmp(varToExpand+1, pTypeName, cchCastTypeName) != 0)
        {
            pBaseTypeNode->Expand(varToExpand);
            return Status;
        }
    }

    return Status;
}


// Value Population functions

//Helper for unwrapping values
HRESULT ExpressionNode::DereferenceAndUnboxValue(ICorDebugValue * pInputValue, ICorDebugValue** ppOutputValue, BOOL * pIsNull)
{
    HRESULT Status = S_OK;
    *ppOutputValue = NULL;
    if(pIsNull != NULL) *pIsNull = FALSE;

    ToRelease<ICorDebugReferenceValue> pReferenceValue;
    Status = pInputValue->QueryInterface(IID_ICorDebugReferenceValue, (LPVOID*) &pReferenceValue);
    if (SUCCEEDED(Status))
    {
        BOOL isNull = FALSE;
        IfFailRet(pReferenceValue->IsNull(&isNull));
        if(!isNull)
        {
            ToRelease<ICorDebugValue> pDereferencedValue;
            IfFailRet(pReferenceValue->Dereference(&pDereferencedValue));
            return DereferenceAndUnboxValue(pDereferencedValue, ppOutputValue);
        }
        else
        {
            if(pIsNull != NULL) *pIsNull = TRUE;
            *ppOutputValue = pInputValue;
            (*ppOutputValue)->AddRef();
            return S_OK;
        }
    }

    ToRelease<ICorDebugBoxValue> pBoxedValue;
    Status = pInputValue->QueryInterface(IID_ICorDebugBoxValue, (LPVOID*) &pBoxedValue);
    if (SUCCEEDED(Status))
    {
        ToRelease<ICorDebugObjectValue> pUnboxedValue;
        IfFailRet(pBoxedValue->GetObject(&pUnboxedValue));
        return DereferenceAndUnboxValue(pUnboxedValue, ppOutputValue);
    }
    *ppOutputValue = pInputValue;
    (*ppOutputValue)->AddRef();
    return S_OK;
}

// Returns TRUE if the value derives from System.Enum
BOOL ExpressionNode::IsEnum(ICorDebugValue * pInputValue)
{
    ToRelease<ICorDebugValue> pValue;
    if(FAILED(DereferenceAndUnboxValue(pInputValue, &pValue, NULL))) return FALSE;

    WCHAR baseTypeName[mdNameLen];
    ToRelease<ICorDebugValue2> pValue2;
    ToRelease<ICorDebugType> pType;
    ToRelease<ICorDebugType> pBaseType;

    if(FAILED(pValue->QueryInterface(IID_ICorDebugValue2, (LPVOID *) &pValue2))) return FALSE;
    if(FAILED(pValue2->GetExactType(&pType))) return FALSE;
    if(FAILED(pType->GetBase(&pBaseType)) || pBaseType == NULL) return FALSE;
    if(FAILED(CalculateTypeName(pBaseType, baseTypeName, mdNameLen))) return  FALSE;

    return (wcsncmp(baseTypeName, L"System.Enum", 11) == 0);
}

// Calculates the value text for nodes that have enum values
HRESULT ExpressionNode::PopulateEnumValue(ICorDebugValue* pEnumValue, BYTE* enumValue)
{
    HRESULT Status = S_OK;

    mdTypeDef currentTypeDef;
    ToRelease<ICorDebugClass> pClass;
    ToRelease<ICorDebugValue2> pValue2;
    ToRelease<ICorDebugType> pType;
    ToRelease<ICorDebugModule> pModule;
    IfFailRet(pEnumValue->QueryInterface(IID_ICorDebugValue2, (LPVOID *) &pValue2));
    IfFailRet(pValue2->GetExactType(&pType));
    IfFailRet(pType->GetClass(&pClass));
    IfFailRet(pClass->GetModule(&pModule));
    IfFailRet(pClass->GetToken(&currentTypeDef));

    ToRelease<IUnknown> pMDUnknown;
    ToRelease<IMetaDataImport> pMD;
    IfFailRet(pModule->GetMetaDataInterface(IID_IMetaDataImport, &pMDUnknown));
    IfFailRet(pMDUnknown->QueryInterface(IID_IMetaDataImport, (LPVOID*) &pMD));


    //First, we need to figure out the underlying enum type so that we can correctly type cast the raw values of each enum constant
    //We get that from the non-static field of the enum variable (I think the field is called __value or something similar)
    ULONG numFields = 0;
    HCORENUM fEnum = NULL;
    mdFieldDef fieldDef;
    CorElementType enumUnderlyingType = ELEMENT_TYPE_END;
    while(SUCCEEDED(pMD->EnumFields(&fEnum, currentTypeDef, &fieldDef, 1, &numFields)) && numFields != 0)
    {
        DWORD             fieldAttr = 0;
        PCCOR_SIGNATURE   pSignatureBlob = NULL;
        ULONG             sigBlobLength = 0;
        if(SUCCEEDED(pMD->GetFieldProps(fieldDef, NULL, NULL, 0, NULL, &fieldAttr, &pSignatureBlob, &sigBlobLength, NULL, NULL, NULL)))
        {
            if((fieldAttr & fdStatic) == 0)
            {
                CorSigUncompressCallingConv(pSignatureBlob);
                enumUnderlyingType = CorSigUncompressElementType(pSignatureBlob);
                break;
            }
        }
    }
    pMD->CloseEnum(fEnum);


    //Now that we know the underlying enum type, let's decode the enum variable into OR-ed, human readable enum contants
    fEnum = NULL;
    bool isFirst = true;
    ULONG64 remainingValue = *((ULONG64*)enumValue);
    WCHAR* pTextValueCursor = pTextValue;
    DWORD cchTextValueCursor = MAX_EXPRESSION;
    while(SUCCEEDED(pMD->EnumFields(&fEnum, currentTypeDef, &fieldDef, 1, &numFields)) && numFields != 0)
    {
        ULONG             nameLen = 0;
        DWORD             fieldAttr = 0;
        WCHAR             mdName[mdNameLen];
        WCHAR             typeName[mdNameLen];
        UVCP_CONSTANT     pRawValue = NULL;
        ULONG             rawValueLength = 0;
        if(SUCCEEDED(pMD->GetFieldProps(fieldDef, NULL, mdName, mdNameLen, &nameLen, &fieldAttr, NULL, NULL, NULL, &pRawValue, &rawValueLength)))
        {
            DWORD enumValueRequiredAttributes = fdPublic | fdStatic | fdLiteral | fdHasDefault;
            if((fieldAttr & enumValueRequiredAttributes) != enumValueRequiredAttributes)
                continue;

            ULONG64 currentConstValue = 0;
            switch (enumUnderlyingType)
            {
            case ELEMENT_TYPE_CHAR:
            case ELEMENT_TYPE_I1:
                currentConstValue = (ULONG64)(*((CHAR*)pRawValue));
                break;
            case ELEMENT_TYPE_U1:
                currentConstValue = (ULONG64)(*((BYTE*)pRawValue));
                break;
            case ELEMENT_TYPE_I2:
                currentConstValue = (ULONG64)(*((SHORT*)pRawValue));
                break;
            case ELEMENT_TYPE_U2:
                currentConstValue = (ULONG64)(*((USHORT*)pRawValue));
                break;
            case ELEMENT_TYPE_I4:
                currentConstValue = (ULONG64)(*((INT32*)pRawValue));
                break;
            case ELEMENT_TYPE_U4:
                currentConstValue = (ULONG64)(*((UINT32*)pRawValue));
                break;
            case ELEMENT_TYPE_I8:
                currentConstValue = (ULONG64)(*((LONG*)pRawValue));
                break;
            case ELEMENT_TYPE_U8:
                currentConstValue = (ULONG64)(*((ULONG*)pRawValue));
                break;
            case ELEMENT_TYPE_I:
                currentConstValue = (ULONG64)(*((int*)pRawValue));
                break;
            case ELEMENT_TYPE_U:
            case ELEMENT_TYPE_R4:
            case ELEMENT_TYPE_R8:
                // Technically U and the floating-point ones are options in the CLI, but not in the CLS or C#, so these are NYI
            default:
                currentConstValue = 0;
            }

            if((currentConstValue == remainingValue) || ((currentConstValue != 0) && ((currentConstValue & remainingValue) == currentConstValue)))
            {
                remainingValue &= ~currentConstValue;
                DWORD charsCopied = 0;
                if(isFirst)
                {
                    charsCopied = _snwprintf_s(pTextValueCursor, cchTextValueCursor, _TRUNCATE, L"= %s", mdName);
                    isFirst = false;
                }
                else 
                    charsCopied = _snwprintf_s(pTextValueCursor, cchTextValueCursor, _TRUNCATE, L" | %s", mdName);

                // if an error or truncation occured, stop copying
                if(charsCopied == -1)
                {
                    cchTextValueCursor = 0;
                    pTextValueCursor = NULL;
                }
                else
                {
                    // charsCopied is the number of characters copied, not counting the terminating null
                    // this advances the cursor to point right at the terminating null so that future copies
                    // will concatenate the string
                    pTextValueCursor += charsCopied;
                    cchTextValueCursor -= charsCopied;
                }
            }
        }
    }
    pMD->CloseEnum(fEnum);

    return Status;
}

// Helper that caches the textual value for nodes that evaluate to a string object
HRESULT ExpressionNode::GetDebuggeeStringValue(ICorDebugValue* pInputValue, __inout_ecount(cchBuffer) WCHAR* wszBuffer, DWORD cchBuffer)
{
    HRESULT Status;

    ToRelease<ICorDebugStringValue> pStringValue;
    IfFailRet(pInputValue->QueryInterface(IID_ICorDebugStringValue, (LPVOID*) &pStringValue));

    ULONG32 cchValueReturned;
    IfFailRet(pStringValue->GetString(cchBuffer, &cchValueReturned, wszBuffer));

    return S_OK;
}

// Retrieves the string value for a constant
HRESULT ExpressionNode::GetConstantStringValue(__inout_ecount(cchBuffer) WCHAR* wszBuffer, DWORD cchBuffer)
{
    // The string encoded in metadata isn't null-terminated
    // so we need to copy it to a null terminated buffer
    DWORD copyLen = cchDefaultValue;
    if(copyLen > cchBuffer-1)
        copyLen = cchDefaultValue;

    wcsncpy_s(wszBuffer, cchBuffer, (WCHAR*)pDefaultValue, copyLen);
    return S_OK;
}

// Helper that caches the textual value for nodes that evaluate to array objects
HRESULT ExpressionNode::PopulateSzArrayValue(ICorDebugValue* pInputValue)
{
    HRESULT Status = S_OK;

    ToRelease<ICorDebugArrayValue> pArrayValue;
    IfFailRet(pInputValue->QueryInterface(IID_ICorDebugArrayValue, (LPVOID*) &pArrayValue));

    ULONG32 nRank;
    IfFailRet(pArrayValue->GetRank(&nRank));
    if (nRank != 1)
    {
        _snwprintf_s(pErrorMessage, MAX_EXPRESSION, _TRUNCATE, L"Multi-dimensional arrays NYI");
        return E_UNEXPECTED;
    }

    ULONG32 cElements;
    IfFailRet(pArrayValue->GetCount(&cElements));

    if (cElements == 0)
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"(empty)");
    else if (cElements == 1) 
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"(1 element)");
    else  
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"(%d elements)", cElements);

    return S_OK;
}

// Helper that caches the textual value for nodes of any type
HRESULT ExpressionNode::PopulateTextValueHelper()
{
    HRESULT Status = S_OK;

    BOOL isNull = TRUE;
    ToRelease<ICorDebugValue> pInnerValue;
    CorElementType corElemType;
    ULONG32 cbSize = 0;
    if(pValue != NULL)
    {
        IfFailRet(DereferenceAndUnboxValue(pValue, &pInnerValue, &isNull));

        if(isNull)
        {
            _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= null");
            return S_OK;
        }
        IfFailRet(pInnerValue->GetSize(&cbSize));
        IfFailRet(pInnerValue->GetType(&corElemType));
    }
    else if(pDefaultValue != NULL)
    {
        if(pTypeCast == NULL)
        {
            // this shouldn't happen, but just print nothing if it does
            return S_OK;
        }
        // This works around an irritating issue in ICorDebug. For default values
        // we have to construct the ICorDebugType ourselves, however ICorDebug
        // doesn't allow type construction using the correct element types. The
        // caller must past CLASS or VALUETYPE even when a more specific short
        // form element type is applicable. That means that later, here, we get
        // back the wrong answer. To work around this we format the type as a
        // string, and check it against all the known types. That allows us determine
        // everything except VALUETYPE/CLASS. Thankfully that distinction is the
        // one piece of data ICorDebugType will tell us if needed.
        if(FAILED(GetCanonicalElementTypeForTypeName(GetTypeName(), &corElemType)))
        {
            pTypeCast->GetType(&corElemType);
        }

        switch(corElemType)
        {
        case ELEMENT_TYPE_BOOLEAN:
        case ELEMENT_TYPE_I1:
        case ELEMENT_TYPE_U1:
            cbSize = 1;
            break;

        case ELEMENT_TYPE_CHAR:
        case ELEMENT_TYPE_I2:
        case ELEMENT_TYPE_U2:
            cbSize = 2;
            break;

        case ELEMENT_TYPE_I:
        case ELEMENT_TYPE_U:
        case ELEMENT_TYPE_I4:
        case ELEMENT_TYPE_U4:
        case ELEMENT_TYPE_R4:
            cbSize = 4;
            break;

        case ELEMENT_TYPE_I8:
        case ELEMENT_TYPE_U8:
        case ELEMENT_TYPE_R8:
            cbSize = 8;
            break;
        }
    }

    if (corElemType == ELEMENT_TYPE_STRING)
    {
        WCHAR buffer[MAX_EXPRESSION];
        buffer[0] = L'\0';
        if(pInnerValue != NULL)
            GetDebuggeeStringValue(pInnerValue, buffer, MAX_EXPRESSION);
        else
            GetConstantStringValue(buffer, MAX_EXPRESSION);
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= \"%s\"", buffer);
    }
    else if (corElemType == ELEMENT_TYPE_SZARRAY)
    {
        return PopulateSzArrayValue(pInnerValue);
    }


    ArrayHolder<BYTE> rgbValue = new BYTE[cbSize];
    memset(rgbValue.GetPtr(), 0, cbSize * sizeof(BYTE));
    if(pInnerValue != NULL)
    {
        ToRelease<ICorDebugGenericValue> pGenericValue;
        IfFailRet(pInnerValue->QueryInterface(IID_ICorDebugGenericValue, (LPVOID*) &pGenericValue));
        IfFailRet(pGenericValue->GetValue((LPVOID) &(rgbValue[0])));
    }
    else
    {
        memcpy((LPVOID) &(rgbValue[0]), pDefaultValue, cbSize);
    }

    //TODO: this should really be calculated from the type
    if(pInnerValue != NULL && IsEnum(pInnerValue))
    {
        Status = PopulateEnumValue(pInnerValue, rgbValue);
        return Status;
    }

    switch (corElemType)
    {
    default:
        _snwprintf_s(pErrorMessage, MAX_ERROR, _TRUNCATE, L"Unhandled CorElementType: 0x%x", corElemType);
        Status = E_FAIL;
        break;

    case ELEMENT_TYPE_PTR:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"<pointer>");
        break;

    case ELEMENT_TYPE_FNPTR:
        {
            CORDB_ADDRESS addr = 0;
            ToRelease<ICorDebugReferenceValue> pReferenceValue = NULL;
            if(SUCCEEDED(pInnerValue->QueryInterface(IID_ICorDebugReferenceValue, (LPVOID*) &pReferenceValue)))
                pReferenceValue->GetValue(&addr);
            _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"<function pointer 0x%x>", addr);
        }
        break;

    case ELEMENT_TYPE_VALUETYPE:
    case ELEMENT_TYPE_CLASS:
    case ELEMENT_TYPE_OBJECT:
        ULONG64 pointer;
        if(pInnerValue != NULL && SUCCEEDED(pInnerValue->GetAddress(&pointer)))
            _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"@ 0x%p", pointer);
        break;

    case ELEMENT_TYPE_BOOLEAN:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= %s", rgbValue[0] == 0 ? L"false" : L"true");
        break;

    case ELEMENT_TYPE_CHAR:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= '%C'", *(WCHAR *) &(rgbValue[0]));
        break;

    case ELEMENT_TYPE_I1:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= %d", *(char*) &(rgbValue[0]));
        break;

    case ELEMENT_TYPE_U1:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= %d", *(unsigned char*) &(rgbValue[0]));
        break;

    case ELEMENT_TYPE_I2:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= %hd", *(short*) &(rgbValue[0]));
        break;

    case ELEMENT_TYPE_U2:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= %hu", *(unsigned short*) &(rgbValue[0]));
        break;

    case ELEMENT_TYPE_I:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= %d", *(int*) &(rgbValue[0]));
        break;

    case ELEMENT_TYPE_U:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= %u", *(unsigned int*) &(rgbValue[0]));
        break;

    case ELEMENT_TYPE_I4:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= %d", *(int*) &(rgbValue[0]));
        break;

    case ELEMENT_TYPE_U4:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= %u", *(unsigned int*) &(rgbValue[0]));
        break;

    case ELEMENT_TYPE_I8:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= %I64d", *(__int64*) &(rgbValue[0]));
        break;

    case ELEMENT_TYPE_U8:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= %I64u", *(unsigned __int64*) &(rgbValue[0]));
        break;

    case ELEMENT_TYPE_R4:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"= %f", (double) *(float*) &(rgbValue[0]));
        break;

    case ELEMENT_TYPE_R8:
        _snwprintf_s(pTextValue, MAX_EXPRESSION, _TRUNCATE, L"%f", *(double*) &(rgbValue[0]));
        break;

        // TODO: The following corElementTypes are not yet implemented here.  Array
        // might be interesting to add, though the others may be of rather limited use:
        // ELEMENT_TYPE_ARRAY          = 0x14,     // MDARRAY <type> <rank> <bcount> <bound1> ... <lbcount> <lb1> ...
        // 
        // ELEMENT_TYPE_GENERICINST    = 0x15,     // GENERICINST <generic type> <argCnt> <arg1> ... <argn>
    }

    return Status;
}

// Caches the textual value of this node
HRESULT ExpressionNode::PopulateTextValue()
{
    if(pErrorMessage[0] != 0)
        return E_UNEXPECTED;
    if(pValue == NULL && pDefaultValue == NULL)
        return S_OK;
    HRESULT Status = PopulateTextValueHelper();
    if(FAILED(Status) && pErrorMessage[0] == 0)
    {
        _snwprintf_s(pErrorMessage, MAX_ERROR, _TRUNCATE, L"Error in PopulateTextValueHelper: 0x%x", Status);
    }
    return Status;
}


// Expression parsing and search

//Callback that searches a frame to determine if it contains a local variable or parameter of a given name
VOID ExpressionNode::EvaluateExpressionFrameScanCallback(ICorDebugFrame* pFrame, VOID* pUserData)
{
    EvaluateExpressionFrameScanData* pData = (EvaluateExpressionFrameScanData*)pUserData;

    // we already found what we were looking for, just continue
    if(pData->pFoundValue != NULL)
        return;

    // if any of these fail we just continue on
    // querying for ILFrame will frequently fail because many frames aren't IL
    ToRelease<ICorDebugILFrame> pILFrame;
    HRESULT Status = pFrame->QueryInterface(IID_ICorDebugILFrame, (LPVOID*) &pILFrame);
    if (FAILED(Status))
    {
        return;
    }
    // we need to save off the first frame we find regardless of whether we find the
    // local or not. We might need this frame later for static field lookup.
    if(pData->pFirstFrame == NULL)
    {
        pData->pFirstFrame = pILFrame;
        pData->pFirstFrame->AddRef();
    }
    // not all IL frames map to an assembly (ex. LCG)
    ToRelease<ICorDebugFunction> pFunction;
    Status = pFrame->GetFunction(&pFunction);
    if (FAILED(Status))
    {
        return;
    }
    // from here down shouldn't generally fail, but just in case
    mdMethodDef methodDef;
    Status = pFunction->GetToken(&methodDef);
    if (FAILED(Status))
    {
        return;
    }
    ToRelease<ICorDebugModule> pModule;
    Status = pFunction->GetModule(&pModule);
    if (FAILED(Status))
    {
        return;
    }
    ToRelease<IUnknown> pMDUnknown;
    Status = pModule->GetMetaDataInterface(IID_IMetaDataImport, &pMDUnknown);
    if (FAILED(Status))
    {
        return;
    }
    ToRelease<IMetaDataImport> pMD;
    Status = pMDUnknown->QueryInterface(IID_IMetaDataImport, (LPVOID*) &pMD);
    if (FAILED(Status))
    {
        return;
    }

    pData->pFoundFrame = pILFrame;
    pData->pFoundFrame->AddRef();
    // Enumerate all the parameters
    EnumerateParameters(pMD, methodDef, pILFrame, EvaluateExpressionVariableScanCallback, pUserData);
    // Enumerate all the locals
    EnumerateLocals(pMD, methodDef, pILFrame, EvaluateExpressionVariableScanCallback, pUserData);

    // if we didn't find it in this frame then clear the frame back out
    if(pData->pFoundValue == NULL)
    {
        pData->pFoundFrame = NULL;
    }

    return;
}

//Callback checks to see if a given local/parameter has name pName
VOID ExpressionNode::EvaluateExpressionVariableScanCallback(ICorDebugValue* pValue, __in_z WCHAR* pName, __out_z WCHAR* pErrorMessage, VOID* pUserData)
{
    EvaluateExpressionFrameScanData* pData = (EvaluateExpressionFrameScanData*)pUserData;
    if(wcscmp(pName, pData->pIdentifier) == 0)
    {
        // found it
        pData->pFoundValue = pValue;
        pValue->AddRef();
    }
    return;
}

//Factory method that recursively parses pExpression and create an ExpressionNode
//  pExpression -          the entire expression being parsed
//  pExpressionRemainder - the portion of the expression that remains to be parsed in this
//                         recursive invocation
//  charactersParsed     - the number of characters that have been parsed from pExpression
//                         so far (todo: this is basically the difference between remainder and
//                         full expression, do we need it?)
//  pParsedValue         - A debuggee value that should be used as the context for interpreting
//                         pExpressionRemainder
//  pParsedType          - A debuggee type that should be used as the context for interpreting
//                         pExpressionRemainder. 
//  pParsedDefaultValue  - A fixed value from metadata that should be used as context for
//                         interpretting pExpressionRemainder
//  cchParsedDefaultValue- Size of pParsedDefaultValue
//  pFrame               - A debuggee IL frame that disambiguates the thread and context needed
//                         to evaluate a thread-static or context-static value
//  ppExpressionNode     - OUT - the resulting expression node
//
//
//  Valid combinations of state comming into this method:
//      The expression up to charactersParsed isn't recognized yet:
//           pParsedValue = pParsedType = pParsedDefaultValue = NULL
//           cchParsedDefaultValue = 0
//      The expression up to charactersParsed is a recognized type:
//           pParsedType = <parsed type>
//           pParsedValue = pParsedDefaultValue = NULL
//           cchParsedDefaultValue = 0
//      The expression up to charactersParsed is a recognized value in the debuggee:
//           pParsedValue = <parsed value>
//           pParsedType = pParsedDefaultValue = NULL
//           cchParsedDefaultValue = 0
//      The expression up to charactersParsed is a recognized default value stored in metadata:
//           pParsedValue = NULL
//           pParsedType = <type calculated from metadata>
//           pParsedDefaultValue = <value from metadata>
//           cchParsedDefaultValue = <size of metadata value>
//
//
// REFACTORING NOTE: This method is very similar (but not identical) to the expansion logic
//                   in ExpressionNode. The primary difference is that the nodes expand all
//                   fields/indices whereas this function only expands along a precise route.
//                   If the ExpressionNode code where enhanced to support expanding precisely
//                   large portions of this function could be disposed of. As soon as the function
//                   matched the initial name it could create an ExpressionNode and then use the
//                   ExpressionNode expansion functions to drill down to the actual node required.
//                   Also need to make sure the nodes manage lifetime ok when a parent is destroyed
//                   but a child node is still referenced.
HRESULT ExpressionNode::CreateExpressionNodeHelper(__in_z WCHAR* pExpression,
                                                          __in_z WCHAR* pExpressionParseRemainder,
                                                          DWORD charactersParsed,
                                                          ICorDebugValue* pParsedValue,
                                                          ICorDebugType* pParsedType,
                                                          UVCP_CONSTANT pParsedDefaultValue,
                                                          ULONG cchParsedDefaultValue,
                                                          ICorDebugILFrame* pFrame,
                                                          ExpressionNode** ppExpressionNode)
{
    HRESULT Status = S_OK;
    WCHAR* pExpressionCursor = pExpressionParseRemainder;
    DWORD currentCharsParsed = charactersParsed;
    WCHAR pIdentifier[mdNameLen];
    pIdentifier[0] = 0;
    BOOL isArray = FALSE;
    WCHAR pResultBuffer[MAX_EXPRESSION];

    // Get the next name from the expression string
    if(FAILED(Status = ParseNextIdentifier(&pExpressionCursor, pIdentifier, mdNameLen, pResultBuffer, MAX_EXPRESSION, &currentCharsParsed, &isArray)))
    {
        *ppExpressionNode = new ExpressionNode(pExpression, pResultBuffer);
        if(*ppExpressionNode == NULL)
            return E_OUTOFMEMORY;
        else
            return S_OK;
    }

    // we've gone as far as we need, nothing left to parse
    if(Status == S_FALSE)
    {
        ToRelease<ICorDebugValue> pValue;
        *ppExpressionNode = new ExpressionNode(pExpression, ChildKind_BaseClass, pExpression, pParsedValue, pParsedType, pFrame, pParsedDefaultValue, cchParsedDefaultValue);
        if(*ppExpressionNode == NULL)
            return E_OUTOFMEMORY;
        else
            return S_OK;
    }
    // if we are just starting and have no context then we need to search locals/parameters/type names
    else if(pParsedValue == NULL && pParsedType == NULL)
    {
        // the first identifier must be a name, not an indexing expression
        if(isArray)
        {
            *ppExpressionNode = new ExpressionNode(pExpression, L"Expression must begin with a local variable, parameter, or fully qualified type name");
            return S_OK;
        }

        // scan for root on stack
        EvaluateExpressionFrameScanData data;
        data.pIdentifier = pIdentifier;
        data.pFoundValue = NULL;
        data.pFoundFrame = NULL;
        data.pFirstFrame = NULL;
        data.pErrorMessage = pResultBuffer;
        data.cchErrorMessage = MAX_EXPRESSION;
        EnumerateFrames(EvaluateExpressionFrameScanCallback, (VOID*) &data);

        if(data.pFoundValue != NULL)
        {
            // found the root, now recurse along the expression
            return CreateExpressionNodeHelper(pExpression, pExpressionCursor, currentCharsParsed, data.pFoundValue, NULL, NULL, 0, data.pFoundFrame, ppExpressionNode);
        }

        // didn't find it - search the type table for a matching name
        WCHAR pName[MAX_EXPRESSION];
        while(true)
        {
            wcsncpy_s(pName, MAX_EXPRESSION, pExpression, currentCharsParsed);
            ToRelease<ICorDebugType> pType;
            if(SUCCEEDED(FindTypeByName(pName, &pType)))
                return CreateExpressionNodeHelper(pExpression, pExpressionCursor, currentCharsParsed, NULL, pType, NULL, 0, data.pFirstFrame, ppExpressionNode);

            if(FAILED(Status = ParseNextIdentifier(&pExpressionCursor, pIdentifier, mdNameLen, pResultBuffer, MAX_EXPRESSION, &currentCharsParsed, &isArray)))
            {
                *ppExpressionNode = new ExpressionNode(pExpression, pResultBuffer);
                return S_OK;
            }
            else if(Status == S_FALSE)
            {
                break;
            }
        }

        WCHAR errorMessage[MAX_ERROR];
        swprintf_s(errorMessage, MAX_ERROR, L"No expression prefix could not be matched to an existing type, parameter, or local");
        *ppExpressionNode = new ExpressionNode(pExpression, errorMessage);
        return S_OK;
    }

    // we've got some context from an earlier portion of the search, now just need to continue
    // by dereferencing and indexing until we reach the end of the expression

    // Figure out the type, module, and metadata from our context information
    ToRelease<ICorDebugType> pType;
    BOOL isNull = TRUE;
    ToRelease<ICorDebugValue> pInnerValue = NULL;
    if(pParsedValue != NULL)
    {
        IfFailRet(DereferenceAndUnboxValue(pParsedValue, &pInnerValue, &isNull));

        if(isNull)
        {
            WCHAR parsedExpression[MAX_EXPRESSION];
            wcsncpy_s(parsedExpression, MAX_EXPRESSION, pExpression, charactersParsed);
            WCHAR errorMessage[MAX_ERROR];
            swprintf_s(errorMessage, MAX_ERROR, L"Dereferencing \'%s\' throws NullReferenceException", parsedExpression);
            *ppExpressionNode = new ExpressionNode(pExpression, errorMessage);
            return S_OK;
        }

        ToRelease<ICorDebugValue2> pValue2;
        IfFailRet(pInnerValue->QueryInterface(IID_ICorDebugValue2, (LPVOID *) &pValue2));
        IfFailRet(pValue2->GetExactType(&pType));
        CorElementType et;
        IfFailRet(pType->GetType(&et));
        while(et == ELEMENT_TYPE_ARRAY || et == ELEMENT_TYPE_SZARRAY || et == ELEMENT_TYPE_BYREF || et == ELEMENT_TYPE_PTR)
        {
            pType->GetFirstTypeParameter(&pType);
            IfFailRet(pType->GetType(&et));
        }
    }
    else
    {
        pType = pParsedType;
        pType->AddRef();
    }
    ToRelease<ICorDebugClass> pClass;
    IfFailRet(pType->GetClass(&pClass));
    ToRelease<ICorDebugModule> pModule;
    IfFailRet(pClass->GetModule(&pModule));
    mdTypeDef currentTypeDef;
    IfFailRet(pClass->GetToken(&currentTypeDef));

    ToRelease<IUnknown> pMDUnknown;
    ToRelease<IMetaDataImport> pMD;
    IfFailRet(pModule->GetMetaDataInterface(IID_IMetaDataImport, &pMDUnknown));
    IfFailRet(pMDUnknown->QueryInterface(IID_IMetaDataImport, (LPVOID*) &pMD));


    // if we are searching along and this is an array index dereference
    if(isArray)
    {
        ToRelease<ICorDebugArrayValue> pArrayValue;
        if(pInnerValue == NULL || FAILED(Status = pInnerValue->QueryInterface(IID_ICorDebugArrayValue, (LPVOID*) &pArrayValue)))
        {
            WCHAR errorMessage[MAX_ERROR];
            swprintf_s(errorMessage, MAX_ERROR, L"Index notation only supported for instances of an array type");
            *ppExpressionNode = new ExpressionNode(pExpression, errorMessage);
            return S_OK;
        }

        ULONG32 nRank;
        IfFailRet(pArrayValue->GetRank(&nRank));
        if (nRank != 1)
        {
            WCHAR errorMessage[MAX_ERROR];
            swprintf_s(errorMessage, MAX_ERROR, L"Multi-dimensional arrays NYI");
            *ppExpressionNode = new ExpressionNode(pExpression, errorMessage);
            return S_OK;
        }

        int index = -1;
        if(swscanf_s(pIdentifier, L"%d", &index) != 1)
        {
            WCHAR errorMessage[MAX_ERROR];
            swprintf_s(errorMessage, MAX_ERROR, L"Failed to parse expression, missing or invalid index expression at character %d", charactersParsed+1);
            *ppExpressionNode = new ExpressionNode(pExpression, errorMessage);
            return S_OK;
        }

        ULONG32 cElements;
        IfFailRet(pArrayValue->GetCount(&cElements));
        if(index < 0 || (ULONG32)index >= cElements)
        {
            WCHAR errorMessage[MAX_ERROR];
            swprintf_s(errorMessage, MAX_ERROR, L"Index is out of range for this array");
            *ppExpressionNode = new ExpressionNode(pExpression, errorMessage);
            return S_OK;
        }

        ToRelease<ICorDebugValue> pElementValue;
        IfFailRet(pArrayValue->GetElementAtPosition(index, &pElementValue));
        return CreateExpressionNodeHelper(pExpression, pExpressionCursor, currentCharsParsed, pElementValue, NULL, NULL, 0, pFrame, ppExpressionNode);
    }
    // if we are searching along and this is field dereference
    else
    {
        ToRelease<ICorDebugType> pBaseType = pType;
        pBaseType->AddRef();

        while(pBaseType != NULL)
        {
            // get the current base type class/token/MD
            ToRelease<ICorDebugClass> pBaseClass;
            IfFailRet(pBaseType->GetClass(&pBaseClass));
            ToRelease<ICorDebugModule> pBaseTypeModule;
            IfFailRet(pBaseClass->GetModule(&pBaseTypeModule));
            mdTypeDef baseTypeDef;
            IfFailRet(pBaseClass->GetToken(&baseTypeDef));
            ToRelease<IUnknown> pBaseTypeMDUnknown;
            ToRelease<IMetaDataImport> pBaseTypeMD;
            IfFailRet(pBaseTypeModule->GetMetaDataInterface(IID_IMetaDataImport, &pBaseTypeMDUnknown));
            IfFailRet(pBaseTypeMDUnknown->QueryInterface(IID_IMetaDataImport, (LPVOID*) &pBaseTypeMD));


            // iterate through all fields at this level of the class hierarchy
            ULONG numFields = 0;
            HCORENUM fEnum = NULL;
            mdFieldDef fieldDef;
            while(SUCCEEDED(pMD->EnumFields(&fEnum, baseTypeDef, &fieldDef, 1, &numFields)) && numFields != 0)
            {
                ULONG             nameLen = 0;
                DWORD             fieldAttr = 0;
                WCHAR             mdName[mdNameLen];
                WCHAR             typeName[mdNameLen];
                CorElementType    fieldDefaultValueEt;
                UVCP_CONSTANT     pDefaultValue;
                ULONG             cchDefaultValue;
                if(SUCCEEDED(pBaseTypeMD->GetFieldProps(fieldDef, NULL, mdName, mdNameLen, &nameLen, &fieldAttr, NULL, NULL, (DWORD*)&fieldDefaultValueEt, &pDefaultValue, &cchDefaultValue)) &&
                    wcscmp(mdName, pIdentifier) == 0)
                {
                    ToRelease<ICorDebugType> pFieldValType = NULL;
                    ToRelease<ICorDebugValue> pFieldVal;
                    if (fieldAttr & fdStatic)
                        pBaseType->GetStaticFieldValue(fieldDef, pFrame, &pFieldVal);
                    else if(pInnerValue != NULL)
                    {
                        ToRelease<ICorDebugObjectValue> pObjValue;
                        if (SUCCEEDED(pInnerValue->QueryInterface(IID_ICorDebugObjectValue, (LPVOID*) &pObjValue)))
                            pObjValue->GetFieldValue(pBaseClass, fieldDef, &pFieldVal);
                    }

                    // we didn't get a value yet and there is default value available
                    // need to calculate the type because there won't be a ICorDebugValue to derive it from
                    if(pFieldVal == NULL && pDefaultValue != NULL)
                    {
                        FindTypeFromElementType(fieldDefaultValueEt, &pFieldValType);
                    }
                    else
                    {
                        // if we aren't using default value, make sure it is cleared out
                        pDefaultValue = NULL;
                        cchDefaultValue = 0;
                    }

                    // if we still don't have a value, check if we are trying to get an instance field from a static type
                    if(pInnerValue == NULL && pFieldVal == NULL && pDefaultValue == NULL)
                    {
                        WCHAR pObjectTypeName[MAX_EXPRESSION];
                        CalculateTypeName(pBaseType, pObjectTypeName, MAX_EXPRESSION);
                        WCHAR errorMessage[MAX_ERROR];
                        swprintf_s(errorMessage, MAX_ERROR, L"Can not evaluate instance field \'%s\' from static type \'%s\'", pIdentifier, pObjectTypeName);
                        *ppExpressionNode = new ExpressionNode(pExpression, errorMessage);
                        return S_OK;
                    }
                    return CreateExpressionNodeHelper(pExpression, pExpressionCursor, currentCharsParsed, pFieldVal, pFieldValType, pDefaultValue, cchDefaultValue, pFrame, ppExpressionNode);
                }
            }

            //advance to next base type
            ICorDebugType* pTemp = NULL;
            pBaseType->GetBase(&pTemp);
            pBaseType = pTemp;
        }

        WCHAR pObjectTypeName[MAX_EXPRESSION];
        CalculateTypeName(pType, pObjectTypeName, MAX_EXPRESSION);
        WCHAR errorMessage[MAX_ERROR];
        swprintf_s(errorMessage, MAX_ERROR, L"Field \'%s\' does not exist in type \'%s\'", pIdentifier, pObjectTypeName);
        *ppExpressionNode = new ExpressionNode(pExpression, errorMessage);
        return S_OK;
    }

    return Status;
}

// Splits apart a C#-like expression and determines the first identifier in the string and updates expression to point
// at the remaining unparsed portion
HRESULT ExpressionNode::ParseNextIdentifier(__in_z WCHAR** expression, __inout_ecount(cchIdentifierName) WCHAR* identifierName, DWORD cchIdentifierName, __inout_ecount(cchErrorMessage) WCHAR* errorMessage, DWORD cchErrorMessage, DWORD* charactersParsed, BOOL* isArrayIndex)
{

    // This algorithm is best understood as a two stage process. The first stage splits
    // the expression into two chunks an identifier and a remaining expression. The second stage
    // normalizes the identifier. The splitting algorithm doesn't care if identifiers are well-formed
    // at all, we do some error checking in the 2nd stage though. For the splitting stage, an identifier is
    // any first character, followed by as many characters as possible that aren't a '.' or a '['.
    // In the 2nd stage any '.' character is removed from the front (the only place it could be)
    // and enclosing braces are removed. An error is recorded if the identifier ends 0 length or the
    // opening bracket isn't matched. Here is an example showing how we would parse an expression
    // which is deliberately not very well formed. Each line is the result of calling this function once... it
    // takes many calls to break the entire expression down.
    //
    // expression              1st stage identifier   2nd stage identifier
    // foo.bar[18..f[1.][2][[ 
    // .bar[18..f[1.][2][[     foo                    foo
    // [18..f[1.][2][[         .bar                   bar
    // ..f[1.][2][[            [18                    error no ]
    // .f[1.][2][[             .                      error 0-length 
    // [1.][2][[               .f                     f
    // .][2][[                 [1                     error no ]
    // [2][[                   .]                     ]  (we don't error check legal CLI identifier name characters)
    // [[                      [2]                    2
    // [                       [                      error no ]
    //                         [                      error no ]

    // not an error, just the end of the expression
    if(*expression == NULL || **expression == 0)
        return S_FALSE;

    WCHAR* expressionStart = *expression;
    DWORD currentCharsParsed = *charactersParsed;
    DWORD identifierLen = (DWORD) wcscspn(expressionStart, L".[");
    // if the first character was a . or [ skip over it. Note that we don't
    // do this always in case the first WCHAR was part of a surrogate pair
    if(identifierLen == 0)
    {
        identifierLen = (DWORD) wcscspn(expressionStart+1, L".[") + 1;
    }

    *expression += identifierLen;
    *charactersParsed += identifierLen;

    // done with the first stage splitting, on to 2nd stage

    // a . should be followed by field name
    if(*expressionStart == L'.')
    {
        if(identifierLen == 1) // 0-length after .
        {
            swprintf_s(errorMessage, cchErrorMessage, L"Failed to parse expression, missing name after character %d", currentCharsParsed+1);
            return E_FAIL;
        }
        if(identifierLen-1 >= cchIdentifierName)
        {
            swprintf_s(errorMessage, cchErrorMessage, L"Failed to parse expression, name at character %d is too long", currentCharsParsed+2);
            return E_FAIL;
        }
        *isArrayIndex = FALSE;
        wcsncpy_s(identifierName, cchIdentifierName, expressionStart+1, identifierLen-1);
        return S_OK;
    }
    // an open bracket should be followed by a decimal value and then a closing bracket
    else if(*expressionStart == L'[')
    {
        if(*(expressionStart+identifierLen-1) != L']')
        {
            swprintf_s(errorMessage, cchErrorMessage, L"Failed to parse expression, missing or invalid index expression at character %d", currentCharsParsed+1);
            return E_FAIL;
        }
        if(identifierLen <= 2) // 0-length between []
        {
            swprintf_s(errorMessage, cchErrorMessage, L"Failed to parse expression, missing index after character %d", currentCharsParsed+1);
            return E_FAIL;
        }
        if(identifierLen-2 >= cchIdentifierName)
        {
            swprintf_s(errorMessage, cchErrorMessage, L"Failed to parse expression, index at character %d is too large", currentCharsParsed+2);
            return E_FAIL;
        }
        *isArrayIndex = TRUE;
        wcsncpy_s(identifierName, cchIdentifierName, expressionStart+1, identifierLen-2);
        return S_OK;
    }
    else // no '.' or '[', this is an initial name
    {
        if(identifierLen == 0) // 0-length
        {
            swprintf_s(errorMessage, cchErrorMessage, L"Failed to parse expression, missing name after character %d", currentCharsParsed+1);
            return E_FAIL;
        }
        if(identifierLen >= cchIdentifierName)
        {
            swprintf_s(errorMessage, cchErrorMessage, L"Failed to parse expression, name at character %d is too long", currentCharsParsed+1);
            return E_FAIL;
        }
        *isArrayIndex = FALSE;
        wcsncpy_s(identifierName, cchIdentifierName, expressionStart, identifierLen);
        return S_OK;
    }
}


// Iterate through all parameters in the ILFrame calling the callback function for each of them
HRESULT ExpressionNode::EnumerateParameters(IMetaDataImport * pMD,
                                                   mdMethodDef methodDef,
                                                   ICorDebugILFrame * pILFrame,
                                                   VariableEnumCallback pCallback,
                                                   VOID* pUserData)
{
    HRESULT Status = S_OK;

    ULONG cParams = 0;
    ToRelease<ICorDebugValueEnum> pParamEnum;
    IfFailRet(pILFrame->EnumerateArguments(&pParamEnum));
    IfFailRet(pParamEnum->GetCount(&cParams));
    DWORD methAttr = 0;
    IfFailRet(pMD->GetMethodProps(methodDef, NULL, NULL, 0, NULL, &methAttr, NULL, NULL, NULL, NULL));
    for (ULONG i=0; i < cParams; i++)
    {
        ULONG paramNameLen = 0;
        mdParamDef paramDef;
        WCHAR paramName[mdNameLen] = L"\0";

        if(i == 0 && (methAttr & mdStatic) == 0)
            swprintf_s(paramName, mdNameLen, L"this\0");
        else 
        {
            int idx = ((methAttr & mdStatic) == 0)? i : (i + 1);
            if(SUCCEEDED(pMD->GetParamForMethodIndex(methodDef, idx, &paramDef)))
                pMD->GetParamProps(paramDef, NULL, NULL, paramName, mdNameLen, &paramNameLen, NULL, NULL, NULL, NULL);
        }
        if(wcslen(paramName) == 0)
            swprintf_s(paramName, mdNameLen, L"param_%d\0", i);

        ToRelease<ICorDebugValue> pValue;
        ULONG cArgsFetched;
        WCHAR pErrorMessage[MAX_ERROR] = L"\0";
        HRESULT hr = pParamEnum->Next(1, &pValue, &cArgsFetched);
        if (FAILED(hr))
        {
            swprintf_s(pErrorMessage, MAX_ERROR, L"  + (Error 0x%x retrieving parameter '%S')\n", hr, paramName);
        }
        if (hr == S_FALSE)
        {
            break;
        }
        pCallback(pValue, paramName, pErrorMessage, pUserData);
    }

    return Status;
}

// Enumerate all locals in the given ILFrame, calling the callback method for each of them
HRESULT ExpressionNode::EnumerateLocals(IMetaDataImport * pMD,
                                               mdMethodDef methodDef,
                                               ICorDebugILFrame * pILFrame,
                                               VariableEnumCallback pCallback,
                                               VOID* pUserData)
{
    HRESULT Status = S_OK;
    ULONG cLocals = 0;
    ToRelease<ICorDebugFunction> pFunction;
    ToRelease<ICorDebugModule> pModule;
    if(SUCCEEDED(pILFrame->GetFunction(&pFunction)))
    {
        IfFailRet(pFunction->GetModule(&pModule));
    }
    ToRelease<ICorDebugValueEnum> pLocalsEnum;
    IfFailRet(pILFrame->EnumerateLocalVariables(&pLocalsEnum));
    IfFailRet(pLocalsEnum->GetCount(&cLocals));
    if (cLocals > 0)
    {
        SymbolReader symReader;
        bool symbolsAvailable = false;
        if(pModule != NULL && SUCCEEDED(symReader.LoadSymbols(pMD, pModule)))
            symbolsAvailable = true;

        for (ULONG i=0; i < cLocals; i++)
        {
            ULONG paramNameLen = 0;
            WCHAR paramName[mdNameLen] = L"\0";
            WCHAR pErrorMessage[MAX_ERROR] = L"\0";
            ToRelease<ICorDebugValue> pValue;
            HRESULT hr = S_OK;
            if(symbolsAvailable)
                hr = symReader.GetNamedLocalVariable(pILFrame, i, paramName, mdNameLen, &pValue);
            else
            {
                ULONG cArgsFetched;
                hr = pLocalsEnum->Next(1, &pValue, &cArgsFetched);
            }
            if(wcslen(paramName) == 0)
                swprintf_s(paramName, mdNameLen, L"local_%d\0", i);

            if (FAILED(hr))
            {
                swprintf_s(pErrorMessage, MAX_ERROR, L"  + (Error 0x%x retrieving local variable '%S')\n", hr, paramName);
            }
            else if (hr == S_FALSE)
            {
                break;
            }
            pCallback(pValue, paramName, pErrorMessage, pUserData);
        }
    }

    return Status;
}

// Iterates over all frames on the current thread's stack, calling the callback function for each of them
HRESULT ExpressionNode::EnumerateFrames(FrameEnumCallback pCallback, VOID* pUserData)
{
    HRESULT Status = S_OK;
    ToRelease<ICorDebugThread> pThread;
    ToRelease<ICorDebugThread3> pThread3;
    ToRelease<ICorDebugStackWalk> pStackWalk;
    ULONG ulThreadID = 0;
    g_ExtSystem->GetCurrentThreadSystemId(&ulThreadID);

    IfFailRet(g_pCorDebugProcess->GetThread(ulThreadID, &pThread));
    IfFailRet(pThread->QueryInterface(IID_ICorDebugThread3, (LPVOID *) &pThread3));
    IfFailRet(pThread3->CreateStackWalk(&pStackWalk));

    InternalFrameManager internalFrameManager;
    IfFailRet(internalFrameManager.Init(pThread3));

    int currentFrame = -1;

    for (Status = S_OK; ; Status = pStackWalk->Next())
    {
        currentFrame++;

        if (Status == CORDBG_S_AT_END_OF_STACK)
        {
            break;
        }
        IfFailRet(Status);

        if (IsInterrupt())
        {
            ExtOut("<interrupted>\n");
            break;
        }

        CROSS_PLATFORM_CONTEXT context;
        ULONG32 cbContextActual;
        if ((Status=pStackWalk->GetContext(
            DT_CONTEXT_FULL, 
            sizeof(context),
            &cbContextActual,
            (BYTE *)&context))!=S_OK)
        {
            ExtOut("GetFrameContext failed: %lx\n",Status);
            break;
        }

        ToRelease<ICorDebugFrame> pFrame;
        IfFailRet(pStackWalk->GetFrame(&pFrame));
        if (Status == S_FALSE)
        {
            Status = S_OK;
            continue;
        }

        pCallback(pFrame, pUserData);
    }

    return Status;
}

// Determines the corresponding ICorDebugType for a given primitive type
HRESULT ExpressionNode::FindTypeFromElementType(CorElementType et, ICorDebugType** ppType)
{
    HRESULT Status;
    switch (et)
    {
    default:
        Status = E_FAIL;
        break;

    case ELEMENT_TYPE_BOOLEAN:
        Status = FindTypeByName(L"System.Boolean", ppType);
        break;

    case ELEMENT_TYPE_CHAR:
        Status = FindTypeByName(L"System.Char", ppType);
        break;

    case ELEMENT_TYPE_I1:
        Status = FindTypeByName(L"System.SByte", ppType);
        break;

    case ELEMENT_TYPE_U1:
        Status = FindTypeByName(L"System.Byte", ppType);
        break;

    case ELEMENT_TYPE_I2:
        Status = FindTypeByName(L"System.Short", ppType);
        break;

    case ELEMENT_TYPE_U2:
        Status = FindTypeByName(L"System.UShort", ppType);
        break;

    case ELEMENT_TYPE_I:
        Status = FindTypeByName(L"System.Int32", ppType);
        break;

    case ELEMENT_TYPE_U:
        Status = FindTypeByName(L"System.UInt32", ppType);
        break;

    case ELEMENT_TYPE_I4:
        Status = FindTypeByName(L"System.Int32", ppType);
        break;

    case ELEMENT_TYPE_U4:
        Status = FindTypeByName(L"System.UInt32", ppType);
        break;

    case ELEMENT_TYPE_I8:
        Status = FindTypeByName(L"System.Int64", ppType);
        break;

    case ELEMENT_TYPE_U8:
        Status = FindTypeByName(L"System.UInt64", ppType);
        break;

    case ELEMENT_TYPE_R4:
        Status = FindTypeByName(L"System.Single", ppType);
        break;

    case ELEMENT_TYPE_R8:
        Status = FindTypeByName(L"System.Double", ppType);
        break;

    case ELEMENT_TYPE_OBJECT:
        Status = FindTypeByName(L"System.Object", ppType);
        break;

    case ELEMENT_TYPE_STRING:
        Status = FindTypeByName(L"System.String", ppType);
        break;
    }
    return Status;
}

// Gets the appropriate element type encoding for well-known fully qualified type names
// This doesn't work for arbitrary types, just types that have CorElementType short forms.
HRESULT ExpressionNode::GetCanonicalElementTypeForTypeName(__in_z WCHAR* pTypeName, CorElementType *et)
{
    //Sadly ICorDebug deliberately prevents creating ICorDebugType instances
    //that use canonical short form element types... seems like an issue to me.

    if(wcscmp(pTypeName, L"System.String")==0)
    {
        *et = ELEMENT_TYPE_STRING;
    }
    else if(wcscmp(pTypeName, L"System.Object")==0)
    {
        *et = ELEMENT_TYPE_OBJECT;
    }
    else if(wcscmp(pTypeName, L"System.Void")==0)
    {
        *et = ELEMENT_TYPE_VOID;
    }
    else if(wcscmp(pTypeName, L"System.Boolean")==0)
    {
        *et = ELEMENT_TYPE_BOOLEAN;
    }
    else if(wcscmp(pTypeName, L"System.Char")==0)
    {
        *et = ELEMENT_TYPE_CHAR;
    }
    else if(wcscmp(pTypeName, L"System.Byte")==0)
    {
        *et = ELEMENT_TYPE_U1;
    }
    else if(wcscmp(pTypeName, L"System.Sbyte")==0)
    {
        *et = ELEMENT_TYPE_I1;
    }
    else if(wcscmp(pTypeName, L"System.Int16")==0)
    {
        *et = ELEMENT_TYPE_I2;
    }
    else if(wcscmp(pTypeName, L"System.UInt16")==0)
    {
        *et = ELEMENT_TYPE_U2;
    }
    else if(wcscmp(pTypeName, L"System.UInt32")==0)
    {
        *et = ELEMENT_TYPE_U4;
    }
    else if(wcscmp(pTypeName, L"System.Int32")==0)
    {
        *et = ELEMENT_TYPE_I4;
    }
    else if(wcscmp(pTypeName, L"System.UInt64")==0)
    {
        *et = ELEMENT_TYPE_U8;
    }
    else if(wcscmp(pTypeName, L"System.Int64")==0)
    {
        *et = ELEMENT_TYPE_I8;
    }
    else if(wcscmp(pTypeName, L"System.Single")==0)
    {
        *et = ELEMENT_TYPE_R4;
    }
    else if(wcscmp(pTypeName, L"System.Double")==0)
    {
        *et = ELEMENT_TYPE_R8;
    }
    else if(wcscmp(pTypeName, L"System.IntPtr")==0)
    {
        *et = ELEMENT_TYPE_U;
    }
    else if(wcscmp(pTypeName, L"System.UIntPtr")==0)
    {
        *et = ELEMENT_TYPE_I;
    }
    else if(wcscmp(pTypeName, L"System.TypedReference")==0)
    {
        *et = ELEMENT_TYPE_TYPEDBYREF;
    }
    else 
    {
        return E_FAIL; // can't tell from a name whether it should be valuetype or class
    }
    return S_OK;
}

// Searches the debuggee for any ICorDebugType that matches the given fully qualified name
// This will search across all AppDomains and Assemblies
HRESULT ExpressionNode::FindTypeByName(__in_z  WCHAR* pTypeName, ICorDebugType** ppType)
{
    HRESULT Status = S_OK;
    ToRelease<ICorDebugAppDomainEnum> pAppDomainEnum;
    IfFailRet(g_pCorDebugProcess->EnumerateAppDomains(&pAppDomainEnum));
    DWORD count;
    IfFailRet(pAppDomainEnum->GetCount(&count));
    for(DWORD i = 0; i < count; i++)
    {
        ToRelease<ICorDebugAppDomain> pAppDomain;
        DWORD countFetched = 0;
        IfFailRet(pAppDomainEnum->Next(1, &pAppDomain, &countFetched));
        Status = FindTypeByName(pAppDomain, pTypeName, ppType);
        if(SUCCEEDED(Status))
            break;
    }

    return Status;
}

// Searches the debuggee for any ICorDebugType that matches the given fully qualified name
// This will search across all Assemblies in the given AppDomain
HRESULT ExpressionNode::FindTypeByName(ICorDebugAppDomain* pAppDomain, __in_z WCHAR* pTypeName, ICorDebugType** ppType)
{
    HRESULT Status = S_OK;
    ToRelease<ICorDebugAssemblyEnum> pAssemblyEnum;
    IfFailRet(pAppDomain->EnumerateAssemblies(&pAssemblyEnum));
    DWORD count;
    IfFailRet(pAssemblyEnum->GetCount(&count));
    for(DWORD i = 0; i < count; i++)
    {
        ToRelease<ICorDebugAssembly> pAssembly;
        DWORD countFetched = 0;
        IfFailRet(pAssemblyEnum->Next(1, &pAssembly, &countFetched));
        Status = FindTypeByName(pAssembly, pTypeName, ppType);
        if(SUCCEEDED(Status))
            break;
    }

    return Status;
}

// Searches the assembly for any ICorDebugType that matches the given fully qualified name
HRESULT ExpressionNode::FindTypeByName(ICorDebugAssembly* pAssembly, __in_z WCHAR* pTypeName, ICorDebugType** ppType)
{
    HRESULT Status = S_OK;
    ToRelease<ICorDebugModuleEnum> pModuleEnum;
    IfFailRet(pAssembly->EnumerateModules(&pModuleEnum));
    DWORD count;
    IfFailRet(pModuleEnum->GetCount(&count));
    for(DWORD i = 0; i < count; i++)
    {
        ToRelease<ICorDebugModule> pModule;
        DWORD countFetched = 0;
        IfFailRet(pModuleEnum->Next(1, &pModule, &countFetched));
        Status = FindTypeByName(pModule, pTypeName, ppType);
        if(SUCCEEDED(Status))
            break;
    }

    return Status;
}

// Searches a given module for any ICorDebugType that matches the given fully qualified type name
HRESULT ExpressionNode::FindTypeByName(ICorDebugModule* pModule, __in_z WCHAR* pTypeName, ICorDebugType** ppType)
{
    HRESULT Status = S_OK;
    ToRelease<IUnknown> pMDUnknown;
    ToRelease<IMetaDataImport> pMD;
    IfFailRet(pModule->GetMetaDataInterface(IID_IMetaDataImport, &pMDUnknown));
    IfFailRet(pMDUnknown->QueryInterface(IID_IMetaDataImport, (LPVOID*) &pMD));

    // If the name contains a generic argument list, extract the type name from
    // before the list
    WCHAR rootName[mdNameLen];
    WCHAR* pRootName = NULL;
    int typeNameLen = (int) wcslen(pTypeName);
    int genericParamListStart = (int) wcscspn(pTypeName, L"<");
    if(genericParamListStart != typeNameLen)
    {
        if(pTypeName[typeNameLen-1] != L'>' || genericParamListStart > mdNameLen)
        {
            return E_FAIL; // mal-formed type name
        }
        else
        {
            wcsncpy_s(rootName, mdNameLen, pTypeName, genericParamListStart);
            pRootName = rootName;
        }
    }
    else
    {
        pRootName = pTypeName;
    }

    // Convert from name to token to ICorDebugClass
    mdTypeDef typeDef;
    IfFailRet(pMD->FindTypeDefByName(pRootName, NULL, &typeDef));
    DWORD flags;
    ULONG nameLen;
    mdToken tkExtends;
    IfFailRet(pMD->GetTypeDefProps(typeDef, NULL, 0, &nameLen, &flags, &tkExtends));
    BOOL isValueType;
    IfFailRet(IsTokenValueTypeOrEnum(tkExtends, pMD, &isValueType));
    CorElementType et = isValueType ? ELEMENT_TYPE_VALUETYPE : ELEMENT_TYPE_CLASS;
    ToRelease<ICorDebugClass> pClass;
    IfFailRet(pModule->GetClassFromToken(typeDef, &pClass));
    ToRelease<ICorDebugClass2> pClass2;
    IfFailRet(pClass->QueryInterface(__uuidof(ICorDebugClass2), (void**)&pClass2));

    // Convert from class to type - if generic then recursively resolve the generic
    // parameter list
    ArrayHolder<ToRelease<ICorDebugType>> typeParams = NULL;
    int countTypeParams = 0;
    if(genericParamListStart != typeNameLen)
    {
        ToRelease<ICorDebugAssembly> pAssembly;
        IfFailRet(pModule->GetAssembly(&pAssembly));
        ToRelease<ICorDebugAppDomain> pDomain;
        IfFailRet(pAssembly->GetAppDomain(&pDomain));

        countTypeParams = 1;
        for(int i = genericParamListStart+1; i < typeNameLen; i++)
        {
            if(pTypeName[i] == L',') countTypeParams++;
        }
        typeParams = new ToRelease<ICorDebugType>[countTypeParams];

        WCHAR* pCurName = pTypeName + genericParamListStart+1;
        for(int i = 0; i < countTypeParams; i++)
        {
            WCHAR typeParamName[mdNameLen];
            WCHAR* pNextComma = wcschr(pCurName, L',');
            int len = (pNextComma != NULL) ? (int)(pNextComma - pCurName) : (int)wcslen(pCurName)-1;
            if(len > mdNameLen)
                return E_FAIL;
            wcsncpy_s(typeParamName, mdNameLen, pCurName, len);
            FindTypeByName(pDomain, typeParamName, &(typeParams[i]));
            pCurName = pNextComma+1;
        }
    }
    IfFailRet(pClass2->GetParameterizedType(et, countTypeParams, &(typeParams[0]), ppType));

    return Status;
}

// Checks whether the given token is or refers to type System.ValueType or System.Enum
HRESULT ExpressionNode::IsTokenValueTypeOrEnum(mdToken token, IMetaDataImport* pMetadata, BOOL* pResult)
{
    // This isn't a 100% correct check because we aren't verifying the module portion of the
    // type identity. Arbitrary assemblies could define a type named System.ValueType or System.Enum.
    // If that happens this code will get the answer wrong... we just assume that happens so rarely
    // that it isn't worth doing all the overhead of assembly resolution to deal with

    HRESULT Status = S_OK;
    CorTokenType type = (CorTokenType)(token & 0xFF000000);

    // only need enough space to hold either System.ValueType or System.Enum
    //System.ValueType -> 16 characters
    //System.Enum -> 11 characters
    WCHAR nameBuffer[17];
    nameBuffer[0] = L'\0';

    if(type == mdtTypeRef)
    {
        ULONG chTypeDef;
        pMetadata->GetTypeRefProps(token, NULL, NULL, 0, &chTypeDef);
        if(chTypeDef > _countof(nameBuffer))
        {
            *pResult = FALSE;
            return Status;
        }
        IfFailRet(pMetadata->GetTypeRefProps(token, NULL, nameBuffer, _countof(nameBuffer), &chTypeDef));
    }
    else if(type == mdtTypeDef)
    {
        ULONG chTypeDef;
        pMetadata->GetTypeDefProps(token, NULL, 0, &chTypeDef, NULL, NULL);
        if(chTypeDef > _countof(nameBuffer))
        {
            *pResult = FALSE;
            return Status;
        }
        IfFailRet(pMetadata->GetTypeDefProps(token, nameBuffer, _countof(nameBuffer), &chTypeDef, NULL, NULL));
    }

    if(wcscmp(nameBuffer, L"System.ValueType") == 0 ||
        wcscmp(nameBuffer, L"System.Enum") == 0)
    {
        *pResult = TRUE;
    }
    else
    {
        *pResult = FALSE;
    }
    return Status;
}

/*backtrace:
#0  0x000000000069cbee in std::string::size (this=0x11ae590) at /usr/include/c++/4.9/bits/basic_string.h:725
No locals.
#1  0x000000000069b2bf in std::string::compare (this=0x11ae590, __s=0x93d976 "{") at /usr/include/c++/4.9/bits/basic_string.tcc:952
        __size = 1340
        __osize = 1
        __len = 1
        __r = 70
#2  0x000000000069ab9b in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=<error reading variable: Cannot access memory at address 0xffffffffffffffec>, __rhs=0x93d976 "{") at /usr/include/c++/4.9/bits/basic_string.h:2540
No locals.
#3  0x000000000089d1f1 in setVarIdClassDeclaration (startToken=0x12c4340, variableId=std::__debug::map with 19 elements = {...}, scopeStartVarId=0, structMembers=0x7fffffffba20, _varId=0x7fffffffc1e0) at lib/tokenize.cpp:2468
        tok = 0x11ae560
        endToken = 0x12ce7a0
        className = ""
        indentlevel = 0
        initListEndToken = 0x0
#4  0x000000000089e1a1 in Tokenizer::setVarId (this=0x7fffffffc120) at lib/tokenize.cpp:2608
        newScope = false
        tok = 0x12ce7a0
        notstart = std::__debug::set with 16 elements = {[0] = "NOT", [1] = "const_cast", [2] = "delete", [3] = "dynamic_cast", [4] = "explicit", [5] = "friend", [6] = "goto", [7] = "new", [8] = "reinterpret_cast", [9] = "return", [10] = "sizeof", [11] = "static_cast", [12] = "throw", [13] = "typedef", [14] = "using", [15] = "virtual"}
        executableScope = std::stack wrapping: std::__debug::deque with 1 elements = {false}
        allMemberVars = std::__debug::list = {[0] = 0x1277d20, [1] = 0x1277b<error reading variable: Cannot access memory at address 0x1010101010101>...}
        scopestartvarid = std::stack wrapping: std::__debug::deque with 1 elements = {0}
        initListEndToken = 0x0
        variableId = std::__debug::map with 19 elements = {["COPY"] = 61, ["DMLFormats"] = 56, ["ICorDebugProcess"] = 11, ["LEN"] = 60, ["_Src"] = 62, ["gElementTypeInfo"] = 3, ["g_ExtData"] = 323, ["g_InMinidumpSafeMode"] = 340, ["g_clrData"] = 9, ["g_snapshot"] = 467, ["g_sos"] = 10, ["g_special_mtCache"] = 428, ["g_special_usefulGlobals"] = 383, ["len"] = 93, ["mPtr"] = 1512, ["moduleInfo"] = 327, ["nMaxHeapSegmentCount"] = 1, ["pObj"] = 361, ["size"] = 360}
        structMembers = std::__debug::map with 19 elements = {[66] = std::__debug::map with 2 elements = {["mLength"] = 68, ["mStr"] = 67}, [72] = std::__debug::map with 2 elements = {["mLength"] = 74, ["mStr"] = 73}, [76] = std::__debug::map with 2 elements = {["mLength"] = 78, ["mStr"] = 77}, [98] = std::__debug::map with 3 elements = {["mDml"] = 101, ["mFormat"] = 100, ["mValue"] = 99}, [145] = std::__debug::map with 1 elements = {["mValue"] = 146}, [153] = std::__debug::map with 1 elements = {["mValue"] = 154}, [413] = std::__debug::map with 6 elements = {["ArrayOfVC"] = 421, ["BaseSize"] = 417, ["ComponentSize"] = 418, ["GCInfo"] = 420, ["GCInfoBuffer"] = 422, ["bContainsPointers"] = 419}, [461] = std::__debug::map with 1 elements = {["HeapCount"] = 464}, [489] = std::__debug::map with 1 elements = {["finalization_fill_pointers"] = 491}, [492] = std::__debug::map with 1 elements = {["finalization_fill_pointers"] = 494}, [576] = std::__debug::map with 3 elements = {["bHasCriticalTransparentInfo"] = 577, ["bIsCritical"] = 578, ["bIsTreatAsSafe"] = 579}, [927] = std::__debug::map with 1 elements = {["Prev"] = 934}, [928] = std::__debug::map with 1 elements = {["Next"] = 935}, [937] = std::__debug::map with 1 elements = {["Prev"] = 938}, [1144] = std::__debug::map with 1 elements = {["pNextSibling"] = 1145}, [1194] = std::__debug::map with 1 elements = {["pNextSibling"] = 1195}, [1313] = std::__debug::map with 3 elements = {["pFirstFrame"] = 1317, ["pFoundFrame"] = 1323, ["pFoundValue"] = 1314}, [1328] = std::__debug::map with 2 elements = {["pFoundValue"] = 1330, ["pIdentifier"] = 1329}, [1347] = std::__debug::map with 6 elements = {["cchErrorMessage"] = 1353, ["pErrorMessage"] = 1352, ["pFirstFrame"] = 1351, ["pFoundFrame"] = 1350, ["pFoundValue"] = 1349, ["pIdentifier"] = 1348}}
        scopeInfo = std::stack wrapping: std::__debug::deque with 0 elements
        allMemberFunctions = std::__debug::list = {[0] = 0x7fffffffbbd0, [1] = <error reading variable allMemberFunctions (Cannot access memory at address 0x10)>
        varlist = std::__debug::map with 140737337357040 elements<error reading variable: Cannot access memory at address 0x18>
#5  0x00000000008a3b4c in Tokenizer::simplifyTokenList1 (this=0x7fffffffc120, FileName=0xc11f48 "crash227.cpp") at lib/tokenize.cpp:3602
No locals.
#6  0x0000000000898d2f in Tokenizer::tokenize (this=0x7fffffffc120, code=..., FileName=0xc11f48 "crash227.cpp", configuration="", noSymbolDB_AST=false) at lib/tokenize.cpp:1626
        __PRETTY_FUNCTION__ = "bool Tokenizer::tokenize(std::istream&, const char*, const string&, bool)"
#7  0x00000000007ce933 in CppCheck::checkFile (this=0x7fffffffd140, code='\n' <repeats 36 times>, "inline void RestoreSOToleranceState() {}", '\n' <repeats 27 times>, "typedef LPCSTR LPCUTF8;\ntypedef LPSTR LPUTF8;\n\nDECLARE_HANDLE(OBJECTHANDLE);\n\nstruct IMDInternalI"..., FileName=0xc11f48 "crash227.cpp", checksums=std::__debug::set with 0 elements) at lib/cppcheck.cpp:330
        istr = <incomplete type>
        timer = {_str = "Tokenizer::tokenize", _timerResults = 0xc09700 <S_timerResults>, _start = 0, _showtimeMode = 0, _stopped = false}
        result = false
        timer3 = {_str = '\000' <repeats 24 times>, "g\020\000\000?\000\000\000\021", '\000' <repeats 39 times>, "\061\000\000\000\000\000\000\000\001\000\000\000\000\000\000\000\001\000\000\000\000\000\000\000\377\377\377\377\000\000\000\000{\000\313\000\000\000\000\000X\030\313\000\000\000\000\000\241\000\000\000\000\000\000\000(\301\377\377\377\177\000\000\270\026\313\000\000\000\000\000\270\026\313", '\000' <repeats 21 times>, "\001\000\000\000YPE_h\027\313\000\000\000\000\000PG\376\000\000\000\000\000\200F\376\000\000\000\000\000"..., _timerResults = 0x8, _start = 13322784, _showtimeMode = 13322784, _stopped = false}
        _tokenizer = {list = {_front = 0xcb1840, _back = 0x11aaa50, _files = std::__debug::vector of length 1, capacity 1 = {"crash227.cpp"}, _settings = 0x7fffffffd170, _isC = false, _isCPP = true}, _settings = 0x7fffffffd170, _errorLogger = 0x7fffffffd140, _symbolDatabase = 0x0, _configuration = "", _typeSize = std::__debug::map with 12 elements = {["*"] = 8, ["bool"] = 1, ["char"] = 1, ["char16_t"] = 2, ["char32_t"] = 4, ["double"] = 8, ["float"] = 4, ["int"] = 4, ["long"] = 8, ["short"] = 2, ["size_t"] = 8, ["wchar_t"] = 4}, _varId = 1512, _codeWithTemplates = true, m_timerResults = 0x0}
#8  0x00000000007cd687 in CppCheck::processFile (this=0x7fffffffd140, filename="crash227.cpp", fileStream=...) at lib/cppcheck.cpp:237
        t = {_str = "Preprocessor::getcode", _timerResults = 0xc09700 <S_timerResults>, _start = 0, _showtimeMode = 0, _stopped = true}
        codeWithoutCfg = '\n' <repeats 36 times>, "inline void RestoreSOToleranceState() {}", '\n' <repeats 27 times>, "typedef LPCSTR LPCUTF8;\ntypedef LPSTR LPUTF8;\n\nDECLARE_HANDLE(OBJECTHANDLE);\n\nstruct IMDInternalI"...
        it = ""
        filedata = '\n' <repeats 11 times>, "#ifndef _EXPRESSION_NODE_\n#define _EXPRESSION_NODE_\n\n#ifdef FEATURE_PAL\n#error\n#endif", '\n' <repeats 14 times>, "#ifndef __util_h__\n#define __util_h__\n\n#define LIMITED_METHOD_CONTRACT\n\n\ninline void Resto"...
        preprocessor = {static macroChar = 1 '\001', static missingIncludeFlag = true, static missingSystemIncludeFlag = true, _settings = 0x7fffffffd170, _errorLogger = 0x7fffffffd140, file0 = "crash227.cpp"}
        configurations = std::__debug::list = {[0] = "", [1] = "DACCESS_COMPILE", [2] = "FEATURE_PAL", [3] = "STRIKE", [4] = "STRIKE;_MSC_VER", [5] = "_DEBUG", [6] = "_MSC_VER", [7] = "_TARGET_WIN64_", [8] = "_WIN64"}
        checksums = std::__debug::set with 0 elements
        checkCount = 1
#9  0x00000000007cc0f6 in CppCheck::check (this=0x7fffffffd140, path="crash227.cpp") at lib/cppcheck.cpp:70
        fin = <incomplete type>
#10 0x00000000006a70ae in CppCheckExecutor::check_internal (this=0x7fffffffdbe0, cppcheck=..., argv=0x7fffffffdd98) at cli/cppcheckexecutor.cpp:775
        i = {first = "crash227.cpp", second = 195917}
        totalfilesize = 195917
        processedsize = 0
        c = 0
        std = true
        posix = true
        windows = true
        returnValue = 0
        settings = @0x7fffffffd170: {_append = "", _enabled = std::__debug::set with 0 elements, _terminate = false, debug = false, debugwarnings = false, debugFalsePositive = false, dump = false, exceptionHandling = false, inconclusive = false, jointSuppressionReport = true, experimental = false, _errorsOnly = false, _inlineSuppressions = false, _verbose = false, _force = false, _relativePaths = false, _basePaths = std::__debug::vector of length 0, capacity 0, _xml = false, _xml_version = 1, _jobs = 1, _loadAverage = 0, _exitCode = 0, _outputFormat = "", _showtime = SHOWTIME_NONE, _includePaths = empty std::__debug::list, _maxConfigs = 12, enforcedLang = Settings::None, nomsg = {_suppressions = std::__debug::map with 0 elements}, nofail = {_suppressions = std::__debug::map with 0 elements}, userDefines = "", userUndefs = std::__debug::set with 0 elements, userIncludes = empty std::__debug::list, configExcludePaths = std::__debug::set with 0 elements, reportProgress = false, library = {use = std::__debug::set with 0 elements, leakignore = std::__debug::set with 466 elements = {[0] = "abs", [1] = "acos", [2] = "acosf", [3] = "acosh", [4] = "acoshf", [5] = "acoshl", [6] = "acosl", [7] = "asctime", [8] = "asin", [9] = "asinf", [10] = "asinh", [11] = "asinhf", [12] = "asinhl", [13] = "asinl", [14] = "assert", [15] = "atan", [16] = "atan2", [17] = "atan2f", [18] = "atan2l", [19] = "atanf", [20] = "atanh", [21] = "atanhf", [22] = "atanhl", [23] = "atanl", [24] = "atexit", [25] = "atof", [26] = "atoi", [27] = "atol", [28] = "atoll", [29] = "bsearch", [30] = "btowc", [31] = "cabs", [32] = "cabsf", [33] = "cacos", [34] = "cacosf", [35] = "cacoshf", [36] = "cacoshl", [37] = "cacosl", [38] = "carg", [39] = "cargf", [40] = "cargl", [41] = "casin", [42] = "casinf", [43] = "casinh", [44] = "casinhf", [45] = "casinhl", [46] = "casinl", [47] = "catan", [48] = "catanf", [49] = "catanh", [50] = "catanhf", [51] = "catanhl", [52] = "catanl", [53] = "cbrt", [54] = "cbrtf", [55] = "cbrtl", [56] = "ccos", [57] = "ccosf", [58] = "ccosh", [59] = "ccoshf", [60] = "ccoshl", [61] = "ccosl", [62] = "ceil", [63] = "ceilf", [64] = "ceill", [65] = "cexp", [66] = "cexpf", [67] = "cexpl", [68] = "cimagf", [69] = "cimagl", [70] = "clearerr", [71] = "clock", [72] = "clog", [73] = "clogf", [74] = "clogl", [75] = "conjf", [76] = "conjl", [77] = "copysign", [78] = "copysignf", [79] = "copysignl", [80] = "cos", [81] = "cosf", [82] = "cosh", [83] = "coshf", [84] = "coshl", [85] = "cosl", [86] = "cpow", [87] = "cpowf", [88] = "cpowl", [89] = "cproj", [90] = "cprojf", [91] = "cprojl", [92] = "creal", [93] = "crealf", [94] = "creall", [95] = "csin", [96] = "csinf", [97] = "csinh", [98] = "csinhf", [99] = "csinhl", [100] = "csinl", [101] = "csqrt", [102] = "csqrtf", [103] = "csqrtl", [104] = "ctan", [105] = "ctanf", [106] = "ctanh", [107] = "ctanhf", [108] = "ctanhl", [109] = "ctanl", [110] = "ctime", [111] = "difftime", [112] = "div", [113] = "erf", [114] = "erfc", [115] = "erfcf", [116] = "erfcl", [117] = "erff", [118] = "erfl", [119] = "exp", [120] = "exp2", [121] = "exp2f", [122] = "exp2l", [123] = "expf", [124] = "expl", [125] = "expm1", [126] = "expm1f", [127] = "expm1l", [128] = "fabs", [129] = "fabsf", [130] = "fabsl", [131] = "fdim", [132] = "fdimf", [133] = "fdiml", [134] = "feclearexcept", [135] = "fegetenv", [136] = "fegetexceptflag", [137] = "fegetround", [138] = "feholdexcept", [139] = "feof", [140] = "feraiseexcept", [141] = "ferror", [142] = "fesetenv", [143] = "fesetexceptflag", [144] = "fesetround", [145] = "fetestexcept", [146] = "feupdateenv", [147] = "fflush", [148] = "fgetc", [149] = "fgetpos", [150] = "fgets", [151] = "fgetwc", [152] = "fgetws", [153] = "floor", [154] = "floorf", [155] = "floorl", [156] = "fma", [157] = "fmaf", [158] = "fmal", [159] = "fmax", [160] = "fmaxf", [161] = "fmaxl", [162] = "fmin", [163] = "fminf", [164] = "fminl", [165] = "fmod", [166] = "fmodf", [167] = "fmodl", [168] = "fnprintf", [169] = "fpclassify", [170] = "fprintf", [171] = "fputc", [172] = "fputs", [173] = "fputwc", [174] = "fputws", [175] = "fread", [176] = "frexp", [177] = "frexpf", [178] = "frexpl", [179] = "fscanf", [180] = "fseek", [181] = "fsetpos", [182] = "ftell", [183] = "fwide", [184] = "fwprintf", [185] = "fwrite", [186] = "fwscanf", [187] = "getc", [188] = "getchar", [189] = "getenv", [190] = "gets", [191] = "getwc", [192] = "getwchar", [193] = "gmtime", [194] = "hypot", [195] = "hypotf", [196] = "hypotl", [197] = "ilogb", [198] = "ilogbf", [199] = "ilogbl"...}, functionconst = std::__debug::set with 0 elements, functionpure = std::__debug::set with 293 elements = {[0] = "abs", [1] = "acos", [2] = "acosf", [3] = "acosh", [4] = "acoshf", [5] = "acoshl", [6] = "acosl", [7] = "asin", [8] = "asinf", [9] = "asinh", [10] = "asinhf", [11] = "asinhl", [12] = "asinl", [13] = "atan", [14] = "atan2", [15] = "atan2f", [16] = "atan2l", [17] = "atanf", [18] = "atanh", [19] = "atanhf", [20] = "atanhl", [21] = "atanl", [22] = "atexit", [23] = "atof", [24] = "atoi", [25] = "atol", [26] = "atoll", [27] = "bsearch", [28] = "btowc", [29] = "cabs", [30] = "cabsf", [31] = "cacos", [32] = "cacosf", [33] = "cacoshf", [34] = "cacoshl", [35] = "cacosl", [36] = "carg", [37] = "cargf", [38] = "cargl", [39] = "casin", [40] = "casinf", [41] = "casinh", [42] = "casinhf", [43] = "casinhl", [44] = "casinl", [45] = "catan", [46] = "catanf", [47] = "catanh", [48] = "catanhf", [49] = "catanhl", [50] = "catanl", [51] = "cbrt", [52] = "cbrtf", [53] = "cbrtl", [54] = "ccos", [55] = "ccosf", [56] = "ccosh", [57] = "ccoshf", [58] = "ccoshl", [59] = "ccosl", [60] = "ceil", [61] = "ceilf", [62] = "ceill", [63] = "cexp", [64] = "cexpf", [65] = "cexpl", [66] = "cimagf", [67] = "cimagl", [68] = "copysign", [69] = "copysignf", [70] = "copysignl", [71] = "cos", [72] = "cosf", [73] = "cosh", [74] = "coshf", [75] = "coshl", [76] = "cosl", [77] = "cpow", [78] = "cpowf", [79] = "cpowl", [80] = "cproj", [81] = "cprojf", [82] = "cprojl", [83] = "creal", [84] = "crealf", [85] = "creall", [86] = "csin", [87] = "csinf", [88] = "csinh", [89] = "csinhf", [90] = "csinhl", [91] = "csinl", [92] = "csqrt", [93] = "csqrtf", [94] = "csqrtl", [95] = "ctan", [96] = "ctanf", [97] = "ctanh", [98] = "ctanhf", [99] = "ctanhl", [100] = "ctanl", [101] = "div", [102] = "erf", [103] = "erfc", [104] = "erfcf", [105] = "erfcl", [106] = "erff", [107] = "erfl", [108] = "exp", [109] = "exp2", [110] = "exp2f", [111] = "exp2l", [112] = "expf", [113] = "expl", [114] = "expm1", [115] = "expm1f", [116] = "expm1l", [117] = "fabs", [118] = "fabsf", [119] = "fabsl", [120] = "fdim", [121] = "fdimf", [122] = "fdiml", [123] = "feclearexcept", [124] = "fegetround", [125] = "floor", [126] = "floorf", [127] = "floorl", [128] = "fma", [129] = "fmaf", [130] = "fmal", [131] = "fmax", [132] = "fmaxf", [133] = "fmaxl", [134] = "fmin", [135] = "fminf", [136] = "fminl", [137] = "fmod", [138] = "fmodf", [139] = "fmodl", [140] = "fpclassify", [141] = "hypot", [142] = "hypotf", [143] = "hypotl", [144] = "ilogb", [145] = "ilogbf", [146] = "ilogbl", [147] = "isalnum", [148] = "isalpha", [149] = "isblank", [150] = "iscntrl", [151] = "isdigit", [152] = "isfinite", [153] = "isgraph", [154] = "isgreater", [155] = "isgreaterequal", [156] = "isinf", [157] = "isless", [158] = "islessequal", [159] = "islessgreater", [160] = "islower", [161] = "isnan", [162] = "isnormal", [163] = "isprint", [164] = "ispunct", [165] = "isspace", [166] = "isunordered", [167] = "isupper", [168] = "iswalnum", [169] = "iswalpha", [170] = "iswblank", [171] = "iswcntrl", [172] = "iswctype", [173] = "iswdigit", [174] = "iswgraph", [175] = "iswlower", [176] = "iswprint", [177] = "iswpunct", [178] = "iswspace", [179] = "iswupper", [180] = "iswxdigit", [181] = "isxdigit", [182] = "labs", [183] = "ldexp", [184] = "ldexpf", [185] = "ldexpl", [186] = "ldiv", [187] = "lgamma", [188] = "lgammaf", [189] = "lgammal", [190] = "llabs", [191] = "lldiv", [192] = "llrint", [193] = "llrintf", [194] = "llrintl", [195] = "llround", [196] = "llroundf", [197] = "llroundl", [198] = "log10", [199] = "log10f"...}, useretval = std::__debug::set with 341 elements = {[0] = "abs", [1] = "acos", [2] = "acosf", [3] = "acosh", [4] = "acoshf", [5] = "acoshl", [6] = "acosl", [7] = "alloca", [8] = "asctime", [9] = "asin", [10] = "asinf", [11] = "asinh", [12] = "asinhf", [13] = "asinhl", [14] = "asinl", [15] = "atan", [16] = "atan2", [17] = "atan2f", [18] = "atan2l", [19] = "atanf", [20] = "atanh", [21] = "atanhf", [22] = "atanhl", [23] = "atanl", [24] = "atof", [25] = "atoi", [26] = "atol", [27] = "atoll", [28] = "bsearch", [29] = "btowc", [30] = "cabs", [31] = "cabsf", [32] = "cacos", [33] = "cacosf", [34] = "cacoshf", [35] = "cacoshl", [36] = "cacosl", [37] = "calloc", [38] = "carg", [39] = "cargf", [40] = "cargl", [41] = "casin", [42] = "casinf", [43] = "casinh", [44] = "casinhf", [45] = "casinhl", [46] = "casinl", [47] = "catan", [48] = "catanf", [49] = "catanh", [50] = "catanhf", [51] = "catanhl", [52] = "catanl", [53] = "cbrt", [54] = "cbrtf", [55] = "cbrtl", [56] = "ccos", [57] = "ccosf", [58] = "ccosh", [59] = "ccoshf", [60] = "ccoshl", [61] = "ccosl", [62] = "ceil", [63] = "ceilf", [64] = "ceill", [65] = "cexp", [66] = "cexpf", [67] = "cexpl", [68] = "cimagf", [69] = "cimagl", [70] = "clock", [71] = "clog", [72] = "clogf", [73] = "clogl", [74] = "conjf", [75] = "conjl", [76] = "copysign", [77] = "copysignf", [78] = "copysignl", [79] = "cos", [80] = "cosf", [81] = "cosh", [82] = "coshf", [83] = "coshl", [84] = "cosl", [85] = "cpow", [86] = "cpowf", [87] = "cpowl", [88] = "cproj", [89] = "cprojf", [90] = "cprojl", [91] = "creal", [92] = "crealf", [93] = "creall", [94] = "csin", [95] = "csinf", [96] = "csinh", [97] = "csinhf", [98] = "csinhl", [99] = "csinl", [100] = "csqrt", [101] = "csqrtf", [102] = "csqrtl", [103] = "ctan", [104] = "ctanf", [105] = "ctanh", [106] = "ctanhf", [107] = "ctanhl", [108] = "ctanl", [109] = "ctime", [110] = "difftime", [111] = "erf", [112] = "erfc", [113] = "erfcf", [114] = "erfcl", [115] = "erff", [116] = "erfl", [117] = "exp", [118] = "exp2", [119] = "exp2f", [120] = "exp2l", [121] = "expf", [122] = "expl", [123] = "expm1", [124] = "expm1f", [125] = "expm1l", [126] = "fabs", [127] = "fabsf", [128] = "fabsl", [129] = "fdim", [130] = "fdimf", [131] = "fdiml", [132] = "fegetround", [133] = "feof", [134] = "ferror", [135] = "floor", [136] = "floorf", [137] = "floorl", [138] = "fma", [139] = "fmaf", [140] = "fmal", [141] = "fmax", [142] = "fmaxf", [143] = "fmaxl", [144] = "fmin", [145] = "fminf", [146] = "fminl", [147] = "fmod", [148] = "fmodf", [149] = "fmodl", [150] = "fopen", [151] = "fpclassify", [152] = "getenv", [153] = "gmtime", [154] = "hypot", [155] = "hypotf", [156] = "hypotl", [157] = "ilogb", [158] = "ilogbf", [159] = "ilogbl", [160] = "isalnum", [161] = "isalpha", [162] = "isblank", [163] = "iscntrl", [164] = "isdigit", [165] = "isfinite", [166] = "isgraph", [167] = "isgreater", [168] = "isgreaterequal", [169] = "isinf", [170] = "isless", [171] = "islessequal", [172] = "islessgreater", [173] = "islower", [174] = "isnan", [175] = "isnormal", [176] = "isprint", [177] = "ispunct", [178] = "isspace", [179] = "isunordered", [180] = "isupper", [181] = "iswalnum", [182] = "iswalpha", [183] = "iswblank", [184] = "iswcntrl", [185] = "iswctype", [186] = "iswdigit", [187] = "iswgraph", [188] = "iswlower", [189] = "iswprint", [190] = "iswpunct", [191] = "iswspace", [192] = "iswupper", [193] = "iswxdigit", [194] = "isxdigit", [195] = "labs", [196] = "ldexp", [197] = "ldexpf", [198] = "ldexpl", [199] = "lgamma"...}, containers = std::__debug::map with 8 elements = {["stdAllString"] = {startPattern = "", endPattern = "> !!::", functions = std::__debug::map with 21 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["c_str"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER_NT}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["length"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}, ["pop_back"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["push_back"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = true, stdStringLike = true}, ["stdArray"] = {startPattern = "std :: array <", endPattern = "> !!::", functions = std::__debug::map with 16 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = 1, arrayLike_indexOp = true, stdStringLike = false}, ["stdBasicString"] = {startPattern = "std :: basic_string <", endPattern = "> !!::", functions = std::__debug::map with 21 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["c_str"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER_NT}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["length"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}, ["pop_back"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["push_back"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = true, stdStringLike = true}, ["stdContainer"] = {startPattern = "", endPattern = "> !!::", functions = std::__debug::map with 12 elements = {["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = false, stdStringLike = false}, ["stdMap"] = {startPattern = "std :: map <", endPattern = "> !!::", functions = std::__debug::map with 14 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 1, size_templateArgNo = -1, arrayLike_indexOp = false, stdStringLike = false}, ["stdSet"] = {startPattern = "std :: set <", endPattern = "> !!::", functions = std::__debug::map with 13 elements = {["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = false, stdStringLike = false}, ["stdString"] = {startPattern = "std :: string|wstring|u16string|u32string", endPattern = "", functions = std::__debug::map with 21 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["c_str"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER_NT}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["length"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}, ["pop_back"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["push_back"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = true, stdStringLike = true}, ["stdVectorDeque"] = {startPattern = "std :: vector|deque <", endPattern = "> !!::", functions = std::__debug::map with 20 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["pop_back"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["pop_front"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["push_back"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["push_front"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = true, stdStringLike = false}}, argumentChecks = std::__debug::map with 466 elements = {["abs"] = std::__debug::map with 1 elements = {[1] = {notbool = true, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acos"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acosf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acosh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acoshf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acoshl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acosl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["alloca"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asctime"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asin"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atan"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atan2"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atan2f"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atan2l"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atexit"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = true, notuninit = false, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atof"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atoi"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atol"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atoll"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["bsearch"] = std::__debug::map with 5 elements = {[1] = {notbool = false, notnull = true, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = true, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [3] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [4] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [5] = {notbool = false, notnull = true, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["btowc"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cabs"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cabsf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacos"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacosf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacoshf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacoshl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacosl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["calloc"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["carg"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cargf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cargl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casin"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catan"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cbrt"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cbrtf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cbrtl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccos"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccosf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccosh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccoshf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccoshl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccosl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ceil"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ceilf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ceill"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cexp"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cexpf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cexpl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cimagf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cimagl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["clearerr"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = true, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["clog"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["clogf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["clogl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["conjf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["conjl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["copysign"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["copysignf"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["copysignl"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cos"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cosf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cosh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["coshf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["coshl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cosl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cpow"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cpowf"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cpowl"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cproj"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cprojf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cprojl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["creal"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["crealf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["creall"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csin"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csinf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csinh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csinhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csinhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}...}, returnuninitdata = std::__debug::set with 1 elements = {[0] = "malloc"}, defines = std::__debug::vector of length 0, capacity 0, allocid = 3, _files = std::__debug::set with 1 elements = {[0] = "/home/martin/cppcheck/cfg/std.cfg"}, _alloc = std::__debug::map with 5 elements = {["calloc"] = 2, ["fopen"] = 3, ["freopen"] = 3, ["malloc"] = 2, ["tmpfile"] = 3}, _dealloc = std::__debug::map with 2 elements = {["fclose"] = 3, ["free"] = 2}, _noreturn = std::__debug::map with 476 elements = {["abort"] = true, ["abs"] = false, ["acos"] = false, ["acosf"] = false, ["acosh"] = false, ["acoshf"] = false, ["acoshl"] = false, ["acosl"] = false, ["alloca"] = false, ["asctime"] = false, ["asin"] = false, ["asinf"] = false, ["asinh"] = false, ["asinhf"] = false, ["asinhl"] = false, ["asinl"] = false, ["atan"] = false, ["atan2"] = false, ["atan2f"] = false, ["atan2l"] = false, ["atanf"] = false, ["atanh"] = false, ["atanhf"] = false, ["atanhl"] = false, ["atanl"] = false, ["atexit"] = false, ["atof"] = false, ["atoi"] = false, ["atol"] = false, ["atoll"] = false, ["bsearch"] = false, ["btowc"] = false, ["cabs"] = false, ["cabsf"] = false, ["cacos"] = false, ["cacosf"] = false, ["cacoshf"] = false, ["cacoshl"] = false, ["cacosl"] = false, ["calloc"] = false, ["carg"] = false, ["cargf"] = false, ["cargl"] = false, ["casin"] = false, ["casinf"] = false, ["casinh"] = false, ["casinhf"] = false, ["casinhl"] = false, ["casinl"] = false, ["catan"] = false, ["catanf"] = false, ["catanh"] = false, ["catanhf"] = false, ["catanhl"] = false, ["catanl"] = false, ["cbrt"] = false, ["cbrtf"] = false, ["cbrtl"] = false, ["ccos"] = false, ["ccosf"] = false, ["ccosh"] = false, ["ccoshf"] = false, ["ccoshl"] = false, ["ccosl"] = false, ["ceil"] = false, ["ceilf"] = false, ["ceill"] = false, ["cexp"] = false, ["cexpf"] = false, ["cexpl"] = false, ["cimagf"] = false, ["cimagl"] = false, ["clearerr"] = false, ["clock"] = false, ["clog"] = false, ["clogf"] = false, ["clogl"] = false, ["conjf"] = false, ["conjl"] = false, ["copysign"] = false, ["copysignf"] = false, ["copysignl"] = false, ["cos"] = false, ["cosf"] = false, ["cosh"] = false, ["coshf"] = false, ["coshl"] = false, ["cosl"] = false, ["cpow"] = false, ["cpowf"] = false, ["cpowl"] = false, ["cproj"] = false, ["cprojf"] = false, ["cprojl"] = false, ["creal"] = false, ["crealf"] = false, ["creall"] = false, ["csin"] = false, ["csinf"] = false, ["csinh"] = false...}, _ignorefunction = std::__debug::set with 0 elements, _reporterrors = std::__debug::map with 0 elements, _processAfterCode = std::__debug::map with 0 elements, _markupExtensions = std::__debug::set with 0 elements, _keywords = std::__debug::map with 0 elements, _executableblocks = std::__debug::map with 0 elements, _exporters = std::__debug::map with 0 elements, _importers = std::__debug::map with 0 elements, _reflection = std::__debug::map with 0 elements, _formatstr = std::__debug::map with 26 elements = {["fnprintf"] = {first = false, second = false}, ["fprintf"] = {first = false, second = false}, ["fscanf"] = {first = true, second = false}, ["fwprintf"] = {first = false, second = false}, ["fwscanf"] = {first = true, second = false}, ["printf"] = {first = false, second = false}, ["scanf"] = {first = true, second = false}, ["snprintf"] = {first = false, second = false}, ["sprintf"] = {first = false, second = false}, ["sscanf"] = {first = true, second = false}, ["swscanf"] = {first = true, second = false}, ["vfprintf"] = {first = false, second = false}, ["vfscanf"] = {first = true, second = false}, ["vfwprintf"] = {first = false, second = false}, ["vfwscanf"] = {first = true, second = false}, ["vprintf"] = {first = false, second = false}, ["vscanf"] = {first = true, second = false}, ["vsnprintf"] = {first = false, second = false}, ["vsprintf"] = {first = false, second = false}, ["vsscanf"] = {first = true, second = false}, ["vswprintf"] = {first = false, second = false}, ["vswscanf"] = {first = true, second = false}, ["vwprintf"] = {first = false, second = false}, ["vwscanf"] = {first = true, second = false}, ["wprintf"] = {first = false, second = false}, ["wscanf"] = {first = true, second = false}}, podtypes = std::__debug::map with 49 elements = {["FILE"] = {size = 0, sign = 0 '\000'}, ["clock_t"] = {size = 0, sign = 0 '\000'}, ["div_t"] = {size = 0, sign = 0 '\000'}, ["double_t"] = {size = 0, sign = 0 '\000'}, ["fenv_t"] = {size = 0, sign = 0 '\000'}, ["fexcept_t"] = {size = 0, sign = 0 '\000'}, ["float_t"] = {size = 0, sign = 0 '\000'}, ["fpos_t"] = {size = 0, sign = 0 '\000'}, ["int16_t"] = {size = 2, sign = 115 's'}, ["int32_t"] = {size = 4, sign = 115 's'}, ["int64_t"] = {size = 8, sign = 115 's'}, ["int8_t"] = {size = 1, sign = 115 's'}, ["int_fast16_t"] = {size = 0, sign = 115 's'}, ["int_fast32_t"] = {size = 0, sign = 115 's'}, ["int_fast64_t"] = {size = 0, sign = 115 's'}, ["int_fast8_t"] = {size = 0, sign = 115 's'}, ["int_least16_t"] = {size = 0, sign = 115 's'}, ["int_least32_t"] = {size = 0, sign = 115 's'}, ["int_least64_t"] = {size = 0, sign = 115 's'}, ["int_least8_t"] = {size = 0, sign = 115 's'}, ["intmax_t"] = {size = 0, sign = 115 's'}, ["intptr_t"] = {size = 0, sign = 115 's'}, ["lconv"] = {size = 0, sign = 0 '\000'}, ["ldiv_t"] = {size = 0, sign = 0 '\000'}, ["lldiv_t"] = {size = 0, sign = 0 '\000'}, ["max_align_t"] = {size = 0, sign = 0 '\000'}, ["mbstate_t"] = {size = 0, sign = 0 '\000'}, ["nullptr_t"] = {size = 0, sign = 0 '\000'}, ["ptrdiff_t"] = {size = 0, sign = 0 '\000'}, ["sig_atomic_t"] = {size = 0, sign = 0 '\000'}, ["size_t"] = {size = 0, sign = 117 'u'}, ["time_t"] = {size = 0, sign = 0 '\000'}, ["tm"] = {size = 0, sign = 0 '\000'}, ["uint16_t"] = {size = 2, sign = 117 'u'}, ["uint32_t"] = {size = 4, sign = 117 'u'}, ["uint64_t"] = {size = 8, sign = 117 'u'}, ["uint8_t"] = {size = 1, sign = 117 'u'}, ["uint_fast16_t"] = {size = 0, sign = 117 'u'}, ["uint_fast32_t"] = {size = 0, sign = 117 'u'}, ["uint_fast64_t"] = {size = 0, sign = 117 'u'}, ["uint_fast8_t"] = {size = 0, sign = 117 'u'}, ["uint_least16_t"] = {size = 0, sign = 117 'u'}, ["uint_least32_t"] = {size = 0, sign = 117 'u'}, ["uint_least64_t"] = {size = 0, sign = 117 'u'}, ["uint_least8_t"] = {size = 0, sign = 117 'u'}, ["uintmax_t"] = {size = 0, sign = 117 'u'}, ["uintptr_t"] = {size = 0, sign = 117 'u'}, ["va_list"] = {size = 0, sign = 0 '\000'}, ["wint_t"] = {size = 0, sign = 0 '\000'}}, platform_types = std::__debug::map with 0 elements, platforms = std::__debug::map with 0 elements}, rules = empty std::__debug::list, checkConfiguration = false, checkLibrary = false, standards = {c = Standards::C11, cpp = Standards::CPP11, posix = false}, sizeof_bool = 1, sizeof_short = 2, sizeof_int = 4, sizeof_long = 8, sizeof_long_long = 8, sizeof_float = 4, sizeof_double = 8, sizeof_long_double = 16, sizeof_wchar_t = 4, sizeof_size_t = 8, sizeof_pointer = 8, platformType = Settings::Unspecified}
#11 0x00000000006a5f2d in CppCheckExecutor::check (this=0x7fffffffdbe0, argc=2, argv=0x7fffffffdd98) at cli/cppcheckexecutor.cpp:200
        cppCheck = {<ErrorLogger> = {_vptr.ErrorLogger = 0x930f10 <vtable for CppCheck+16>}, _errorList = empty std::__debug::list, _settings = {_append = "", _enabled = std::__debug::set with 0 elements, _terminate = false, debug = false, debugwarnings = false, debugFalsePositive = false, dump = false, exceptionHandling = false, inconclusive = false, jointSuppressionReport = true, experimental = false, _errorsOnly = false, _inlineSuppressions = false, _verbose = false, _force = false, _relativePaths = false, _basePaths = std::__debug::vector of length 0, capacity 0, _xml = false, _xml_version = 1, _jobs = 1, _loadAverage = 0, _exitCode = 0, _outputFormat = "", _showtime = SHOWTIME_NONE, _includePaths = empty std::__debug::list, _maxConfigs = 12, enforcedLang = Settings::None, nomsg = {_suppressions = std::__debug::map with 0 elements}, nofail = {_suppressions = std::__debug::map with 0 elements}, userDefines = "", userUndefs = std::__debug::set with 0 elements, userIncludes = empty std::__debug::list, configExcludePaths = std::__debug::set with 0 elements, reportProgress = false, library = {use = std::__debug::set with 0 elements, leakignore = std::__debug::set with 466 elements = {[0] = "abs", [1] = "acos", [2] = "acosf", [3] = "acosh", [4] = "acoshf", [5] = "acoshl", [6] = "acosl", [7] = "asctime", [8] = "asin", [9] = "asinf", [10] = "asinh", [11] = "asinhf", [12] = "asinhl", [13] = "asinl", [14] = "assert", [15] = "atan", [16] = "atan2", [17] = "atan2f", [18] = "atan2l", [19] = "atanf", [20] = "atanh", [21] = "atanhf", [22] = "atanhl", [23] = "atanl", [24] = "atexit", [25] = "atof", [26] = "atoi", [27] = "atol", [28] = "atoll", [29] = "bsearch", [30] = "btowc", [31] = "cabs", [32] = "cabsf", [33] = "cacos", [34] = "cacosf", [35] = "cacoshf", [36] = "cacoshl", [37] = "cacosl", [38] = "carg", [39] = "cargf", [40] = "cargl", [41] = "casin", [42] = "casinf", [43] = "casinh", [44] = "casinhf", [45] = "casinhl", [46] = "casinl", [47] = "catan", [48] = "catanf", [49] = "catanh", [50] = "catanhf", [51] = "catanhl", [52] = "catanl", [53] = "cbrt", [54] = "cbrtf", [55] = "cbrtl", [56] = "ccos", [57] = "ccosf", [58] = "ccosh", [59] = "ccoshf", [60] = "ccoshl", [61] = "ccosl", [62] = "ceil", [63] = "ceilf", [64] = "ceill", [65] = "cexp", [66] = "cexpf", [67] = "cexpl", [68] = "cimagf", [69] = "cimagl", [70] = "clearerr", [71] = "clock", [72] = "clog", [73] = "clogf", [74] = "clogl", [75] = "conjf", [76] = "conjl", [77] = "copysign", [78] = "copysignf", [79] = "copysignl", [80] = "cos", [81] = "cosf", [82] = "cosh", [83] = "coshf", [84] = "coshl", [85] = "cosl", [86] = "cpow", [87] = "cpowf", [88] = "cpowl", [89] = "cproj", [90] = "cprojf", [91] = "cprojl", [92] = "creal", [93] = "crealf", [94] = "creall", [95] = "csin", [96] = "csinf", [97] = "csinh", [98] = "csinhf", [99] = "csinhl", [100] = "csinl", [101] = "csqrt", [102] = "csqrtf", [103] = "csqrtl", [104] = "ctan", [105] = "ctanf", [106] = "ctanh", [107] = "ctanhf", [108] = "ctanhl", [109] = "ctanl", [110] = "ctime", [111] = "difftime", [112] = "div", [113] = "erf", [114] = "erfc", [115] = "erfcf", [116] = "erfcl", [117] = "erff", [118] = "erfl", [119] = "exp", [120] = "exp2", [121] = "exp2f", [122] = "exp2l", [123] = "expf", [124] = "expl", [125] = "expm1", [126] = "expm1f", [127] = "expm1l", [128] = "fabs", [129] = "fabsf", [130] = "fabsl", [131] = "fdim", [132] = "fdimf", [133] = "fdiml", [134] = "feclearexcept", [135] = "fegetenv", [136] = "fegetexceptflag", [137] = "fegetround", [138] = "feholdexcept", [139] = "feof", [140] = "feraiseexcept", [141] = "ferror", [142] = "fesetenv", [143] = "fesetexceptflag", [144] = "fesetround", [145] = "fetestexcept", [146] = "feupdateenv", [147] = "fflush", [148] = "fgetc", [149] = "fgetpos", [150] = "fgets", [151] = "fgetwc", [152] = "fgetws", [153] = "floor", [154] = "floorf", [155] = "floorl", [156] = "fma", [157] = "fmaf", [158] = "fmal", [159] = "fmax", [160] = "fmaxf", [161] = "fmaxl", [162] = "fmin", [163] = "fminf", [164] = "fminl", [165] = "fmod", [166] = "fmodf", [167] = "fmodl", [168] = "fnprintf", [169] = "fpclassify", [170] = "fprintf", [171] = "fputc", [172] = "fputs", [173] = "fputwc", [174] = "fputws", [175] = "fread", [176] = "frexp", [177] = "frexpf", [178] = "frexpl", [179] = "fscanf", [180] = "fseek", [181] = "fsetpos", [182] = "ftell", [183] = "fwide", [184] = "fwprintf", [185] = "fwrite", [186] = "fwscanf", [187] = "getc", [188] = "getchar", [189] = "getenv", [190] = "gets", [191] = "getwc", [192] = "getwchar", [193] = "gmtime", [194] = "hypot", [195] = "hypotf", [196] = "hypotl", [197] = "ilogb", [198] = "ilogbf", [199] = "ilogbl"...}, functionconst = std::__debug::set with 0 elements, functionpure = std::__debug::set with 293 elements = {[0] = "abs", [1] = "acos", [2] = "acosf", [3] = "acosh", [4] = "acoshf", [5] = "acoshl", [6] = "acosl", [7] = "asin", [8] = "asinf", [9] = "asinh", [10] = "asinhf", [11] = "asinhl", [12] = "asinl", [13] = "atan", [14] = "atan2", [15] = "atan2f", [16] = "atan2l", [17] = "atanf", [18] = "atanh", [19] = "atanhf", [20] = "atanhl", [21] = "atanl", [22] = "atexit", [23] = "atof", [24] = "atoi", [25] = "atol", [26] = "atoll", [27] = "bsearch", [28] = "btowc", [29] = "cabs", [30] = "cabsf", [31] = "cacos", [32] = "cacosf", [33] = "cacoshf", [34] = "cacoshl", [35] = "cacosl", [36] = "carg", [37] = "cargf", [38] = "cargl", [39] = "casin", [40] = "casinf", [41] = "casinh", [42] = "casinhf", [43] = "casinhl", [44] = "casinl", [45] = "catan", [46] = "catanf", [47] = "catanh", [48] = "catanhf", [49] = "catanhl", [50] = "catanl", [51] = "cbrt", [52] = "cbrtf", [53] = "cbrtl", [54] = "ccos", [55] = "ccosf", [56] = "ccosh", [57] = "ccoshf", [58] = "ccoshl", [59] = "ccosl", [60] = "ceil", [61] = "ceilf", [62] = "ceill", [63] = "cexp", [64] = "cexpf", [65] = "cexpl", [66] = "cimagf", [67] = "cimagl", [68] = "copysign", [69] = "copysignf", [70] = "copysignl", [71] = "cos", [72] = "cosf", [73] = "cosh", [74] = "coshf", [75] = "coshl", [76] = "cosl", [77] = "cpow", [78] = "cpowf", [79] = "cpowl", [80] = "cproj", [81] = "cprojf", [82] = "cprojl", [83] = "creal", [84] = "crealf", [85] = "creall", [86] = "csin", [87] = "csinf", [88] = "csinh", [89] = "csinhf", [90] = "csinhl", [91] = "csinl", [92] = "csqrt", [93] = "csqrtf", [94] = "csqrtl", [95] = "ctan", [96] = "ctanf", [97] = "ctanh", [98] = "ctanhf", [99] = "ctanhl", [100] = "ctanl", [101] = "div", [102] = "erf", [103] = "erfc", [104] = "erfcf", [105] = "erfcl", [106] = "erff", [107] = "erfl", [108] = "exp", [109] = "exp2", [110] = "exp2f", [111] = "exp2l", [112] = "expf", [113] = "expl", [114] = "expm1", [115] = "expm1f", [116] = "expm1l", [117] = "fabs", [118] = "fabsf", [119] = "fabsl", [120] = "fdim", [121] = "fdimf", [122] = "fdiml", [123] = "feclearexcept", [124] = "fegetround", [125] = "floor", [126] = "floorf", [127] = "floorl", [128] = "fma", [129] = "fmaf", [130] = "fmal", [131] = "fmax", [132] = "fmaxf", [133] = "fmaxl", [134] = "fmin", [135] = "fminf", [136] = "fminl", [137] = "fmod", [138] = "fmodf", [139] = "fmodl", [140] = "fpclassify", [141] = "hypot", [142] = "hypotf", [143] = "hypotl", [144] = "ilogb", [145] = "ilogbf", [146] = "ilogbl", [147] = "isalnum", [148] = "isalpha", [149] = "isblank", [150] = "iscntrl", [151] = "isdigit", [152] = "isfinite", [153] = "isgraph", [154] = "isgreater", [155] = "isgreaterequal", [156] = "isinf", [157] = "isless", [158] = "islessequal", [159] = "islessgreater", [160] = "islower", [161] = "isnan", [162] = "isnormal", [163] = "isprint", [164] = "ispunct", [165] = "isspace", [166] = "isunordered", [167] = "isupper", [168] = "iswalnum", [169] = "iswalpha", [170] = "iswblank", [171] = "iswcntrl", [172] = "iswctype", [173] = "iswdigit", [174] = "iswgraph", [175] = "iswlower", [176] = "iswprint", [177] = "iswpunct", [178] = "iswspace", [179] = "iswupper", [180] = "iswxdigit", [181] = "isxdigit", [182] = "labs", [183] = "ldexp", [184] = "ldexpf", [185] = "ldexpl", [186] = "ldiv", [187] = "lgamma", [188] = "lgammaf", [189] = "lgammal", [190] = "llabs", [191] = "lldiv", [192] = "llrint", [193] = "llrintf", [194] = "llrintl", [195] = "llround", [196] = "llroundf", [197] = "llroundl", [198] = "log10", [199] = "log10f"...}, useretval = std::__debug::set with 341 elements = {[0] = "abs", [1] = "acos", [2] = "acosf", [3] = "acosh", [4] = "acoshf", [5] = "acoshl", [6] = "acosl", [7] = "alloca", [8] = "asctime", [9] = "asin", [10] = "asinf", [11] = "asinh", [12] = "asinhf", [13] = "asinhl", [14] = "asinl", [15] = "atan", [16] = "atan2", [17] = "atan2f", [18] = "atan2l", [19] = "atanf", [20] = "atanh", [21] = "atanhf", [22] = "atanhl", [23] = "atanl", [24] = "atof", [25] = "atoi", [26] = "atol", [27] = "atoll", [28] = "bsearch", [29] = "btowc", [30] = "cabs", [31] = "cabsf", [32] = "cacos", [33] = "cacosf", [34] = "cacoshf", [35] = "cacoshl", [36] = "cacosl", [37] = "calloc", [38] = "carg", [39] = "cargf", [40] = "cargl", [41] = "casin", [42] = "casinf", [43] = "casinh", [44] = "casinhf", [45] = "casinhl", [46] = "casinl", [47] = "catan", [48] = "catanf", [49] = "catanh", [50] = "catanhf", [51] = "catanhl", [52] = "catanl", [53] = "cbrt", [54] = "cbrtf", [55] = "cbrtl", [56] = "ccos", [57] = "ccosf", [58] = "ccosh", [59] = "ccoshf", [60] = "ccoshl", [61] = "ccosl", [62] = "ceil", [63] = "ceilf", [64] = "ceill", [65] = "cexp", [66] = "cexpf", [67] = "cexpl", [68] = "cimagf", [69] = "cimagl", [70] = "clock", [71] = "clog", [72] = "clogf", [73] = "clogl", [74] = "conjf", [75] = "conjl", [76] = "copysign", [77] = "copysignf", [78] = "copysignl", [79] = "cos", [80] = "cosf", [81] = "cosh", [82] = "coshf", [83] = "coshl", [84] = "cosl", [85] = "cpow", [86] = "cpowf", [87] = "cpowl", [88] = "cproj", [89] = "cprojf", [90] = "cprojl", [91] = "creal", [92] = "crealf", [93] = "creall", [94] = "csin", [95] = "csinf", [96] = "csinh", [97] = "csinhf", [98] = "csinhl", [99] = "csinl", [100] = "csqrt", [101] = "csqrtf", [102] = "csqrtl", [103] = "ctan", [104] = "ctanf", [105] = "ctanh", [106] = "ctanhf", [107] = "ctanhl", [108] = "ctanl", [109] = "ctime", [110] = "difftime", [111] = "erf", [112] = "erfc", [113] = "erfcf", [114] = "erfcl", [115] = "erff", [116] = "erfl", [117] = "exp", [118] = "exp2", [119] = "exp2f", [120] = "exp2l", [121] = "expf", [122] = "expl", [123] = "expm1", [124] = "expm1f", [125] = "expm1l", [126] = "fabs", [127] = "fabsf", [128] = "fabsl", [129] = "fdim", [130] = "fdimf", [131] = "fdiml", [132] = "fegetround", [133] = "feof", [134] = "ferror", [135] = "floor", [136] = "floorf", [137] = "floorl", [138] = "fma", [139] = "fmaf", [140] = "fmal", [141] = "fmax", [142] = "fmaxf", [143] = "fmaxl", [144] = "fmin", [145] = "fminf", [146] = "fminl", [147] = "fmod", [148] = "fmodf", [149] = "fmodl", [150] = "fopen", [151] = "fpclassify", [152] = "getenv", [153] = "gmtime", [154] = "hypot", [155] = "hypotf", [156] = "hypotl", [157] = "ilogb", [158] = "ilogbf", [159] = "ilogbl", [160] = "isalnum", [161] = "isalpha", [162] = "isblank", [163] = "iscntrl", [164] = "isdigit", [165] = "isfinite", [166] = "isgraph", [167] = "isgreater", [168] = "isgreaterequal", [169] = "isinf", [170] = "isless", [171] = "islessequal", [172] = "islessgreater", [173] = "islower", [174] = "isnan", [175] = "isnormal", [176] = "isprint", [177] = "ispunct", [178] = "isspace", [179] = "isunordered", [180] = "isupper", [181] = "iswalnum", [182] = "iswalpha", [183] = "iswblank", [184] = "iswcntrl", [185] = "iswctype", [186] = "iswdigit", [187] = "iswgraph", [188] = "iswlower", [189] = "iswprint", [190] = "iswpunct", [191] = "iswspace", [192] = "iswupper", [193] = "iswxdigit", [194] = "isxdigit", [195] = "labs", [196] = "ldexp", [197] = "ldexpf", [198] = "ldexpl", [199] = "lgamma"...}, containers = std::__debug::map with 8 elements = {["stdAllString"] = {startPattern = "", endPattern = "> !!::", functions = std::__debug::map with 21 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["c_str"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER_NT}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["length"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}, ["pop_back"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["push_back"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = true, stdStringLike = true}, ["stdArray"] = {startPattern = "std :: array <", endPattern = "> !!::", functions = std::__debug::map with 16 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = 1, arrayLike_indexOp = true, stdStringLike = false}, ["stdBasicString"] = {startPattern = "std :: basic_string <", endPattern = "> !!::", functions = std::__debug::map with 21 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["c_str"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER_NT}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["length"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}, ["pop_back"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["push_back"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = true, stdStringLike = true}, ["stdContainer"] = {startPattern = "", endPattern = "> !!::", functions = std::__debug::map with 12 elements = {["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = false, stdStringLike = false}, ["stdMap"] = {startPattern = "std :: map <", endPattern = "> !!::", functions = std::__debug::map with 14 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 1, size_templateArgNo = -1, arrayLike_indexOp = false, stdStringLike = false}, ["stdSet"] = {startPattern = "std :: set <", endPattern = "> !!::", functions = std::__debug::map with 13 elements = {["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = false, stdStringLike = false}, ["stdString"] = {startPattern = "std :: string|wstring|u16string|u32string", endPattern = "", functions = std::__debug::map with 21 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["c_str"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER_NT}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["length"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}, ["pop_back"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["push_back"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = true, stdStringLike = true}, ["stdVectorDeque"] = {startPattern = "std :: vector|deque <", endPattern = "> !!::", functions = std::__debug::map with 20 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["pop_back"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["pop_front"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["push_back"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["push_front"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = true, stdStringLike = false}}, argumentChecks = std::__debug::map with 466 elements = {["abs"] = std::__debug::map with 1 elements = {[1] = {notbool = true, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acos"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acosf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acosh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acoshf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acoshl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acosl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["alloca"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asctime"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asin"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atan"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atan2"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atan2f"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atan2l"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atexit"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = true, notuninit = false, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atof"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atoi"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atol"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atoll"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["bsearch"] = std::__debug::map with 5 elements = {[1] = {notbool = false, notnull = true, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = true, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [3] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [4] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [5] = {notbool = false, notnull = true, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["btowc"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cabs"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cabsf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacos"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacosf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacoshf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacoshl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacosl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["calloc"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["carg"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cargf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cargl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casin"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catan"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cbrt"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cbrtf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cbrtl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccos"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccosf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccosh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccoshf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccoshl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccosl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ceil"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ceilf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ceill"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cexp"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cexpf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cexpl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cimagf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cimagl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["clearerr"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = true, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["clog"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["clogf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["clogl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["conjf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["conjl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["copysign"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["copysignf"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["copysignl"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cos"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cosf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cosh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["coshf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["coshl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cosl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cpow"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cpowf"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cpowl"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cproj"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cprojf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cprojl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["creal"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["crealf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["creall"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csin"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csinf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csinh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csinhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csinhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}...}, returnuninitdata = std::__debug::set with 1 elements = {[0] = "malloc"}, defines = std::__debug::vector of length 0, capacity 0, allocid = 3, _files = std::__debug::set with 1 elements = {[0] = "/home/martin/cppcheck/cfg/std.cfg"}, _alloc = std::__debug::map with 5 elements = {["calloc"] = 2, ["fopen"] = 3, ["freopen"] = 3, ["malloc"] = 2, ["tmpfile"] = 3}, _dealloc = std::__debug::map with 2 elements = {["fclose"] = 3, ["free"] = 2}, _noreturn = std::__debug::map with 476 elements = {["abort"] = true, ["abs"] = false, ["acos"] = false, ["acosf"] = false, ["acosh"] = false, ["acoshf"] = false, ["acoshl"] = false, ["acosl"] = false, ["alloca"] = false, ["asctime"] = false, ["asin"] = false, ["asinf"] = false, ["asinh"] = false, ["asinhf"] = false, ["asinhl"] = false, ["asinl"] = false, ["atan"] = false, ["atan2"] = false, ["atan2f"] = false, ["atan2l"] = false, ["atanf"] = false, ["atanh"] = false, ["atanhf"] = false, ["atanhl"] = false, ["atanl"] = false, ["atexit"] = false, ["atof"] = false, ["atoi"] = false, ["atol"] = false, ["atoll"] = false, ["bsearch"] = false, ["btowc"] = false, ["cabs"] = false, ["cabsf"] = false, ["cacos"] = false, ["cacosf"] = false, ["cacoshf"] = false, ["cacoshl"] = false, ["cacosl"] = false, ["calloc"] = false, ["carg"] = false, ["cargf"] = false, ["cargl"] = false, ["casin"] = false, ["casinf"] = false, ["casinh"] = false, ["casinhf"] = false, ["casinhl"] = false, ["casinl"] = false, ["catan"] = false, ["catanf"] = false, ["catanh"] = false, ["catanhf"] = false, ["catanhl"] = false, ["catanl"] = false, ["cbrt"] = false, ["cbrtf"] = false, ["cbrtl"] = false, ["ccos"] = false, ["ccosf"] = false, ["ccosh"] = false, ["ccoshf"] = false, ["ccoshl"] = false, ["ccosl"] = false, ["ceil"] = false, ["ceilf"] = false, ["ceill"] = false, ["cexp"] = false, ["cexpf"] = false, ["cexpl"] = false, ["cimagf"] = false, ["cimagl"] = false, ["clearerr"] = false, ["clock"] = false, ["clog"] = false, ["clogf"] = false, ["clogl"] = false, ["conjf"] = false, ["conjl"] = false, ["copysign"] = false, ["copysignf"] = false, ["copysignl"] = false, ["cos"] = false, ["cosf"] = false, ["cosh"] = false, ["coshf"] = false, ["coshl"] = false, ["cosl"] = false, ["cpow"] = false, ["cpowf"] = false, ["cpowl"] = false, ["cproj"] = false, ["cprojf"] = false, ["cprojl"] = false, ["creal"] = false, ["crealf"] = false, ["creall"] = false, ["csin"] = false, ["csinf"] = false, ["csinh"] = false...}, _ignorefunction = std::__debug::set with 0 elements, _reporterrors = std::__debug::map with 0 elements, _processAfterCode = std::__debug::map with 0 elements, _markupExtensions = std::__debug::set with 0 elements, _keywords = std::__debug::map with 0 elements, _executableblocks = std::__debug::map with 0 elements, _exporters = std::__debug::map with 0 elements, _importers = std::__debug::map with 0 elements, _reflection = std::__debug::map with 0 elements, _formatstr = std::__debug::map with 26 elements = {["fnprintf"] = {first = false, second = false}, ["fprintf"] = {first = false, second = false}, ["fscanf"] = {first = true, second = false}, ["fwprintf"] = {first = false, second = false}, ["fwscanf"] = {first = true, second = false}, ["printf"] = {first = false, second = false}, ["scanf"] = {first = true, second = false}, ["snprintf"] = {first = false, second = false}, ["sprintf"] = {first = false, second = false}, ["sscanf"] = {first = true, second = false}, ["swscanf"] = {first = true, second = false}, ["vfprintf"] = {first = false, second = false}, ["vfscanf"] = {first = true, second = false}, ["vfwprintf"] = {first = false, second = false}, ["vfwscanf"] = {first = true, second = false}, ["vprintf"] = {first = false, second = false}, ["vscanf"] = {first = true, second = false}, ["vsnprintf"] = {first = false, second = false}, ["vsprintf"] = {first = false, second = false}, ["vsscanf"] = {first = true, second = false}, ["vswprintf"] = {first = false, second = false}, ["vswscanf"] = {first = true, second = false}, ["vwprintf"] = {first = false, second = false}, ["vwscanf"] = {first = true, second = false}, ["wprintf"] = {first = false, second = false}, ["wscanf"] = {first = true, second = false}}, podtypes = std::__debug::map with 49 elements = {["FILE"] = {size = 0, sign = 0 '\000'}, ["clock_t"] = {size = 0, sign = 0 '\000'}, ["div_t"] = {size = 0, sign = 0 '\000'}, ["double_t"] = {size = 0, sign = 0 '\000'}, ["fenv_t"] = {size = 0, sign = 0 '\000'}, ["fexcept_t"] = {size = 0, sign = 0 '\000'}, ["float_t"] = {size = 0, sign = 0 '\000'}, ["fpos_t"] = {size = 0, sign = 0 '\000'}, ["int16_t"] = {size = 2, sign = 115 's'}, ["int32_t"] = {size = 4, sign = 115 's'}, ["int64_t"] = {size = 8, sign = 115 's'}, ["int8_t"] = {size = 1, sign = 115 's'}, ["int_fast16_t"] = {size = 0, sign = 115 's'}, ["int_fast32_t"] = {size = 0, sign = 115 's'}, ["int_fast64_t"] = {size = 0, sign = 115 's'}, ["int_fast8_t"] = {size = 0, sign = 115 's'}, ["int_least16_t"] = {size = 0, sign = 115 's'}, ["int_least32_t"] = {size = 0, sign = 115 's'}, ["int_least64_t"] = {size = 0, sign = 115 's'}, ["int_least8_t"] = {size = 0, sign = 115 's'}, ["intmax_t"] = {size = 0, sign = 115 's'}, ["intptr_t"] = {size = 0, sign = 115 's'}, ["lconv"] = {size = 0, sign = 0 '\000'}, ["ldiv_t"] = {size = 0, sign = 0 '\000'}, ["lldiv_t"] = {size = 0, sign = 0 '\000'}, ["max_align_t"] = {size = 0, sign = 0 '\000'}, ["mbstate_t"] = {size = 0, sign = 0 '\000'}, ["nullptr_t"] = {size = 0, sign = 0 '\000'}, ["ptrdiff_t"] = {size = 0, sign = 0 '\000'}, ["sig_atomic_t"] = {size = 0, sign = 0 '\000'}, ["size_t"] = {size = 0, sign = 117 'u'}, ["time_t"] = {size = 0, sign = 0 '\000'}, ["tm"] = {size = 0, sign = 0 '\000'}, ["uint16_t"] = {size = 2, sign = 117 'u'}, ["uint32_t"] = {size = 4, sign = 117 'u'}, ["uint64_t"] = {size = 8, sign = 117 'u'}, ["uint8_t"] = {size = 1, sign = 117 'u'}, ["uint_fast16_t"] = {size = 0, sign = 117 'u'}, ["uint_fast32_t"] = {size = 0, sign = 117 'u'}, ["uint_fast64_t"] = {size = 0, sign = 117 'u'}, ["uint_fast8_t"] = {size = 0, sign = 117 'u'}, ["uint_least16_t"] = {size = 0, sign = 117 'u'}, ["uint_least32_t"] = {size = 0, sign = 117 'u'}, ["uint_least64_t"] = {size = 0, sign = 117 'u'}, ["uint_least8_t"] = {size = 0, sign = 117 'u'}, ["uintmax_t"] = {size = 0, sign = 117 'u'}, ["uintptr_t"] = {size = 0, sign = 117 'u'}, ["va_list"] = {size = 0, sign = 0 '\000'}, ["wint_t"] = {size = 0, sign = 0 '\000'}}, platform_types = std::__debug::map with 0 elements, platforms = std::__debug::map with 0 elements}, rules = empty std::__debug::list, checkConfiguration = false, checkLibrary = false, standards = {c = Standards::C11, cpp = Standards::CPP11, posix = false}, sizeof_bool = 1, sizeof_short = 2, sizeof_int = 4, sizeof_long = 8, sizeof_long_long = 8, sizeof_float = 4, sizeof_double = 8, sizeof_long_double = 16, sizeof_wchar_t = 4, sizeof_size_t = 8, sizeof_pointer = 8, platformType = Settings::Unspecified}, _errorLogger = @0x7fffffffdbe0, cfg = "", exitcode = 0, _useGlobalSuppressions = true, tooManyConfigs = false, _simplify = true, fileInfo = empty std::__debug::list}
        settings = @0x7fffffffd170: {_append = "", _enabled = std::__debug::set with 0 elements, _terminate = false, debug = false, debugwarnings = false, debugFalsePositive = false, dump = false, exceptionHandling = false, inconclusive = false, jointSuppressionReport = true, experimental = false, _errorsOnly = false, _inlineSuppressions = false, _verbose = false, _force = false, _relativePaths = false, _basePaths = std::__debug::vector of length 0, capacity 0, _xml = false, _xml_version = 1, _jobs = 1, _loadAverage = 0, _exitCode = 0, _outputFormat = "", _showtime = SHOWTIME_NONE, _includePaths = empty std::__debug::list, _maxConfigs = 12, enforcedLang = Settings::None, nomsg = {_suppressions = std::__debug::map with 0 elements}, nofail = {_suppressions = std::__debug::map with 0 elements}, userDefines = "", userUndefs = std::__debug::set with 0 elements, userIncludes = empty std::__debug::list, configExcludePaths = std::__debug::set with 0 elements, reportProgress = false, library = {use = std::__debug::set with 0 elements, leakignore = std::__debug::set with 466 elements = {[0] = "abs", [1] = "acos", [2] = "acosf", [3] = "acosh", [4] = "acoshf", [5] = "acoshl", [6] = "acosl", [7] = "asctime", [8] = "asin", [9] = "asinf", [10] = "asinh", [11] = "asinhf", [12] = "asinhl", [13] = "asinl", [14] = "assert", [15] = "atan", [16] = "atan2", [17] = "atan2f", [18] = "atan2l", [19] = "atanf", [20] = "atanh", [21] = "atanhf", [22] = "atanhl", [23] = "atanl", [24] = "atexit", [25] = "atof", [26] = "atoi", [27] = "atol", [28] = "atoll", [29] = "bsearch", [30] = "btowc", [31] = "cabs", [32] = "cabsf", [33] = "cacos", [34] = "cacosf", [35] = "cacoshf", [36] = "cacoshl", [37] = "cacosl", [38] = "carg", [39] = "cargf", [40] = "cargl", [41] = "casin", [42] = "casinf", [43] = "casinh", [44] = "casinhf", [45] = "casinhl", [46] = "casinl", [47] = "catan", [48] = "catanf", [49] = "catanh", [50] = "catanhf", [51] = "catanhl", [52] = "catanl", [53] = "cbrt", [54] = "cbrtf", [55] = "cbrtl", [56] = "ccos", [57] = "ccosf", [58] = "ccosh", [59] = "ccoshf", [60] = "ccoshl", [61] = "ccosl", [62] = "ceil", [63] = "ceilf", [64] = "ceill", [65] = "cexp", [66] = "cexpf", [67] = "cexpl", [68] = "cimagf", [69] = "cimagl", [70] = "clearerr", [71] = "clock", [72] = "clog", [73] = "clogf", [74] = "clogl", [75] = "conjf", [76] = "conjl", [77] = "copysign", [78] = "copysignf", [79] = "copysignl", [80] = "cos", [81] = "cosf", [82] = "cosh", [83] = "coshf", [84] = "coshl", [85] = "cosl", [86] = "cpow", [87] = "cpowf", [88] = "cpowl", [89] = "cproj", [90] = "cprojf", [91] = "cprojl", [92] = "creal", [93] = "crealf", [94] = "creall", [95] = "csin", [96] = "csinf", [97] = "csinh", [98] = "csinhf", [99] = "csinhl", [100] = "csinl", [101] = "csqrt", [102] = "csqrtf", [103] = "csqrtl", [104] = "ctan", [105] = "ctanf", [106] = "ctanh", [107] = "ctanhf", [108] = "ctanhl", [109] = "ctanl", [110] = "ctime", [111] = "difftime", [112] = "div", [113] = "erf", [114] = "erfc", [115] = "erfcf", [116] = "erfcl", [117] = "erff", [118] = "erfl", [119] = "exp", [120] = "exp2", [121] = "exp2f", [122] = "exp2l", [123] = "expf", [124] = "expl", [125] = "expm1", [126] = "expm1f", [127] = "expm1l", [128] = "fabs", [129] = "fabsf", [130] = "fabsl", [131] = "fdim", [132] = "fdimf", [133] = "fdiml", [134] = "feclearexcept", [135] = "fegetenv", [136] = "fegetexceptflag", [137] = "fegetround", [138] = "feholdexcept", [139] = "feof", [140] = "feraiseexcept", [141] = "ferror", [142] = "fesetenv", [143] = "fesetexceptflag", [144] = "fesetround", [145] = "fetestexcept", [146] = "feupdateenv", [147] = "fflush", [148] = "fgetc", [149] = "fgetpos", [150] = "fgets", [151] = "fgetwc", [152] = "fgetws", [153] = "floor", [154] = "floorf", [155] = "floorl", [156] = "fma", [157] = "fmaf", [158] = "fmal", [159] = "fmax", [160] = "fmaxf", [161] = "fmaxl", [162] = "fmin", [163] = "fminf", [164] = "fminl", [165] = "fmod", [166] = "fmodf", [167] = "fmodl", [168] = "fnprintf", [169] = "fpclassify", [170] = "fprintf", [171] = "fputc", [172] = "fputs", [173] = "fputwc", [174] = "fputws", [175] = "fread", [176] = "frexp", [177] = "frexpf", [178] = "frexpl", [179] = "fscanf", [180] = "fseek", [181] = "fsetpos", [182] = "ftell", [183] = "fwide", [184] = "fwprintf", [185] = "fwrite", [186] = "fwscanf", [187] = "getc", [188] = "getchar", [189] = "getenv", [190] = "gets", [191] = "getwc", [192] = "getwchar", [193] = "gmtime", [194] = "hypot", [195] = "hypotf", [196] = "hypotl", [197] = "ilogb", [198] = "ilogbf", [199] = "ilogbl"...}, functionconst = std::__debug::set with 0 elements, functionpure = std::__debug::set with 293 elements = {[0] = "abs", [1] = "acos", [2] = "acosf", [3] = "acosh", [4] = "acoshf", [5] = "acoshl", [6] = "acosl", [7] = "asin", [8] = "asinf", [9] = "asinh", [10] = "asinhf", [11] = "asinhl", [12] = "asinl", [13] = "atan", [14] = "atan2", [15] = "atan2f", [16] = "atan2l", [17] = "atanf", [18] = "atanh", [19] = "atanhf", [20] = "atanhl", [21] = "atanl", [22] = "atexit", [23] = "atof", [24] = "atoi", [25] = "atol", [26] = "atoll", [27] = "bsearch", [28] = "btowc", [29] = "cabs", [30] = "cabsf", [31] = "cacos", [32] = "cacosf", [33] = "cacoshf", [34] = "cacoshl", [35] = "cacosl", [36] = "carg", [37] = "cargf", [38] = "cargl", [39] = "casin", [40] = "casinf", [41] = "casinh", [42] = "casinhf", [43] = "casinhl", [44] = "casinl", [45] = "catan", [46] = "catanf", [47] = "catanh", [48] = "catanhf", [49] = "catanhl", [50] = "catanl", [51] = "cbrt", [52] = "cbrtf", [53] = "cbrtl", [54] = "ccos", [55] = "ccosf", [56] = "ccosh", [57] = "ccoshf", [58] = "ccoshl", [59] = "ccosl", [60] = "ceil", [61] = "ceilf", [62] = "ceill", [63] = "cexp", [64] = "cexpf", [65] = "cexpl", [66] = "cimagf", [67] = "cimagl", [68] = "copysign", [69] = "copysignf", [70] = "copysignl", [71] = "cos", [72] = "cosf", [73] = "cosh", [74] = "coshf", [75] = "coshl", [76] = "cosl", [77] = "cpow", [78] = "cpowf", [79] = "cpowl", [80] = "cproj", [81] = "cprojf", [82] = "cprojl", [83] = "creal", [84] = "crealf", [85] = "creall", [86] = "csin", [87] = "csinf", [88] = "csinh", [89] = "csinhf", [90] = "csinhl", [91] = "csinl", [92] = "csqrt", [93] = "csqrtf", [94] = "csqrtl", [95] = "ctan", [96] = "ctanf", [97] = "ctanh", [98] = "ctanhf", [99] = "ctanhl", [100] = "ctanl", [101] = "div", [102] = "erf", [103] = "erfc", [104] = "erfcf", [105] = "erfcl", [106] = "erff", [107] = "erfl", [108] = "exp", [109] = "exp2", [110] = "exp2f", [111] = "exp2l", [112] = "expf", [113] = "expl", [114] = "expm1", [115] = "expm1f", [116] = "expm1l", [117] = "fabs", [118] = "fabsf", [119] = "fabsl", [120] = "fdim", [121] = "fdimf", [122] = "fdiml", [123] = "feclearexcept", [124] = "fegetround", [125] = "floor", [126] = "floorf", [127] = "floorl", [128] = "fma", [129] = "fmaf", [130] = "fmal", [131] = "fmax", [132] = "fmaxf", [133] = "fmaxl", [134] = "fmin", [135] = "fminf", [136] = "fminl", [137] = "fmod", [138] = "fmodf", [139] = "fmodl", [140] = "fpclassify", [141] = "hypot", [142] = "hypotf", [143] = "hypotl", [144] = "ilogb", [145] = "ilogbf", [146] = "ilogbl", [147] = "isalnum", [148] = "isalpha", [149] = "isblank", [150] = "iscntrl", [151] = "isdigit", [152] = "isfinite", [153] = "isgraph", [154] = "isgreater", [155] = "isgreaterequal", [156] = "isinf", [157] = "isless", [158] = "islessequal", [159] = "islessgreater", [160] = "islower", [161] = "isnan", [162] = "isnormal", [163] = "isprint", [164] = "ispunct", [165] = "isspace", [166] = "isunordered", [167] = "isupper", [168] = "iswalnum", [169] = "iswalpha", [170] = "iswblank", [171] = "iswcntrl", [172] = "iswctype", [173] = "iswdigit", [174] = "iswgraph", [175] = "iswlower", [176] = "iswprint", [177] = "iswpunct", [178] = "iswspace", [179] = "iswupper", [180] = "iswxdigit", [181] = "isxdigit", [182] = "labs", [183] = "ldexp", [184] = "ldexpf", [185] = "ldexpl", [186] = "ldiv", [187] = "lgamma", [188] = "lgammaf", [189] = "lgammal", [190] = "llabs", [191] = "lldiv", [192] = "llrint", [193] = "llrintf", [194] = "llrintl", [195] = "llround", [196] = "llroundf", [197] = "llroundl", [198] = "log10", [199] = "log10f"...}, useretval = std::__debug::set with 341 elements = {[0] = "abs", [1] = "acos", [2] = "acosf", [3] = "acosh", [4] = "acoshf", [5] = "acoshl", [6] = "acosl", [7] = "alloca", [8] = "asctime", [9] = "asin", [10] = "asinf", [11] = "asinh", [12] = "asinhf", [13] = "asinhl", [14] = "asinl", [15] = "atan", [16] = "atan2", [17] = "atan2f", [18] = "atan2l", [19] = "atanf", [20] = "atanh", [21] = "atanhf", [22] = "atanhl", [23] = "atanl", [24] = "atof", [25] = "atoi", [26] = "atol", [27] = "atoll", [28] = "bsearch", [29] = "btowc", [30] = "cabs", [31] = "cabsf", [32] = "cacos", [33] = "cacosf", [34] = "cacoshf", [35] = "cacoshl", [36] = "cacosl", [37] = "calloc", [38] = "carg", [39] = "cargf", [40] = "cargl", [41] = "casin", [42] = "casinf", [43] = "casinh", [44] = "casinhf", [45] = "casinhl", [46] = "casinl", [47] = "catan", [48] = "catanf", [49] = "catanh", [50] = "catanhf", [51] = "catanhl", [52] = "catanl", [53] = "cbrt", [54] = "cbrtf", [55] = "cbrtl", [56] = "ccos", [57] = "ccosf", [58] = "ccosh", [59] = "ccoshf", [60] = "ccoshl", [61] = "ccosl", [62] = "ceil", [63] = "ceilf", [64] = "ceill", [65] = "cexp", [66] = "cexpf", [67] = "cexpl", [68] = "cimagf", [69] = "cimagl", [70] = "clock", [71] = "clog", [72] = "clogf", [73] = "clogl", [74] = "conjf", [75] = "conjl", [76] = "copysign", [77] = "copysignf", [78] = "copysignl", [79] = "cos", [80] = "cosf", [81] = "cosh", [82] = "coshf", [83] = "coshl", [84] = "cosl", [85] = "cpow", [86] = "cpowf", [87] = "cpowl", [88] = "cproj", [89] = "cprojf", [90] = "cprojl", [91] = "creal", [92] = "crealf", [93] = "creall", [94] = "csin", [95] = "csinf", [96] = "csinh", [97] = "csinhf", [98] = "csinhl", [99] = "csinl", [100] = "csqrt", [101] = "csqrtf", [102] = "csqrtl", [103] = "ctan", [104] = "ctanf", [105] = "ctanh", [106] = "ctanhf", [107] = "ctanhl", [108] = "ctanl", [109] = "ctime", [110] = "difftime", [111] = "erf", [112] = "erfc", [113] = "erfcf", [114] = "erfcl", [115] = "erff", [116] = "erfl", [117] = "exp", [118] = "exp2", [119] = "exp2f", [120] = "exp2l", [121] = "expf", [122] = "expl", [123] = "expm1", [124] = "expm1f", [125] = "expm1l", [126] = "fabs", [127] = "fabsf", [128] = "fabsl", [129] = "fdim", [130] = "fdimf", [131] = "fdiml", [132] = "fegetround", [133] = "feof", [134] = "ferror", [135] = "floor", [136] = "floorf", [137] = "floorl", [138] = "fma", [139] = "fmaf", [140] = "fmal", [141] = "fmax", [142] = "fmaxf", [143] = "fmaxl", [144] = "fmin", [145] = "fminf", [146] = "fminl", [147] = "fmod", [148] = "fmodf", [149] = "fmodl", [150] = "fopen", [151] = "fpclassify", [152] = "getenv", [153] = "gmtime", [154] = "hypot", [155] = "hypotf", [156] = "hypotl", [157] = "ilogb", [158] = "ilogbf", [159] = "ilogbl", [160] = "isalnum", [161] = "isalpha", [162] = "isblank", [163] = "iscntrl", [164] = "isdigit", [165] = "isfinite", [166] = "isgraph", [167] = "isgreater", [168] = "isgreaterequal", [169] = "isinf", [170] = "isless", [171] = "islessequal", [172] = "islessgreater", [173] = "islower", [174] = "isnan", [175] = "isnormal", [176] = "isprint", [177] = "ispunct", [178] = "isspace", [179] = "isunordered", [180] = "isupper", [181] = "iswalnum", [182] = "iswalpha", [183] = "iswblank", [184] = "iswcntrl", [185] = "iswctype", [186] = "iswdigit", [187] = "iswgraph", [188] = "iswlower", [189] = "iswprint", [190] = "iswpunct", [191] = "iswspace", [192] = "iswupper", [193] = "iswxdigit", [194] = "isxdigit", [195] = "labs", [196] = "ldexp", [197] = "ldexpf", [198] = "ldexpl", [199] = "lgamma"...}, containers = std::__debug::map with 8 elements = {["stdAllString"] = {startPattern = "", endPattern = "> !!::", functions = std::__debug::map with 21 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["c_str"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER_NT}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["length"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}, ["pop_back"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["push_back"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = true, stdStringLike = true}, ["stdArray"] = {startPattern = "std :: array <", endPattern = "> !!::", functions = std::__debug::map with 16 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = 1, arrayLike_indexOp = true, stdStringLike = false}, ["stdBasicString"] = {startPattern = "std :: basic_string <", endPattern = "> !!::", functions = std::__debug::map with 21 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["c_str"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER_NT}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["length"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}, ["pop_back"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["push_back"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = true, stdStringLike = true}, ["stdContainer"] = {startPattern = "", endPattern = "> !!::", functions = std::__debug::map with 12 elements = {["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = false, stdStringLike = false}, ["stdMap"] = {startPattern = "std :: map <", endPattern = "> !!::", functions = std::__debug::map with 14 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 1, size_templateArgNo = -1, arrayLike_indexOp = false, stdStringLike = false}, ["stdSet"] = {startPattern = "std :: set <", endPattern = "> !!::", functions = std::__debug::map with 13 elements = {["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = false, stdStringLike = false}, ["stdString"] = {startPattern = "std :: string|wstring|u16string|u32string", endPattern = "", functions = std::__debug::map with 21 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["c_str"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER_NT}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["find"] = {action = Library::Container::FIND, yield = Library::Container::NO_YIELD}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["length"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}, ["pop_back"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["push_back"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = true, stdStringLike = true}, ["stdVectorDeque"] = {startPattern = "std :: vector|deque <", endPattern = "> !!::", functions = std::__debug::map with 20 elements = {["at"] = {action = Library::Container::NO_ACTION, yield = Library::Container::AT_INDEX}, ["back"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["begin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["cend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["clear"] = {action = Library::Container::CLEAR, yield = Library::Container::NO_YIELD}, ["crbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["crend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["data"] = {action = Library::Container::NO_ACTION, yield = Library::Container::BUFFER}, ["empty"] = {action = Library::Container::NO_ACTION, yield = Library::Container::EMPTY}, ["end"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["front"] = {action = Library::Container::NO_ACTION, yield = Library::Container::ITEM}, ["pop_back"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["pop_front"] = {action = Library::Container::POP, yield = Library::Container::NO_YIELD}, ["push_back"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["push_front"] = {action = Library::Container::PUSH, yield = Library::Container::NO_YIELD}, ["rbegin"] = {action = Library::Container::NO_ACTION, yield = Library::Container::START_ITERATOR}, ["rend"] = {action = Library::Container::NO_ACTION, yield = Library::Container::END_ITERATOR}, ["resize"] = {action = Library::Container::RESIZE, yield = Library::Container::NO_YIELD}, ["size"] = {action = Library::Container::NO_ACTION, yield = Library::Container::SIZE}}, type_templateArgNo = 0, size_templateArgNo = -1, arrayLike_indexOp = true, stdStringLike = false}}, argumentChecks = std::__debug::map with 466 elements = {["abs"] = std::__debug::map with 1 elements = {[1] = {notbool = true, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acos"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acosf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acosh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acoshf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acoshl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["acosl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["alloca"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asctime"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asin"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["asinl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atan"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atan2"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atan2f"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atan2l"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atanl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atexit"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = true, notuninit = false, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atof"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atoi"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atol"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["atoll"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["bsearch"] = std::__debug::map with 5 elements = {[1] = {notbool = false, notnull = true, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = true, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [3] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [4] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [5] = {notbool = false, notnull = true, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["btowc"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cabs"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cabsf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacos"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacosf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacoshf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacoshl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cacosl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["calloc"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["carg"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cargf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cargl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casin"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["casinl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catan"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["catanl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cbrt"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cbrtf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cbrtl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccos"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccosf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccosh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccoshf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccoshl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ccosl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ceil"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ceilf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["ceill"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cexp"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cexpf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cexpl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cimagf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cimagl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["clearerr"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = true, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["clog"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["clogf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["clogl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["conjf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["conjl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["copysign"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["copysignf"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["copysignl"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cos"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cosf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cosh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["coshf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["coshl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cosl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cpow"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cpowf"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cpowl"] = std::__debug::map with 2 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}, [2] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cproj"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cprojf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["cprojl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["creal"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["crealf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["creall"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csin"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csinf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csinh"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csinhf"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}, ["csinhl"] = std::__debug::map with 1 elements = {[1] = {notbool = false, notnull = false, notuninit = true, formatstr = false, strz = false, valid = "", minsizes = empty std::__debug::list}}...}, returnuninitdata = std::__debug::set with 1 elements = {[0] = "malloc"}, defines = std::__debug::vector of length 0, capacity 0, allocid = 3, _files = std::__debug::set with 1 elements = {[0] = "/home/martin/cppcheck/cfg/std.cfg"}, _alloc = std::__debug::map with 5 elements = {["calloc"] = 2, ["fopen"] = 3, ["freopen"] = 3, ["malloc"] = 2, ["tmpfile"] = 3}, _dealloc = std::__debug::map with 2 elements = {["fclose"] = 3, ["free"] = 2}, _noreturn = std::__debug::map with 476 elements = {["abort"] = true, ["abs"] = false, ["acos"] = false, ["acosf"] = false, ["acosh"] = false, ["acoshf"] = false, ["acoshl"] = false, ["acosl"] = false, ["alloca"] = false, ["asctime"] = false, ["asin"] = false, ["asinf"] = false, ["asinh"] = false, ["asinhf"] = false, ["asinhl"] = false, ["asinl"] = false, ["atan"] = false, ["atan2"] = false, ["atan2f"] = false, ["atan2l"] = false, ["atanf"] = false, ["atanh"] = false, ["atanhf"] = false, ["atanhl"] = false, ["atanl"] = false, ["atexit"] = false, ["atof"] = false, ["atoi"] = false, ["atol"] = false, ["atoll"] = false, ["bsearch"] = false, ["btowc"] = false, ["cabs"] = false, ["cabsf"] = false, ["cacos"] = false, ["cacosf"] = false, ["cacoshf"] = false, ["cacoshl"] = false, ["cacosl"] = false, ["calloc"] = false, ["carg"] = false, ["cargf"] = false, ["cargl"] = false, ["casin"] = false, ["casinf"] = false, ["casinh"] = false, ["casinhf"] = false, ["casinhl"] = false, ["casinl"] = false, ["catan"] = false, ["catanf"] = false, ["catanh"] = false, ["catanhf"] = false, ["catanhl"] = false, ["catanl"] = false, ["cbrt"] = false, ["cbrtf"] = false, ["cbrtl"] = false, ["ccos"] = false, ["ccosf"] = false, ["ccosh"] = false, ["ccoshf"] = false, ["ccoshl"] = false, ["ccosl"] = false, ["ceil"] = false, ["ceilf"] = false, ["ceill"] = false, ["cexp"] = false, ["cexpf"] = false, ["cexpl"] = false, ["cimagf"] = false, ["cimagl"] = false, ["clearerr"] = false, ["clock"] = false, ["clog"] = false, ["clogf"] = false, ["clogl"] = false, ["conjf"] = false, ["conjl"] = false, ["copysign"] = false, ["copysignf"] = false, ["copysignl"] = false, ["cos"] = false, ["cosf"] = false, ["cosh"] = false, ["coshf"] = false, ["coshl"] = false, ["cosl"] = false, ["cpow"] = false, ["cpowf"] = false, ["cpowl"] = false, ["cproj"] = false, ["cprojf"] = false, ["cprojl"] = false, ["creal"] = false, ["crealf"] = false, ["creall"] = false, ["csin"] = false, ["csinf"] = false, ["csinh"] = false...}, _ignorefunction = std::__debug::set with 0 elements, _reporterrors = std::__debug::map with 0 elements, _processAfterCode = std::__debug::map with 0 elements, _markupExtensions = std::__debug::set with 0 elements, _keywords = std::__debug::map with 0 elements, _executableblocks = std::__debug::map with 0 elements, _exporters = std::__debug::map with 0 elements, _importers = std::__debug::map with 0 elements, _reflection = std::__debug::map with 0 elements, _formatstr = std::__debug::map with 26 elements = {["fnprintf"] = {first = false, second = false}, ["fprintf"] = {first = false, second = false}, ["fscanf"] = {first = true, second = false}, ["fwprintf"] = {first = false, second = false}, ["fwscanf"] = {first = true, second = false}, ["printf"] = {first = false, second = false}, ["scanf"] = {first = true, second = false}, ["snprintf"] = {first = false, second = false}, ["sprintf"] = {first = false, second = false}, ["sscanf"] = {first = true, second = false}, ["swscanf"] = {first = true, second = false}, ["vfprintf"] = {first = false, second = false}, ["vfscanf"] = {first = true, second = false}, ["vfwprintf"] = {first = false, second = false}, ["vfwscanf"] = {first = true, second = false}, ["vprintf"] = {first = false, second = false}, ["vscanf"] = {first = true, second = false}, ["vsnprintf"] = {first = false, second = false}, ["vsprintf"] = {first = false, second = false}, ["vsscanf"] = {first = true, second = false}, ["vswprintf"] = {first = false, second = false}, ["vswscanf"] = {first = true, second = false}, ["vwprintf"] = {first = false, second = false}, ["vwscanf"] = {first = true, second = false}, ["wprintf"] = {first = false, second = false}, ["wscanf"] = {first = true, second = false}}, podtypes = std::__debug::map with 49 elements = {["FILE"] = {size = 0, sign = 0 '\000'}, ["clock_t"] = {size = 0, sign = 0 '\000'}, ["div_t"] = {size = 0, sign = 0 '\000'}, ["double_t"] = {size = 0, sign = 0 '\000'}, ["fenv_t"] = {size = 0, sign = 0 '\000'}, ["fexcept_t"] = {size = 0, sign = 0 '\000'}, ["float_t"] = {size = 0, sign = 0 '\000'}, ["fpos_t"] = {size = 0, sign = 0 '\000'}, ["int16_t"] = {size = 2, sign = 115 's'}, ["int32_t"] = {size = 4, sign = 115 's'}, ["int64_t"] = {size = 8, sign = 115 's'}, ["int8_t"] = {size = 1, sign = 115 's'}, ["int_fast16_t"] = {size = 0, sign = 115 's'}, ["int_fast32_t"] = {size = 0, sign = 115 's'}, ["int_fast64_t"] = {size = 0, sign = 115 's'}, ["int_fast8_t"] = {size = 0, sign = 115 's'}, ["int_least16_t"] = {size = 0, sign = 115 's'}, ["int_least32_t"] = {size = 0, sign = 115 's'}, ["int_least64_t"] = {size = 0, sign = 115 's'}, ["int_least8_t"] = {size = 0, sign = 115 's'}, ["intmax_t"] = {size = 0, sign = 115 's'}, ["intptr_t"] = {size = 0, sign = 115 's'}, ["lconv"] = {size = 0, sign = 0 '\000'}, ["ldiv_t"] = {size = 0, sign = 0 '\000'}, ["lldiv_t"] = {size = 0, sign = 0 '\000'}, ["max_align_t"] = {size = 0, sign = 0 '\000'}, ["mbstate_t"] = {size = 0, sign = 0 '\000'}, ["nullptr_t"] = {size = 0, sign = 0 '\000'}, ["ptrdiff_t"] = {size = 0, sign = 0 '\000'}, ["sig_atomic_t"] = {size = 0, sign = 0 '\000'}, ["size_t"] = {size = 0, sign = 117 'u'}, ["time_t"] = {size = 0, sign = 0 '\000'}, ["tm"] = {size = 0, sign = 0 '\000'}, ["uint16_t"] = {size = 2, sign = 117 'u'}, ["uint32_t"] = {size = 4, sign = 117 'u'}, ["uint64_t"] = {size = 8, sign = 117 'u'}, ["uint8_t"] = {size = 1, sign = 117 'u'}, ["uint_fast16_t"] = {size = 0, sign = 117 'u'}, ["uint_fast32_t"] = {size = 0, sign = 117 'u'}, ["uint_fast64_t"] = {size = 0, sign = 117 'u'}, ["uint_fast8_t"] = {size = 0, sign = 117 'u'}, ["uint_least16_t"] = {size = 0, sign = 117 'u'}, ["uint_least32_t"] = {size = 0, sign = 117 'u'}, ["uint_least64_t"] = {size = 0, sign = 117 'u'}, ["uint_least8_t"] = {size = 0, sign = 117 'u'}, ["uintmax_t"] = {size = 0, sign = 117 'u'}, ["uintptr_t"] = {size = 0, sign = 117 'u'}, ["va_list"] = {size = 0, sign = 0 '\000'}, ["wint_t"] = {size = 0, sign = 0 '\000'}}, platform_types = std::__debug::map with 0 elements, platforms = std::__debug::map with 0 elements}, rules = empty std::__debug::list, checkConfiguration = false, checkLibrary = false, standards = {c = Standards::C11, cpp = Standards::CPP11, posix = false}, sizeof_bool = 1, sizeof_short = 2, sizeof_int = 4, sizeof_long = 8, sizeof_long_long = 8, sizeof_float = 4, sizeof_double = 8, sizeof_long_double = 16, sizeof_wchar_t = 4, sizeof_size_t = 8, sizeof_pointer = 8, platformType = Settings::Unspecified}
#12 0x00000000006b1d6d in main (argc=2, argv=0x7fffffffdd98) at cli/main.cpp:129
        exec = {<ErrorLogger> = {_vptr.ErrorLogger = 0x908cd0 <vtable for CppCheckExecutor+16>}, _settings = 0x7fffffffd170, _errorList = std::__debug::set with 0 elements, _files = std::__debug::map with 1 elements = {["crash227.cpp"] = 195917}, time1 = 0, static exceptionOutput = "", errorlist = false}


registers:
rax            0xffffffffffffffec	-20
rbx            0x0	0
rcx            0x0	0
rdx            0x93d976	9689462
rsi            0x93d976	9689462
rdi            0x11ae590	18539920
rbp            0x7fffffffb4a0	0x7fffffffb4a0
rsp            0x7fffffffb490	0x7fffffffb490
r8             0x7fffffffc1e0	140737488339424
r9             0x7fffffffb580	140737488336256
r10            0xfffffffffffffec9	-311
r11            0x7ffff7102790	140737338419088
r12            0x7fffffffc1e0	140737488339424
r13            0x1	1
r14            0x1	1
r15            0x0	0
rip            0x69cbee	0x69cbee <std::string::size() const+24>
eflags         0x10293	[ CF AF SF IF RF ]
cs             0x33	51
ss             0x2b	43
ds             0x0	0
es             0x0	0
fs             0x0	0
gs             0x0	0


current instructions:
=> 0x69cbee <std::string::size() const+24>:	mov    (%rax),%rax
   0x69cbf1 <std::string::size() const+27>:	leaveq 
   0x69cbf2 <std::string::size() const+28>:	retq   
   0x69cbf3 <std::string::_S_construct<char const*>(char const*, char const*, std::allocator<char> const&)>:	push   %rbp
   0x69cbf4 <std::string::_S_construct<char const*>(char const*, char const*, std::allocator<char> const&)+1>:	mov    %rsp,%rbp
   0x69cbf7 <std::string::_S_construct<char const*>(char const*, char const*, std::allocator<char> const&)+4>:	sub    $0x30,%rsp
   0x69cbfb <std::string::_S_construct<char const*>(char const*, char const*, std::allocator<char> const&)+8>:	mov    %rdi,-0x18(%rbp)
   0x69cbff <std::string::_S_construct<char const*>(char const*, char const*, std::allocator<char> const&)+12>:	mov    %rsi,-0x20(%rbp)
   0x69cc03 <std::string::_S_construct<char const*>(char const*, char const*, std::allocator<char> const&)+16>:	mov    %rdx,-0x28(%rbp)
   0x69cc07 <std::string::_S_construct<char const*>(char const*, char const*, std::allocator<char> const&)+20>:	mov    %fs:0x28,%rax
   0x69cc10 <std::string::_S_construct<char const*>(char const*, char const*, std::allocator<char> const&)+29>:	mov    %rax,-0x8(%rbp)
   0x69cc14 <std::string::_S_construct<char const*>(char const*, char const*, std::allocator<char> const&)+33>:	xor    %eax,%eax
   0x69cc16 <std::string::_S_construct<char const*>(char const*, char const*, std::allocator<char> const&)+35>:	sub    $0x8,%rsp
   0x69cc1a <std::string::_S_construct<char const*>(char const*, char const*, std::allocator<char> const&)+39>:	mov    -0x28(%rbp),%rdx
   0x69cc1e <std::string::_S_construct<char const*>(char const*, char const*, std::allocator<char> const&)+43>:	mov    -0x20(%rbp),%rcx
   0x69cc22 <std::string::_S_construct<char const*>(char const*, char const*, std::allocator<char> const&)+47>:	mov    -0x18(%rbp),%rax


threads backtrace:

Thread 1 (Thread 0x7ffff7fc8780 (LWP 18672)):
#0  0x000000000069cbee in std::string::size (this=0x11ae590) at /usr/include/c++/4.9/bits/basic_string.h:725
#1  0x000000000069b2bf in std::string::compare (this=0x11ae590, __s=0x93d976 "{") at /usr/include/c++/4.9/bits/basic_string.tcc:952
#2  0x000000000069ab9b in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=<error reading variable: Cannot access memory at address 0xffffffffffffffec>, __rhs=0x93d976 "{") at /usr/include/c++/4.9/bits/basic_string.h:2540
#3  0x000000000089d1f1 in setVarIdClassDeclaration (startToken=0x12c4340, variableId=std::__debug::map with 19 elements = {...}, scopeStartVarId=0, structMembers=0x7fffffffba20, _varId=0x7fffffffc1e0) at lib/tokenize.cpp:2468
#4  0x000000000089e1a1 in Tokenizer::setVarId (this=0x7fffffffc120) at lib/tokenize.cpp:2608
#5  0x00000000008a3b4c in Tokenizer::simplifyTokenList1 (this=0x7fffffffc120, FileName=0xc11f48 "crash227.cpp") at lib/tokenize.cpp:3602
#6  0x0000000000898d2f in Tokenizer::tokenize (this=0x7fffffffc120, code=..., FileName=0xc11f48 "crash227.cpp", configuration="", noSymbolDB_AST=false) at lib/tokenize.cpp:1626
#7  0x00000000007ce933 in CppCheck::checkFile (this=0x7fffffffd140, code='\n' <repeats 36 times>, "inline void RestoreSOToleranceState() {}", '\n' <repeats 27 times>, "typedef LPCSTR LPCUTF8;\ntypedef LPSTR LPUTF8;\n\nDECLARE_HANDLE(OBJECTHANDLE);\n\nstruct IMDInternalI"..., FileName=0xc11f48 "crash227.cpp", checksums=std::__debug::set with 0 elements) at lib/cppcheck.cpp:330
#8  0x00000000007cd687 in CppCheck::processFile (this=0x7fffffffd140, filename="crash227.cpp", fileStream=...) at lib/cppcheck.cpp:237
#9  0x00000000007cc0f6 in CppCheck::check (this=0x7fffffffd140, path="crash227.cpp") at lib/cppcheck.cpp:70
#10 0x00000000006a70ae in CppCheckExecutor::check_internal (this=0x7fffffffdbe0, cppcheck=..., argv=0x7fffffffdd98) at cli/cppcheckexecutor.cpp:775
#11 0x00000000006a5f2d in CppCheckExecutor::check (this=0x7fffffffdbe0, argc=2, argv=0x7fffffffdd98) at cli/cppcheckexecutor.cpp:200
#12 0x00000000006b1d6d in main (argc=2, argv=0x7fffffffdd98) at cli/main.cpp:129
*/
