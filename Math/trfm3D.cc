#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <GL/glut.h>
#include "tools.h"
#include "trfm3D.h"


Trfm3D::Trfm3D() : m_c1(Vector3::UNIT_X), m_c2(Vector3::UNIT_Y), m_c3(Vector3::UNIT_Z),
				   m_tr(Vector3::ZERO), m_d(Vector3::ZERO), m_scl(1.0f), m_w(1.0f) {}

Trfm3D::Trfm3D(const Trfm3D & T) : m_c1(T.m_c1), m_c2(T.m_c2), m_c3(T.m_c3),
								   m_tr(T.m_tr), m_d(T.m_d), m_scl(T.m_scl), m_w(T.m_w) {}

void Trfm3D::swap(Trfm3D & T) {
	m_c1.swap(T.m_c1);
	m_c2.swap(T.m_c2);
	m_c3.swap(T.m_c3);
	m_tr.swap(T.m_tr);
	m_d.swap(T.m_d);
	std::swap(m_scl, T.m_scl);
	std::swap(m_w, T.m_w);
}


// Compute this*add and leave the result in this

void Trfm3D::add( const Trfm3D &T ) {

	float    newR1X, newR1Y, newR1Z;
	float    newR2X, newR2Y, newR2Z;
	float    newR3X, newR3Y, newR3Z;

	float    newDX, newDY, newDZ, newDW;

	float    newTrX, newTrY, newTrZ;

	float c10 = m_c1[0];
	float c11 = m_c1[1];
	float c12 = m_c1[2];
	float c20 = m_c2[0];
	float c21 = m_c2[1];
	float c22 = m_c2[2];
	float c30 = m_c3[0];
	float c31 = m_c3[1];
	float c32 = m_c3[2];
	float tr0 = m_tr[0];
	float tr1 = m_tr[1];
	float tr2 = m_tr[2];

	// New rotation Matrix

	newR1X = newR1Y = newR1Z = 0.0;

	newR1X += c10 * T.m_c1[0];
	newR1X += c20 * T.m_c1[1];
	newR1X += c30 * T.m_c1[2];
	newR1X += tr0 * T.m_d[0] ;

	newR1Y += c11 * T.m_c1[0];
	newR1Y += c21 * T.m_c1[1];
	newR1Y += c31 * T.m_c1[2];
	newR1Y += tr1 * T.m_d[0] ;

	newR1Z += c12 * T.m_c1[0];
	newR1Z += c22 * T.m_c1[1];
	newR1Z += c32 * T.m_c1[2];
	newR1Z += tr2 * T.m_d[0] ;

	newR2X = newR2Y = newR2Z = 0.0;

	newR2X += c10 * T.m_c2[0];
	newR2X += c20 * T.m_c2[1];
	newR2X += c30 * T.m_c2[2];
	newR2X += tr0 * T.m_d[1] ;

	newR2Y += c11 * T.m_c2[0];
	newR2Y += c21 * T.m_c2[1];
	newR2Y += c31 * T.m_c2[2];
	newR2Y += tr1 * T.m_d[1] ;

	newR2Z += c12 * T.m_c2[0];
	newR2Z += c22 * T.m_c2[1];
	newR2Z += c32 * T.m_c2[2];
	newR2Z += tr2 * T.m_d[1] ;

	newR3X = newR3Y = newR3Z = 0.0;

	newR3X += c10 * T.m_c3[0];
	newR3X += c20 * T.m_c3[1];
	newR3X += c30 * T.m_c3[2];
	newR3X += tr0 * T.m_d[2] ;

	newR3Y += c11 * T.m_c3[0];
	newR3Y += c21 * T.m_c3[1];
	newR3Y += c31 * T.m_c3[2];
	newR3Y += tr1 * T.m_d[2] ;

	newR3Z += c12 * T.m_c3[0];
	newR3Z += c22 * T.m_c3[1];
	newR3Z += c32 * T.m_c3[2];
	newR3Z += tr2 * T.m_d[2] ;

	newDX = newDY = newDZ = newDW = 0.0;

	newDX += this->m_d[0] * T.m_c1[0];
	newDX += this->m_d[1] * T.m_c1[1];
	newDX += this->m_d[2] * T.m_c1[2];
	newDX += this->m_w * T.m_d[0];

	newDY += this->m_d[0] * T.m_c2[0];
	newDY += this->m_d[1] * T.m_c2[1];
	newDY += this->m_d[2] * T.m_c2[2];
	newDY += this->m_w * T.m_d[1];

	newDZ += this->m_d[0] * T.m_c3[0];
	newDZ += this->m_d[1] * T.m_c3[1];
	newDZ += this->m_d[2] * T.m_c3[2];
	newDZ += this->m_w * T.m_d[2];

	newDW += this->m_d[0] * T.m_tr[0];
	newDW += this->m_d[1] * T.m_tr[1];
	newDW += this->m_d[2] * T.m_tr[2];
	newDW += this->m_w * T.m_w;

	// New Translation

	newTrX = 0;
	newTrX += this->m_scl * c10 * T.m_tr[0];
	newTrX += this->m_scl * c20 * T.m_tr[1];
	newTrX += this->m_scl * c30 * T.m_tr[2];
	newTrX += tr0 * T.m_w;

	newTrY = 0;
	newTrY += this->m_scl * c11 * T.m_tr[0];
	newTrY += this->m_scl * c21 * T.m_tr[1];
	newTrY += this->m_scl * c31 * T.m_tr[2];
	newTrY += tr1 * T.m_w;

	newTrZ = 0;
	newTrZ += this->m_scl * c12 * T.m_tr[0];
	newTrZ += this->m_scl * c22 * T.m_tr[1];
	newTrZ += this->m_scl * c32 * T.m_tr[2];
	newTrZ += tr2 * T.m_w;

	// New scale

	this->m_scl = T.m_scl * this->m_scl;

	// Copy values

	this->m_c1[0] = newR1X;
	this->m_c1[1] = newR1Y;
	this->m_c1[2] = newR1Z;

	this->m_c2[0] = newR2X;
	this->m_c2[1] = newR2Y;
	this->m_c2[2] = newR2Z;

	this->m_c3[0] = newR3X;
	this->m_c3[1] = newR3Y;
	this->m_c3[2] = newR3Z;

	this->m_tr[0] = newTrX;
	this->m_tr[1] = newTrY;
	this->m_tr[2] = newTrZ;

	this->m_d[0] = newDX;
	this->m_d[1] = newDY;
	this->m_d[2] = newDZ;
	this->m_w = newDW;

}

