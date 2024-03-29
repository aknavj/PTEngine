#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

/*
 */
class CLine {

	public:
		CVector m_vOrigin;
		CVector m_vDirection;

	public:
		
		/*
		 */
		CLine() {
		
		}
		
		/*
		 */
		CLine(const CVector &p1, const CVector &p2) {
			m_vOrigin = p1;
			m_vDirection = p2 - p1;
		}
};

/*
 */
class CRay {

	public:
		CVector m_vOrigin;
		CVector m_vDirection;

	public:
		
		/*
		 */
		CRay() {
		
		}

		/*
		 */
		CRay(const CVector &p1, const CVector &p2) {
			m_vOrigin = p1;
			m_vDirection = p2 - p1;
		}
};

/*
 */
class CSegment {
	
	public:
		CVector m_vOrigin;
		CVector m_vDirection;

	public:

		/*
		 */
		CSegment() {
		
		}
		
		/*
		 */
		CSegment(const CVector &p1, const CVector &p2) {
			m_vOrigin = p1;
			m_vDirection = p2 - p1;
		}
};

/*
 */
class CPlane {

	public:
		CVector m_vNormal;
		float m_fConstant;

	public:
		
		/*
		 */
		CPlane() {
		
		}
		
		/*
		 */
		CPlane(const CVector &p1, const CVector &p2, const CVector &p3) {
			m_vNormal = NormalVector(p1, p2, p3);
			m_fConstant = -(p1 | m_vNormal);
		}

		/*
		 */
		CPlane(const CVector &vNormal, const CVector &p) {
			m_vNormal = vNormal;
			m_fConstant = -(p | m_vNormal);
		}
		
		/*
		 */
		CPlane(const CVector &vNormal, const float f) {
			m_vNormal = vNormal;
			m_fConstant = f;
		}

		/*
		 */
		void Init(CVector &p1, CVector &p2, CVector &p3) {
			CVector n = NormalVector(p1, p2, p3);
			Init(n, p1);
		}

		/*
		 */
		void Init(CVector &vNormal, CVector &p) {
			m_vNormal = vNormal;
			m_fConstant = -(p | m_vNormal);
		}

		/*
		 */
		float Distance(const CVector &p) const {
			return (m_vNormal | p) + m_fConstant;
		}

		/*
		 */
		bool Intersection(CVector &vPos, CVector &vDir) {
			float f = m_vNormal | vDir;
			if(MathLib::Abs(f) < DELTA) return false;
			vPos -= vDir * (Distance(vPos) / f);
			return true;
		}
};

/*
 */
class CFrustum {

	protected:
		CPlane m_plFrustum[6];

	public:

		/*
		 */
		CFrustum() {
	
		}
	
