#pragma once

#include "PhysicsEngine.h"
#include <iostream>
#include <iomanip>

namespace PhysicsEngine
{
	///Plane class
	class Plane : public StaticActor
	{
	public:
		//A plane with default paramters: XZ plane centred at (0,0,0)
		Plane(PxVec3 normal=PxVec3(0.f, 1.f, 0.f), PxReal distance=0.f) 
			: StaticActor(PxTransformFromPlaneEquation(PxPlane(normal, distance)))
		{
			CreateShape(PxPlaneGeometry());
		}
	};

	///Sphere class
	class Sphere : public DynamicActor
	{
	public:
		//a sphere with default parameters:
		// - pose in 0,0,0
		// - dimensions: 1m
		// - denisty: 1kg/m^3
		Sphere(const PxTransform& pose=PxTransform(PxIdentity), PxReal radius=1.f, PxReal density=1.f) 
			: DynamicActor(pose)
		{ 
			CreateShape(PxSphereGeometry(radius), density);
		}
	};

	///Box class
	class Box : public DynamicActor
	{
	public:
		//a Box with default parameters:
		// - pose in 0,0,0
		// - dimensions: 1m x 1m x 1m
		// - denisty: 1kg/m^3
		Box(const PxTransform& pose=PxTransform(PxIdentity), PxVec3 dimensions=PxVec3(.5f,.5f,.5f), PxReal density=1.f) 
			: DynamicActor(pose)
		{ 
			CreateShape(PxBoxGeometry(dimensions), density);
		}
	};

	///Static Box Class
	class StBox : public StaticActor
	{
	public:
		StBox(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(.5f, .5f, .5f))
			: StaticActor(pose)
		{
			CreateShape(PxBoxGeometry(dimensions));
		}
	};

	class Capsule : public DynamicActor
	{
	public:
		Capsule(const PxTransform& pose=PxTransform(PxIdentity), PxVec2 dimensions=PxVec2(1.f,1.f), PxReal density=1.f) 
			: DynamicActor(pose)
		{
			CreateShape(PxCapsuleGeometry(dimensions.x, dimensions.y), density);
		}
	};

	///The ConvexMesh class
	class ConvexMesh : public DynamicActor
	{
	public:
		//constructor
		ConvexMesh(const std::vector<PxVec3>& verts, const PxTransform& pose=PxTransform(PxIdentity), PxReal density=1.f)
			: DynamicActor(pose)
		{
			PxConvexMeshDesc mesh_desc;
			mesh_desc.points.count = (PxU32)verts.size();
			mesh_desc.points.stride = sizeof(PxVec3);
			mesh_desc.points.data = &verts.front();
			mesh_desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
			mesh_desc.vertexLimit = 256;

			CreateShape(PxConvexMeshGeometry(CookMesh(mesh_desc)), density);
		}

		//mesh cooking (preparation)
		PxConvexMesh* CookMesh(const PxConvexMeshDesc& mesh_desc)
		{
			PxDefaultMemoryOutputStream stream;

			if(!GetCooking()->cookConvexMesh(mesh_desc, stream))
				throw new Exception("ConvexMesh::CookMesh, cooking failed.");

			PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

			return GetPhysics()->createConvexMesh(input);
		}
	};

	///The TriangleMesh class
	class TriangleMesh : public StaticActor
	{
	public:
		//constructor
		TriangleMesh(const std::vector<PxVec3>& verts, const std::vector<PxU32>& trigs, const PxTransform& pose=PxTransform(PxIdentity))
			: StaticActor(pose)
		{
			PxTriangleMeshDesc mesh_desc;
			mesh_desc.points.count = (PxU32)verts.size();
			mesh_desc.points.stride = sizeof(PxVec3);
			mesh_desc.points.data = &verts.front();
			mesh_desc.triangles.count = (PxU32)trigs.size();
			mesh_desc.triangles.stride = 3*sizeof(PxU32);
			mesh_desc.triangles.data = &trigs.front();

			CreateShape(PxTriangleMeshGeometry(CookMesh(mesh_desc)));
		}

