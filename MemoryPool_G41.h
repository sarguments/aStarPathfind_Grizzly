#pragma once

/*---------------------------------------------------------------

�޸� Ǯ Ŭ���� (������Ʈ Ǯ)
Ư�� ����Ÿ(����ü,Ŭ����,����)�� ������ �Ҵ� �� ��������.

- ����.

CMemoryPool<DATA> MemPool(300, false);
DATA *pData = MemPool.Alloc();

pData ���

MemPool.Free(pData);

----------------------------------------------------------------*/

template <class DATA>
class CMemoryPool
{
private:
	// ����ü sizeof �� ��� �е����� ���� ��� ��. pragma pack ���ص� ��
	struct st_BLOCK_NODE
	{
		__int64 _code;
		st_BLOCK_NODE* _nextNode;
		DATA _block;
	};

public:
	//////////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//
	// Parameters:	(int) �ִ� �� ����.
	//				(bool) ������ ȣ�� ����.
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CMemoryPool(int iBlockNum, bool bPlacementNew = false);
	virtual	~CMemoryPool();

	//////////////////////////////////////////////////////////////////////////
	// �� �ϳ��� �Ҵ�޴´�.
	//
	// Parameters: ����.
	// Return: (DATA *) ����Ÿ �� ������.
	//////////////////////////////////////////////////////////////////////////
	DATA	*Alloc(void);

	//////////////////////////////////////////////////////////////////////////
	// ������̴� ���� �����Ѵ�.
	//
	// Parameters: (DATA *) �� ������.
	// Return: (BOOL) TRUE, FALSE.
	//////////////////////////////////////////////////////////////////////////
	bool	Free(DATA *pData);

	//////////////////////////////////////////////////////////////////////////
	// ���� Ȯ�� �� �� ������ ��´�. (�޸�Ǯ ������ ��ü ����)
	//
	// Parameters: ����.
	// Return: (int) �޸� Ǯ ���� ��ü ����
	//////////////////////////////////////////////////////////////////////////
	int		GetAllocCount(void) { return _allocCount; }

	//////////////////////////////////////////////////////////////////////////
	// ���� ������� �� ������ ��´�.
	//
	// Parameters: ����.
	// Return: (int) ������� �� ����.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return _useCount; }

private:
	int _useCount;
	int _allocCount;
	st_BLOCK_NODE* _topPtr;
	char* _poolBuf;
	bool _isPlacement;
	bool _isDynamic;
};

// ������. iBlockNum �� 0�̸� ����(��������Ʈ)
template <class DATA>
CMemoryPool<DATA>::CMemoryPool(int iBlockNum, bool bPlacementNew)
	: _useCount(0), _allocCount(iBlockNum), _topPtr(nullptr),
	_poolBuf(nullptr), _isPlacement(bPlacementNew), _isDynamic(false)
{
	if (_allocCount == 0)
	{
		_isDynamic = true;
		return;
	}

	// �Ҵ� �� ���� ����ü[] �� �����ؼ� ��������� �ʱ�ȭ(new �Ⱦ���)
	_poolBuf = (char*)malloc(sizeof(st_BLOCK_NODE) * _allocCount);
	st_BLOCK_NODE* localNodePtr = (st_BLOCK_NODE*)_poolBuf;

	// plcaement �� ��� �ʱ�ȭ X
	if (_isPlacement == true)
	{
		for (int i = 0; i < _allocCount; i++)
		{
			localNodePtr[i]._code = 0x2525252525252525;
			++_useCount;
		}
	}
	else
	{
		for (int i = 0; i < _allocCount; i++)
		{
			localNodePtr[i]._code = 0x2525252525252525;
			new (&localNodePtr[i]._block) DATA;
			++_useCount;
		}
	}

	// �̱� ����Ʈ ����
	for (int i = 0; i < _allocCount - 1; i++)
	{
		// <top>
		// localNodePtr[0] --> localNodePtr[1] --> localNodePtr[2] ...
		localNodePtr[i]._nextNode = &localNodePtr[i + 1];
	}

	// ���� nextNode�� nullptr
	localNodePtr[_allocCount - 1]._nextNode = nullptr;

	// �ʱ� topPtr�� ù ���
	_topPtr = &localNodePtr[0];
}

template <class DATA>
CMemoryPool<DATA>::~CMemoryPool()
{
	free(_poolBuf);
}

template <class DATA>
DATA * CMemoryPool<DATA>::Alloc(void)
{
	if (_isDynamic == true)
	{
		// ��������Ʈ�� �̹� �Ҵ��� �� �߿� ���� �ִ°� ������ ���� ���� ��.. top�� ������ Ȯ��
		if (_topPtr == nullptr)
		{
			// top�� ���̸� ���� ���� �޸� �Ҵ�
			++_allocCount;
			++_useCount;

			char* newMem = (char*)malloc(sizeof(st_BLOCK_NODE));
			new (newMem) st_BLOCK_NODE;

			st_BLOCK_NODE* newNode = (st_BLOCK_NODE*)newMem;
			newNode->_code = 0x2525252525252525;
			newNode->_nextNode = nullptr;

			return &newNode->_block;
		}
	}

	// ��尡 �ƴ� �� ����
	DATA* toReturnPtr = &(_topPtr->_block);

	// placement true�� ���
	if (_isPlacement == true)
	{
		new (toReturnPtr) DATA;
	}

	// topPtr ���� ����
	_topPtr = _topPtr->_nextNode;

	// ������� ������ ����
	++_useCount;

	// ������Ʈ ��ȯ
	return toReturnPtr;
}

template <class DATA>
bool CMemoryPool<DATA>::Free(DATA * pData)
{
	// �� �����ͷ� ��� ������ ���Ѵ�.
	st_BLOCK_NODE* toFreeNode = (st_BLOCK_NODE*)((char*)pData - (sizeof(__int64) + sizeof(st_BLOCK_NODE*)));

	// �ڵ� üũ(���� ���� ������Ʈ ����)
	if (toFreeNode->_code != 0x2525252525252525)
	{
		return false;
	}

	// placement new �ɼ� true�̸� Free�Ҷ��� ����� �Ҹ��� ȣ��
	if (_isPlacement == true)
	{
		pData->~DATA();
	}

	// ���� ȸ���Ǵ� ����� next�� ���� topPtr
	toFreeNode->_nextNode = _topPtr;

	// topPtr ����
	_topPtr = toFreeNode;

	// ������� ������ ����
	--_useCount;

	return true;
}