		/*
		 */
		void Init() {

			float   proj[16];
			float   modl[16];
			float   clip[16];
			float   t;

			glGetFloatv(GL_PROJECTION_MATRIX, proj);
			glGetFloatv(GL_MODELVIEW_MATRIX, modl);

			clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
			clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
			clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
			clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

			clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
			clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
			clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
			clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

			clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
			clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
			clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
			clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

			clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
			clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
			clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
			clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

			m_plFrustum[0].m_vNormal.x = clip[ 3] - clip[ 0];
			m_plFrustum[0].m_vNormal.y = clip[ 7] - clip[ 4];
			m_plFrustum[0].m_vNormal.z = clip[11] - clip[ 8];
			m_plFrustum[0].m_fConstant = clip[15] - clip[12];
			t = m_plFrustum[0].m_vNormal.Magnitude();
			m_plFrustum[0].m_vNormal /= t;
			m_plFrustum[0].m_fConstant /= t;

			m_plFrustum[1].m_vNormal.x = clip[ 3] + clip[ 0];
			m_plFrustum[1].m_vNormal.y = clip[ 7] + clip[ 4];
			m_plFrustum[1].m_vNormal.z = clip[11] + clip[ 8];
			m_plFrustum[1].m_fConstant = clip[15] + clip[12];
			t = m_plFrustum[1].m_vNormal.Magnitude();
			m_plFrustum[1].m_vNormal /= t;
			m_plFrustum[1].m_fConstant /= t;

			m_plFrustum[2].m_vNormal.x = clip[ 3] + clip[ 1];
			m_plFrustum[2].m_vNormal.y = clip[ 7] + clip[ 5];
			m_plFrustum[2].m_vNormal.z = clip[11] + clip[ 9];
			m_plFrustum[2].m_fConstant = clip[15] + clip[13];
			t = m_plFrustum[2].m_vNormal.Magnitude();
			m_plFrustum[2].m_vNormal /= t;
			m_plFrustum[2].m_fConstant /= t;

			m_plFrustum[3].m_vNormal.x = clip[ 3] - clip[ 1];
			m_plFrustum[3].m_vNormal.y = clip[ 7] - clip[ 5];
			m_plFrustum[3].m_vNormal.z = clip[11] - clip[ 9];
			m_plFrustum[3].m_fConstant = clip[15] - clip[13];
			t = m_plFrustum[3].m_vNormal.Magnitude();
			m_plFrustum[3].m_vNormal /= t;
			m_plFrustum[3].m_fConstant /= t;

			m_plFrustum[4].m_vNormal.x = clip[ 3] - clip[ 2];
			m_plFrustum[4].m_vNormal.y = clip[ 7] - clip[ 6];
			m_plFrustum[4].m_vNormal.z = clip[11] - clip[10];
			m_plFrustum[4].m_fConstant = clip[15] - clip[14];
			t = m_plFrustum[4].m_vNormal.Magnitude();
			m_plFrustum[4].m_vNormal /= t;
			m_plFrustum[4].m_fConstant /= t;

			m_plFrustum[5].m_vNormal.x = clip[ 3] + clip[ 2];
			m_plFrustum[5].m_vNormal.y = clip[ 7] + clip[ 6];
			m_plFrustum[5].m_vNormal.z = clip[11] + clip[10];
			m_plFrustum[5].m_fConstant = clip[15] + clip[14];
			t = m_plFrustum[5].m_vNormal.Magnitude();
			m_plFrustum[5].m_vNormal /= t;
			m_plFrustum[5].m_fConstant /= t;
		}

		/*
		 */
		bool IsInFrustum(const CVector &vPos, const float fRadius) const {
			for(int i=0; i<4; i++) {
				if(m_plFrustum[i].Distance(vPos) < -fRadius) return false;
			}

			return true;
		}

};

/*
 */
class CTriangle {

	public:
		/*
		 */
		CVector m_vPoint[3];
};

/*
 */
class CRectangle {

	public:
		CVector m_vCenter;
		float m_fExtents[2];
		CQuaternion m_qOrientation;
};

/*
 */
class CBox {

	public:
		CVector m_vCenter;
		float m_fExtents[3];
		CQuaternion m_qOrientation;
};

/*
 */
class CEllipsoid {

	public:
		CVector m_vCenter;
		float m_fExtents[3];
		CQuaternion m_qOrientation;
};

/*
 */
class CCylinder {

	public:
		CSegment m_seg;
		float m_fRadius;
};

/*
 */
class CCapsule {

	public:
		CSegment m_seg;
		float m_fRadius;
};

/*
 */
class CLozenge {

	public:
		CRectangle m_rect;
		float m_fRadius;
};

/*
 */
inline float GetDistance(const CPlane &plane, const CVector &p) {
	return (plane.m_vNormal | p) + plane.m_fConstant;
}

/*
 */
inline float GetDistanceSquared(const CPlane &plane, const CVector &p) {
	float f = GetDistance(plane, p);
	return f*f;
}

/*
 */
inline bool Intersects(const CPlane &plane, const CLine &line) {
	float f = plane.m_vNormal | line.m_vDirection;
	if(MathLib::Abs(f) < DELTA) return false;
	return true;
}

/*
 */
inline bool Intersects(const CPlane &plane, const CRay &ray) {
	float f = plane.m_vNormal | ray.m_vDirection;
	if(f < DELTA) return false;
	float t = -(GetDistance(plane, ray.m_vOrigin) / f);
	return t >= 0;
}

/*
 */
inline bool Intersects(const CPlane &plane, const CSegment &seg) {
	float f = plane.m_vNormal | seg.m_vDirection;
	if(f < DELTA) return false;
	float t = -(GetDistance(plane, seg.m_vOrigin) / f);
	return t >= 0 && t <= 1;
}

#endif // __GEOMETRY_H__
