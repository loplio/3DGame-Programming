#include "Method.h"
HermiteSpline::HermiteSpline()
{
}

HermiteSpline::~HermiteSpline()
{
	if (p_position) delete[] p_position;
	if (p_vector) delete[] p_vector;
}

void HermiteSpline::SetDiffEq()
{
	DiffEq_x = XMFLOAT4{ position_1.x, position_2.x, vector_1.x, vector_2.x };
	DiffEq_y = XMFLOAT4{ position_1.y, position_2.y, vector_1.y, vector_2.y };
	DiffEq_z = XMFLOAT4{ position_1.z, position_2.z, vector_1.z, vector_2.z };
}

void HermiteSpline::SetPosition(XMFLOAT3* p, int size)
{
	p_position = p;
	n_Loop = size - 1;
}

void HermiteSpline::SetPosition(const XMFLOAT3& p1, const XMFLOAT3& p2)
{
	position_1 = p1;
	position_2 = p2;
}

void HermiteSpline::SetVector(XMFLOAT3* v)
{
	p_vector = v;
}

void HermiteSpline::SetVector(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	vector_1 = v1;
	vector_2 = v2;
}

void HermiteSpline::SetVar()
{
	xmfVar_x = XMFLOAT4{
	Vector4::DotProduct(HermiteSpline_1, DiffEq_x),
	Vector4::DotProduct(HermiteSpline_2, DiffEq_x),
	Vector4::DotProduct(HermiteSpline_3, DiffEq_x),
	Vector4::DotProduct(HermiteSpline_4, DiffEq_x)
	};

	xmfVar_y = XMFLOAT4{
		Vector4::DotProduct(HermiteSpline_1, DiffEq_y),
		Vector4::DotProduct(HermiteSpline_2, DiffEq_y),
		Vector4::DotProduct(HermiteSpline_3, DiffEq_y),
		Vector4::DotProduct(HermiteSpline_4, DiffEq_y),
	};

	xmfVar_z = XMFLOAT4{
		Vector4::DotProduct(HermiteSpline_1, DiffEq_z),
		Vector4::DotProduct(HermiteSpline_2, DiffEq_z),
		Vector4::DotProduct(HermiteSpline_3, DiffEq_z),
		Vector4::DotProduct(HermiteSpline_4, DiffEq_z),
	};
}

void HermiteSpline::SetEnd(int n)
{
	SetPosition(p_position[n], p_position[n + 1]);

	if (n == 0 && n_Loop > 2) {
		SetVector(p_vector[n], Vector3::Subtract(p_position[n + 1], p_position[n - 1]));
	}
	else if (n == n_Loop && n_Loop > 2) {
		SetVector(p_vector[n - 1], Vector3::Subtract(p_position[n + 1], p_position[n - 1]));
	}
	if (n != 0 && n != n_Loop - 1)
		SetVector(p_vector[n - 1], Vector3::Subtract(p_position[n + 1], p_position[n - 1]));

	SetVector(p_vector[n], p_vector[n + 1]);
	SetDiffEq();
	SetVar();
}

void HermiteSpline::GetInfo(XMFLOAT3* info, int size)
{
	float sep_lenght = 1 / float(size);
	for (int n = 0; n < n_Loop; ++n) {
		float sep_t = 0.0f;
		SetEnd(n);
		for (int i = n * size; i < (n + 1) * size; ++i) {
			XMFLOAT4 xmf_sep{ powf(sep_t,3), powf(sep_t,2) ,powf(sep_t,1) ,1 };
			info[i].x = Vector4::DotProduct(xmf_sep, xmfVar_x);
			info[i].y = Vector4::DotProduct(xmf_sep, xmfVar_y);
			info[i].z = Vector4::DotProduct(xmf_sep, xmfVar_z);
			sep_t += sep_lenght;
		}
	}
}