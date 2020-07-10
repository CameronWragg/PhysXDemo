#pragma once

#include "BasicActors.h"
#include <iostream>
#include <iomanip>
//#include "Stats.h"


namespace PhysicsEngine
{
	using namespace std;
	//Color Palette:
	static const PxVec3 color_palette[] = {PxVec3(96.f / 255.f, 128.f / 255.f, 56.f / 255.f), PxVec3(234.f / 255.f, 192.f / 255.f, 134.f / 255.f), 
		PxVec3(62.f / 255.f, 47.f / 255.f, 30.f / 255.f), PxVec3(255.f/255.f,140.f/255.f,54.f/255.f),PxVec3(4.f/255.f,117.f/255.f,111.f/255.f)};
	//0: Grass Green
	//1: Off White/Peach
	//2: Leather Brown
	//3: Orange/Yellow

	//OAK (HARDWOOD) DENSITY - 600-900 (kg/m3)
	//RUGBY UNION BALL DENSITY -  6.13 (kg/m3)

	static PxVec3 turret_verts[] = {PxVec3(0,8,0), PxVec3(8,0,0), PxVec3(-8,0,0), PxVec3(0,0,8), PxVec3(0,0,-8)};

	static PxU32 turret_trigs[] = {1, 4, 0, 3, 1, 0, 2, 3, 0, 4, 2, 0, 3, 2, 1, 2, 4, 1};

	static PxVec3 catabase_verts[] = { PxVec3(0,0,0), PxVec3(0,6,0), PxVec3(0,0,5), PxVec3(1,0,0), PxVec3(1,6,0), PxVec3(1,0,5), PxVec3(1,6,1), PxVec3(0,6,1)};
	
	//RUGBY UNION BALL DIMENSIONS: 30CM X 62CM
	static PxVec3 rugbyball_verts[] = { PxVec3(.0f, .15f, .0f), PxVec3(.0f, -.15f, .0f), PxVec3(.31f, .0f, .0f), PxVec3(-.31f, .0f, .0f), PxVec3(.0f, .0f, .15f), PxVec3(0,0,-.15f) };

	class Turret : public TriangleMesh
	{
	public:
		Turret(PxTransform pose=PxTransform(PxIdentity)) :
			TriangleMesh(vector<PxVec3>(begin(turret_verts),end(turret_verts)), vector<PxU32>(begin(turret_trigs),end(turret_trigs)), pose)
		{
		}
	};

	/*class CataBase : public ConvexMesh
	{
	public:
		CataBase(PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.f) :
			ConvexMesh(vector<PxVec3>(begin(catabase_verts), end(catabase_verts)), pose, density)
		{
		}
	};*/

	class RugbyBall : public ConvexMesh
	{
	public:
		RugbyBall(PxTransform pose = PxTransform(PxIdentity), PxReal density = 6.13f) :
			ConvexMesh(vector<PxVec3>(begin(rugbyball_verts), end(rugbyball_verts)), pose, density)
		{
		}
	};

	struct FilterGroup
	{
		enum Enum
		{
			ACTOR0		= (1 << 0),
			ACTOR1		= (1 << 1),
			ACTOR2		= (1 << 2)
			//add more if you need
		};
	};

	///An example class showing the use of springs (distance joints).
	//class CatabaseFull
	//{
	//	vector<DistanceJoint*> springs;
	//	CataBase* left, * right;
	//	StBox* front, * back;