		//mesh cooking (preparation)
		PxTriangleMesh* CookMesh(const PxTriangleMeshDesc& mesh_desc)
		{
			PxDefaultMemoryOutputStream stream;

			if(!GetCooking()->cookTriangleMesh(mesh_desc, stream))
				throw new Exception("TriangleMesh::CookMesh, cooking failed.");

			PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

			return GetPhysics()->createTriangleMesh(input);
		}
	};

	//Distance joint with the springs switched on
	class DistanceJoint : public Joint
	{
	public:
		DistanceJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
		{
			PxRigidActor* px_actor0 = 0;
			if (actor0)
				px_actor0 = (PxRigidActor*)actor0->Get();

			joint = (PxJoint*)PxDistanceJointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
			joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
			((PxDistanceJoint*)joint)->setDistanceJointFlag(PxDistanceJointFlag::eSPRING_ENABLED, true);
			Damping(1.f);
			Stiffness(1.f);
		}

		void Stiffness(PxReal value)
		{
			((PxDistanceJoint*)joint)->setStiffness(value);
		}

		PxReal Stiffness()
		{
			return ((PxDistanceJoint*)joint)->getStiffness();		
		}

		void Damping(PxReal value)
		{
			((PxDistanceJoint*)joint)->setDamping(value);
		}

		PxReal Damping()
		{
			return ((PxDistanceJoint*)joint)->getDamping();
		}
	};

	///Revolute Joint
	class RevoluteJoint : public Joint
	{
	public:
		RevoluteJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
		{
			PxRigidActor* px_actor0 = 0;
			if (actor0)
				px_actor0 = (PxRigidActor*)actor0->Get();

			joint = PxRevoluteJointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
			joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION,true);
			((PxRevoluteJoint*)joint)->setLimit(PxJointAngularLimitPair(-(170 * PxPi) / 180, -(30 * PxPi) / 180, 0.5f));
			((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
		}

		void DriveVelocity(PxReal value)
		{
			//wake up the attached actors
			PxRigidDynamic *actor_0, *actor_1;
			((PxRevoluteJoint*)joint)->getActors((PxRigidActor*&)actor_0, (PxRigidActor*&)actor_1);
			if (actor_0)
			{
				if (actor_0->isSleeping())
					actor_0->wakeUp();
			}
			if (actor_1)
			{
				if (actor_1->isSleeping())
					actor_1->wakeUp();
			}
			((PxRevoluteJoint*)joint)->setDriveVelocity(value);
			((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
		}

		PxReal DriveVelocity()
		{
			return ((PxRevoluteJoint*)joint)->getDriveVelocity();
		}

		void SetLimits(PxReal lower, PxReal upper)
		{
			((PxRevoluteJoint*)joint)->setLimit(PxJointAngularLimitPair(lower, upper));
			((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
		}
	};
	///WORLD SHAPES
	class Pitch : public StaticActor
	{
	public:
		Pitch(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 lengthdimensions = PxVec3(.25f, .05f, 72.f), PxVec3 widthdimensions = PxVec3(35.0f, .015f, .25f))
			: StaticActor(pose)
		{
			CreateShape(PxBoxGeometry(lengthdimensions));
			CreateShape(PxBoxGeometry(lengthdimensions));
			CreateShape(PxBoxGeometry(widthdimensions));
			CreateShape(PxBoxGeometry(widthdimensions));
			GetShape(0)->setLocalPose(PxTransform(PxVec3(-35.f, 0.f, 0.f)));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(35.f, 0.f, 0.f)));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(0.f, 0.f, 72.f)));
			GetShape(3)->setLocalPose(PxTransform(PxVec3(0.f, 0.f, -72.f)));
			for (int i = 0; i < 4; i++){ GetShape(i)->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false); }
		}
	};

