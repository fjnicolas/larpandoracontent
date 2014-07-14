/**
 *  @file   LArContent/src/LArThreeDReco/LArShowerMatching/ClearShowersTool.cc
 * 
 *  @brief  Implementation of the clear showers tool class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "LArThreeDReco/LArShowerMatching/ClearShowersTool.h"

using namespace pandora;

namespace lar
{

bool ClearShowersTool::HasLargeDirectConnections(IteratorList::const_iterator iIter, const IteratorList &iteratorList)
{
    for (IteratorList::const_iterator iIter2 = iteratorList.begin(), iIter2End = iteratorList.end(); iIter2 != iIter2End; ++iIter2)
    {
        if (iIter == iIter2)
            continue;

        if (((*iIter)->GetClusterU() == (*iIter2)->GetClusterU()) || ((*iIter)->GetClusterV() == (*iIter2)->GetClusterV()) || ((*iIter)->GetClusterW() == (*iIter2)->GetClusterW()) )
            return true;
    }

    return false;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool ClearShowersTool::IsLargerThanDirectConnections(IteratorList::const_iterator iIter, const TensorType::ElementList &elementList,
    const unsigned int minMatchedSamplingPointRatio, const float minXOverlapSpanRatio, const pandora::ClusterList &usedClusters)
{
    const unsigned int nMatchedSamplingPoints((*iIter)->GetOverlapResult().GetNMatchedSamplingPoints());
    const unsigned int xOverlapSpan((*iIter)->GetOverlapResult().GetXOverlap().GetXOverlapSpan());

    for (TensorType::ElementList::const_iterator eIter = elementList.begin(); eIter != elementList.end(); ++eIter)
    {
        if ((*iIter) == eIter)
            continue;

        if (usedClusters.count(eIter->GetClusterU()) || usedClusters.count(eIter->GetClusterV()) || usedClusters.count(eIter->GetClusterW()))
            continue;

        if (((*iIter)->GetClusterU() != eIter->GetClusterU()) && ((*iIter)->GetClusterV() != eIter->GetClusterV()) && ((*iIter)->GetClusterW() != eIter->GetClusterW()))
            continue;

        if (nMatchedSamplingPoints < minMatchedSamplingPointRatio * eIter->GetOverlapResult().GetNMatchedSamplingPoints())
            return false;

        if (xOverlapSpan < minXOverlapSpanRatio * eIter->GetOverlapResult().GetXOverlap().GetXOverlapSpan())
            return false;
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool ClearShowersTool::Run(ThreeDShowersAlgorithm *pAlgorithm, TensorType &overlapTensor)
{
    if (PandoraSettings::ShouldDisplayAlgorithmInfo())
       std::cout << "----> Running Algorithm Tool: " << this << ", " << m_algorithmToolType << std::endl;

    ProtoParticleVector protoParticleVector;
    this->FindClearShowers(overlapTensor, protoParticleVector);

    const bool particlesMade(pAlgorithm->CreateThreeDParticles(protoParticleVector));
    return particlesMade;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ClearShowersTool::FindClearShowers(const TensorType &overlapTensor, ProtoParticleVector &protoParticleVector) const
{
    ClusterList usedClusters;

    for (TensorType::const_iterator iterU = overlapTensor.begin(), iterUEnd = overlapTensor.end(); iterU != iterUEnd; ++iterU)
    {
        if (!iterU->first->IsAvailable())
            continue;

        unsigned int nU(0), nV(0), nW(0);
        TensorType::ElementList elementList;
        overlapTensor.GetConnectedElements(iterU->first, true, elementList, nU, nV, nW);

        IteratorList iteratorList;
        this->SelectLargeShowerElements(elementList, usedClusters, iteratorList);
ClusterList allClusterListU, allClusterListV, allClusterListW;
        // Check that elements are not directly connected and are significantly longer than any other directly connected elements
        for (IteratorList::const_iterator iIter = iteratorList.begin(), iIterEnd = iteratorList.end(); iIter != iIterEnd; ++iIter)
        {
            if (ClearShowersTool::HasLargeDirectConnections(iIter, iteratorList))
                continue;

            if (!ClearShowersTool::IsLargerThanDirectConnections(iIter, elementList, m_minMatchedSamplingPointRatio, m_minXOverlapSpanRatio, usedClusters))
                continue;

allClusterListU.insert((*iIter)->GetClusterU());
allClusterListV.insert((*iIter)->GetClusterV());
allClusterListW.insert((*iIter)->GetClusterW());
std::cout << " Selected Element: MatchedFraction " << (*iIter)->GetOverlapResult().GetMatchedFraction()
<< ", MatchedSamplingPoints " << (*iIter)->GetOverlapResult().GetNMatchedSamplingPoints()
<< ", xSpanU " << (*iIter)->GetOverlapResult().GetXOverlap().GetXSpanU()
<< ", xSpanV " << (*iIter)->GetOverlapResult().GetXOverlap().GetXSpanV()
<< ", xSpanW " << (*iIter)->GetOverlapResult().GetXOverlap().GetXSpanW()
<< ", xOverlapSpan " << (*iIter)->GetOverlapResult().GetXOverlap().GetXOverlapSpan()
<< ", Availability (" << (*iIter)->GetClusterU()->IsAvailable() << (*iIter)->GetClusterV()->IsAvailable() << (*iIter)->GetClusterW()->IsAvailable() << ") "
<< std::endl;

            if ((*iIter)->GetClusterU()->IsAvailable() && (*iIter)->GetClusterV()->IsAvailable() &&(*iIter)->GetClusterW()->IsAvailable())
            {
std::cout << "Will create shower " << std::endl;
                ProtoParticle protoParticle;
                protoParticle.m_clusterListU.insert((*iIter)->GetClusterU());
                protoParticle.m_clusterListV.insert((*iIter)->GetClusterV());
                protoParticle.m_clusterListW.insert((*iIter)->GetClusterW());
                protoParticleVector.push_back(protoParticle);
            }

            if (((*iIter)->GetClusterU()->IsAvailable() != (*iIter)->GetClusterV()->IsAvailable()) || ((*iIter)->GetClusterU()->IsAvailable() != (*iIter)->GetClusterW()->IsAvailable()))
                std::cout << "ClearShowersTool: Cluster track/shower usage issues detected... " << std::endl;

            usedClusters.insert((*iIter)->GetClusterU());
            usedClusters.insert((*iIter)->GetClusterV());
            usedClusters.insert((*iIter)->GetClusterW());
        }
if (!allClusterListU.empty() || !allClusterListV.empty() || !allClusterListW.empty())
{
std::cout << " All Connected Clusters " << std::endl;
PANDORA_MONITORING_API(SetEveDisplayParameters(false, DETECTOR_VIEW_XZ));
PANDORA_MONITORING_API(VisualizeClusters(&allClusterListU, "AllUClusters", RED));
PANDORA_MONITORING_API(VisualizeClusters(&allClusterListV, "AllVClusters", GREEN));
PANDORA_MONITORING_API(VisualizeClusters(&allClusterListW, "AllWClusters", BLUE));
PANDORA_MONITORING_API(ViewEvent());
}
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ClearShowersTool::SelectLargeShowerElements(const TensorType::ElementList &elementList, const pandora::ClusterList &usedClusters,
    IteratorList &iteratorList) const
{
    for (TensorType::ElementList::const_iterator eIter = elementList.begin(); eIter != elementList.end(); ++eIter)
    {
        if (usedClusters.count(eIter->GetClusterU()) || usedClusters.count(eIter->GetClusterV()) || usedClusters.count(eIter->GetClusterW()))
            continue;

        if (eIter->GetOverlapResult().GetMatchedFraction() < m_minMatchedFraction)
            continue;

        if (eIter->GetOverlapResult().GetNMatchedSamplingPoints() < m_minMatchedSamplingPoints)
            continue;

        const XOverlap &xOverlap(eIter->GetOverlapResult().GetXOverlap());

        if ((xOverlap.GetXSpanU() > std::numeric_limits<float>::epsilon()) && (xOverlap.GetXOverlapSpan() / xOverlap.GetXSpanU() > m_minXOverlapFraction) &&
            (xOverlap.GetXSpanV() > std::numeric_limits<float>::epsilon()) && (xOverlap.GetXOverlapSpan() / xOverlap.GetXSpanV() > m_minXOverlapFraction) &&
            (xOverlap.GetXSpanW() > std::numeric_limits<float>::epsilon()) && (xOverlap.GetXOverlapSpan() / xOverlap.GetXSpanW() > m_minXOverlapFraction))
        {
            iteratorList.push_back(eIter);
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode ClearShowersTool::ReadSettings(const TiXmlHandle xmlHandle)
{
    m_minMatchedFraction = 0.2f;
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "MinMatchedFraction", m_minMatchedFraction));

    m_minMatchedSamplingPoints = 20;
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "MinMatchedSamplingPoints", m_minMatchedSamplingPoints));

    m_minXOverlapFraction = 0.5f;
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "MinXOverlapFraction", m_minXOverlapFraction));

    m_minMatchedSamplingPointRatio = 2;
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "MinMatchedSamplingPointRatio", m_minMatchedSamplingPointRatio));

    m_minXOverlapSpanRatio = 2.f;
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "MinXOverlapSpanRatio", m_minXOverlapSpanRatio));

    return STATUS_CODE_SUCCESS;
}

} // namespace lar
