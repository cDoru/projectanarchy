/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#ifndef HKAI_QUERY_MEDIATOR
#define HKAI_QUERY_MEDIATOR

#include <Common/Base/Object/hkReferencedObject.h>
#include <Ai/Pathfinding/hkaiBaseTypes.h>
#include <Ai/Pathfinding/NavMesh/hkaiNavMesh.h>
#include <Ai/Pathfinding/Collide/hkaiSpatialQueryHitFilter.h>

extern const class hkClass hkaiNavMeshQueryMediatorClass;

class hkaiNavMeshInstance;


	/// This is an interface for proximity and ray cast queries against nav mesh faces
class hkaiNavMeshQueryMediator : public hkReferencedObject
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AI);
	HK_DECLARE_REFLECTION();

	inline hkaiNavMeshQueryMediator();

	inline hkaiNavMeshQueryMediator( hkFinishLoadedObjectFlag& f );


		/// Base class for raycast, getClosestPoint, and AABB queries.
		/// Contains the basic filtering information as will as an optional pointer to the filter.
	struct QueryInputBase
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_AI, GetClosestPointInput);
			
			/// Default constructor initializes all fields to 0.
		QueryInputBase();

		inline void setInstanceAndTransform( const hkaiNavMeshInstance* instance );

			/// Filtering information that is passed to the hkaiSpatialQueryHitFilter.
			/// For queries generated by an hkaiCharacter, this will be set to hkaiCharacter::m_agentFilterInfo
		hkUint32 m_filterInfo;

			/// User defined data.
			/// For queries generated by an hkaiCharacter, this will be set to hkaiCharacter::m_userData
		hkUlong m_userData; 

			/// Optional filter pointer. If this is HK_NULL (the default), no filtering will be performed.
			/// Note that this pointer is not reference counted.
		const hkaiSpatialQueryHitFilter* m_hitFilter;

			/// Optional pointer to the hkaiNavMeshInstance* being queried (if any).
		const hkaiNavMeshInstance* m_instance;

			/// Optional local-to-world transform. 
			/// This will generally be set to an hkaiNavMeshInstance's transform internally.
		const hkTransform* m_localToWorldTransform;

	};

		/// Input information for getClosestPoint queries
	struct GetClosestPointInput : public QueryInputBase
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_AI, GetClosestPointInput);
			
			/// Default constructor - m_position is set to HK_REAL_MAX, and m_queryRadius is set to 5.0
		inline GetClosestPointInput();

			/// Sets m_position and m_queryRadius to the provided values
		inline GetClosestPointInput( hkVector4Parameter position, hkSimdRealParameter radius );

			/// Copies filter info, userdata, and hit filter pointer from the base input.
		inline GetClosestPointInput(const QueryInputBase& base);

			/// Target position of the closestPoint query.
		hkVector4 m_position;
			
			/// Maximum distance at which hits will be considered. For some mediator implementations, using a large value here will
			/// greatly increase the query time.
		hkSimdReal m_queryRadius;

			/// Optional nav mesh cutter. Used to determine the cut faces corresponding to an original face in the instance.
		const class hkaiNavMeshCutter* m_navMeshCutter;
	};

		/// Output the closest point to the given position.
		/// The closest face key is also returned. This will be HKAI_INVALID_PACKED_KEY if no face is found.
	virtual hkaiPackedKey getClosestPoint( const GetClosestPointInput& input, hkVector4& closestPointOut ) const = 0;

		/// Input information for getClosestBoundaryEdge queries
	struct GetClosestBoundaryEdgeInput : public GetClosestPointInput
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_AI, GetClosestBoundaryEdgeInput);

		inline GetClosestBoundaryEdgeInput();

		/// Sets m_position and m_queryRadius to the provided values
		inline GetClosestBoundaryEdgeInput( hkVector4Parameter position, hkSimdRealParameter radius );

		/// Copies filter info, userdata, and hit filter pointer from the base input.
		inline GetClosestBoundaryEdgeInput(const QueryInputBase& base);

			/// Direction to project the boundary and query point before computing distances.
		hkVector4 m_projectionDirection;
	};

		/// Output the closest point on a boundary edge to the given position. The distance squared is stored in the w component.
		/// The closest edge key is also returned. This will be HKAI_INVALID_PACKED_KEY if no edge is found.
	virtual hkaiPackedKey getClosestBoundaryEdge( const GetClosestBoundaryEdgeInput& input, hkVector4& closestPointOut ) const = 0;

		/// Input information for raycast queries
	struct RaycastInput : public QueryInputBase
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_AI, RaycastInput);
		
			///
		inline RaycastInput();
		
			/// Copies filter info, userdata, and hit filter pointer from the base input.
		inline RaycastInput(const QueryInputBase& base);

			/// Ray start
		hkVector4 m_from;
		
			/// Ray end
		hkVector4 m_to;
	};

		/// Input information for castBidirectionalRay queries
	struct BidirectionalRaycastInput : public QueryInputBase
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_AI, BidirectionalRaycastInput);

		///
		inline BidirectionalRaycastInput();

		/// Copies filter info, userdata, and hit filter pointer from the base input.
		inline BidirectionalRaycastInput(const QueryInputBase& base);

		/// The starting point for the rays in both directions
		hkVector4 m_center;

		/// Ray end in the forward direction; the other ray end is the same distance from 
		/// m_center in the other direction
		hkVector4 m_forwardTo;
	};

		/// Closest hit information returned by castRay.
	struct HitDetails 
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_AI, HitDetails);
		
			/// The fraction along the ray the closest hit occurs at. For bidirectional rays,
			/// a negative fraction indicates a closest hit along the backward ray.
		hkSimdReal m_hitFraction;

			/// The closest face along the ray.
		hkaiPackedKey m_hitFaceKey;
	};

		/// Returns the HitDetails for the closest face along a ray defined by from and to.
	virtual bool castRay( const RaycastInput& input, HitDetails& hitOut ) const = 0;

	/// Returns the HitDetails for the closest face along a bidirectional ray (a set of two rays in opposite
	/// directions from the same point). The default implementation does two raycasts from the center, and 
	/// returns the closest (or only) hit. m_hitFraction is between 0 and 1 for forward hits, and between 
	/// 0 and -1 for backward hits.
	virtual bool castBidirectionalRay( const BidirectionalRaycastInput& input, HitDetails& hitOut ) const;

		/// Input information for AABB queries
	struct AabbQueryInput : public QueryInputBase
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_AI, AabbQueryInput);
		
			///
		inline AabbQueryInput();

			/// AABB to search
		hkAabb m_aabb;
	};

		/// Returns the array of faces keys that are touched by the AABB.
	virtual void queryAabb( const AabbQueryInput& input, hkArray<hkaiPackedKey>::Temp& hits ) const = 0;

	//
	// Coherent queries
	//
	// These use hit results from previous frames to avoid full queries on the mediator
	// Note that these are non-virtual but will potentially call the virtual implementation if
	// the previous hit is too far away.
	//

		/// Input for coherent raycast and closest point queries.
	struct CoherentInput
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_AI, CoherentInput);
		inline CoherentInput();

			/// The previous closest point or ray hit point.
		hkVector4 m_previousPoint;

			/// The up vector. Used to determine which neighboring face to check.
		hkVector4 m_up;
		
			/// The face key of the previous hit
		hkaiPackedKey m_previousFaceKey;

			/// The tolerenace used to determine whether input is still on the previous face.
			/// This value is passed to hkaiNavMeshUtils::isPointOnFace().
		hkSimdReal m_isOnFaceTolerance; //+default(1e-3f)

			/// Defines the extent to which local closest points are considered global.
		hkSimdReal m_coherencyTolerance; //+default(0.1f)
	};

		/// Get the closest point, reusing previous closest point information.
		/// This may not be the globally closest point, but will be close to it and is much faster than a full query.
	hkaiPackedKey coherentGetClosestPoint( const class hkaiStreamingCollection* collection, const GetClosestPointInput& input, const CoherentInput& cinput,  hkVector4& closestPointOut ) const;

		/// Cast the ray, reusing previous raycast information if possible.
		/// A full (non-coherent) cast will be performed if the distance from the ray's m_from to the hit point
		/// is greater than cinput.m_coherencyTolerance.
	bool coherentCastRay( const class hkaiStreamingCollection* collection, const RaycastInput& input, const CoherentInput& cinput, HitDetails& hitOut ) const;

		/// Cast the bidirectional ray, reusing previous raycast information if possible.
		/// A full (non-coherent) cast will be performed if the distance from the ray's m_center to the hit point
		/// is greater than cinput.m_coherencyTolerance.
	bool coherentCastBidirectionalRay( const class hkaiStreamingCollection* collection, const BidirectionalRaycastInput& input, const CoherentInput& cinput, HitDetails& hitOut ) const;

	//
	// Old mediator interface - no filtering is performed
	//
