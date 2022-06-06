/**
 *  @file   larpandoracontent/LArShowerRefinement/LArProtoShower.h
 *
 *  @brief  Header file for the ProtoShower class.
 *
 *  $Log: $
 */
#ifndef LAR_PROTO_SHOWER_H
#define LAR_PROTO_SHOWER_H 1

#include "Pandora/PandoraInternal.h"

#include "Objects/CartesianVector.h"

namespace lar_content
{

class ShowerCore
{
public:
    ShowerCore();

    ShowerCore(const pandora::CartesianVector &startPosition, const pandora::CartesianVector &startDirection, const pandora::CaloHitList &coreHitList);

    void SetStartPosition(const pandora::CartesianVector &startPosition);

    void SetStartDirection(const pandora::CartesianVector &startDirection);

    // when you're not feeling lazy, change this to private 
    pandora::CartesianVector m_startPosition;
    pandora::CartesianVector m_startDirection;
    pandora::CaloHitList m_coreHitList;
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline ShowerCore::ShowerCore(const pandora::CartesianVector &startPosition, const pandora::CartesianVector &startDirection, const pandora::CaloHitList &coreHitList) : 
    m_startPosition(startPosition), m_startDirection(startDirection), m_coreHitList(coreHitList)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline ShowerCore::ShowerCore() : m_startPosition(0.f, 0.f, 0.f), m_startDirection(0.f, 0.f, 0.f), m_coreHitList()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void ShowerCore::SetStartPosition(const pandora::CartesianVector &startPosition)
{
    m_startPosition = startPosition;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void ShowerCore::SetStartDirection(const pandora::CartesianVector &startDirection)
{
    m_startDirection = startDirection;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

class ConnectionPathway
{
public:
    ConnectionPathway();

    ConnectionPathway(const pandora::CartesianVector &startPosition, const pandora::CartesianVector &startDirection, const pandora::CaloHitList &pathwayHitList);

    // when you're not feeling lazy, change this to private
    pandora::CartesianVector m_startPosition;
    pandora::CartesianVector m_startDirection;
    pandora::CaloHitList m_pathwayHitList;
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline ConnectionPathway::ConnectionPathway(const pandora::CartesianVector &startPosition, const pandora::CartesianVector &startDirection, const pandora::CaloHitList &pathwayHitList) : 
    m_startPosition(startPosition), m_startDirection(startDirection), m_pathwayHitList(pathwayHitList)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline ConnectionPathway::ConnectionPathway() : m_startPosition(0.f, 0.f, 0.f), m_startDirection(0.f, 0.f, 0.f), m_pathwayHitList()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

class ProtoShower
{
public: 
    ProtoShower();

    ProtoShower(const ShowerCore &showerCore, const ConnectionPathway &connectionPathway, const pandora::CaloHitList &spineHitList, const bool isHelper,
        const pandora::CaloHitList &ambiguousHitList, const pandora::CartesianPointVector &ambiguousDirectionVector);

    // when you're not feeling lazy, change this to private  
    ShowerCore m_showerCore;
    ConnectionPathway m_connectionPathway;
    pandora::CaloHitList m_spineHitList;
    bool m_isHelper;
    pandora::CaloHitList m_ambiguousHitList;
    pandora::CartesianPointVector m_ambiguousDirectionVector;
};

typedef std::vector<ProtoShower> ProtoShowerVector;

//------------------------------------------------------------------------------------------------------------------------------------------

inline ProtoShower::ProtoShower(const ShowerCore &showerCore, const ConnectionPathway &connectionPathway, const pandora::CaloHitList &spineHitList, 
    const bool isHelper, const pandora::CaloHitList &ambiguousHitList, const pandora::CartesianPointVector &ambiguousDirectionVector) : 
        m_showerCore(showerCore), m_connectionPathway(connectionPathway), m_spineHitList(spineHitList), m_isHelper(isHelper), 
        m_ambiguousHitList(ambiguousHitList), m_ambiguousDirectionVector(ambiguousDirectionVector)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline ProtoShower::ProtoShower() : m_showerCore(), m_connectionPathway(), m_spineHitList(), m_isHelper(false), m_ambiguousHitList(), m_ambiguousDirectionVector()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

class ElectronProtoShower : public ProtoShower
{
public: 
    ElectronProtoShower(const ShowerCore &showerCore, const ConnectionPathway &connectionPathway, const pandora::CaloHitList &spineHitList, 
        const bool isHelper, const pandora::CaloHitList &ambiguousHitList, const pandora::CartesianPointVector &ambiguousDirectionVector, const pandora::CaloHitList &hitsToAdd);

    pandora::CaloHitList m_hitsToAdd;
};

typedef std::vector<ElectronProtoShower> ElectronProtoShowerVector;

//------------------------------------------------------------------------------------------------------------------------------------------

inline ElectronProtoShower::ElectronProtoShower(const ShowerCore &showerCore, const ConnectionPathway &connectionPathway, const pandora::CaloHitList &spineHitList, 
    const bool isHelper, const pandora::CaloHitList &ambiguousHitList, const pandora::CartesianPointVector &ambiguousDirectionVector, const pandora::CaloHitList &hitsToAdd) : 
        ProtoShower(showerCore, connectionPathway, spineHitList, isHelper, ambiguousHitList, ambiguousDirectionVector), m_hitsToAdd(hitsToAdd)
{
}

} // namespace lar_content

#endif // #ifndef LAR_PROTO_SHOWER_H
