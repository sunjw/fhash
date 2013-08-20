

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Aug 20 19:24:21 2013
 */
/* Compiler settings for .\fHashShlExt.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __fHashShlExt_i_h__
#define __fHashShlExt_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IfHashShellExt_FWD_DEFINED__
#define __IfHashShellExt_FWD_DEFINED__
typedef interface IfHashShellExt IfHashShellExt;
#endif 	/* __IfHashShellExt_FWD_DEFINED__ */


#ifndef __fHashShellExt_FWD_DEFINED__
#define __fHashShellExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class fHashShellExt fHashShellExt;
#else
typedef struct fHashShellExt fHashShellExt;
#endif /* __cplusplus */

#endif 	/* __fHashShellExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IfHashShellExt_INTERFACE_DEFINED__
#define __IfHashShellExt_INTERFACE_DEFINED__

/* interface IfHashShellExt */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IfHashShellExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D182DF41-B00D-4895-9C5E-620E33332062")
    IfHashShellExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IfHashShellExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfHashShellExt * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfHashShellExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfHashShellExt * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfHashShellExt * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfHashShellExt * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfHashShellExt * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfHashShellExt * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IfHashShellExtVtbl;

    interface IfHashShellExt
    {
        CONST_VTBL struct IfHashShellExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfHashShellExt_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfHashShellExt_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfHashShellExt_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfHashShellExt_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfHashShellExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfHashShellExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfHashShellExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IfHashShellExt_INTERFACE_DEFINED__ */



#ifndef __fHashShlExtLib_LIBRARY_DEFINED__
#define __fHashShlExtLib_LIBRARY_DEFINED__

/* library fHashShlExtLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_fHashShlExtLib;

EXTERN_C const CLSID CLSID_fHashShellExt;

#ifdef __cplusplus

class DECLSPEC_UUID("2B1ADB34-87ED-40E4-84E2-012D298F5370")
fHashShellExt;
#endif
#endif /* __fHashShlExtLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