void Trfm3D::add(const Trfm3D * T ) { add(*T); }

void Trfm3D::clone( const Trfm3D &T ) {
	m_c1 = T.m_c1;
	m_c2 = T.m_c2;
	m_c3 = T.m_c3;
	m_tr = T.m_tr;
	m_d  = T.m_d;
	m_scl = T.m_scl;
	m_w  = T.m_w;
}

void Trfm3D::clone( const Trfm3D *T ) {	clone(*T); }


////////////////////////////////////////////////////////////////////////////////////
// Transform points, vectors, planes ...

// @@ TODO. Transform a point
//
// IMPORTANT NOTE: suppose that m_w == 1




Vector3 Trfm3D::transformPoint(const Vector3 & P) const {
	Vector3 res;
	res[0] = m_c1[0]*P[0] + m_c1[1]*P[1] + m_c1[2]*P[2];
	res[1] = m_c2[0]*P[0] + m_c2[1]*P[1] + m_c2[2]*P[2];
	res[2] = m_c3[0]*P[0] + m_c3[1]*P[1] + m_c3[2]*P[2];
	return res;
}

// @@ TODO. Transform a vector.
//
// IMPORTANT NOTE: suppose that m_w == 1
//
//  also remember: vectors don't translate

Vector3 Trfm3D::transformVector(const Vector3 & V) const {
	Vector3 res;
	res[0] = m_c1[0]*V[0] + m_c1[1]*V[1] + m_c1[2]*V[2];
	res[1] = m_c2[0]*V[0] + m_c2[1]*V[1] + m_c2[2]*V[2];
	res[2] = m_c3[0]*V[0] + m_c3[1]*V[1] + m_c3[2]*V[2];
	return res;
}

