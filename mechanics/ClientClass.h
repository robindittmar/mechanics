/// 
/// Source Engine declarations
///

#ifndef __CLIENTCLASS_H__
#define __CLIENTCLASS_H__

#include <Windows.h>

enum class SendPropType {
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY, // Only encodes the XY of a vector, ignores Z
	DPT_String,
	DPT_Array,     // An array of the base types (can't be of datatables).
	DPT_DataTable,
	DPT_Int64,
	DPT_NUMSendPropTypes
};

class RecvProp;
class RecvTable {
public:
	RecvProp *m_pProps;
	int m_nProps;
	void *m_pDecoder;
	char *m_pNetTableName;
	bool m_bInitialized;
	bool m_bInMainList;
};

struct DVariant {
	union {
		float m_Float;
		long m_Int;
		char* m_pString;
		void* m_pData;
		float m_Vector[3];
		__int64 m_Int64;
	};

	int	m_Type;
};

struct CRecvProxyData {
	const RecvProp* m_pRecvProp;
	DVariant m_Value;
	int m_iElement;
	int m_ObjectID;
};

class RecvProp {
public:
	char *m_pVarName;
	SendPropType m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	bool m_bInsideArray;
	const void *m_pExtraData;
	RecvProp *m_pArrayProp;
	void *m_ArrayLengthProxy;
	void *m_ProxyFn;
	void *m_DataTableProxyFn;
	RecvTable *m_pDataTable;
	int m_Offset;
	int m_ElementStride;
	int m_nElements;
	const char *m_pParentArrayPropName;
};

class ClientClass {
public:
	void *m_pCreateFn;
	void *m_pCreateEventFn;
	char *m_pNetworkName;
	RecvTable *m_pRecvTable;
	ClientClass *m_pNext;
	int m_ClassID;
	const char* GetName()
	{
		return m_pNetworkName;
	}

	int GetName(wchar_t* p, int len) {
		mbstowcs(p, m_pNetworkName, len);
		return lstrlenW(p);
	}
};

#endif // __CLIENTCLASS_H__