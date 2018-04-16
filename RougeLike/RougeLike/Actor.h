#pragma once

//Included Classes
#include "Sprite.h"
#include "Items.h"
#include "Objects.h"

public enum EActor
{
	//UNDEAD ACTORS
	ZOMBIE_RABBIT, MUMMY_RABBIT, NORMAL_SKELETON, BASIC01, SMALL_GHOST, NORMAL_REAPER, GRIM_REAPER01, BASIC02, BASIC03, WALKING_MOUSE_BIG,
	ZOMBIE_BASIC04, MUMMY_BASIC04, ORANGE_SKELETON, BASIC01_01, NORMAL_GHOST, SICK_REAPER, GRIM_REAPER02, BASIC02_01, SHAODW_GUY, WALKING_MOUSE_SMALL,
	ZOMBIE_KNIFE, MUMMY_KNIFE, GREEN_SKELETON, BASIC01_02, CRASY_GHOST, STICK_REAPER, OLD_MAN, BASIC02_02, BLANK01, BLANK02,
	ZOMBIE_WIZARD, MUMMY_WIZARD, DARK_SKELETON, BASIC01_03, SMALL_SHADOW_GUY, BLANK03, BLANK04, BLANK05, BLANK06, BLANK07,
	ZOMBIE_GIRL, MUMMY_GIRL, NORMAL_SKELETON_CLOTHING, BLANK08, BLANK09, BLANK10, BLANK11, BLANK12, BLANK13, BLANK14,
	ZOMBIE_BOY, MUMMY_BOY, GOLD_SKELETON_CLOTHING, BLANK15, BLANK16, BLANK17, BLANK18, BLANK19, BLANK20, BLANK21,
	ZOMBIE_TWOHEADS, MUMMY_TWOHEADS, KING_SKELETON_CLOTHING, BLANK22, BLANK23, BLANK24, BLANK25, BLANK26, BLANK27, BLANK28,
	ZOMBIE_TOUGH, MUMMY_TOUGH, EVIL_GUY, ENUM_LENGTH_ACTORS
};
//Enum for directions
public enum EDirection { EAST, SOUTH, WEST, NORTH };
//Enum class for bound actions
public enum EBoundsAction { BOUNCE, STOP, WRAP, DIE };
//State of the Actor
public enum EActorState { WANDERING, SEEKING, GATHERINGITEM, USE_ITEM, ATTACKING, AVOIDING, IDLE, ORIENTATION, DEAD };
//Enum for Seeking item or actor
public enum ESeekingState { SEEKITEM, SEEKACTOR };
//Enum for the Stats
public enum EActorStats { HEALTH, ARMOR, MONEY, SPEED, SEEK_CHANCE, ATTACK_CHANCE, ORIENTATION_CHANCE, GATHER_ITEM_CHANCE, USE_ITEM_CHANCE, USE_MAGIC_CHANCE, CAN_NOCLIP, ATTACK_DAMAGE, ENUM_LENGTH_ACTOR_STATS };
//Enum for attacking style
public enum EActorAttackState { HAND, MAGIC };

//==================================================================================
// This class contains the actors for the game. It's a child of Sprite
//==================================================================================
ref class Actor abstract : public Sprite
{
protected:
	array<Point>^ velocityDirections; //Movemovent Directions
	array<Sprite^>^ items; //Items the actor is holding
	EDirection spriteDirection; //Enum for directions
	EActorState actorState; //State of the Actor
	ESeekingState seekingState; //The seeking state of the actor
	EActorAttackState attackState;
	EActor actor;
	Rectangle detectRange; //Detect range for the Actor
	Rectangle attackRange; //Attack range for the Actor
	Actor^ actorWtihinDetection;
	Actor^ actorWithinAttackRange;
	Sprite^ itemWithinDetection;
	Sprite^ itemToUse;
	EBoundsAction boundingAction; //The Action that happens when the AI leaves the bounding area
	Rectangle boundingArea; //The bounding area which the Actor usual has to stay in
	//Items^ itemWithinDetection;
	array<int>^ stats;
	String^ name;
	Point currentPositionCheck;
	int default_health;
	bool isAlive; //If Actor is Alive
	bool isPlayer;
	bool wantsToUseMagic;
	bool canLogText;
	int amountHolding;
	int inventorySlotsFill;
	int nDirections;
	int nTicksTillAttack;
	int nTicksTillUnstuck;


public:
	//Actor^ Next;
public:
	Actor(Graphics ^ startCanvas, TileMap ^ startTilemap, Random ^ startRGen, array<Bitmap^>^ startSpriteSheets, Logger ^ startLogger, int startNFrames, int startNDrections, bool startIsplayer);
	void choseRandomDirection();
	virtual void updateDetectRange();
	void updateAttackRange();
	void attackMelee();
	void attackMagic();
	bool checkWithinDetectRange(Actor ^ otherActor);
	bool checkWithinDetectRange(Sprite ^ otherActor);
	bool unStuckActor();
	void checkActorStuck();
	bool canSeeSprite(Sprite ^ otherSprite);
	bool checkWithinAttackRange(Actor ^ otherActor);
	bool checkWithinGatheringRange(Sprite ^ otherActor);
	bool checkActorOntopOfSprite(Sprite ^ item);
	void removeItem(int index);
	void markedItemForDeletion(int index);
	bool gatherItem(Sprite ^ item);
	void useItem(Sprite ^ itemToUse);
	bool useObject(Sprite ^ objectToUse);
	bool checkTrapCanDamageActor(Objects ^ trap);
	void trapDamageActor(Objects ^ trap);
	void magicDamageActor();
	void dropInventory();
	void dropInventorySlot(int slot);
	virtual void UpdateState() abstract;
	virtual void PerformAction() abstract;

	bool UpdateTileArea();
	void checkWithinBoundingArea();
	void boundsAction();

	array<int>^ getPlayerStats();
	array<int>^ getAIStats(EActor actor);

	//Get and Sets
	void SpriteDirection(EDirection direction) { spriteDirection = direction; }
	bool getIsAlive() { return isAlive; }
	void setCanLog(bool newCanLog) { canLogText = newCanLog; }
	void setIsAlive(bool newIsAlive) { isAlive = newIsAlive; }
	bool getMagicStatus() { return wantsToUseMagic; }
	array<int>^ getActorStats() { return stats; }
	array<Sprite^>^ getActorInventory() { return items; }
	Sprite^ getActorWtihinDetection() { return actorWtihinDetection; }
	bool getIsPlayer() { return isPlayer; }
	void setIsPlayer(bool newPlayer) { isPlayer = newPlayer; }
	void setBoundingArea();
};