Vector3 Trfm3D::transformNormal(const Vector3 & N) const {
	// this = M = S*R*T = S*R (no translations to normals)
	// auxTrfm = (M^{-1})^{T} == S^{-1}*R
	Trfm3D auxTrfm(*this);
	auxTrfm.m_scl = 1.0f / m_scl; // S^{-1}
	auxTrfm.m_tr = Vector3::ZERO;
	// X = aurTrfm*N = S^{-1}*R*N
	Vector3 X = auxTrfm.transformVector(N);
	if ( m_scl != 1.0) {
		X.normalize();
	}
	return X;
}

void Trfm3D::transformPlane(Plane * plane) const {
	Vector3 & pnormal = plane->m_n;
	float & pd = plane->m_d;
	// this  == M = S*R*T = S*R (no translations to normals)
	// auxTrfm = (M^{-1})^{T} == S^{-1}*R
	Trfm3D auxTrfm(*this);
	auxTrfm.m_scl = 1.0f / m_scl; // S^{-1}
	auxTrfm.m_tr = Vector3::ZERO;
	// X = aurTrfm = S^{-1}*R*N
	Vector3 X(pnormal);
	auxTrfm.transformVector(X);
	// d1 =  d + T*X
	float d1 = pd + m_tr.dot(X);
	if (m_scl != 1.0) {
		// normalize X
		float factor = X.normalize();
		d1 /= factor;
	}
	pnormal = X;
	pd = d1;
}

Vector3 Trfm3D::projectPoint(const Vector3 & P) const {

	Vector3 tmpV;
	float tmpW;
	float scale;

	scale = m_scl;

	//Px' = Px*m_c1[0] + Py*m_c2[0] + Pz*m_c3[0] + m_tr[0]
	tmpV[0] = P[0] * m_c1[0] * scale + P[1] * m_c2[0] * scale + P[2] * m_c3[0] * scale + m_tr[0];

	//Py' = Px*m_c1[1]*scale + Py*m_c2[1]*scale + Pz*m_c3[1]*scale + m_tr[1]
	tmpV[1] = P[0] * m_c1[1] * scale + P[1] * m_c2[1] * scale + P[2] * m_c3[1] * scale + m_tr[1];

	//Pz' = Px*m_c1[2]*scale + Py*m_c2[2]*scale + Pz*m_c3[2]*scale + m_tr[2]
	tmpV[2] = P[0] * m_c1[2] * scale + P[1] * m_c2[2] * scale + P[2] * m_c3[2] * scale + m_tr[2];

	//Pw' = Px * dx + Py * dy + Pz * dz + dw
	tmpW = P[0] * m_d[0] + P[1] * m_d[1] + P[2] * m_d[2] + m_w;

	if(distance_is_zero(tmpW)) {
		fprintf(stderr, "[W] Trfm3D::projectPoint: zero W\n");
	} else {
		tmpV /= tmpW;
	}
	return tmpV;
}

////////////////////////////////////////////////////////////////////////////////////
// Obtain OpenGL matrix (float[16]) which is equivalent to Trfm3D

void Trfm3D::getGLMatrix(float *glmatrix) const {
	glmatrix[0]  = m_c1[0] * m_scl;
	glmatrix[1]  = m_c1[1] * m_scl;
	glmatrix[2]  = m_c1[2] * m_scl;
	glmatrix[3]  = m_d[0];
	glmatrix[4]  = m_c2[0] * m_scl;
	glmatrix[5]  = m_c2[1] * m_scl;
	glmatrix[6]  = m_c2[2] * m_scl;
	glmatrix[7]  = m_d[1];
	glmatrix[8]  = m_c3[0] * m_scl;
	glmatrix[9]  = m_c3[1] * m_scl;
	glmatrix[10] = m_c3[2] * m_scl;
	glmatrix[11] = m_d[2];
	glmatrix[12] = m_tr[0];
	glmatrix[13] = m_tr[1];
	glmatrix[14] = m_tr[2];
	glmatrix[15] = m_w;
}


