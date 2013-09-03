// dllmain.h : 模块类的声明。

class CfHashShlExtModule : public CAtlDllModuleT< CfHashShlExtModule >
{
public :
	DECLARE_LIBID(LIBID_fHashShlExtLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_FHASHSHLEXT, "{AA6F714E-CBFA-4DA6-B363-B5692D2E5BA0}")
};

extern class CfHashShlExtModule _AtlModule;