	//public:
	//	CatabaseFull(const PxTransform& pose = PxTransform(PxIdentity), PxReal stiffness=0.f, PxReal damping=0.f)
	//	{
	//		right = new CataBase(PxTransform(PxVec3(2.f, 0.f, 0.f)));
	//		left = new CataBase(PxTransform(PxVec3(-3.f, 0.f, 0.f)));
	//		front = new StBox(PxTransform(PxVec3(0.f, 0.25f, 0.25f)), PxVec3(3.f, 0.25f, 0.25f));
	//		back = new StBox(PxTransform(PxVec3(0.f, 0.25f, 4.25f)), PxVec3(3.f, 0.25f, 0.25f));
	//		left->Color(color_palette[1]);
	//		right->Color(color_palette[1]);
	//		front->Color(color_palette[1]);
	//		back->Color(color_palette[1]);
	//		springs.resize(8);
	//		springs[0] = new DistanceJoint(left, PxTransform(PxVec3(1, 5, 3)), right, PxTransform(PxVec3(-1, 5, 3)));
	//		springs[1] = new DistanceJoint(left, PxTransform(PxVec3(1, 5, 0)), right, PxTransform(PxVec3(-1, 5, 0)));
	//		springs[2] = new DistanceJoint(left, PxTransform(PxVec3(1, 1, 3)), right, PxTransform(PxVec3(-1, 1, 3)));
	//		springs[3] = new DistanceJoint(left, PxTransform(PxVec3(1, 1, 0)), right, PxTransform(PxVec3(-1, 1, 0)));
	//		//Front and Back
	//		springs[4] = new DistanceJoint(left, PxTransform(PxVec3(0, 0, 0)), front, PxTransform(PxVec3(0, 0, 0)));
	//		springs[5] = new DistanceJoint(front, PxTransform(PxVec3(0, 0, 0)), right, PxTransform(PxVec3(0, 0, 0)));
	//		springs[6] = new DistanceJoint(left, PxTransform(PxVec3(1, 1, 0)), back, PxTransform(PxVec3(-1, 1, 0)));
	//		springs[7] = new DistanceJoint(back, PxTransform(PxVec3(1, 1, 0)), right, PxTransform(PxVec3(-1, 1, 0)));

	//		for (unsigned int i = 0; i < springs.size(); i++)
	//		{
	//			  springs[i]->Stiffness(stiffness);
	//			  springs[i]->Damping(damping);
	//		}
	//	}

	//	void AddToScene(Scene* scene)
	//	{
	//		scene->Add(right);
	//		scene->Add(left);
	//		scene->Add(front);
	//		scene->Add(back);
	//	}

	//	~CatabaseFull()
	//	{
	//		for (unsigned int i = 0; i < springs.size(); i++)
	//			delete springs[i];
	//	}
	//};

	///A customised collision class, implemneting various callbacks
	class GoalPostTrigger : public PxSimulationEventCallback
	{
	public:
		//an example variable that will be checked in the main simulation loop
		bool trigger;
		int trigscore = 0;
		GoalPostTrigger() : trigger(false) {}

		///Method called when the contact with the trigger object is detected.
		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) 
		{
			//you can read the trigger information here
			for (PxU32 i = 0; i < count; i++)
			{
				//filter out contact with the planes
				if (pairs[i].otherShape->getGeometryType() != PxGeometryType::ePLANE)
				{
					//check if eNOTIFY_TOUCH_FOUND trigger
					if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
					{
						cerr << "onTrigger::eNOTIFY_TOUCH_FOUND" << endl;
						trigscore += 3;
						trigger = true;
					}
					//check if eNOTIFY_TOUCH_LOST trigger
					if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
					{
						cerr << "onTrigger::eNOTIFY_TOUCH_LOST" << endl;
						//cout << "Score: " << Stats::Score(0) << endl;
						trigger = false;
					}
				}
			}
		}

		///Method called when the contact by the filter shader is detected.
		virtual void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) 
		{
			cerr << "Contact found between " << pairHeader.actors[0]->getName() << " " << pairHeader.actors[1]->getName() << endl;

			//check all pairs
			for (PxU32 i = 0; i < nbPairs; i++)
			{
				//check eNOTIFY_TOUCH_FOUND
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				{
					cerr << "onContact::eNOTIFY_TOUCH_FOUND" << endl;
				}
				//check eNOTIFY_TOUCH_LOST
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_LOST)
				{
					cerr << "onContact::eNOTIFY_TOUCH_LOST" << endl;
				}
			}
		}

		virtual void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {}
		virtual void onWake(PxActor **actors, PxU32 count) {}
		virtual void onSleep(PxActor **actors, PxU32 count) {}
	};

	//A simple filter shader based on PxDefaultSimulationFilterShader - without group filtering
	static PxFilterFlags CustomFilterShader( PxFilterObjectAttributes attributes0,	PxFilterData filterData0,
		PxFilterObjectAttributes attributes1,	PxFilterData filterData1,
		PxPairFlags& pairFlags,	const void* constantBlock,	PxU32 constantBlockSize)
	{
		// let triggers through
		if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlags();
		}

		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
		//enable continous collision detection