#ifndef HKAI_DISABLE_OLD_MEDIATOR_INTERFACE
	hkaiPackedKey getClosestPoint( hkVector4Parameter p, hkReal radius ,hkVector4& closestPointOut  ) const
	{
		GetClosestPointInput input(p, hkSimdReal::fromFloat(radius) );
		return getClosestPoint(input, closestPointOut );
	}

	bool castRay( hkVector4Parameter from, hkVector4Parameter to, HitDetails& hitOut ) const
	{
		RaycastInput input;
		input.m_from = from;
		input.m_to = to;
		return castRay(input, hitOut );
	}

	void queryAabb( const hkAabb& aabb, hkArray<hkaiPackedKey>::Temp& hits ) const
	{
		AabbQueryInput input;
		input.m_aabb = aabb;
		queryAabb(input, hits );
	}
#endif //HKAI_ENABLE_OLD_MEDIATOR_INTERFACE


		/// Utility method to forward the QueryInputBase fields to its hit filter (if non-NULL)
	static inline hkBool32 HK_CALL isInstanceEnabled( const QueryInputBase& input, const hkaiNavMeshInstance* instance )
	{
		return !input.m_hitFilter || input.m_hitFilter->isInstanceEnabled( instance, input.m_filterInfo, input.m_userData) ;
	}

		/// Utility method to forward the QueryInputBase fields to its hit filter (if non-NULL)
	static inline hkBool32 HK_CALL isFaceEnabled( const QueryInputBase& input, const hkaiNavMeshInstance* instance, hkaiNavMesh::FaceIndex faceIndex )
	{
		return !input.m_hitFilter || input.m_hitFilter->isFaceEnabled( instance, faceIndex, input.m_filterInfo, input.m_userData) ;
	}

protected:
		
		/// Get the runtime index that's used to return the hit key.
		/// Instances which are unloaded (runtimeId() == -1) are treated as having id 0;
	static inline hkaiRuntimeIndex getRuntimeIndexForMeshInstance( const hkaiNavMeshInstance* mesh );
};

#include <Ai/Pathfinding/Collide/NavMesh/hkaiNavMeshQueryMediator.inl>

#endif // HKAI_QUERY_MEDIATOR

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
