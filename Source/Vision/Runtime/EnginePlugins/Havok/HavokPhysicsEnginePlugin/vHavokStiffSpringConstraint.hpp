/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#ifndef V_HAVOK_STIFF_SPRING_CONSTRAINT_HPP_INCLUDED
#define V_HAVOK_STIFF_SPRING_CONSTRAINT_HPP_INCLUDED

#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokConstraint.hpp>

/// \brief
///   Descriptor for vHavokPhysics Stiff-Spring constraints.
class vHavokStiffSpringConstraintDesc : public vHavokConstraintDesc
{
public:
  VHAVOK_IMPEXP vHavokStiffSpringConstraintDesc();
  
  VHAVOK_IMPEXP virtual void Reset() HKV_OVERRIDE;

  V_DECLARE_SERIAL_DLLEXP(vHavokStiffSpringConstraintDesc, VHAVOK_IMPEXP)
  VHAVOK_IMPEXP virtual void Serialize(VArchive &ar) HKV_OVERRIDE;

private:
  static const unsigned int s_iSerialVersion; ///< The current serialization version for objects of this class.
};

/// \brief
///   Implementation of the vHavokPhysics Stiff-Spring constraint.
class vHavokStiffSpringConstraint : public vHavokConstraint
{
public:
  // serialization and type management
  V_DECLARE_SERIAL_DLLEXP(vHavokStiffSpringConstraint, VHAVOK_IMPEXP)
  VHAVOK_IMPEXP virtual void Serialize(VArchive &ar) HKV_OVERRIDE;

protected:
  VHAVOK_IMPEXP virtual hkpConstraintData* CreateConstraintData() HKV_OVERRIDE;
  VHAVOK_IMPEXP virtual vHavokConstraintDesc *CreateConstraintDesc() HKV_OVERRIDE;
  VHAVOK_IMPEXP virtual void InitConstraintDataFromDesc(hkpConstraintData& data, 
    vHavokConstraintDesc const& desc) HKV_OVERRIDE;
  VHAVOK_IMPEXP virtual void SaveToDesc(vHavokConstraintDesc& desc) HKV_OVERRIDE;

private:
  static const unsigned int s_iSerialVersion; ///< The current serialization version for objects of this class.
};

#endif // V_HAVOK_STIFF_SPRING_CONSTRAINT_HPP_INCLUDED

/*
 * Havok SDK - Base file, BUILD(#20131019)
 * 
 * Confidential Information of Havok.  (C) Copyright 1999-2013
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
 * rights, and intellectual property rights in the Havok software remain in
 * Havok and/or its suppliers.
 * 
 * Use of this software for evaluation purposes is subject to and indicates
 * acceptance of the End User licence Agreement for this product. A copy of
 * the license is included with this software and is also available from salesteam@havok.com.
 * 
 */
