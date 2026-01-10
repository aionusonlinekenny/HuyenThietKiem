#ifndef KObstacleH
#define	KObstacleH

enum Obstacle_Type			
{
	Obstacle_Empty = 0,		
	Obstacle_Full,			
	Obstacle_LT,			
	Obstacle_RT,			
	Obstacle_LB,			
	Obstacle_RB,			
	Obstacle_Type_Num,		
};

enum Obstacle_Kind
{
	Obstacle_NULL = 0,		
	Obstacle_Normal,		
	Obstacle_Fly,			
	Obstacle_Jump,			
	Obstacle_JumpFly,		
	Obstacle_Kind_Num,		
};
#endif