////////////////////////////////////////////////////////////////////////////////////
// Set functions

void Trfm3D::setUnit() {
	Trfm3D T;
	swap(T);
}

void Trfm3D::setRotX(float angle) {
	m_c1 = Vector3( 1.0f,  0.0f,         0.0f );
	m_c2 = Vector3( 0.0f,  cosf(angle),  sinf(angle) );
	m_c3 = Vector3( 0.0f, -sinf(angle ), cosf(angle) );
	m_scl = 1.0f;
	m_tr = Vector3::ZERO;
	m_d = Vector3::ZERO;
	m_w  = 1.0f;
}

void Trfm3D::setRotY(float angle ) {
	m_c1 = Vector3( cosf(angle),  0.0f, -sinf(angle) );
	m_c2 = Vector3( 0.0f,         1.0f,   0.0f );
	m_c3 = Vector3( sinf(angle),  0.0f,  cosf(angle) );
	m_scl = 1.0f;
	m_tr = Vector3::ZERO;
	m_d = Vector3::ZERO;
	m_w  = 1.0f;
}

void Trfm3D::setRotZ(float angle ) {
	m_c1 = Vector3(  cosf(angle), sinf(angle), 0.0f );
	m_c2 = Vector3( -sinf(angle), cos(angle),  0.0f );
	m_c3 = Vector3(  0.0f,         0.0f,       1.0f );
	m_scl = 1.0f;
	m_tr = Vector3::ZERO;
	m_d = Vector3::ZERO;
	m_w  = 1.0f;
}

/*
  Rotate by angle theta around an arbitrary axis r

  Positive angles are anticlockwise looking down the axis
  towards the origin.
  Assume right hand coordinate system.
*/

void Trfm3D::setRotVec(const Vector3 & VV, float theta ) {

	Vector3 V(VV);
	V.normalize();
	float costheta = cosf(theta);
	float sintheta = sinf(theta);

	m_c1[0] = (costheta + (1 - costheta) * V[0] * V[0]);
	m_c2[0] = ((1 - costheta) * V[0] * V[1] - V[2] * sintheta);
	m_c3[0] = ((1 - costheta) * V[0] * V[2] + V[1] * sintheta);

	m_c1[1] = ((1 - costheta) * V[0] * V[1] + V[2] * sintheta);
	m_c2[1] = (costheta + (1 - costheta) * V[1] * V[1]);
	m_c3[1] = ((1 - costheta) * V[1] * V[2] - V[0] * sintheta);

	m_c1[2] = ((1 - costheta) * V[0] * V[2] - V[1] * sintheta);
	m_c2[2] = ((1 - costheta) * V[1] * V[2] + V[0] * sintheta);
	m_c3[2] = (costheta + (1 - costheta) * V[2] * V[2]);

	m_tr = Vector3::ZERO;
	m_d = Vector3::ZERO;
	m_scl = 1.0f;
	m_w  = 1.0f;
}


void Trfm3D::setTrans(const Vector3 & tr) {
	m_c1 = Vector3::UNIT_X;
	m_c2 = Vector3::UNIT_Y;
	m_c3 = Vector3::UNIT_Z;
	m_tr = tr;
	m_d = Vector3::ZERO;
	m_scl = 1.0f;
	m_w  = 1.0f;
}

void Trfm3D::setScale(float scale ) {
	m_c1 = Vector3::UNIT_X;
	m_c2 = Vector3::UNIT_Y;
	m_c3 = Vector3::UNIT_Z;
	m_tr = Vector3::ZERO;
	m_d = Vector3::ZERO;
	m_scl = scale;
	m_w  = 1.0f;
}

// @@ TODO: Rotate angle radians about an axis defined by vector and located at point
//

void Trfm3D::setRotAxis(const Vector3 & V, const Vector3 & P, float angle ) {
	//Trfm3D M;
	this->setTrans(P);
	this->addRotVec(V, angle);
	this->addTrans(-1*P);
	//this->transformVector(V);

}


