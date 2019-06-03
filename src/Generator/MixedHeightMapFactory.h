#ifndef __MIXED_HEIGHT_MAP_FACTORY_H__
#define __MIXED_HEIGHT_MAP_FACTORY_H__

#include "PlanetaryObject.h"
#include "PlanetaryMapNode.h"

/*
 */
class CMixedHeightMapFactory : public CSimpleHeightMapFactory {

	protected:
		int m_nBufferSize;
		CPixelBuffer m_pb[6];

	public:
		typedef TReference<CMixedHeightMapFactory> Ref;

		CMixedHeightMapFactory(CObjectType *pType) : CSimpleHeightMapFactory(pType) { }

		virtual void Init(CPropertySet &prop);
		virtual void BuildNode(CPlanetaryMapNode *pNode);

		float GetHeight(const CPlanetaryMapCoord &coord);
};

DECLARE_GENERIC_TYPE_CLASS(CMixedHeightMapFactory, CSimpleHeightMapFactoryType);

#endif /* __MIXED_HEIGHT_MAP_FACTORY_H__ */