//		pairFlags |= PxPairFlag::eCCD_LINEAR;
		
		
		//customise collision filtering here
		//e.g.

		// trigger the contact callback for pairs (A,B) where 
		// the filtermask of A contains the ID of B and vice versa.
		if((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		{
			//trigger onContact callback for this pair of objects
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
//			pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
		}

		return PxFilterFlags();
	};

	///Custom scene class
	class MyScene : public Scene
	{
		Box* player;
		Plane* plane;
		StBox* goal, * goal2;
		Pitch* pitch;
		GoalLines* glines;
		FieldLine* ttmline, * ttmline2, * hwline;
		DashFieldLine* tenmline, * tenmline2;
		GoalPost* gpost, * gpost2;
		GoalPostTrigger* goalScore;
		RugbyBall* ball;
		CastleArena* carena;
		Turret* t1, * t2, * t3, * t4;
		CataThrow* catathrow;
		CataFound* catafoundation;
		RevoluteJoint* joint;
		SpikeFlail* sw1;
		int score = 0;

	public:
		//specify your custom filter shader here
		//PxDefaultSimulationFilterShader by default
		MyScene() : Scene() {};

		///A custom scene class
		void SetVisualisation()
		{
			px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, true);
		}

		//Custom scene initialisation
		virtual void CustomInit() 
		{
			SetVisualisation();			

			GetMaterial()->setDynamicFriction(.5f);

			///Initialise and set the customised event callback
			goalScore = new GoalPostTrigger();
			px_scene->setSimulationEventCallback(goalScore);

			plane = new Plane();
			plane->Color(color_palette[0]);
			Add(plane);

			player = new Box(PxTransform(PxVec3(0.f, 5.f, 0.f)), PxVec3(0.25f, 0.25f, 0.25f), 6);
			player->Color(color_palette[2]);
			//Add(player);
			
			goal = new StBox(PxTransform(PxVec3(0.f, 7.5f, -50.f)), PxVec3(2.625f, 4.325f, 0.01f));
			goal2 = new StBox(PxTransform(PxVec3(0.f, 7.5f, 50.f)), PxVec3(2.625f, 4.325f, 0.01f));
			goal->Color(color_palette[1]);
			goal->SetTrigger(true);
			goal2->Color(color_palette[1]);

			pitch = new Pitch();
			pitch->Color(color_palette[3]);

			glines = new GoalLines();
			glines->Color(color_palette[3]);

			gpost = new GoalPost();
			gpost->Color(color_palette[3]);

			gpost2 = new GoalPost(PxTransform(PxVec3(0.0f, 0.0f, 100.f)));
			gpost2->Color(color_palette[3]);

			ttmline = new FieldLine(PxTransform(PxVec3(0.0f, 0.0f, -28.f)));
			ttmline->Color(color_palette[5]);

			ttmline2 = new FieldLine(PxTransform(PxVec3(0.0f, 0.0f, 28.f)));
			ttmline2->Color(color_palette[5]);

			hwline = new FieldLine();
			hwline->Color(color_palette[5]);

			tenmline = new DashFieldLine(PxTransform(PxVec3(0.0f, 0.0f, -10.f)));
			tenmline->Color(color_palette[5]);

			tenmline2 = new DashFieldLine(PxTransform(PxVec3(0.0f, 0.0f, 10.f)));
			tenmline2->Color(color_palette[5]);

			carena = new CastleArena();
			carena->Color(color_palette[1]);

			t1 = new Turret(PxTransform(PxVec3(-48.f, 15.f, -85.f)));
			t1->Color(color_palette[1]);

			t2 = new Turret(PxTransform(PxVec3(48.f, 15.f, -85.f)));
			t2->Color(color_palette[1]);

			t3 = new Turret(PxTransform(PxVec3(-48.f, 15.f, 85.f)));
			t3->Color(color_palette[1]);

			t4 = new Turret(PxTransform(PxVec3(48.f, 15.f, 85.f)));
			t4->Color(color_palette[1]);

			catathrow = new CataThrow();
			catathrow->Color(color_palette[1]);

			catafoundation = new CataFound();
			catafoundation->Color(color_palette[1]);

			ball = new RugbyBall(PxTransform(PxVec3(0.0f, 10.0f, 5.f)));
			ball->Color(color_palette[2]);
			ball->Material(CreateMaterial(0.9f, 0.5f, 0.8f));

			sw1 = new SpikeFlail(PxTransform(PxVec3(0.0f, 10.0f, -20.f)));
			sw1->Color(color_palette[5]);

			//catahelper = new Box(PxTransform(PxVec3(0.f, 1.f, -.75f)), PxVec3(.25f, 3.f, .25f), 1000.f);
			//catahelper->Color(color_palette[1]);

			//set collision filter flags
			catafoundation->SetupFiltering(FilterGroup::ACTOR0, FilterGroup::ACTOR1);
			//use | operator to combine more actors e.g.
			//box->SetupFiltering(FilterGroup::ACTOR0, FilterGroup::ACTOR1 | FilterGroup::ACTOR2);
			//don't forget to set your flags for the matching actor as well, e.g.:
			catathrow->SetupFiltering(FilterGroup::ACTOR1, FilterGroup::ACTOR0);

			goal->Name("GoalTrigger");
			Add(goal);
			goal2->Name("GoalTrigger2");
			Add(goal2);

			pitch->Name("Pitch");
			Add(pitch);

			glines->Name("GoalLines");
			Add(glines);

			gpost->Name("GoalPost");
			Add(gpost);

			gpost2->Name("GoalPost2");
			Add(gpost2);
			
			ttmline->Name("22mLine");
			Add(ttmline);

			ttmline2->Name("22mLine2");
			Add(ttmline2);

			hwline->Name("HalfwayLine");
			Add(hwline);

			tenmline->Name("10mLine");
			Add(tenmline);

			tenmline2->Name("10mLine2");
			Add(tenmline2);

			carena->Name("CastleArena");
			Add(carena);

			t1->Name("Turret1");
			Add(t1);

			t2->Name("Turret2");
			Add(t2);

			t3->Name("Turret3");
			Add(t3);

			t4->Name("Turret4");
			Add(t4);

			catathrow->Name("CatapultLauncher");
			Add(catathrow);

			catafoundation->Name("CatapultFoundation");
			Add(catafoundation);

			ball->Name("RugbyBall");
			Add(ball);

			sw1->Name("SpikeFlail1");
			//Add(sw1);


			//joint two boxes together
			//the joint is fixed to the centre of the first box, oriented by 90 degrees around the Y axis
			//and has the second object attached 5 meters away along the Y axis from the first object.
			joint = new RevoluteJoint(catafoundation, PxTransform(PxVec3(.0f,2.f,.0f),PxQuat(PxHalfPi,PxVec3(1.f,0.f,0.f))), catathrow, PxTransform(PxVec3(0.f,0.f,0.f)));
			joint->DriveVelocity(1.f);
		}

		//Custom update function
		virtual void CustomUpdate() 
		{
			ScoreAdd();
		}

		virtual int ScoreUpdate()
		{
			return score;
		}

		virtual void ScoreAdd()
		{
			score = goalScore->trigscore;
		}

		/// An example use of key release handling
		void CatapultKeyReleaseHandler()
		{
			cerr << "Catapult Resetting" << endl;
			joint->DriveVelocity(1.f);
		}

		/// An example use of key presse handling
		void CatapultKeyPressHandler()
		{
			cerr << "Catapult Launching" << endl;
			joint->DriveVelocity(-5.f);
		}

		virtual void AddBall(MyScene* scene)
		{
			RugbyBall* rball, *rball2, * rball3, * rball4, * rball5;
			int r = rand() % 255 + 1;
			int g = rand() % 255 + 1;
			int b = rand() % 255 + 1;

			rball = new RugbyBall(PxTransform(PxVec3(0.0f, 4.0f, 5.f)));
			rball->Color(PxVec3(r / 255.f, g / 255.f, b / 255.f));
			rball->Material(CreateMaterial(0.9f, 0.5f, 0.8f));

			scene->Add(rball);
		}

		virtual void SpamBall(MyScene* scene)
		{
			RugbyBall* rball, *rball2, * rball3, * rball4, * rball5;
			int r = rand() % 255 + 1;
			int g = rand() % 255 + 1;
			int b = rand() % 255 + 1;
			int x = rand() % 100 + (-51);
			int y = rand() % 50 + 1;
			int z = rand() % 100 + (-51);

			rball = new RugbyBall(PxTransform(PxVec3(x, y, z)));
			rball->Color(PxVec3(r / 255.f, g / 255.f, b / 255.f));
			rball->Material(CreateMaterial(0.9f, 0.5f, 0.8f));

			rball2 = new RugbyBall(PxTransform(PxVec3(x, y, z)));
			rball2->Color(PxVec3(r / 255.f, g / 255.f, b / 255.f));
			rball2->Material(CreateMaterial(0.9f, 0.5f, 0.8f));

			rball3 = new RugbyBall(PxTransform(PxVec3(x, y, z)));
			rball3->Color(PxVec3(r / 255.f, g / 255.f, b / 255.f));
			rball3->Material(CreateMaterial(0.9f, 0.5f, 0.8f));

			rball4 = new RugbyBall(PxTransform(PxVec3(x, y, z)));
			rball4->Color(PxVec3(r / 255.f, g / 255.f, b / 255.f));
			rball4->Material(CreateMaterial(0.9f, 0.5f, 0.8f));

			rball5 = new RugbyBall(PxTransform(PxVec3(x, y, z)));
			rball5->Color(PxVec3(r / 255.f, g / 255.f, b / 255.f));
			rball5->Material(CreateMaterial(0.9f, 0.5f, 0.8f));

			scene->Add(rball);
			scene->Add(rball2);
			scene->Add(rball3);
			scene->Add(rball4);
			scene->Add(rball5);
		}

		virtual void SpamCastle(MyScene* scene)
		{
			CastleArena* carena, *carena2, *carena3, *carena4, *carena5;
			int r = rand() % 255 + 1;
			int g = rand() % 255 + 1;
			int b = rand() % 255 + 1;
			int x = rand() % 100 + (-51);
			int y = rand() % 50 + 1;
			int z = rand() % 100 + (-51);

			carena = new CastleArena(PxTransform(PxVec3(x, y, z)));
			carena->Color(PxVec3(r / 255.f, g / 255.f, b / 255.f));

			carena2 = new CastleArena(PxTransform(PxVec3(x, y, z)));
			carena2->Color(PxVec3(r / 255.f, g / 255.f, b / 255.f));

			carena3 = new CastleArena(PxTransform(PxVec3(x, y, z)));
			carena3->Color(PxVec3(r / 255.f, g / 255.f, b / 255.f));

			carena4 = new CastleArena(PxTransform(PxVec3(x, y, z)));
			carena4->Color(PxVec3(r / 255.f, g / 255.f, b / 255.f));

			carena5 = new CastleArena(PxTransform(PxVec3(x, y, z)));
			carena5->Color(PxVec3(r / 255.f, g / 255.f, b / 255.f));

			scene->Add(carena);
			scene->Add(carena2);
			scene->Add(carena3);
			scene->Add(carena4);
			scene->Add(carena5);
		}
	};
}
