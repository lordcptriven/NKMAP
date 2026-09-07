

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 04:14:07 2038
 */
/* Compiler settings for ..\DXSDK\Include\DShowIDL\amstream.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0628 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IDirectShowStream,0x7DB01C96,0xC0C3,0x11d0,0x8F,0xF1,0x00,0xC0,0x4F,0xD9,0x18,0x9D);


MIDL_DEFINE_GUID(IID, IID_IAMMultiMediaStream,0xBEBE595C,0x9A6F,0x11d0,0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D);


MIDL_DEFINE_GUID(IID, IID_IAMMediaStream,0xBEBE595D,0x9A6F,0x11d0,0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D);


MIDL_DEFINE_GUID(IID, IID_IMediaStreamFilter,0xBEBE595E,0x9A6F,0x11d0,0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D);


MIDL_DEFINE_GUID(IID, IID_IDirectDrawMediaSampleAllocator,0xAB6B4AFC,0xF6E4,0x11d0,0x90,0x0D,0x00,0xC0,0x4F,0xD9,0x18,0x9D);


MIDL_DEFINE_GUID(IID, IID_IDirectDrawMediaSample,0xAB6B4AFE,0xF6E4,0x11d0,0x90,0x0D,0x00,0xC0,0x4F,0xD9,0x18,0x9D);


MIDL_DEFINE_GUID(IID, IID_IAMMediaTypeStream,0xAB6B4AFA,0xF6E4,0x11d0,0x90,0x0D,0x00,0xC0,0x4F,0xD9,0x18,0x9D);


MIDL_DEFINE_GUID(IID, IID_IAMMediaTypeSample,0xAB6B4AFB,0xF6E4,0x11d0,0x90,0x0D,0x00,0xC0,0x4F,0xD9,0x18,0x9D);


MIDL_DEFINE_GUID(IID, LIBID_DirectShowStreamLib,0x4E6CDE29,0xC0C4,0x11d0,0x8F,0xF1,0x00,0xC0,0x4F,0xD9,0x18,0x9D);


MIDL_DEFINE_GUID(CLSID, CLSID_AMMultiMediaStream,0x49c47ce5,0x9ba4,0x11d0,0x82,0x12,0x00,0xc0,0x4f,0xc3,0x2c,0x45);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