void Trfm3D::setOrtho(float left, float right,
					  float bottom, float top,
					  float near, float far) {
	m_scl = 1.0f;
	float a =  2.0f / (right - left) ;
	float b =  2.0f / (bottom - top) ;
	float c = -2.0f / (far - near) ;
	float tx = (right + left) / ( right - left) ;
	float ty = (bottom + top) / ( bottom - top) ;
	float tz = (far + near) / (far - near) ;
	m_c1[0] = a;    m_c2[0] = 0.0f;  m_c3[0] = 0.0f; m_tr[0] = -tx;
	m_c1[1] = 0.0f; m_c2[1] = b;     m_c3[1] = 0.0f; m_tr[1] = -ty;
	m_c1[2] = 0.0f; m_c2[2] = 0.0f;  m_c3[2] = c;    m_tr[2] = -tz;
	m_d[0]  = 0.0f; m_d[1]  = 0.0f;  m_d[2]  = 0.0f; m_w  = 1.0f;

}

void Trfm3D::setFrustum(float left, float right,
						float bottom, float top,
						float near, float far) {
	float x, y, a, b, c, d;
	x = (2.0f * near) / (right - left);
	y = (2.0f * near) / (top - bottom);
	a = (right + left) / (right - left);
	b = (top + bottom) / (top - bottom);
	c = -(far + near) / (far - near);
	d = -(2.0f * far * near) / (far - near);
	m_scl = 1.0f;
	m_c1[0] = x;    m_c2[0] = 0.0f;  m_c3[0] = a;     m_tr[0] = 0.0f;
	m_c1[1] = 0.0f; m_c2[1] = y;     m_c3[1] = b;     m_tr[1] = 0.0f;
	m_c1[2] = 0.0f; m_c2[2] = 0.0f;  m_c3[2] = c;     m_tr[2] = d;
	m_d[0]  = 0.0f; m_d[1]  = 0.0f;  m_d[2]  = -1.0f; m_w  = 0.0f;
}

void Trfm3D::setLocal2World(const Vector3 & P,
							const Vector3 & R,
							const Vector3 & U,
							const Vector3 & D) {
	m_c1 = R;
	m_c2 = U;
	m_c3 = D;
	m_tr = P;
	m_scl = 1.0;
	m_d = Vector3::ZERO;
	m_w  = 1.0f;
}

void Trfm3D::setWorld2Local(const Vector3 & P,
							const Vector3 & R,
							const Vector3 & U,
							const Vector3 & D) {
	m_c1[0] = R[0];
	m_c1[1] = U[0];
	m_c1[2] = D[0];

	m_c2[0] = R[1];
	m_c2[1] = U[1];
	m_c2[2] = D[1];

	m_c3[0] = R[2];
	m_c3[1] = U[2];
	m_c3[2] = D[2];

	m_tr[0] = -1.0f * R.dot(P);
	m_tr[1] = -1.0f * U.dot(P);
	m_tr[2] = -1.0f * D.dot(P);

	m_scl = 1.0f;
	m_d = Vector3::ZERO;
	m_w  = 1.0f;
}

void Trfm3D::setWorld2LocalFrame(const Trfm3D &frameTrfm) {

	m_c1[0] = frameTrfm.m_c1[0];
	m_c1[1] = frameTrfm.m_c2[0];
	m_c1[2] = frameTrfm.m_c3[0];

	m_c2[0] = frameTrfm.m_c1[1];
	m_c2[1] = frameTrfm.m_c2[1];
	m_c2[2] = frameTrfm.m_c3[1];

	m_c3[0] = frameTrfm.m_c1[2];
	m_c3[1] = frameTrfm.m_c2[2];
	m_c3[2] = frameTrfm.m_c3[2];

	m_tr[0] = -1.0 * m_c1.dot(frameTrfm.m_tr);
	m_tr[1] = -1.0 * m_c2.dot(frameTrfm.m_tr);
	m_tr[2] = -1.0 * m_c3.dot(frameTrfm.m_tr);

	m_d = Vector3::ZERO;
	m_scl = 1.0;
	m_w = 1.0f;

}