	class GoalLines : public StaticActor
	{
	public:
		GoalLines(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 widthdimensions = PxVec3(35.0f, .05f, .25f))
			: StaticActor(pose)
		{
			CreateShape(PxBoxGeometry(widthdimensions));
			CreateShape(PxBoxGeometry(widthdimensions));
			GetShape(0)->setLocalPose(PxTransform(PxVec3(0.f, 0.f, 50.f)));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(0.f, 0.f, -50.f)));
			for (int i = 0; i < 2; i++) { GetShape(i)->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false); }
		}
	};

	class FieldLine : public StaticActor
	{
	public:
		FieldLine(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 widthdimensions = PxVec3(35.0f, .05f, .25f))
			: StaticActor(pose)
		{
			CreateShape(PxBoxGeometry(widthdimensions));
			GetShape(0)->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		}
	};

	class DashFieldLine : public StaticActor
	{
	public:
		DashFieldLine(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 widthdimensions = PxVec3(2.5f, .05f, .25f))
			: StaticActor(pose)
		{
			CreateShape(PxBoxGeometry(widthdimensions));
			CreateShape(PxBoxGeometry(widthdimensions));
			CreateShape(PxBoxGeometry(widthdimensions));
			CreateShape(PxBoxGeometry(widthdimensions));
			CreateShape(PxBoxGeometry(widthdimensions));
			CreateShape(PxBoxGeometry(widthdimensions));
			CreateShape(PxBoxGeometry(widthdimensions));
			GetShape(0)->setLocalPose(PxTransform(PxVec3(-30.f, 0.f, 0.f)));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(-20.f, 0.f, 0.f)));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(-10.f, 0.f, 0.f)));
			GetShape(4)->setLocalPose(PxTransform(PxVec3(10.f, 0.f, 0.f)));
			GetShape(5)->setLocalPose(PxTransform(PxVec3(20.f, 0.f, 0.f)));
			GetShape(6)->setLocalPose(PxTransform(PxVec3(30.f, 0.f, 0.f)));
			for (int i = 0; i < 7; i++) { GetShape(i)->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false); }
		}
	};

	class GoalPost : public StaticActor
	{
	public:
		GoalPost(const PxTransform& pose = PxTransform(PxIdentity), PxVec2 postdimensions = PxVec2(0.15f, 6.f), PxVec2 gpostdimensions = PxVec2(0.15f, 2.8f), PxVec2 basedimensions = PxVec2(0.3f, 1.25f))
			: StaticActor(pose)
		{
			CreateShape(PxCapsuleGeometry(postdimensions.x, postdimensions.y));
			CreateShape(PxCapsuleGeometry(postdimensions.x, postdimensions.y));
			CreateShape(PxCapsuleGeometry(basedimensions.x, basedimensions.y));
			CreateShape(PxCapsuleGeometry(basedimensions.x, basedimensions.y));
			CreateShape(PxCapsuleGeometry(gpostdimensions.x, gpostdimensions.y));
			GetShape(0)->setLocalPose(PxTransform(PxVec3(-2.8f, 6.f, -50.f), (PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f)))));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(2.8f, 6.f, -50.f), (PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f)))));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(-2.8f, 1.25f, -50.f), (PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f)))));
			GetShape(3)->setLocalPose(PxTransform(PxVec3(2.8f, 1.25f, -50.f), (PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f)))));
			GetShape(4)->setLocalPose(PxTransform(PxVec3(0.f, 3.f, -50.f)));
		}
	};

	class CastleArena : public StaticActor
	{
	public:
		CastleArena(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 lengthdimensions = PxVec3(1.f, 5.f, 85.f), PxVec3 widthdimensions = PxVec3(48.0f, 5.f, 1.f), PxVec2 turretdimensions = PxVec2(4.f, 8.f))
			: StaticActor(pose)
		{
			CreateShape(PxBoxGeometry(lengthdimensions));
			CreateShape(PxBoxGeometry(lengthdimensions));
			CreateShape(PxBoxGeometry(widthdimensions));
			CreateShape(PxBoxGeometry(widthdimensions));
			CreateShape(PxCapsuleGeometry(turretdimensions.x, turretdimensions.y));
			CreateShape(PxCapsuleGeometry(turretdimensions.x, turretdimensions.y));
			CreateShape(PxCapsuleGeometry(turretdimensions.x, turretdimensions.y));
			CreateShape(PxCapsuleGeometry(turretdimensions.x, turretdimensions.y));
			GetShape(0)->setLocalPose(PxTransform(PxVec3(-48.f, 5.f, 0.f)));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(48.f, 5.f, 0.f)));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(0.f, 5.f, -85.f)));
			GetShape(3)->setLocalPose(PxTransform(PxVec3(0.f, 5.f, 85.f)));
			GetShape(4)->setLocalPose(PxTransform(PxVec3(-48.f, 8.f, -85.f), (PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f)))));
			GetShape(5)->setLocalPose(PxTransform(PxVec3(-48.f, 8.f, 85.f), (PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f)))));
			GetShape(6)->setLocalPose(PxTransform(PxVec3(48.f, 8.f, -85.f), (PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f)))));
			GetShape(7)->setLocalPose(PxTransform(PxVec3(48.f, 8.f, 85.f), (PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f)))));
		}
	};
	///CUSTOM DYNAMIC ACTORS
	class CataThrow : public DynamicActor
	{
	public:
		CataThrow(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 bardimensions = PxVec3(.05f, .05f, 2.5f), PxVec2 swingdimensions = PxVec2(.25f, 2.5f), PxVec3 boxdimensions = PxVec3(.5f, .1f, .5f))
			: DynamicActor(pose)
		{
			CreateShape(PxBoxGeometry(bardimensions), 1.f);
			CreateShape(PxBoxGeometry(bardimensions), 1.f);
			CreateShape(PxCapsuleGeometry(swingdimensions.x, swingdimensions.y), 1.f);
			CreateShape(PxBoxGeometry(boxdimensions), 1.f);
			CreateShape(PxBoxGeometry(boxdimensions), 1.f);
			CreateShape(PxBoxGeometry(boxdimensions), 1.f);
			CreateShape(PxBoxGeometry(boxdimensions), 1.f);
			GetShape(0)->setLocalPose(PxTransform(PxVec3(-.5f, 2.f, 1.5f), (PxQuat(PxHalfPi, PxVec3(-.5f, 0.f, 0.f)))));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(.5f, 2.f, 1.5f), (PxQuat(PxHalfPi, PxVec3(-.5f, 0.f, 0.f)))));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(0.f, 0.f, 0.f), (PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 0.f)))));
			GetShape(3)->setLocalPose(PxTransform(PxVec3(0.f, 4.f, 3.2f), (PxQuat(PxHalfPi, PxVec3(-.5f, 0.f, 0.f)))));
			GetShape(4)->setLocalPose(PxTransform(PxVec3(-.5f, 4.f, 3.f), (PxQuat(PxHalfPi, PxVec3(.0f, .25f, 1.f)))));
			GetShape(5)->setLocalPose(PxTransform(PxVec3(.5f, 4.f, 3.f), (PxQuat(PxHalfPi, PxVec3(.0f, -.25f, 1.f)))));
			GetShape(6)->setLocalPose(PxTransform(PxVec3(0.f, 4.25f, 3.25f), (PxQuat(PxHalfPi, PxVec3(0.3f, 0.f, 0.f)))));
		}
	};
	class CataFound : public DynamicActor
	{
	public:
		CataFound(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 bardimensions = PxVec3(.25f, .25f, 3.f))
			: DynamicActor(pose)
		{
			CreateShape(PxBoxGeometry(bardimensions), 1.f);
			CreateShape(PxBoxGeometry(bardimensions), 1.f);
			CreateShape(PxBoxGeometry(bardimensions), 1.f);
			CreateShape(PxBoxGeometry(bardimensions), 1.f);
			CreateShape(PxBoxGeometry(bardimensions), 1.f);
			CreateShape(PxBoxGeometry(bardimensions), 1.f);
			CreateShape(PxBoxGeometry(bardimensions), 1.f);
			CreateShape(PxBoxGeometry(bardimensions), 1.f);
			CreateShape(PxBoxGeometry(bardimensions), 1.f);
			CreateShape(PxBoxGeometry(bardimensions), 1.f);
			GetShape(0)->setLocalPose(PxTransform(PxVec3(-2.5f, 3.5f, -2.5f), (PxQuat(PxHalfPi, PxVec3(1.f, 0.f, 0.f)))));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(-2.5f, 3.f, -.75f), (PxQuat(PxHalfPi, PxVec3(.5f, 0.f, 0.f)))));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(-2.5f, 0.5f, 0.f), (PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 0.f)))));

			GetShape(3)->setLocalPose(PxTransform(PxVec3(2.5f, 3.5f, -2.5f), (PxQuat(PxHalfPi, PxVec3(1.f, 0.f, 0.f)))));
			GetShape(4)->setLocalPose(PxTransform(PxVec3(2.5f, 3.f, -.75f), (PxQuat(PxHalfPi, PxVec3(.5f, 0.f, 0.f)))));
			GetShape(5)->setLocalPose(PxTransform(PxVec3(2.5f, 0.5f, 0.f), (PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 0.f)))));

			GetShape(6)->setLocalPose(PxTransform(PxVec3(0.f, 0.5f, -2.5f), (PxQuat(PxHalfPi, PxVec3(0.f, 1.f, 0.f)))));
			GetShape(7)->setLocalPose(PxTransform(PxVec3(0.f, 0.5f, 2.5f), (PxQuat(PxHalfPi, PxVec3(0.f, 1.f, 0.f)))));

			GetShape(8)->setLocalPose(PxTransform(PxVec3(0.f, 0.5f, 2.5f), (PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f)))));
			GetShape(9)->setLocalPose(PxTransform(PxVec3(0.f, 2.5f, -2.5f), (PxQuat(PxHalfPi, PxVec3(0.f, 1.f, 0.f)))));
		}
	};

	class SpikeFlail : public DynamicActor
	{
	public:
		SpikeFlail(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 bardimensions = PxVec3(6.f, .05f, 2.5f), PxVec3 spikedimensions = PxVec3(0.5f, 0.5f, 0.5f))
			: DynamicActor(pose)
		{
			CreateShape(PxBoxGeometry(spikedimensions), 1.f);
			CreateShape(PxBoxGeometry(spikedimensions), 1.f);
			CreateShape(PxBoxGeometry(spikedimensions), 1.f);
			CreateShape(PxBoxGeometry(spikedimensions), 1.f);
			CreateShape(PxBoxGeometry(spikedimensions), 1.f);
			CreateShape(PxBoxGeometry(spikedimensions), 1.f);
			CreateShape(PxBoxGeometry(spikedimensions), 1.f);
			CreateShape(PxBoxGeometry(spikedimensions), 1.f);
			GetShape(0)->setLocalPose(PxTransform(PxVec3(0.f, 0.f, 0.f), (PxQuat(PxHalfPi, PxVec3(0.f, .5f, .5f)))));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(0.f, 0.f, 0.f), (PxQuat(PxHalfPi, PxVec3(0.f, .5f, .5f)))));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(1.f, 0.f, 0.f), (PxQuat(PxHalfPi, PxVec3(0.f, .5f, .5f)))));
			GetShape(3)->setLocalPose(PxTransform(PxVec3(2.f, 0.f, 0.f), (PxQuat(PxHalfPi, PxVec3(0.f, .5f, .5f)))));
			GetShape(4)->setLocalPose(PxTransform(PxVec3(3.f, 0.f, 0.f), (PxQuat(PxHalfPi, PxVec3(0.f, .5f, .5f)))));
			GetShape(5)->setLocalPose(PxTransform(PxVec3(4.f, 0.f, 0.f), (PxQuat(PxHalfPi, PxVec3(0.f, .5f, .5f)))));
			GetShape(6)->setLocalPose(PxTransform(PxVec3(0.f, 0.f, 0.f), (PxQuat(PxHalfPi, PxVec3(0.f, .5f, .5f)))));
			GetShape(7)->setLocalPose(PxTransform(PxVec3(0.f, 0.f, 0.f), (PxQuat(PxHalfPi, PxVec3(0.f, .5f, .5f)))));
		}
	};

}