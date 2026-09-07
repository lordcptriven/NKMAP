

/* this ALWAYS GENERATED file contains the proxy stub code */


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

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "amstream.h"

#define TYPE_FORMAT_STRING_SIZE   271                               
#define PROC_FORMAT_STRING_SIZE   715                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   1            

typedef struct _amstream_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } amstream_MIDL_TYPE_FORMAT_STRING;

typedef struct _amstream_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } amstream_MIDL_PROC_FORMAT_STRING;

typedef struct _amstream_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } amstream_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax_2_0 = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

#if defined(_CONTROL_FLOW_GUARD_XFG)
#define XFG_TRAMPOLINES(ObjectType)\
NDR_SHAREABLE unsigned long ObjectType ## _UserSize_XFG(unsigned long * pFlags, unsigned long Offset, void * pObject)\
{\
return  ObjectType ## _UserSize(pFlags, Offset, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserMarshal_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserMarshal(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserUnmarshal_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserUnmarshal(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE void ObjectType ## _UserFree_XFG(unsigned long * pFlags, void * pObject)\
{\
ObjectType ## _UserFree(pFlags, (ObjectType *)pObject);\
}
#define XFG_TRAMPOLINES64(ObjectType)\
NDR_SHAREABLE unsigned long ObjectType ## _UserSize64_XFG(unsigned long * pFlags, unsigned long Offset, void * pObject)\
{\
return  ObjectType ## _UserSize64(pFlags, Offset, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserMarshal64_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserMarshal64(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserUnmarshal64_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserUnmarshal64(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE void ObjectType ## _UserFree64_XFG(unsigned long * pFlags, void * pObject)\
{\
ObjectType ## _UserFree64(pFlags, (ObjectType *)pObject);\
}
#define XFG_BIND_TRAMPOLINES(HandleType, ObjectType)\
static void* ObjectType ## _bind_XFG(HandleType pObject)\
{\
return ObjectType ## _bind((ObjectType) pObject);\
}\
static void ObjectType ## _unbind_XFG(HandleType pObject, handle_t ServerHandle)\
{\
ObjectType ## _unbind((ObjectType) pObject, ServerHandle);\
}
#define XFG_TRAMPOLINE_FPTR(Function) Function ## _XFG
#define XFG_TRAMPOLINE_FPTR_DEPENDENT_SYMBOL(Symbol) Symbol ## _XFG
#else
#define XFG_TRAMPOLINES(ObjectType)
#define XFG_TRAMPOLINES64(ObjectType)
#define XFG_BIND_TRAMPOLINES(HandleType, ObjectType)
#define XFG_TRAMPOLINE_FPTR(Function) Function
#define XFG_TRAMPOLINE_FPTR_DEPENDENT_SYMBOL(Symbol) Symbol
#endif


extern const amstream_MIDL_TYPE_FORMAT_STRING amstream__MIDL_TypeFormatString;
extern const amstream_MIDL_PROC_FORMAT_STRING amstream__MIDL_ProcFormatString;
extern const amstream_MIDL_EXPR_FORMAT_STRING amstream__MIDL_ExprFormatString;

#ifdef __cplusplus
namespace {
#endif

extern const MIDL_STUB_DESC Object_StubDesc;
#ifdef __cplusplus
}
#endif


extern const MIDL_SERVER_INFO IDirectShowStream_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IDirectShowStream_ProxyInfo;

#ifdef __cplusplus
namespace {
#endif

extern const MIDL_STUB_DESC Object_StubDesc;
#ifdef __cplusplus
}
#endif


extern const MIDL_SERVER_INFO IAMMultiMediaStream_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAMMultiMediaStream_ProxyInfo;

#ifdef __cplusplus
namespace {
#endif

extern const MIDL_STUB_DESC Object_StubDesc;
#ifdef __cplusplus
}
#endif


extern const MIDL_SERVER_INFO IAMMediaStream_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAMMediaStream_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif
#if !(TARGET_IS_NT60_OR_LATER)
#error You need Windows Vista or later to run this stub because it uses these features:
#error   forced complex structure or array, new range semantics, compiled for Windows Vista.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const amstream_MIDL_PROC_FORMAT_STRING amstream__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure get_FileName */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0x8,		/* 8 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 18 */	NdrFcShort( 0x1 ),	/* 1 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 24 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	NdrFcShort( 0x2a ),	/* Type Offset=42 */

	/* Return value */

/* 30 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_FileName */

/* 36 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 38 */	NdrFcLong( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x8 ),	/* 8 */
/* 44 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x8 ),	/* 8 */
/* 50 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 52 */	0x8,		/* 8 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x1 ),	/* 1 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 60 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 62 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 64 */	NdrFcShort( 0x38 ),	/* Type Offset=56 */

	/* Return value */

/* 66 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 68 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 70 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Video */

/* 72 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 74 */	NdrFcLong( 0x0 ),	/* 0 */
/* 78 */	NdrFcShort( 0x9 ),	/* 9 */
/* 80 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 82 */	NdrFcShort( 0x0 ),	/* 0 */
/* 84 */	NdrFcShort( 0x24 ),	/* 36 */
/* 86 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 88 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 90 */	NdrFcShort( 0x0 ),	/* 0 */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 96 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 98 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 100 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Return value */

/* 102 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 104 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 106 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Video */

/* 108 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 110 */	NdrFcLong( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0xa ),	/* 10 */
/* 116 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 118 */	NdrFcShort( 0x8 ),	/* 8 */
/* 120 */	NdrFcShort( 0x8 ),	/* 8 */
/* 122 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 124 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 132 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 134 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 136 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Return value */

/* 138 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 140 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Audio */

/* 144 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 146 */	NdrFcLong( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0xb ),	/* 11 */
/* 152 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 156 */	NdrFcShort( 0x24 ),	/* 36 */
/* 158 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 160 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 168 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 170 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 172 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Return value */

/* 174 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 176 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Audio */

/* 180 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 182 */	NdrFcLong( 0x0 ),	/* 0 */
/* 186 */	NdrFcShort( 0xc ),	/* 12 */
/* 188 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 190 */	NdrFcShort( 0x8 ),	/* 8 */
/* 192 */	NdrFcShort( 0x8 ),	/* 8 */
/* 194 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 196 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 204 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 206 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 208 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Return value */

/* 210 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 212 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Initialize */

/* 216 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 218 */	NdrFcLong( 0x0 ),	/* 0 */
/* 222 */	NdrFcShort( 0xc ),	/* 12 */
/* 224 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 226 */	NdrFcShort( 0xe ),	/* 14 */
/* 228 */	NdrFcShort( 0x8 ),	/* 8 */
/* 230 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 232 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter StreamType */

/* 240 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 242 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 244 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter dwFlags */

/* 246 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 248 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pFilterGraph */

/* 252 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 254 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 256 */	NdrFcShort( 0x46 ),	/* Type Offset=70 */

	/* Return value */

/* 258 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 260 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 262 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetFilterGraph */

/* 264 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 266 */	NdrFcLong( 0x0 ),	/* 0 */
/* 270 */	NdrFcShort( 0xd ),	/* 13 */
/* 272 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 274 */	NdrFcShort( 0x0 ),	/* 0 */
/* 276 */	NdrFcShort( 0x8 ),	/* 8 */
/* 278 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 280 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 282 */	NdrFcShort( 0x0 ),	/* 0 */
/* 284 */	NdrFcShort( 0x0 ),	/* 0 */
/* 286 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppGraphBuilder */

/* 288 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 290 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 292 */	NdrFcShort( 0x58 ),	/* Type Offset=88 */

	/* Return value */

/* 294 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 296 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 298 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetFilter */

/* 300 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 302 */	NdrFcLong( 0x0 ),	/* 0 */
/* 306 */	NdrFcShort( 0xe ),	/* 14 */
/* 308 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 310 */	NdrFcShort( 0x0 ),	/* 0 */
/* 312 */	NdrFcShort( 0x8 ),	/* 8 */
/* 314 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 316 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 318 */	NdrFcShort( 0x0 ),	/* 0 */
/* 320 */	NdrFcShort( 0x0 ),	/* 0 */
/* 322 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppFilter */

/* 324 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 326 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 328 */	NdrFcShort( 0x5c ),	/* Type Offset=92 */

	/* Return value */

/* 330 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 332 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 334 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddMediaStream */

/* 336 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 338 */	NdrFcLong( 0x0 ),	/* 0 */
/* 342 */	NdrFcShort( 0xf ),	/* 15 */
/* 344 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 346 */	NdrFcShort( 0x4c ),	/* 76 */
/* 348 */	NdrFcShort( 0x8 ),	/* 8 */
/* 350 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 352 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 354 */	NdrFcShort( 0x0 ),	/* 0 */
/* 356 */	NdrFcShort( 0x0 ),	/* 0 */
/* 358 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pStreamObject */

/* 360 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 362 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 364 */	NdrFcShort( 0x72 ),	/* Type Offset=114 */

	/* Parameter PurposeId */

/* 366 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 368 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 370 */	NdrFcShort( 0x8e ),	/* Type Offset=142 */

	/* Parameter dwFlags */

/* 372 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 374 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 376 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppNewStream */

/* 378 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 380 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 382 */	NdrFcShort( 0x9a ),	/* Type Offset=154 */

	/* Return value */

/* 384 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 386 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 388 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OpenFile */

/* 390 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 392 */	NdrFcLong( 0x0 ),	/* 0 */
/* 396 */	NdrFcShort( 0x10 ),	/* 16 */
/* 398 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 400 */	NdrFcShort( 0x8 ),	/* 8 */
/* 402 */	NdrFcShort( 0x8 ),	/* 8 */
/* 404 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 406 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 408 */	NdrFcShort( 0x0 ),	/* 0 */
/* 410 */	NdrFcShort( 0x0 ),	/* 0 */
/* 412 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pszFileName */

/* 414 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 416 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 418 */	NdrFcShort( 0xb2 ),	/* Type Offset=178 */

	/* Parameter dwFlags */

/* 420 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 422 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 424 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 426 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 428 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 430 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OpenMoniker */

/* 432 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 434 */	NdrFcLong( 0x0 ),	/* 0 */
/* 438 */	NdrFcShort( 0x11 ),	/* 17 */
/* 440 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 442 */	NdrFcShort( 0x8 ),	/* 8 */
/* 444 */	NdrFcShort( 0x8 ),	/* 8 */
/* 446 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 448 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 450 */	NdrFcShort( 0x0 ),	/* 0 */
/* 452 */	NdrFcShort( 0x0 ),	/* 0 */
/* 454 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCtx */

/* 456 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 458 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 460 */	NdrFcShort( 0xb4 ),	/* Type Offset=180 */

	/* Parameter pMoniker */

/* 462 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 464 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 466 */	NdrFcShort( 0xc6 ),	/* Type Offset=198 */

	/* Parameter dwFlags */

/* 468 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 470 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 472 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 474 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 476 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 478 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Render */

/* 480 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 482 */	NdrFcLong( 0x0 ),	/* 0 */
/* 486 */	NdrFcShort( 0x12 ),	/* 18 */
/* 488 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 490 */	NdrFcShort( 0x8 ),	/* 8 */
/* 492 */	NdrFcShort( 0x8 ),	/* 8 */
/* 494 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 496 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 498 */	NdrFcShort( 0x0 ),	/* 0 */
/* 500 */	NdrFcShort( 0x0 ),	/* 0 */
/* 502 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dwFlags */

/* 504 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 506 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 508 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 510 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 512 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 514 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Initialize */

/* 516 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 518 */	NdrFcLong( 0x0 ),	/* 0 */
/* 522 */	NdrFcShort( 0x9 ),	/* 9 */
/* 524 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 526 */	NdrFcShort( 0x52 ),	/* 82 */
/* 528 */	NdrFcShort( 0x8 ),	/* 8 */
/* 530 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 532 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 534 */	NdrFcShort( 0x0 ),	/* 0 */
/* 536 */	NdrFcShort( 0x0 ),	/* 0 */
/* 538 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pSourceObject */

/* 540 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 542 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 544 */	NdrFcShort( 0x72 ),	/* Type Offset=114 */

	/* Parameter dwFlags */

/* 546 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 548 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 550 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter PurposeId */

/* 552 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 554 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 556 */	NdrFcShort( 0x8e ),	/* Type Offset=142 */

	/* Parameter StreamType */

/* 558 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 560 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 562 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 564 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 566 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 568 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetState */

/* 570 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 572 */	NdrFcLong( 0x0 ),	/* 0 */
/* 576 */	NdrFcShort( 0xa ),	/* 10 */
/* 578 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 580 */	NdrFcShort( 0x6 ),	/* 6 */
/* 582 */	NdrFcShort( 0x8 ),	/* 8 */
/* 584 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 586 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 588 */	NdrFcShort( 0x0 ),	/* 0 */
/* 590 */	NdrFcShort( 0x0 ),	/* 0 */
/* 592 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter State */

/* 594 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 596 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 598 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 600 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 602 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 604 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure JoinAMMultiMediaStream */

/* 606 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 608 */	NdrFcLong( 0x0 ),	/* 0 */
/* 612 */	NdrFcShort( 0xb ),	/* 11 */
/* 614 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 616 */	NdrFcShort( 0x0 ),	/* 0 */
/* 618 */	NdrFcShort( 0x8 ),	/* 8 */
/* 620 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 622 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 624 */	NdrFcShort( 0x0 ),	/* 0 */
/* 626 */	NdrFcShort( 0x0 ),	/* 0 */
/* 628 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAMMultiMediaStream */

/* 630 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 632 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 634 */	NdrFcShort( 0xd8 ),	/* Type Offset=216 */

	/* Return value */

/* 636 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 638 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 640 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure JoinFilter */

/* 642 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 644 */	NdrFcLong( 0x0 ),	/* 0 */
/* 648 */	NdrFcShort( 0xc ),	/* 12 */
/* 650 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 652 */	NdrFcShort( 0x0 ),	/* 0 */
/* 654 */	NdrFcShort( 0x8 ),	/* 8 */
/* 656 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 658 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 660 */	NdrFcShort( 0x0 ),	/* 0 */
/* 662 */	NdrFcShort( 0x0 ),	/* 0 */
/* 664 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pMediaStreamFilter */

/* 666 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 668 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 670 */	NdrFcShort( 0xea ),	/* Type Offset=234 */

	/* Return value */

/* 672 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 674 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 676 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure JoinFilterGraph */

/* 678 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 680 */	NdrFcLong( 0x0 ),	/* 0 */
/* 684 */	NdrFcShort( 0xd ),	/* 13 */
/* 686 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 688 */	NdrFcShort( 0x0 ),	/* 0 */
/* 690 */	NdrFcShort( 0x8 ),	/* 8 */
/* 692 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 694 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 696 */	NdrFcShort( 0x0 ),	/* 0 */
/* 698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 700 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pFilterGraph */

/* 702 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 704 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 706 */	NdrFcShort( 0xfc ),	/* Type Offset=252 */

	/* Return value */

/* 708 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 710 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 712 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const amstream_MIDL_TYPE_FORMAT_STRING amstream__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/*  4 */	NdrFcShort( 0x26 ),	/* Offset= 38 (42) */
/*  6 */	
			0x13, 0x0,	/* FC_OP */
/*  8 */	NdrFcShort( 0x18 ),	/* Offset= 24 (32) */
/* 10 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 12 */	NdrFcShort( 0x2 ),	/* 2 */
/* 14 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 16 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 18 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 20 */	0x0 , 
			0x0,		/* 0 */
/* 22 */	NdrFcLong( 0x0 ),	/* 0 */
/* 26 */	NdrFcLong( 0x0 ),	/* 0 */
/* 30 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 32 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 34 */	NdrFcShort( 0x8 ),	/* 8 */
/* 36 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (10) */
/* 38 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 40 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 42 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 44 */	NdrFcShort( 0x0 ),	/* 0 */
/* 46 */	NdrFcShort( 0x4 ),	/* 4 */
/* 48 */	NdrFcShort( 0x0 ),	/* 0 */
/* 50 */	NdrFcShort( 0xffd4 ),	/* Offset= -44 (6) */
/* 52 */	
			0x12, 0x0,	/* FC_UP */
/* 54 */	NdrFcShort( 0xffea ),	/* Offset= -22 (32) */
/* 56 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */
/* 60 */	NdrFcShort( 0x4 ),	/* 4 */
/* 62 */	NdrFcShort( 0x0 ),	/* 0 */
/* 64 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (52) */
/* 66 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 68 */	0xe,		/* FC_ENUM32 */
			0x5c,		/* FC_PAD */
/* 70 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 72 */	NdrFcLong( 0x56a868a9 ),	/* 1453877417 */
/* 76 */	NdrFcShort( 0xad4 ),	/* 2772 */
/* 78 */	NdrFcShort( 0x11ce ),	/* 4558 */
/* 80 */	0xb0,		/* 176 */
			0x3a,		/* 58 */
/* 82 */	0x0,		/* 0 */
			0x20,		/* 32 */
/* 84 */	0xaf,		/* 175 */
			0xb,		/* 11 */
/* 86 */	0xa7,		/* 167 */
			0x70,		/* 112 */
/* 88 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 90 */	NdrFcShort( 0xffec ),	/* Offset= -20 (70) */
/* 92 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 94 */	NdrFcShort( 0x2 ),	/* Offset= 2 (96) */
/* 96 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 98 */	NdrFcLong( 0xbebe595e ),	/* -1094821538 */
/* 102 */	NdrFcShort( 0x9a6f ),	/* -26001 */
/* 104 */	NdrFcShort( 0x11d0 ),	/* 4560 */
/* 106 */	0x8f,		/* 143 */
			0xde,		/* 222 */
/* 108 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 110 */	0x4f,		/* 79 */
			0xd9,		/* 217 */
/* 112 */	0x18,		/* 24 */
			0x9d,		/* 157 */
/* 114 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 116 */	NdrFcLong( 0x0 ),	/* 0 */
/* 120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 122 */	NdrFcShort( 0x0 ),	/* 0 */
/* 124 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 126 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 128 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 130 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 132 */	
			0x11, 0x0,	/* FC_RP */
/* 134 */	NdrFcShort( 0x8 ),	/* Offset= 8 (142) */
/* 136 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 138 */	NdrFcShort( 0x8 ),	/* 8 */
/* 140 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 142 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 144 */	NdrFcShort( 0x10 ),	/* 16 */
/* 146 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 148 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 150 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (136) */
			0x5b,		/* FC_END */
/* 154 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 156 */	NdrFcShort( 0x2 ),	/* Offset= 2 (158) */
/* 158 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 160 */	NdrFcLong( 0xb502d1bd ),	/* -1258106435 */
/* 164 */	NdrFcShort( 0x9a57 ),	/* -26025 */
/* 166 */	NdrFcShort( 0x11d0 ),	/* 4560 */
/* 168 */	0x8f,		/* 143 */
			0xde,		/* 222 */
/* 170 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 172 */	0x4f,		/* 79 */
			0xd9,		/* 217 */
/* 174 */	0x18,		/* 24 */
			0x9d,		/* 157 */
/* 176 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 178 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 180 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 182 */	NdrFcLong( 0xe ),	/* 14 */
/* 186 */	NdrFcShort( 0x0 ),	/* 0 */
/* 188 */	NdrFcShort( 0x0 ),	/* 0 */
/* 190 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 192 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 194 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 196 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 198 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 200 */	NdrFcLong( 0xf ),	/* 15 */
/* 204 */	NdrFcShort( 0x0 ),	/* 0 */
/* 206 */	NdrFcShort( 0x0 ),	/* 0 */
/* 208 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 210 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 212 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 214 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 216 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 218 */	NdrFcLong( 0xbebe595c ),	/* -1094821540 */
/* 222 */	NdrFcShort( 0x9a6f ),	/* -26001 */
/* 224 */	NdrFcShort( 0x11d0 ),	/* 4560 */
/* 226 */	0x8f,		/* 143 */
			0xde,		/* 222 */
/* 228 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 230 */	0x4f,		/* 79 */
			0xd9,		/* 217 */
/* 232 */	0x18,		/* 24 */
			0x9d,		/* 157 */
/* 234 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 236 */	NdrFcLong( 0xbebe595e ),	/* -1094821538 */
/* 240 */	NdrFcShort( 0x9a6f ),	/* -26001 */
/* 242 */	NdrFcShort( 0x11d0 ),	/* 4560 */
/* 244 */	0x8f,		/* 143 */
			0xde,		/* 222 */
/* 246 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 248 */	0x4f,		/* 79 */
			0xd9,		/* 217 */
/* 250 */	0x18,		/* 24 */
			0x9d,		/* 157 */
/* 252 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 254 */	NdrFcLong( 0x56a8689f ),	/* 1453877407 */
/* 258 */	NdrFcShort( 0xad4 ),	/* 2772 */
/* 260 */	NdrFcShort( 0x11ce ),	/* 4558 */
/* 262 */	0xb0,		/* 176 */
			0x3a,		/* 58 */
/* 264 */	0x0,		/* 0 */
			0x20,		/* 32 */
/* 266 */	0xaf,		/* 175 */
			0xb,		/* 11 */
/* 268 */	0xa7,		/* 167 */
			0x70,		/* 112 */

			0x0
        }
    };