void Trfm3D::setWorld2LocalFrame(const Trfm3D *frameTrfm) {
	setWorld2LocalFrame(*frameTrfm);
}

void Trfm3D::normalize() {
	/*   Column1=Normalized(CrossProduct(Column2,Column3)); */
	/*   Column2=Normalized(CrossProduct(Column3,Column1)); */
	/*   Normalize(Column3); */
	m_c1 = crossVectors(m_c2, m_c3);
	m_c1.normalize();
	m_c2 = crossVectors(m_c3, m_c1);
	m_c2.normalize();
	m_c3.normalize();
}


////////////////////////////////////////////////////////////////////////////////////
// Add functions

void Trfm3D::addRotX(float angle ) {
	static Trfm3D localT;
	localT.setRotX(angle );
	add(localT);
}

void Trfm3D::addRotY(float angle ) {
	static Trfm3D localT;
	localT.setRotY(angle);
	add(localT);
}

void Trfm3D::addRotZ(float angle ) {
	static Trfm3D localT;
	localT.setRotZ(angle);
	add(localT);
}

void Trfm3D::addRotVec(const Vector3 & V,
					   float theta ) {
	static Trfm3D localT;
	localT.setRotVec(V, theta);
	add(localT);
}

void Trfm3D::addRotAxis(const Vector3 & V,
						const Vector3 & P,
						float angle) {
	static Trfm3D localT;
	localT.setRotAxis(V, P, angle);
	add(localT);
}

void Trfm3D::addTrans(const Vector3 & T) {

	m_tr[0] += T[0] * m_scl * m_c1[0];
	m_tr[0] += T[1] * m_scl * m_c2[0];
	m_tr[0] += T[2] * m_scl * m_c3[0];

	m_tr[1] += T[0] * m_scl * m_c1[1];
	m_tr[1] += T[1] * m_scl * m_c2[1];
	m_tr[1] += T[2] * m_scl * m_c3[1];

	m_tr[2] += T[0] * m_scl * m_c1[2];
	m_tr[2] += T[1] * m_scl * m_c2[2];
	m_tr[2] += T[2] * m_scl * m_c3[2];
}

void Trfm3D::addScale(float scale) {
	m_scl *= scale;
}

void Trfm3D::addFrustum( float left, float right,
						 float top, float bottom,
						 float near, float far) {
	static Trfm3D localT;
	localT.setFrustum(left, right, top, bottom, near, far);
	add(localT);
}


void Trfm3D::addWorld2Local(const Vector3 & P,
							const Vector3 & R,
							const Vector3 & U,
							const Vector3 & D) {

	static Trfm3D localT;
	localT.setWorld2Local(P, R, U, D);
	add(localT);
}

void Trfm3D::addLocal2World(const Vector3 & P,
							const Vector3 & R,
							const Vector3 & U,
							const Vector3 & D) {

	static Trfm3D localT;
	localT.setLocal2World(P, R, U, D);
	add(localT);
}


////////////////////////////////////////////////////////////////////////////////////
// Misc

int Trfm3D::cmp(const Trfm3D &rhs) const {

	if (!distance_is_zero(m_c1[0] - rhs.m_c1[0])) return -2;
	if (!distance_is_zero(m_c1[1] - rhs.m_c1[1])) return -2;
	if (!distance_is_zero(m_c1[2] - rhs.m_c1[2])) return -2;

	if (!distance_is_zero(m_c2[0] - rhs.m_c2[0])) return -2;
	if (!distance_is_zero(m_c2[1] - rhs.m_c2[1])) return -2;
	if (!distance_is_zero(m_c2[2] - rhs.m_c2[2])) return -2;

	if (!distance_is_zero(m_c3[0] - rhs.m_c3[0])) return -2;
	if (!distance_is_zero(m_c3[1] - rhs.m_c3[1])) return -2;
	if (!distance_is_zero(m_c3[2] - rhs.m_c3[2])) return -2;

	if (!distance_is_zero(m_tr[0] - rhs.m_tr[0])) return -3;
	if (!distance_is_zero(m_tr[1] - rhs.m_tr[1])) return -3;
	if (!distance_is_zero(m_tr[2] - rhs.m_tr[2])) return -3;

	if (!distance_is_zero(m_scl - rhs.m_scl)) return -4;

	return 0;
}

