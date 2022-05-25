#pragma once
#include "stdafx.h"

class CMethod
{public:
	CMethod() {}
	~CMethod() {}
};

class HermiteSpline : CMethod {
public:
	HermiteSpline();
	~HermiteSpline();

private:
	XMFLOAT3* p_position = NULL;
	XMFLOAT3* p_vector = NULL;

	XMFLOAT4 HermiteSpline_1{ 2.0f, -2.0f, 1.0f, 1.0f };
	XMFLOAT4 HermiteSpline_2{ -3.0f, 3.0f, -2.0f, -1.0f };
	XMFLOAT4 HermiteSpline_3{ 0.0f, 0.0f, 1.0f, 0.0f };
	XMFLOAT4 HermiteSpline_4{ 1.0f, 0.0f, 0.0f, 0.0f };

	XMFLOAT3 position_1{};
	XMFLOAT3 position_2{};
	XMFLOAT3 vector_1{};
	XMFLOAT3 vector_2{};

	XMFLOAT4 DiffEq_x{};
	XMFLOAT4 DiffEq_y{};
	XMFLOAT4 DiffEq_z{};

	XMFLOAT4 xmfVar_x{};
	XMFLOAT4 xmfVar_y{};
	XMFLOAT4 xmfVar_z{};

	UINT n_Loop = 1;

public:
	void SetDiffEq();
	void SetPosition(XMFLOAT3* p, int size);
	void SetPosition(const XMFLOAT3& p1, const XMFLOAT3& p2);
	void SetVector(XMFLOAT3* v);
	void SetVector(const XMFLOAT3& v1, const XMFLOAT3& v2);
	void SetVar();
	void SetEnd(int n);
	void GetInfo(XMFLOAT3* info, int size);
};