XFG_TRAMPOLINES(BSTR)

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            (USER_MARSHAL_SIZING_ROUTINE)XFG_TRAMPOLINE_FPTR(BSTR_UserSize)
            ,(USER_MARSHAL_MARSHALLING_ROUTINE)XFG_TRAMPOLINE_FPTR(BSTR_UserMarshal)
            ,(USER_MARSHAL_UNMARSHALLING_ROUTINE)XFG_TRAMPOLINE_FPTR(BSTR_UserUnmarshal)
            ,(USER_MARSHAL_FREEING_ROUTINE)XFG_TRAMPOLINE_FPTR(BSTR_UserFree)
            
            }
            

        };



/* Standard interface: __MIDL_itf_amstream_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDirectShowStream, ver. 0.0,
   GUID={0x7DB01C96,0xC0C3,0x11d0,{0x8F,0xF1,0x00,0xC0,0x4F,0xD9,0x18,0x9D}} */

#pragma code_seg(".orpc")
static const unsigned short IDirectShowStream_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    144,
    180
    };

static const MIDL_STUBLESS_PROXY_INFO IDirectShowStream_ProxyInfo =
    {
    &Object_StubDesc,
    amstream__MIDL_ProcFormatString.Format,
    &IDirectShowStream_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IDirectShowStream_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    amstream__MIDL_ProcFormatString.Format,
    &IDirectShowStream_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IDirectShowStreamProxyVtbl = 
{
    &IDirectShowStream_ProxyInfo,
    &IID_IDirectShowStream,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IDirectShowStream::get_FileName */ ,
    (void *) (INT_PTR) -1 /* IDirectShowStream::put_FileName */ ,
    (void *) (INT_PTR) -1 /* IDirectShowStream::get_Video */ ,
    (void *) (INT_PTR) -1 /* IDirectShowStream::put_Video */ ,
    (void *) (INT_PTR) -1 /* IDirectShowStream::get_Audio */ ,
    (void *) (INT_PTR) -1 /* IDirectShowStream::put_Audio */
};


EXTERN_C DECLSPEC_SELECTANY const PRPC_STUB_FUNCTION IDirectShowStream_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IDirectShowStreamStubVtbl =
{
    &IID_IDirectShowStream,
    &IDirectShowStream_ServerInfo,
    13,
    &IDirectShowStream_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IMultiMediaStream, ver. 0.0,
   GUID={0xB502D1BC,0x9A57,0x11d0,{0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D}} */


/* Object interface: IAMMultiMediaStream, ver. 0.0,
   GUID={0xBEBE595C,0x9A6F,0x11d0,{0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D}} */

#pragma code_seg(".orpc")
static const unsigned short IAMMultiMediaStream_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    216,
    264,
    300,
    336,
    390,
    432,
    480
    };

static const MIDL_STUBLESS_PROXY_INFO IAMMultiMediaStream_ProxyInfo =
    {
    &Object_StubDesc,
    amstream__MIDL_ProcFormatString.Format,
    &IAMMultiMediaStream_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAMMultiMediaStream_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    amstream__MIDL_ProcFormatString.Format,
    &IAMMultiMediaStream_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(19) _IAMMultiMediaStreamProxyVtbl = 
{
    &IAMMultiMediaStream_ProxyInfo,
    &IID_IAMMultiMediaStream,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IMultiMediaStream::GetInformation */ ,
    0 /* IMultiMediaStream::GetMediaStream */ ,
    0 /* IMultiMediaStream::EnumMediaStreams */ ,
    0 /* IMultiMediaStream::GetState */ ,
    0 /* IMultiMediaStream::SetState */ ,
    0 /* IMultiMediaStream::GetTime */ ,
    0 /* IMultiMediaStream::GetDuration */ ,
    0 /* IMultiMediaStream::Seek */ ,
    0 /* IMultiMediaStream::GetEndOfStreamEventHandle */ ,
    (void *) (INT_PTR) -1 /* IAMMultiMediaStream::Initialize */ ,
    (void *) (INT_PTR) -1 /* IAMMultiMediaStream::GetFilterGraph */ ,
    (void *) (INT_PTR) -1 /* IAMMultiMediaStream::GetFilter */ ,
    (void *) (INT_PTR) -1 /* IAMMultiMediaStream::AddMediaStream */ ,
    (void *) (INT_PTR) -1 /* IAMMultiMediaStream::OpenFile */ ,
    (void *) (INT_PTR) -1 /* IAMMultiMediaStream::OpenMoniker */ ,
    (void *) (INT_PTR) -1 /* IAMMultiMediaStream::Render */
};


EXTERN_C DECLSPEC_SELECTANY const PRPC_STUB_FUNCTION IAMMultiMediaStream_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAMMultiMediaStreamStubVtbl =
{
    &IID_IAMMultiMediaStream,
    &IAMMultiMediaStream_ServerInfo,
    19,
    &IAMMultiMediaStream_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IMediaStream, ver. 0.0,
   GUID={0xB502D1BD,0x9A57,0x11d0,{0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D}} */


/* Object interface: IAMMediaStream, ver. 0.0,
   GUID={0xBEBE595D,0x9A6F,0x11d0,{0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D}} */

#pragma code_seg(".orpc")
static const unsigned short IAMMediaStream_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    516,
    570,
    606,
    642,
    678
    };

static const MIDL_STUBLESS_PROXY_INFO IAMMediaStream_ProxyInfo =
    {
    &Object_StubDesc,
    amstream__MIDL_ProcFormatString.Format,
    &IAMMediaStream_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAMMediaStream_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    amstream__MIDL_ProcFormatString.Format,
    &IAMMediaStream_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(14) _IAMMediaStreamProxyVtbl = 
{
    &IAMMediaStream_ProxyInfo,
    &IID_IAMMediaStream,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IMediaStream::GetMultiMediaStream */ ,
    0 /* IMediaStream::GetInformation */ ,
    0 /* IMediaStream::SetSameFormat */ ,
    0 /* IMediaStream::AllocateSample */ ,
    0 /* IMediaStream::CreateSharedSample */ ,
    0 /* IMediaStream::SendEndOfStream */ ,
    (void *) (INT_PTR) -1 /* IAMMediaStream::Initialize */ ,
    (void *) (INT_PTR) -1 /* IAMMediaStream::SetState */ ,
    (void *) (INT_PTR) -1 /* IAMMediaStream::JoinAMMultiMediaStream */ ,
    (void *) (INT_PTR) -1 /* IAMMediaStream::JoinFilter */ ,
    (void *) (INT_PTR) -1 /* IAMMediaStream::JoinFilterGraph */
};


EXTERN_C DECLSPEC_SELECTANY const PRPC_STUB_FUNCTION IAMMediaStream_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAMMediaStreamStubVtbl =
{
    &IID_IAMMediaStream,
    &IAMMediaStream_ServerInfo,
    14,
    &IAMMediaStream_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPersist, ver. 0.0,
   GUID={0x0000010c,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IMediaFilter, ver. 0.0,
   GUID={0x56a86899,0x0ad4,0x11ce,{0xb0,0x3a,0x00,0x20,0xaf,0x0b,0xa7,0x70}} */


/* Object interface: IBaseFilter, ver. 0.0,
   GUID={0x56a86895,0x0ad4,0x11ce,{0xb0,0x3a,0x00,0x20,0xaf,0x0b,0xa7,0x70}} */


/* Object interface: IMediaStreamFilter, ver. 0.0,
   GUID={0xBEBE595E,0x9A6F,0x11d0,{0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D}} */


/* Object interface: IDirectDrawMediaSampleAllocator, ver. 0.0,
   GUID={0xAB6B4AFC,0xF6E4,0x11d0,{0x90,0x0D,0x00,0xC0,0x4F,0xD9,0x18,0x9D}} */


/* Object interface: IDirectDrawMediaSample, ver. 0.0,
   GUID={0xAB6B4AFE,0xF6E4,0x11d0,{0x90,0x0D,0x00,0xC0,0x4F,0xD9,0x18,0x9D}} */


/* Object interface: IAMMediaTypeStream, ver. 0.0,
   GUID={0xAB6B4AFA,0xF6E4,0x11d0,{0x90,0x0D,0x00,0xC0,0x4F,0xD9,0x18,0x9D}} */


/* Object interface: IStreamSample, ver. 0.0,
   GUID={0xB502D1BE,0x9A57,0x11d0,{0x8F,0xDE,0x00,0xC0,0x4F,0xD9,0x18,0x9D}} */


/* Object interface: IAMMediaTypeSample, ver. 0.0,
   GUID={0xAB6B4AFB,0xF6E4,0x11d0,{0x90,0x0D,0x00,0xC0,0x4F,0xD9,0x18,0x9D}} */


/* Standard interface: __MIDL_itf_amstream_0000_0009, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */

#ifdef __cplusplus
namespace {
#endif
static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    amstream__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x60001, /* Ndr library version */
    0,
    0x8010274, /* MIDL Version 8.1.628 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#ifdef __cplusplus
}
#endif

const CInterfaceProxyVtbl * const _amstream_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IAMMultiMediaStreamProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAMMediaStreamProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IDirectShowStreamProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _amstream_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IAMMultiMediaStreamStubVtbl,
    ( CInterfaceStubVtbl *) &_IAMMediaStreamStubVtbl,
    ( CInterfaceStubVtbl *) &_IDirectShowStreamStubVtbl,
    0
};

PCInterfaceName const _amstream_InterfaceNamesList[] = 
{
    "IAMMultiMediaStream",
    "IAMMediaStream",
    "IDirectShowStream",
    0
};

const IID *  const _amstream_BaseIIDList[] = 
{
    &IID_IMultiMediaStream,
    &IID_IMediaStream,
    &IID_IDispatch,
    0
};


#define _amstream_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _amstream, pIID, n)

int __stdcall _amstream_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _amstream, 3, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _amstream, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _amstream, 3, *pIndex )
    
}

EXTERN_C const ExtendedProxyFileInfo amstream_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _amstream_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _amstream_StubVtblList,
    (const PCInterfaceName * ) & _amstream_InterfaceNamesList,
    (const IID ** ) & _amstream_BaseIIDList,
    & _amstream_IID_Lookup, 
    3,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