int Trfm3D::cmp(const Trfm3D *rhs) const {

	if (rhs == 0) return -1;
	return cmp(*rhs);
}

void Trfm3D::setInverse() {

	//            | sR   T |
	//      M   = |  0   1 |
	//
	//                 |(1/s)R^{T}  -(1/s)R^{T}*T |
	//      inv(M)   = |     0              1     |
	//

	// Compute inv(R) = R^{T}

	if (distance_is_zero(m_d[0]) || distance_is_zero(m_d[1]) ||
		distance_is_zero(m_d[2]) || distance_is_zero(1.0f - m_w)) {
		fprintf(stderr, "[E] Can't compute inverse on a projective Trfm3D!\n");
		exit(1);
	}

	if (distance_is_zero(m_scl)) {
		fprintf(stderr, "[E] Can't compute inverse: source Trfm3D has zero scale!\n");
		exit(1);
	}
	Trfm3D auxTrfm(*this); // copy of this
	// Compute inv(S) = 1/s
	auxTrfm.m_scl = 1.0f / m_scl;
	// Compute inv(T) = -(1/s)R^{T}*T
	auxTrfm.m_tr[0] = m_tr[0]*auxTrfm.m_c1[0] + m_tr[1]*auxTrfm.m_c2[0] + m_tr[2]*auxTrfm.m_c3[0];
	auxTrfm.m_tr[0]*= -auxTrfm.m_scl;
	auxTrfm.m_tr[1] = m_tr[0]*auxTrfm.m_c1[1] + m_tr[1]*auxTrfm.m_c2[1] + m_tr[2]*auxTrfm.m_c3[1];
	auxTrfm.m_tr[1]*= -auxTrfm.m_scl;
	auxTrfm.m_tr[2] = m_tr[0]*auxTrfm.m_c1[2] + m_tr[1]*auxTrfm.m_c2[2] + m_tr[2]*auxTrfm.m_c3[2];
	auxTrfm.m_tr[2]*= -auxTrfm.m_scl;
	// Store result
	swap(auxTrfm);
}

Vector3 Trfm3D::getTranslation() const {
	return m_tr;
};

void Trfm3D::abs() {

	m_c1[0] = fabs(m_c1[0]);
	m_c1[1] = fabs(m_c1[1]);
	m_c1[2] = fabs(m_c1[2]);
	m_c2[0] = fabs(m_c2[0]);
	m_c2[1] = fabs(m_c2[1]);
	m_c2[2] = fabs(m_c2[2]);
	m_c3[0] = fabs(m_c3[0]);
	m_c3[1] = fabs(m_c3[1]);
	m_c3[2] = fabs(m_c3[2]);
	m_scl = fabs(m_scl);
	m_tr[0] = fabs(m_tr[0]);
	m_tr[1] = fabs(m_tr[1]);
	m_tr[2] = fabs(m_tr[2]);
}

// Aux
void Trfm3D::print( ) const {
	printf( "% 0.4f scale\n", m_scl );
	printf( "% 0.4f %0.4f %0.4f  %0.4f\n",
			m_c1[0], m_c2[0], m_c3[0], m_tr[0] );
	printf( "%0.4f %0.4f %0.4f  %0.4f\n",
			m_c1[1], m_c2[1], m_c3[1], m_tr[1] );
	printf( "%0.4f %0.4f %0.4f  %0.4f\n",
			m_c1[2], m_c2[2], m_c3[2], m_tr[2] );
	printf( "% 0.4f % 0.4f % 0.4f  %0.4f\n",
			m_d[0], m_d[1], m_d[2], m_w );
	printf("-------- -------- --------  ---\n");
}
